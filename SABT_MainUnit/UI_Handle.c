/**
 * @file UI_Handle.c
 * @brief This file controls both User interface to Main control and
 *        computer ineractions
 * @ref   tech_report.pdf
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"

/**
 * @brief  reads modes from MODES.DAT file and tells computer how many modes and 
 *         what they are
 * @return bool  - return true on succesful completion of transmission, false on 
 *         failure
 */
bool UI_CheckModes(void)
{
  /*
  The modes.dat contains the numbers of modes that need to be activated in this device. It contains the mode numbers of the required modes as follows:
  MODENO#MODENO#$ ETC. (Last character is $)
  For example if we need modes 2, 5 and 14 to be activated:
  <2><5><14>$
  */
  unsigned char FileContent[100];
  unsigned char ModeID[3];
  // TODO should PCPrintContent be of length 4?
  unsigned char PCPrintContent[2];
  int i = 0;
  int iMoN;
  bool bBoNFound;
  const char* ModesFile = "MODES.DAT";

  Number_of_modes=0;
  for(i=0;i<100;i++)
    FileContent[i]=0;
  if(readAndRetreiveFileContents (ModesFile,FileContent)>0)
  {
    return false;
  }
  USART_transmitStringToPC(&FileContent);
  TX_NEWLINE_PC;
  bBoNFound=false;
  i=0;
  while(FileContent[i]!='$')
  {
    if(FileContent[i]=='>')
    {
      UI_Modes[Number_of_modes]=atoi(ModeID);
      Number_of_modes++;
      bBoNFound=false;
    }
    if(!bBoNFound)
    {
      ModeID[0]=0;
      ModeID[1]=0;
      ModeID[2]=0;
      iMoN=0;
    }
    else
    {
      if(iMoN==3)
      {
        return false;
      }
      ModeID[iMoN++]=FileContent[i];
    }
    if(FileContent[i]=='<')
    {
      bBoNFound=true;
    }


    i++;    
  }
  USART_transmitStringToPCFromFlash(PSTR("Number of modes selected: "));
  PCPrintContent[0]=0;
  PCPrintContent[1]=0;
  sprintf(PCPrintContent, "%d", Number_of_modes);
  USART_transmitStringToPC(&PCPrintContent);
  TX_NEWLINE_PC;
  USART_transmitStringToPCFromFlash(PSTR("And the modes are; "));
  for(i=0;i<Number_of_modes;i++)
  {
    sprintf(PCPrintContent, "%d, ", UI_Modes[i]);
    USART_transmitStringToPC(&PCPrintContent);
  }  
  TX_NEWLINE_PC;
  return true; 
}

/**
 * @brief calculate CRC (cyclic redundancy check)
 * @param pstrMsg message (?)
 * @return CRC value
 */
uint16_t UI_calculate_CRC(unsigned char* pstrMsg)
{
  unsigned char msglen=*(pstrMsg+2)-5;//Not including the checksum bytes
  uint16_t chksum=0;

  pstrMsg+=3;
  while(msglen > 1)
  {
    chksum+=(*(pstrMsg)<<8) | *(pstrMsg+1);
    chksum = chksum & 0xffff;
    msglen-=2;
    pstrMsg+=2;
  }
  if(msglen>0) //If the packet size is odd numbered
  {
    chksum = chksum^ (int)*(pstrMsg++);
  }
  return (chksum);
}

//TODO this seems pointless?
bool UI_buildMessage(char MessageType)
{
  return true;
}


/**
 * @brief  Reads a message in USART_UI_RecievedPacket. Then determines what type of
 *         message it has recieved. It then interacts with the mode accordingly and
 *         sends a message of recieval at the end. 
 *
 * UI string: [U][I][msglen][msg_number][msgtype][payload][CRC1][CRC2]
 * msgtypes: 
 * @param  IsPlaying -  bool    determines whether or not an MP3 Files is playing
 * @return  bool - returns true if succesfully parsed, understood, and used message
 */
