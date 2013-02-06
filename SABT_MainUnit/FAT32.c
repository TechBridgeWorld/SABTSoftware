/**
 * @file FAT32.c
 * @brief    Reads data from File Allocation Table 32 (FAT 32) format.  
 *           This system defines clusters, which are contiguous regions of memory.
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

unsigned char getBootSectorData (void)
{
  struct BS_Structure *bpb; //mapping the buffer onto the structure
  struct MBRinfo_Structure *mbr;
  struct partitionInfo_Structure *partition;
  unsigned long dataSectors;

  unusedSectors = 0;

  SD_readSingleBlock(0);
  bpb = (struct BS_Structure *)buffer;

  if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB)   //check if it is boot sector
  {
    mbr = (struct MBRinfo_Structure *) buffer;       //if it is not boot sector, it must be MBR

    if(mbr->signature != 0xaa55) return 1;       //if it is not even MBR then it's not FAT32

    partition = (struct partitionInfo_Structure *)(mbr->partitionData);//first partition
    unusedSectors = partition->firstSector; //the unused sectors, hidden to the FAT

    SD_readSingleBlock(partition->firstSector);//read the bpb sector
    bpb = (struct BS_Structure *)buffer;
    if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB) return 1; 
  }

  bytesPerSector = bpb->bytesPerSector;
  //transmitHex(INT, bytesPerSector); USART_transmitByteToPC(' ');
  sectorPerCluster = bpb->sectorPerCluster;
  //transmitHex(INT, sectorPerCluster); USART_transmitByteToPC(' ');
  reservedSectorCount = bpb->reservedSectorCount;
  rootCluster = bpb->rootCluster;// + (sector / sectorPerCluster) +1;
  firstDataSector = bpb->hiddenSectors + reservedSectorCount + (bpb->numberofFATs * bpb->FATsize_F32);

  dataSectors = bpb->totalSectors_F32
                - bpb->reservedSectorCount
                - ( bpb->numberofFATs * bpb->FATsize_F32);
  totalClusters = dataSectors / sectorPerCluster;
  //transmitHex(LONG, totalClusters); USART_transmitByteToPC(' ');

  if((getSetFreeCluster (TOTAL_FREE, GET, 0)) > totalClusters)  //check if FSinfo free clusters count is valid
       freeClusterCountUpdated = 0;
  else
     freeClusterCountUpdated = 1;
  return 0;
}

//***************************************************************************
//Function: to calculate first sector address of any given cluster
//Arguments: cluster number for which first sector is to be found
//return: first sector address
//***************************************************************************
unsigned long getFirstSector(unsigned long clusterNumber)
{
  return (((clusterNumber - 2) * sectorPerCluster) + firstDataSector);
}

//***************************************************************************
//Function: get cluster entry value from FAT to find out the next cluster in the chain
//or set new cluster entry in FAT
//Arguments: 1. current cluster number, 2. get_set (=GET, if next cluster is to be found or = SET,
//if next cluster is to be set 3. next cluster number, if argument#2 = SET, else 0
//return: next cluster number, if if argument#2 = GET, else 0
//****************************************************************************
unsigned long getSetNextCluster (unsigned long clusterNumber,
                                 unsigned char get_set,
                                 unsigned long clusterEntry)
{
  unsigned int FATEntryOffset;
  unsigned long *FATEntryValue;
  unsigned long FATEntrySector;
  unsigned char retry = 0;

  //get sector number of the cluster entry in the FAT
  FATEntrySector = unusedSectors + reservedSectorCount + ((clusterNumber * 4) / bytesPerSector) ;

  //get the offset address in that sector number
  FATEntryOffset = (unsigned int) ((clusterNumber * 4) % bytesPerSector);

  //read the sector into a buffer
  while(retry <10)
  { if(!SD_readSingleBlock(FATEntrySector)) break; retry++;}

  //get the cluster address from the buffer
  FATEntryValue = (unsigned long *) &buffer[FATEntryOffset];

  if(get_set == GET)
    return ((*FATEntryValue) & 0x0fffffff);


  *FATEntryValue = clusterEntry;   //for setting new value in cluster entry in FAT

  SD_writeSingleBlock(FATEntrySector);

  return (0);
}

//********************************************************************************************
//Function: to get or set next free cluster or total free clusters in FSinfo sector of SD card
//Arguments: 1.flag:TOTAL_FREE or NEXT_FREE, 
//       2.flag: GET or SET 
//       3.new FS entry, when argument2 is SET; or 0, when argument2 is GET
//return: next free cluster, if arg1 is NEXT_FREE & arg2 is GET
//        total number of free clusters, if arg1 is TOTAL_FREE & arg2 is GET
//      0xffffffff, if any error or if arg2 is SET
//********************************************************************************************
unsigned long getSetFreeCluster(unsigned char totOrNext, unsigned char get_set, unsigned long FSEntry)
{
  struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &buffer;
  unsigned char error;

  SD_readSingleBlock(unusedSectors + 1);

  if((FS->leadSignature != 0x41615252) || (FS->structureSignature != 0x61417272) || (FS->trailSignature !=0xaa550000))
  return 0xffffffff;

  if(get_set == GET)
  {
  if(totOrNext == TOTAL_FREE)
    return(FS->freeClusterCount);
  else // when totOrNext = NEXT_FREE
    return(FS->nextFreeCluster);
  }
  else
  {
  if(totOrNext == TOTAL_FREE)
    FS->freeClusterCount = FSEntry;
  else // when totOrNext = NEXT_FREE
    FS->nextFreeCluster = FSEntry;

  error = SD_writeSingleBlock(unusedSectors + 1);  //update FSinfo
  }
  return 0xffffffff;
}

//***************************************************************************
//Function: to get DIR/FILE list or a single file address (cluster number) or to delete a specified file
//Arguments: #1 - flag: GET_LIST, GET_FILE or DELETE #2 - pointer to file name (0 if arg#1 is GET_LIST)
//return: first cluster of the file, if flag = GET_FILE
//        print file/dir list of the root directory, if flag = GET_LIST
//      Delete the file mentioned in arg#2, if flag = DELETE
//****************************************************************************
struct dir_Structure* findFiles (unsigned char flag, unsigned char *fileName)
{
  unsigned long cluster, sector, firstSector, firstCluster, nextCluster;
  struct dir_Structure *dir;
  unsigned int i;
  unsigned char j;

  cluster = rootCluster; //root cluster

  while(1)
  {
     firstSector = getFirstSector (cluster);

     for(sector = 0; sector < sectorPerCluster; sector++)
     {
       SD_readSingleBlock (firstSector + sector);
  

       for(i=0; i<bytesPerSector; i+=32)
       {
        dir = (struct dir_Structure *) &buffer[i];

          if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
      {
        if((flag == GET_FILE) || (flag == DELETE))
            USART_transmitStringToPCFromFlash(PSTR("File does not exist!"));
        return 0;   
      }
      if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
          {
            if((flag == GET_FILE) || (flag == DELETE))
            {
              for(j=0; j<11; j++)
              if(dir->name[j] != fileName[j]) break;
              if(j == 11)
        {
          if(flag == GET_FILE)
                {
            appendFileSector = firstSector + sector;
          appendFileLocation = i;
          appendStartCluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;
          fileSize = dir->fileSize;
            return (dir);
          }  
          else    //when flag = DELETE
          {
             TX_NEWLINE_PC;
           USART_transmitStringToPCFromFlash(PSTR("Deleting.."));
           TX_NEWLINE_PC;
           TX_NEWLINE_PC;
           firstCluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;
                
           //mark file as 'deleted' in FAT table
           dir->name[0] = DELETED;    
           SD_writeSingleBlock (firstSector+sector);
                
           freeMemoryUpdate (ADD, dir->fileSize);

           //update next free cluster entry in FSinfo sector
           cluster = getSetFreeCluster (NEXT_FREE, GET, 0); 
           if(firstCluster < cluster)
               getSetFreeCluster (NEXT_FREE, SET, firstCluster);

           //mark all the clusters allocated to the file as 'free'
             while(1)  
             {
                nextCluster = getSetNextCluster (firstCluster, GET, 0);
            getSetNextCluster (firstCluster, SET, 0);
            if(nextCluster > 0x0ffffff6) 
               {USART_transmitStringToPCFromFlash(PSTR("File deleted!"));return 0;}
            firstCluster = nextCluster;
             } 
          }
              }
            }
            else  //when flag = GET_LIST
        {
           TX_NEWLINE_PC;
         for(j=0; j<11; j++)
           {
           if(j == 8) USART_transmitByteToPC(' ');
           USART_transmitByteToPC (dir->name[j]);
         }
           USART_transmitStringToPCFromFlash (PSTR("   "));
           if((dir->attrib != 0x10) && (dir->attrib != 0x08))
         {
             USART_transmitStringToPCFromFlash (PSTR("FILE" ));
               USART_transmitStringToPCFromFlash (PSTR("   "));
             displayMemory (LOW, dir->fileSize);
         }
         else
           USART_transmitStringToPCFromFlash ((dir->attrib == 0x10)? PSTR("DIR") : PSTR("ROOT"));
        }
         }
       }
     }

     cluster = (getSetNextCluster (cluster, GET, 0));

     if(cluster > 0x0ffffff6)
        return 0;
     if(cluster == 0) 
     {USART_transmitStringToPCFromFlash(PSTR("Error in getting cluster"));  return 0;}
   }
  return 0;
}

//***************************************************************************
//Function: if flag=READ then to read file from SD card and send contents to UART 
//if flag=VERIFY then functions will verify whether a specified file is already existing
//Arguments: flag (READ or VERIFY) and pointer to the file name
//return: 0, if normal operation or flag is READ
//        1, if file is already existing and flag = VERIFY
//      2, if file name is incompatible
//***************************************************************************

unsigned char readFile (unsigned char flag, unsigned char *fileName)
{
  struct dir_Structure *dir;
  unsigned long cluster, byteCounter = 0, fileSize, firstSector;
  unsigned int k;//,iCntForSingleAudioWrite;
  unsigned char j, error;
  //unsigned int iAudioByteCnt;
  bool bEndOfFile=false;

  error = convertFileName (fileName); //convert fileName into FAT format
  if(error) return 2;

  dir = findFiles (GET_FILE, fileName); //get the file location
  if(dir == 0) 
    return (0);

  if(flag == VERIFY) return (1);  //specified file name is already existing

  cluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

  fileSize = dir->fileSize;

  //TX_NEWLINE_PC;
  //TX_NEWLINE_PC;

  while(1)
  {
    firstSector = getFirstSector (cluster);

    for(j=0; j<sectorPerCluster; j++)
    {
      SD_readSingleBlock(firstSector + j);


      for(k=0; k<512; k++)
      {
        if ((byteCounter++) >= fileSize ) bEndOfFile=true;
      }
      if(bEndOfFile)
      {
        return 0;
      }
    }
    cluster = getSetNextCluster (cluster, GET, 0);
    if(cluster == 0) {USART_transmitStringToPCFromFlash(PSTR("Error in getting cluster")); return 0;}
  }
  return 0;
}

/**
 * @brief
 * @param fileName    char *   That contains the file name of what you want to read
          dataString  char *   Where the data is put, can store 100 bytes
 * @return unsigned char - number where zero means success
 */
unsigned char readAndRetreiveFileContents (unsigned char *fileName, unsigned char *dataString)
{
  struct dir_Structure *dir;
  unsigned long cluster, byteCounter = 0, fileSize, firstSector;
  unsigned int k;//,iCntForSingleAudioWrite;
  unsigned char j, error;
  unsigned int iReadByteCnt;
  bool bEndOfFile=false;

  error = convertFileName (fileName); //convert fileName into FAT format
  if(error) return 1;

  dir = findFiles (GET_FILE, fileName); //get the file location
  if(dir == 0) 
    return (2);

//  if(flag == VERIFY) return (1);  //specified file name is already existing

  cluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

  fileSize = dir->fileSize;

  //TX_NEWLINE_PC;
  //TX_NEWLINE_PC;

  while(1)
  {
    firstSector = getFirstSector (cluster);

    for(j=0; j<sectorPerCluster; j++)
    {
      SD_readSingleBlock(firstSector + j);


      for(k=0; k<512; k++)
      {
        if ((byteCounter++) >= fileSize ) bEndOfFile=true;
        
      }
      while(iReadByteCnt<k)
      {
        *dataString++=buffer[iReadByteCnt];
        if(iReadByteCnt++==k)return 0;  
      }
      if(bEndOfFile)
      {
        return 0;
      }
    }
    cluster = getSetNextCluster (cluster, GET, 0);
    if(cluster == 0) {USART_transmitStringToPCFromFlash(PSTR("Error in getting cluster")); return 3;}
  }
  return 4;
}