bool UI_parse_message(bool mp3_is_playing)
{
  //First things first, check the CRC

  unsigned char message_len = USART_UI_ReceivedPacket[2];
  unsigned char message_number;
  unsigned char message_type;

  unsigned char ADCmsg[10];
  //unsigned char message_payload[20];
  //unsigned char i=0;

  uint16_t chksum = UI_calculate_CRC(&USART_UI_ReceivedPacket);
  
  // TODO test this
  PRINTF("[UI_parse_message] Entering function");
  
  // Check the checksum
  if (chksum == (USART_UI_ReceivedPacket[message_len-2] << 8 
                | USART_UI_ReceivedPacket[message_len-1]))
  {
    // If correct, store the message elements
    message_number = USART_UI_ReceivedPacket[3];
    message_type = USART_UI_ReceivedPacket[4];

    // Process the message
    if(mp3_is_playing) //If a MP3 file is being played, only the commands are processed
    {
      if(message_type == 'D')               // Control key pressed
      {
        UI_ControlKeyPressed();
        USART_UI_Message_ready = true;      // Once control handled, exit
        return true;
      }
    }
    
    // Handle each type of message separately
    switch(message_type)
    {
      case 'A':                             // Single braille dot
        UI_InputDotToCurrentMode(USART_UI_ReceivedPacket[5]);
        break;
      case 'B':                             // Single Braille cell
        UI_InputCellToCurrentMode(USART_UI_ReceivedPacket[5]);
        break;
      case 'C':                             // Error message
        PRINTF("[UI_parse_message] An error occurred in the UI.");
        break;
      case 'D':                             // Control button
        UI_ControlKeyPressed();
        break;
      case 'E':                             // Acknowledgement
        // TODO what does this section do? 'E' marked misceaneous in doc
        ADCmsg[0]=USART_UI_ReceivedPacket[5];
        ADCmsg[1]=USART_UI_ReceivedPacket[6];
        ADCmsg[2]=USART_UI_ReceivedPacket[7];
        USART_transmitStringToPCFromFlash(PSTR("Analog Input channel,MSB,LSB :"));
        sprintf(ADCmsg,
                "%d,%d,%d",
                USART_UI_ReceivedPacket[5],
                USART_UI_ReceivedPacket[6],
                USART_UI_ReceivedPacket[7]);
        USART_transmitStringToPC(&ADCmsg);
        TX_NEWLINE_PC;
        TX_NEWLINE_PC;
        break;
      default:
        break;
    }
    // In the end: send acknowledgement to the sender (with the message number, of course !!!)
  } else // Checksum not valid
  {
    USART_UI_Message_ready = false;
    return false;
  }

  USART_UI_Message_ready=false;
  return true;
}

/**
 * @brief (?) 
 * @return Void
 */
void UI_ControlKeyPressed(void)
{
  switch(USART_UI_ReceivedPacket[5])
  {
    case UI_CMD_NONE:
      break;
    case UI_CMD_ENT1: //Enter into a mode
      USART_transmitStringToPCFromFlash(PSTR("Enter 1 pressed"));
      TX_NEWLINE_PC;  
      if(!UI_MODE_SELECTED) //Then this command is to select the mode
      {
        if(UI_Selected_Mode>0)
        {
          UI_MODE_SELECTED=true;
          UI_ResetTheCurrentMode();
        }
        else
        {
          // TODO commented (?)
          //RequestToPlayMP3file("ERR1.MP3");
        }
      }
      else //Then this the "YES" command in the mode, so call the function in the mode
      {
        UI_CallModeYesAnswer();
      }
      break;
    case UI_CMD_ENT2: //Exit from a mode
      USART_transmitStringToPCFromFlash(PSTR("Enter 2 pressed"));
      TX_NEWLINE_PC;
      if(UI_MODE_SELECTED) //This might be an exit from mode command or "NO" command in the mode
      {
        if(USART_UI_ReceivedPacket[6]==69) //If the next byte is 'E', this is exit command (when the user pressed E2 for more than 5 secs)
        {
          UI_MODE_SELECTED=false;
          RequestToPlayMP3file("MM.MP3");
        }
        else //Then this a "NO" answer, call the mode function for this
        {
          UI_CallModeNoAnswer();
        }
      }
      //This has no effect when no mode is selected
      break;
    case UI_CMD_MFOR:
      USART_transmitStringToPCFromFlash(PSTR("Mode 1 pressed"));
      TX_NEWLINE_PC;
      if(!UI_MODE_SELECTED)
      {
        UI_Selected_Mode++;
        if(UI_Selected_Mode>Number_of_modes)
        {
          UI_Selected_Mode--;
          UI_Current_Mode=UI_Modes[UI_Selected_Mode-1];
        }
        else
        {
          UI_Current_Mode=UI_Modes[UI_Selected_Mode-1];
          VS1053_SKIP_PLAY=true;
          UI_Play_Intro_Currentmode();
        }
      }
      break;
    case UI_CMD_MREV:
      USART_transmitStringToPCFromFlash(PSTR("Mode 2 pressed"));
      TX_NEWLINE_PC;
      if(!UI_MODE_SELECTED)
      {
        UI_Selected_Mode--;
        if(UI_Selected_Mode<1)
        {
          UI_Selected_Mode=1;
          UI_Current_Mode=UI_Modes[UI_Selected_Mode-1];
        }
        else
        {
          UI_Current_Mode=UI_Modes[UI_Selected_Mode-1];
          VS1053_SKIP_PLAY=true;
          UI_Play_Intro_Currentmode();
        }  
      }    
      break;
    case UI_CMD_VOLU:
      USART_transmitStringToPCFromFlash(PSTR("Vol UP pressed"));
      TX_NEWLINE_PC;
      VS1053_IncreaseVol();
      break;
    case UI_CMD_VOLD:
      USART_transmitStringToPCFromFlash(PSTR("Vol DOWN pressed"));
      TX_NEWLINE_PC;
      VS1053_DecreaseVol();
      break;
    default:
      break;
  }
}