/*
This function plays a given MP3 files, until:
1. The files reach the end of file
2. Stop playing command issued from the controller

*/

unsigned char PlayMP3file (unsigned char *fileName)
{
  struct dir_Structure *dir;
  unsigned long cluster, byteCounter = 0, fileSize, firstSector;
  unsigned int k,iCntForSingleAudioWrite;
  unsigned char j, error;
  unsigned int iAudioByteCnt;
  bool bEndOfFile=false;

  UI_MP3_file_Pending=false;

  error = convertFileName (fileName); //convert fileName into FAT format
  if(error) return 2;

  dir = findFiles (GET_FILE, fileName); //get the file location
  if(dir == 0) 
    return (0);

  cluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

  fileSize = dir->fileSize;
  VS1053_SKIP_PLAY=false;
  while(1)
  {
    firstSector = getFirstSector (cluster);

    for(j=0; j<sectorPerCluster; j++)
    {
      SD_readSingleBlock(firstSector + j);


      for(k=0; k<512; k++)
      {
        if ((byteCounter++) >= fileSize ) bEndOfFile=true;
      }
      //After reading each sector in the file --> send them to MP3 decoder in 32 byte segments
      iAudioByteCnt=0;
      while(iAudioByteCnt<k)
      {
        if(VS1053_SKIP_PLAY)
        {
          VS1053_SKIP_PLAY=false;
          VS1053_SoftwareReset();
          return 0;//playing stopped by user
        }
        if((PINB & (1<<MP3_DREQ)))
        {
          for(iCntForSingleAudioWrite=0;iCntForSingleAudioWrite<32;iCntForSingleAudioWrite++)
          {
            VS1053_WriteData(buffer[iAudioByteCnt]);
            if(iAudioByteCnt++==k)return 0;    
          }  
        }
        //After playing a 32 bytes of data, check the user inputs
        if(USART_Keypad_DATA_RDY)
        {
          USART_Keypad_ReceiveAction();
        }
        if(USART_PC_DATA_RDY)
        {
          USART_PC_ReceiveAction();
        }    
        if(USART_UI_Message_ready) //If a message ready from the user interface, process it
        {
          UI_parse_message(true);
        }
      }
      if(bEndOfFile)
      {
        return 0;
      }

    }

    cluster = getSetNextCluster (cluster, GET, 0);
    if(cluster == 0) 
    {
      USART_transmitStringToPCFromFlash(PSTR("Error in getting cluster")); 
      return 0;
    }
  }
  return 0;
}


//***************************************************************************
//Function: to convert normal short file name into FAT format
//Arguments: pointer to the file name
//return: 0, if successful else 1.
//***************************************************************************

/**
 * @brief Converts the input fileName (which is in FAT format) in the following fashion:
 * <filename.ext> -----> <filename[padding to 8 chars]ext>
 * filename must be <= 8 chars and ext must be <= 3 chars.
 * Thus, INT.MP3 becomes [INT     MP3]. Also, capitalizes lowercase files.
 * @param fileName unsigned char* string which contains the file name that needs to be converted
 * @return unsigned char 1 for failure 0 for victory
 * @TODO Currently overwrites the passed in buffer so if you pass in the same buffer twice, it 
 * will have already converted the fileName (specifically removed the '.' which is what the invalid
 * file check is done off of)
*/ 
unsigned char convertFileName (unsigned char *fileName)
{
  unsigned char fileNameFAT[11];
  unsigned char j, k;

  DPRINTF("|%s |\n\r", fileName);

  for(j=0; j<12; j++) {
    DPRINTF("%c", fileName[j]);
    if(fileName[j] == '.') 
      break;
  }
  DPRINTF("\n\r");

  
  if (j == 12)
    // assume that a string without any dots is already converted
    return 0;

  if(j>8) {
    USART_transmitStringToPCFromFlash(PSTR("Invalid fileName.")); 
    return 1;
  }

  for(k=0; k<j; k++) //setting file name
    fileNameFAT[k] = fileName[k];

  for(k=j; k<=7; k++) //filling file name trail with blanks
    fileNameFAT[k] = ' ';

  j++;
  for(k=8; k<11; k++) //setting file extention
  {
    if(fileName[j] != 0)
    {
      fileNameFAT[k] = fileName[j++];
    }
    else //filling extension trail with blanks
    {
      while(k<11)
      {
        fileNameFAT[k++] = ' ';
      }
    }
  }

  for(j=0; j<11; j++) //converting small letters to caps
  {
    if((fileNameFAT[j] >= 0x61) && (fileNameFAT[j] <= 0x7a))
    {
        fileNameFAT[j] -= 0x20;
    }
  }


  for(j=0; j<11; j++)
    fileName[j] = fileNameFAT[j];


  DPRINTF("[%s]\n\r", fileName);

  return 0;
}

/*
Modified write file function with replacing all the text with new text given
*/