/**
 * @brief play the MP3 associated with the current mode
 * @return Void
 */
void UI_Play_Intro_Currentmode(void)
{
  switch(UI_Current_Mode)
  {
    case 1:
      RequestToPlayMP3file("MD1.MP3");
      break;
    case 2:
      RequestToPlayMP3file("MD2.MP3");
      break;
    case 3:
      RequestToPlayMP3file("MD3.MP3");
      break;
    default:
      break;
      //RequestToPlayMP3file("ERR1.MP3");
  }
}

/**
 * @brief (?)
 * @return Void
 */
void UI_CallModeYesAnswer(void)
{
  switch(UI_Current_Mode)
  {
    case 1:
      MD1_CallModeYesAnswer();
      break;
    case 2:
      MD2_CallModeYesAnswer();
      break;
    case 3:
      MD3_CallModeYesAnswer();
      break;
    default:
      break;
  }
}

/**
 * @brief (?)
 * @return Void
 */
void UI_CallModeNoAnswer(void)
{
  switch(UI_Current_Mode)
  {
    case 1:
      MD1_CallModeNoAnswer();
      break;
    case 2:
      MD2_CallModeNoAnswer();
      break;
    case 3:
      MD3_CallModeNoAnswer();
      break;
    default:
      break;
  }
}

/**
 * @brief (?)
 * @return Void
 */
void UI_InputDotToCurrentMode(char thisDot)
{
  switch(UI_Current_Mode)
  {
    case 1:
      MD1_InputDot(thisDot);
      break;
    case 2:
      MD2_InputDot(thisDot);
      break;
    case 3:
      MD3_InputDot(thisDot);
      break;
    default:
      break;
  }
}

/**
 * @brief (?)
 * @return Void
 */
void UI_InputCellToCurrentMode(char thisCell)
{
  switch(UI_Current_Mode)
  {
    case 1:
      MD1_InputCell(thisCell);
      break;
    case 2:
      MD2_InputCell(thisCell);
      break;
    case 3:
      MD3_InputCell(thisCell);
      break;
    default:
      break;
  }
}

/**
 * @brief   Decides which of the three UI modes to go into based on UI_Current_Mode
 * @return  Void
 */
void UI_RunMainOfCurrentMode(void)
{
  if(UI_MODE_SELECTED){
    switch(UI_Current_Mode)
    {
      case 1:
        MD1_Main();
        break;
      case 2:
        MD2_Main();
        break;
      case 3:
        MD3_Main();
        break;
      default:
        break;
    }
  }
}

/**
 * @brief (?)
 * @return Void
 */
void UI_ResetTheCurrentMode(void)
{
  if(UI_MODE_SELECTED){
    switch(UI_Current_Mode)
    {
      case 1:
        MD1_Reset();
        break;
      case 2:
        MD2_Reset();
        break;
      case 3:
        MD3_Reset();
        break;
      default:
        break;
    }
  }
}

/*
void RequestToPlayMP3file(const char* thisFile)
{
  int i=0;
  while(*thisFile!='.')
  {
    fileName[i++]=*(thisFile++);
  }
  fileName[i++]=*(thisFile++);
  fileName[i++]=*(thisFile++);
  fileName[i++]=*(thisFile++);
  fileName[i++]=*(thisFile++);
  UI_MP3_file_Pending=true;
}
*/