int ReplaceTheContentOfThisFileWith (unsigned char *fileName, unsigned char *fileContent)
{
  unsigned char j, data, error, fileCreatedFlag = 0, start = 0, appendFile = 0, sectorEndFlag = 0, sector;
  unsigned int i, firstClusterHigh, firstClusterLow;
  struct dir_Structure *dir;
  unsigned long cluster, nextCluster, prevCluster, firstSector, clusterCount, extraMemory;

  //j = readFile (VERIFY, fileName);

  if(readFile (VERIFY, fileName) == 1) 
  {
    //USART_transmitStringToPCFromFlash(PSTR("File found, replacing existing information...")); 
    //TX_NEWLINE_PC;
    appendFile = 1;
    cluster = appendStartCluster;
    clusterCount=0;
    firstSector = getFirstSector (cluster);
    startBlock = getFirstSector (cluster);
    i=0;
    j=0;
    while(*fileContent!='$')
    {      
      buffer[i++]=*fileContent;
      fileContent++;
      if(i >= 512)
      {        
        i=0;
        error = SD_writeSingleBlock (startBlock);
        j++;
        if(j == sectorPerCluster) {j = 0; break;}
        startBlock++; 
      }
    }
    //fileContent--;
    if(*fileContent=='$')
    {
      buffer[i++]='$';
      for(;i<512;i++)  //fill the rest of the buffer with 0x00
        buffer[i]= 0x00;
      error = SD_writeSingleBlock (startBlock);
      _delay_ms(100);
      //USART_transmitStringToPCFromFlash(PSTR("Successfully replace the file content")); 
      //TX_NEWLINE_PC;
    }
    SD_readSingleBlock (firstSector);    
    _delay_ms(100);
    dir = (struct dir_Structure *) &buffer[0]; 
    //extraMemory = fileSize - dir->fileSize;
    dir->fileSize = fileSize;
    SD_writeSingleBlock (firstSector);
    _delay_ms(100);
    return 0;
    //freeMemoryUpdate (REMOVE, extraMemory); //updating free memory count in FSinfo sector;

  }
  else
  {
    return 1;
    //USART_transmitStringToPCFromFlash(PSTR("SABT-ERR001")); 
    //TX_NEWLINE_PC;
  }
  return 2;
}


//************************************************************************************
//Function: to create a file in FAT32 format in the root directory if given 
//      file name does not exist; if the file already exists then append the data
//Arguments: pointer to the file name
//return: none
//************************************************************************************
void writeFile (unsigned char *fileName)
{
unsigned char j, data, error, fileCreatedFlag = 0, start = 0, appendFile = 0, sectorEndFlag = 0, sector;
unsigned int i, firstClusterHigh, firstClusterLow;
struct dir_Structure *dir;
unsigned long cluster, nextCluster, prevCluster, firstSector, clusterCount, extraMemory;

j = readFile (VERIFY, fileName);

if(j == 1) 
{
  USART_transmitStringToPCFromFlash(PSTR("  File already existing, appending data..")); 
  appendFile = 1;
  cluster = appendStartCluster;
  clusterCount=0;
  while(1)
  {
    nextCluster = getSetNextCluster (cluster, GET, 0);
    if(nextCluster == EOF) break;
  cluster = nextCluster;
  clusterCount++;
  }

  sector = (fileSize - (clusterCount * sectorPerCluster * bytesPerSector)) / bytesPerSector; //last sector number of the last cluster of the file
  start = 1;
//  appendFile();
//  return;
}
else if(j == 2) 
   return; //invalid file name
else
{
//  TX_NEWLINE_PC;
//  USART_transmitStringToPCFromFlash(PSTR(" Creating File.."));

  cluster = getSetFreeCluster (NEXT_FREE, GET, 0);
  if(cluster > totalClusters)
     cluster = rootCluster;

  cluster = searchNextFreeCluster(cluster);
   if(cluster == 0)
   {
//      TX_NEWLINE_PC;
//      USART_transmitStringToPCFromFlash(PSTR(" No free cluster!"));
    return;
   }
  getSetNextCluster(cluster, SET, EOF);   //last cluster of the file, marked EOF
   
  firstClusterHigh = (unsigned int) ((cluster & 0xffff0000) >> 16 );
  firstClusterLow = (unsigned int) ( cluster & 0x0000ffff);
  fileSize = 0;
}



while(1)
{
   if(start)
   {
      start = 0;
    startBlock = getFirstSector (cluster) + sector;
    SD_readSingleBlock (startBlock);
    i = fileSize % bytesPerSector;
    j = sector;
   }
   else
   {
      startBlock = getFirstSector (cluster);
    i=0;
    j=0;
   }
   

//   TX_NEWLINE_PC;
//   USART_transmitStringToPCFromFlash(PSTR(" Enter text (end with ~):"));
   
   do
   {
     if(sectorEndFlag == 1) //special case when the last character in previous sector was '\r'
   {
     USART_transmitByteToPC ('\n');
        buffer[i++] = '\n'; //appending 'Line Feed (LF)' character
    fileSize++;
   }

  sectorEndFlag = 0;

   data = 0;//receiveByte();
   if(data == 0x08)  //'Back Space' key pressed
   { 
     if(i != 0)
     { 
//       USART_transmitByteToPC(data);
//     USART_transmitByteToPC(' '); 
//       USART_transmitByteToPC(data); 
       i--; 
     fileSize--;
     } 
     continue;     
   }
//   USART_transmitByteToPC(data);
     buffer[i++] = data;
   fileSize++;
     if(data == '\r')  //'Carriege Return (CR)' character
     {
        if(i == 512)
       sectorEndFlag = 1;  //flag to indicate that the appended '\n' char should be put in the next sector
      else
    { 
//       USART_transmitByteToPC ('\n');
           buffer[i++] = '\n'; //appending 'Line Feed (LF)' character
       fileSize++;
      }
     }
   
     if(i >= 512)   //though 'i' will never become greater than 512, it's kept here to avoid 
   {        //infinite loop in case it happens to be greater than 512 due to some data corruption
     i=0;
     error = SD_writeSingleBlock (startBlock);
       j++;
     if(j == sectorPerCluster) {j = 0; break;}
     startBlock++; 
     }
  }while (data != '~');

   if(data == '~') 
   {
      fileSize--;  //to remove the last entered '~' character
    i--;
    for(;i<512;i++)  //fill the rest of the buffer with 0x00
        buffer[i]= 0x00;
       error = SD_writeSingleBlock (startBlock);

      break;
   } 
    
   prevCluster = cluster;

   cluster = searchNextFreeCluster(prevCluster); //look for a free cluster starting from the current cluster

   if(cluster == 0)
   {
//      TX_NEWLINE_PC;
//      USART_transmitStringToPCFromFlash(PSTR(" No free cluster!"));
    return;
   }

   getSetNextCluster(prevCluster, SET, cluster);
   getSetNextCluster(cluster, SET, EOF);   //last cluster of the file, marked EOF
}        

getSetFreeCluster (NEXT_FREE, SET, cluster); //update FSinfo next free cluster entry

if(appendFile)  //executes this loop if file is to be appended
{
  SD_readSingleBlock (appendFileSector);    
  dir = (struct dir_Structure *) &buffer[appendFileLocation]; 
  extraMemory = fileSize - dir->fileSize;
  dir->fileSize = fileSize;
  SD_writeSingleBlock (appendFileSector);
  freeMemoryUpdate (REMOVE, extraMemory); //updating free memory count in FSinfo sector;

  
//  TX_NEWLINE_PC;
//  USART_transmitStringToPCFromFlash(PSTR(" File appended!"));
//  TX_NEWLINE_PC;
  return;
}

//executes following portion when new file is created

prevCluster = rootCluster; //root cluster

while(1)
{
   firstSector = getFirstSector (prevCluster);

   for(sector = 0; sector < sectorPerCluster; sector++)
   {
     SD_readSingleBlock (firstSector + sector);
  

     for(i=0; i<bytesPerSector; i+=32)
     {
      dir = (struct dir_Structure *) &buffer[i];

    if(fileCreatedFlag)   //to mark last directory entry with 0x00 (empty) mark
     {             //indicating end of the directory file list
       dir->name[0] = 0x00;
           return;
         }

        if((dir->name[0] == EMPTY) || (dir->name[0] == DELETED))  //looking for an empty slot to enter file info
    {
      for(j=0; j<11; j++)
        dir->name[j] = fileName[j];
      dir->attrib = ATTR_ARCHIVE;  //settting file attribute as 'archive'
      dir->NTreserved = 0;      //always set to 0
      dir->timeTenth = 0;      //always set to 0
      dir->createTime = 0x9684;    //fixed time of creation
      dir->createDate = 0x3a37;    //fixed date of creation
      dir->lastAccessDate = 0x3a37;  //fixed date of last access
      dir->writeTime = 0x9684;    //fixed time of last write
      dir->writeDate = 0x3a37;    //fixed date of last write
      dir->firstClusterHI = firstClusterHigh;
      dir->firstClusterLO = firstClusterLow;
      dir->fileSize = fileSize;

      SD_writeSingleBlock (firstSector + sector);
      fileCreatedFlag = 1;

//      TX_NEWLINE_PC;
//      TX_NEWLINE_PC;
//      USART_transmitStringToPCFromFlash(PSTR(" File Created!"));

      freeMemoryUpdate (REMOVE, fileSize); //updating free memory count in FSinfo sector
       
        }
     }
   }

   cluster = getSetNextCluster (prevCluster, GET, 0);

   if(cluster > 0x0ffffff6)
   {
      if(cluster == EOF)   //this situation will come when total files in root is multiple of (32*sectorPerCluster)
    {  
    cluster = searchNextFreeCluster(prevCluster); //find next cluster for root directory entries
    getSetNextCluster(prevCluster, SET, cluster); //link the new cluster of root to the previous cluster
    getSetNextCluster(cluster, SET, EOF);  //set the new cluster as end of the root directory
      } 

      else
      {  
//      USART_transmitStringToPCFromFlash(PSTR("End of Cluster Chain")); 
      return;
      }
   }
   if(cluster == 0) {
//       USART_transmitStringToPCFromFlash(PSTR("Error in getting cluster")); 
    return;
  }
   
   prevCluster = cluster;
 }
 
 return;
}


//***************************************************************************
//Function: to search for the next free cluster in the root directory
//          starting from a specified cluster
//Arguments: Starting cluster
//return: the next free cluster
//****************************************************************
unsigned long searchNextFreeCluster (unsigned long startCluster)
{
  unsigned long cluster, *value, sector;
  unsigned char i;
    
  startCluster -=  (startCluster % 128);   //to start with the first file in a FAT sector
    for(cluster =startCluster; cluster <totalClusters; cluster+=128) 
    {
      sector = unusedSectors + reservedSectorCount + ((cluster * 4) / bytesPerSector);
      SD_readSingleBlock(sector);
      for(i=0; i<128; i++)
      {
          value = (unsigned long *) &buffer[i*4];
         if(((*value) & 0x0fffffff) == 0)
            return(cluster+i);
      }  
    } 

 return 0;
}

//***************************************************************************
//Function: to display total memory and free memory of SD card, using UART
//Arguments: none
//return: none
//Note: this routine can take upto 15sec for 1GB card (@1MHz clock)
//it tries to read from SD whether a free cluster count is stored, if it is stored
//then it will return immediately. Otherwise it will count the total number of
//free clusters, which takes time
//****************************************************************************
void memoryStatistics (void)
{
unsigned long freeClusters, totalClusterCount, cluster;
unsigned long totalMemory, freeMemory;
unsigned long sector, *value;
unsigned int i;


totalMemory = totalClusters * sectorPerCluster / 1024;
totalMemory *= bytesPerSector;

TX_NEWLINE_PC;
TX_NEWLINE_PC;
//USART_transmitStringToPCFromFlash(PSTR("Total Memory: "));

displayMemory (HIGH, totalMemory);

freeClusters = getSetFreeCluster (TOTAL_FREE, GET, 0);
//freeClusters = 0xffffffff;    

if(freeClusters > totalClusters)
{
   freeClusterCountUpdated = 0;
   freeClusters = 0;
   totalClusterCount = 0;
   cluster = rootCluster;    
    while(1)
    {
      sector = unusedSectors + reservedSectorCount + ((cluster * 4) / bytesPerSector) ;
      SD_readSingleBlock(sector);
      for(i=0; i<128; i++)
      {
           value = (unsigned long *) &buffer[i*4];
         if(((*value)& 0x0fffffff) == 0)
            freeClusters++;;
        
         totalClusterCount++;
         if(totalClusterCount == (totalClusters+2)) break;
      }  
      if(i < 128) break;
      cluster+=128;
    } 
}

if(!freeClusterCountUpdated)
  getSetFreeCluster (TOTAL_FREE, SET, freeClusters); //update FSinfo next free cluster entry
freeClusterCountUpdated = 1;  //set flag
freeMemory = freeClusters * sectorPerCluster / 1024;
freeMemory *= bytesPerSector ;
//TX_NEWLINE_PC;
//USART_transmitStringToPCFromFlash(PSTR(" Free Memory: "));
displayMemory (HIGH, freeMemory);
//TX_NEWLINE_PC; 
}

//************************************************************
//Function: To convert the unsigned long value of memory into 
//          text string and send to UART
//Arguments: 1. unsigned char flag. If flag is HIGH, memory will be displayed in KBytes, else in Bytes. 
//       2. unsigned long memory value
//return: none
//************************************************************
void displayMemory (unsigned char flag, unsigned long memory)
{
  unsigned char memoryString[] = "              Bytes"; //19 character long string for memory display
  unsigned char i;
  for(i=12; i>0; i--) //converting freeMemory into ASCII string
  {
    if(i==5 || i==9) 
  {
     memoryString[i-1] = ',';  
     i--;
  }
    memoryString[i-1] = (memory % 10) | 0x30;
    memory /= 10;
  if(memory == 0) break;
  }
  if(flag == HIGH)  memoryString[13] = 'K';
  USART_transmitStringToPC(memoryString);
}

//********************************************************************
//Function: to delete a specified file from the root directory
//Arguments: pointer to the file name
//return: none
//********************************************************************
void deleteFile (unsigned char *fileName)
{
  unsigned char error;

  error = convertFileName (fileName);
  if(error) return;

  findFiles (DELETE, fileName);
}

//********************************************************************
//Function: update the free memory count in the FSinfo sector. 
//      Whenever a file is deleted or created, this function will be called
//      to ADD or REMOVE clusters occupied by the file
//Arguments: #1.flag ADD or REMOVE #2.file size in Bytes
//return: none
//********************************************************************
void freeMemoryUpdate (unsigned char flag, unsigned long size)
{
  unsigned long freeClusters;
  //convert file size into number of clusters occupied
  if((size % 512) == 0) size = size / 512;
  else size = (size / 512) +1;
  if((size % 8) == 0) size = size / 8;
  else size = (size / 8) +1;

  if(freeClusterCountUpdated)
  {
  freeClusters = getSetFreeCluster (TOTAL_FREE, GET, 0);
  if(flag == ADD)
       freeClusters = freeClusters + size;
  else  //when flag = REMOVE
     freeClusters = freeClusters - size;
  getSetFreeCluster (TOTAL_FREE, SET, freeClusters);
  }
}

/**
 * @brief Checks the SD card, communicates with computer to let know what type of 
 *        card and sets up the rest of the SD card.
 * @param bVerb  bool   Determines whether of not you are communicating found data
 *        to the computer
 * @ref   www.dharmanitech.com
 * @return Void
 */
void InitSDCard(bool bVerb)
{
  unsigned char iInit=0;
  unsigned char option, error, data, FAT32_active;
  unsigned int i;
  cardType = 0;

  for (i=0; i<10; i++)
  {
    error = SD_init();
    if(!error) break;
  }

  if(bVerb)
  {
    if(error)
    {
      if(error == 1) USART_transmitStringToPCFromFlash(PSTR("SD card not detected.."));
      if(error == 2) USART_transmitStringToPCFromFlash(PSTR("Card Initialization failed.."));
        while(1);  //wait here forever if error in SD init   
    }


    switch (cardType)
    {
      case 1:USART_transmitStringToPCFromFlash(PSTR("Standard Capacity Card (Ver 1.x) Detected"));
           break;
      case 2:USART_transmitStringToPCFromFlash(PSTR("High Capacity Card Detected"));
           break;
      case 3:USART_transmitStringToPCFromFlash(PSTR("Standard Capacity Card (Ver 2.x) Detected"));
           break;
      default:USART_transmitStringToPCFromFlash(PSTR("Unknown SD Card Detected"));
           break; 
    }
  

    _delay_ms(1);   //some delay

    FAT32_active = 1;
    error = getBootSectorData (); //read boot sector and keep necessary data in global variables
    if(error)   
    {
      TX_NEWLINE_PC;
      USART_transmitStringToPCFromFlash (PSTR("FAT32 not found!"));  //FAT32 incompatible drive
      FAT32_active = 0;
    }else{
      TX_NEWLINE_PC;
      USART_transmitStringToPCFromFlash (PSTR("FAT32 file system detected..."));
    }
    TX_NEWLINE_PC;  
    iInit=VS1053_Initialize();
    if(iInit==0){
      USART_transmitStringToPCFromFlash (PSTR("VS1053 MP3 chip sucessfully initialized"));
      TX_NEWLINE_PC;  
    }else{
      USART_transmitStringToPCFromFlash (PSTR("Error initializing VS1053 - CODE "));
      USART_transmitByteToPC(iInit+64);
      TX_NEWLINE_PC;
    }
  }
  else
  {
    if(error)
    {
        while(1);  //wait here forever if error in SD init   
    }
    _delay_ms(1);   //some delay

    FAT32_active = 1;
    error = getBootSectorData (); //read boot sector and keep necessary data in global variables
    if(error)   
    {
      FAT32_active = 0;
    }else{

    }
    iInit=VS1053_Initialize();
  }
}
