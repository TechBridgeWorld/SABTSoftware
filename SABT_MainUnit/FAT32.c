/**
 * @file FAT32.c
 * @brief Reads data from File Allocation Table 32 (FAT 32) format.
 *        This system defines clusters, which are contiguous regions of memory.
 *        Can walk down clusters for each file (clustor is a group of sectors)
 *        and each cluster points to next cluster or end of file 
 * @ref http://en.wikipedia.org/wiki/File_Allocation_Table#Logical_sectored_FAT
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "ui_handle.h"

/**
 * @brief Reads in data data from boot sector and
 * checks to make sure that it is the FAT32 standard
 * @return unsigned char - return 0 on successful read
 *                         return 1 on error of file system or if not read properly
 */
unsigned char get_boot_sector_data (void) {
    struct BS_Structure *bpb; //mapping the buffer onto the structure
    struct MBRinfo_Structure *mbr;
    struct partitionInfo_Structure *partition;
    unsigned long data_sectors;

    unused_sectors = 0;

    sd_read_single_block(0);
    bpb = (struct BS_Structure *)buffer;

    if (bpb->jump_boot[0]!=0xE9 && bpb->jump_boot[0]!=0xEB) {   //check if it is boot sector
        mbr = (struct MBRinfo_Structure *) buffer;              //if it is not boot sector, it must be MBR

        if(mbr->signature != 0xaa55) return 1;                  //if it is not even MBR then it's not FAT32

        partition = (struct partitionInfo_Structure *)(mbr->partition_data);  //first partition
        unused_sectors = partition->first_sector;                             //the unused sectors, hidden to the FAT

        sd_read_single_block(partition->first_sector);          //read the bpb sector
        bpb = (struct BS_Structure *)buffer;
        if(bpb->jump_boot[0]!=0xE9 && bpb->jump_boot[0]!=0xEB) return 1; 
    }

    bytes_per_sector = bpb->bytes_per_sector;
    sector_per_cluster = bpb->sector_per_cluster;
    reserved_sector_count = bpb->reserved_sector_count;
    root_cluster = bpb->root_cluster;
    first_data_sector = bpb->hidden_sectors + reserved_sector_count 
        + (bpb->number_of_fats * bpb->fat_size_f32);

    data_sectors = bpb->total_sectors_f32
        - bpb->reserved_sector_count
        - ( bpb->number_of_fats * bpb->fat_size_f32);
    total_clusters = data_sectors / sector_per_cluster;

    // Check if FSinfo free clusters count is valid
    if((get_set_free_cluster (TOTAL_FREE, GET, 0)) > total_clusters)  
        free_cluster_count_updated = 0;
    else
        free_cluster_count_updated = 1;
    return 0;
}


/**
 * @brief calculates the first sector address of any given cluster
 * @param cluster_number - unsigned long, cluster number on which first sector found
 * @return unsgined long - first secotr address
 */
unsigned long get_first_sector(unsigned long cluster_number) {
    return (((cluster_number - 2) * sector_per_cluster) + first_data_sector);
}


/**
 * @brief get cluster entry value from FAT to find out the next cluster in chain
 * @param cluster_number - unsinged long, current cluster number
 * @param get_set - unsigned char, get - if next sector is to be got, and set if 
 *                  next sector is to be set
 * @param cluster_entry - unsigned long, next cluster number if arg2 = Set, else = 0
 * @return unsinged long - next cluster number if arg2 = get, else 0
 */
unsigned long get_set_next_cluster (unsigned long cluster_number,
        unsigned char get_set,
        unsigned long cluster_entry) {
    unsigned int fat_entry_offset;
    unsigned long *fat_entry_value, fat_entry_sector;
    unsigned char retry = 0;

    // Get sector number of the cluster entry in the FAT
    fat_entry_sector = unused_sectors + reserved_sector_count 
        + ((cluster_number * 4) / bytes_per_sector);

    // Get the offset address in that sector number
    fat_entry_offset = (unsigned int) ((cluster_number * 4) % bytes_per_sector);

    // Read the sector into a buffer
    while(retry < 10) {
        if(!sd_read_single_block(fat_entry_sector)) break; 
        retry++;
    }

    // Get the cluster address from the buffer
    fat_entry_value = (unsigned long *) &buffer[fat_entry_offset];

    if(get_set == GET) return ((*fat_entry_value) & 0x0fffffff);

    // For setting new value in cluster entry in FAT
    *fat_entry_value = cluster_entry;

    sd_write_single_block(fat_entry_sector);

    return 0;
}


/**
 * @brief Function is used to get or set next free cluster or total free clusters
 *        in FSinfo sector of SD card
 * @param tot_or_next - unsigned char, can be either TOTAL_FREE or NEXT_FREE
 * @param get_set - unsigned char, flag that can be GET or SEt
 * @param fs_entry - unsigned long, new FS entry when arg2 is SET or 0 when GET
 * @return unsigned long - next free cluster if arg1 is NEXT_FREE and arg2 is GET
 *                       total # of free clusters if arg1 is TOTAL_FREE and arg2
 *                       GET.  0xffffffff if any error or arg2 is Set
 */
unsigned long get_set_free_cluster(unsigned char tot_or_next, 
        unsigned char get_set, 
        unsigned long fs_entry) {
    struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &buffer;

    sd_read_single_block(unused_sectors + 1);

    if ((FS->lead_signature != 0x41615252) || (FS->structure_signature != 0x61417272) 
        || (FS->trail_signature !=0xaa550000))
        return 0xffffffff;

    if(get_set == GET) {
        if (tot_or_next == TOTAL_FREE)
            return (FS->free_cluster_count);
        else
            return(FS->next_free_cluster);
    }
    else {
        if (tot_or_next == TOTAL_FREE)
            FS->free_cluster_count = fs_entry;
        else // when tot_or_next = NEXT_FREE
            FS->next_free_cluster = fs_entry;
    }
    return 0xffffffff;
}

/**
 * @brief function gets DIR/FILE list or a single file address or deletes
 *         specified file.  Will find starting location for the file.
 *         Globals used in this function appendFileSector, 
 *         appendFileLocation, appendStartCluster, file_size
 * @param flag - unsigned char, can be GET_LIST, GET_FILE or DELETE
 * @param file_name - unsinged char *, pointer to the file name to operate on
 * @return struct dir_Structure * - first cluster of file if flag = GET_FILE
 *         print file/dir list if flag = GET_LIST. Delete file if flag = DELETE
 */
struct dir_Structure* find_files (unsigned char flag, unsigned char *file_name) {
    struct dir_Structure *dir;
    unsigned long cluster, sector, first_sector, first_cluster, next_cluster;
    unsigned int i;
    unsigned char j;

    cluster = root_cluster; //root cluster

    while(1) {
        first_sector = get_first_sector (cluster);

        for (sector = 0; sector < sector_per_cluster; sector++) {
            sd_read_single_block(first_sector + sector);

            for(i = 0; i < bytes_per_sector; i += 32) {
                dir = (struct dir_Structure *) &buffer[i];
                if (dir->name[0] == EMPTY) { //indicates end of the file list of the directory
                    if ((flag == GET_FILE) || (flag == DELETE))
                        usart_transmit_string_to_pc_from_flash(PSTR("[FAT32] File does not exist\n\r"));
                    return 0;   
                }
                if ((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME)) {
                    if ((flag == GET_FILE) || (flag == DELETE)) {
                        //loop over the name[] array till you do not see a match,(DID NOT match)
                        for (j = 0; j < 11; j++)
                            if (dir->name[j] != file_name[j])
                                break;
                            if(j == 11) {
                            if(flag == GET_FILE) {
                                append_file_sector = first_sector + sector;
                                append_file_location = i;
                                append_start_cluster = (((unsigned long) dir->first_cluster_hi) << 16) 
                                    | dir->first_cluster_lo;
                                file_size = dir->file_size;
                                return (dir);
                            }  
                            else {   //when flag = DELETE
                                TX_NEWLINE_PC;
                                usart_transmit_string_to_pc_from_flash(PSTR("Deleting.."));
                                TX_NEWLINE_PC;
                                TX_NEWLINE_PC;
                                first_cluster = (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;

                                //mark file as 'deleted' in FAT table
                                dir->name[0] = DELETED;    
                                sd_write_single_block (first_sector+sector);

                                free_memory_update (ADD, dir->file_size);

                                //update next free cluster entry in FSinfo sector
                                cluster = get_set_free_cluster (NEXT_FREE, GET, 0); 
                                if (first_cluster < cluster)
                                    get_set_free_cluster (NEXT_FREE, SET, first_cluster);

                                //mark all the clusters allocated to the file as 'free'
                                while(1) {
                                    next_cluster = get_set_next_cluster (first_cluster, GET, 0);
                                    get_set_next_cluster (first_cluster, SET, 0);
                                    if(next_cluster > 0x0ffffff6) 
                                    {usart_transmit_string_to_pc_from_flash(PSTR("File deleted!"));return 0;}
                                    first_cluster = next_cluster;
                                } 
                            }
                        }
                    }
                    else { //when flag = GET_LIST
                        TX_NEWLINE_PC;
                        for (j = 0; j < 11; j++) {
                            if (j == 8)
                                usart_transmit_byte_to_pc(' ');
                            usart_transmit_byte_to_pc (dir->name[j]);
                        }
                        usart_transmit_string_to_pc_from_flash (PSTR("   "));
                        if((dir->attrib != 0x10) && (dir->attrib != 0x08)) {
                            usart_transmit_string_to_pc_from_flash (PSTR("FILE" ));
                            usart_transmit_string_to_pc_from_flash (PSTR("   "));
                            display_memory (LOW, dir->file_size);
                        }
                        else
                            usart_transmit_string_to_pc_from_flash ((dir->attrib == 0x10)? PSTR("DIR") : PSTR("ROOT"));
                    }
                }
            }
        }

        cluster = (get_set_next_cluster (cluster, GET, 0));

        //out of bonds?? TODO - look at
        if (cluster > 0x0ffffff6)
            return 0;
        if (cluster == 0) {
            usart_transmit_string_to_pc_from_flash(PSTR("Error in getting cluster"));
            return 0;
        }
    }
    return 0;
}


/**
 * @brief  Can act as a reader or verifier depending on the flag sent in in flag
 *         If it is VERIFY- just wil let you know if the file is there.  If it is 
 *         read then it actually reads in the contents of the file. 
 * @param  flag - unsigned char, flag=READ then to read file from SD card and send 
 *                contents to UART. if flag=VERIFY then functions will verify 
 *                whether a specified file is already existing
 * @param  file_name - unsigned char *, the file you are reading or verifying
 * @return char - returns 0 on success
 *                returns 1 if files already exists and flag = Verify
 *                returns 2 on error convertingFileName
 */
unsigned char read_file (unsigned char flag, unsigned char *file_name) {
    struct dir_Structure *dir;
    unsigned long cluster, byteCounter = 0, file_size, first_sector;
    unsigned int k;
    unsigned char j, error;
    bool end_of_file = false;

    error = convert_file_name (file_name); //convert file_name into FAT format
    if (error)
        return 2;

    dir = find_files (GET_FILE, file_name); //get the file location
    if (dir == 0) 
        return 0;

    if (flag == VERIFY)
        return 1;  //specified file name is already existing

    cluster = (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;

    file_size = dir->file_size;

    while(1) {
        first_sector = get_first_sector (cluster);

        for (j = 0; j < sector_per_cluster; j++) {
            sd_read_single_block(first_sector + j);

            for(k = 0; k < 512; k++)
                if ((byteCounter++) >= file_size ) end_of_file = true;

            if(end_of_file)
                return 0;
        }

        cluster = get_set_next_cluster (cluster, GET, 0);
        if (cluster == 0) {
            usart_transmit_string_to_pc_from_flash(PSTR("Error in getting cluster"));
            return 0;
        }
    }
    return 0;
}

/**
 * @brief This file just reads in data from the given file_name if it can find and 
 *         and process the file.
 * @param file_name    char *   That contains the file name of what you want to read
 data_string  char *   Where the data is put, can store 100 bytes
 * @return unsigned char - number where 0 means success
 *                         1 means that you could not convert file_name
 *                         2 means file does not exists
 *                         3 will mean inability to read a cluster
 */
unsigned char read_and_retrieve_file_contents(unsigned char *file_name, unsigned char *data_string) {
    struct dir_Structure *dir;
    unsigned long cluster, byteCounter = 0, file_size, first_sector;
    unsigned int k;
    unsigned char j, error;
    unsigned int num_bytes_read = 0;
    bool end_of_file = false;

    error = convert_file_name (file_name);    //convert file_name into FAT format
    if(error) {
        TX_NEWLINE_PC;
        return 1;
    }

    dir = find_files (GET_FILE, file_name);   //get the file location
    if (dir == 0) {
        TX_NEWLINE_PC;
        return 2;
    }

    cluster = (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;

    file_size = dir->file_size;

    while(1) {
        first_sector = get_first_sector (cluster);

        for (j = 0; j < sector_per_cluster; j++) {
            sd_read_single_block(first_sector + j);


            for(k = 0; k < 512; k++)
                if ((byteCounter++) >= file_size ) end_of_file=true;
            while(num_bytes_read < k) {
                *data_string++ = buffer[num_bytes_read];
                if(num_bytes_read++ == k)return 0;  
            }
            if(end_of_file)
                return 0;
        }
        cluster = get_set_next_cluster (cluster, GET, 0);
        if (cluster == 0)  {
            TX_NEWLINE_PC;
            usart_transmit_string_to_pc_from_flash(PSTR("Error in getting cluster")); 
            return 3;
        }
    }

    return 4;
}


/**
 * @brief  This function plays a given MP3 files, until:
 *          1. The files reach the end of file
 *          2. Stop playing command issued from the controller
 * @param file_name - unsighed char *, simply name of the file to operate on
 * @return unsigned char - return 0 on success
 *                         return 2 on error converting file_name
 */

unsigned char play_mp3_file(unsigned char *file_name){
    struct dir_Structure *dir;
    unsigned long cluster, byteCounter = 0, file_size, first_sector;
    unsigned int k,iCntForSingleAudioWrite;
    unsigned char j, error;
    unsigned int iAudioByteCnt;
    bool end_of_file=false;

    uint16_t calc_crc, msg_crc;
    uint8_t msg_len;

    playing_sound = true;

    error = convert_file_name (file_name); //convert file_name into FAT format
    if (error)
        return 2;

    dir = find_files (GET_FILE, file_name); //get the file location
    if (dir == 0) 
        return (0);

    cluster = (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;

    file_size = dir->file_size;
    vs1053_skip_play=false;
    while(1) {
        first_sector = get_first_sector (cluster);

        for(j = 0; j < sector_per_cluster; j++) {
            sd_read_single_block(first_sector + j);

            //for all sectors in this cluster, read in their data (512 bytes)
            for (k = 0; k < 512; k++)
                if ((byteCounter++) >= file_size ) end_of_file=true;

            //After reading each sector in the file --> send them to MP3 decoder in 32 byte segments
            iAudioByteCnt = 0;
            while(iAudioByteCnt<k) {
                if (vs1053_skip_play) {
                    vs1053_skip_play = false;
                    vs1053_software_reset();
                    playing_sound = false;
                    return 0; //playing stopped by user
                }
                if ((PINB & (1<<MP3_DREQ))) {
                    for (iCntForSingleAudioWrite = 0; iCntForSingleAudioWrite < 32;
                        iCntForSingleAudioWrite++) {
                        vs1053_write_data(buffer[iAudioByteCnt]);
                        if(iAudioByteCnt++==k)return 0;    
                    }  
                }

                /*
                // KORY CHANGED
                // After playing a 32 bytes of data, check the user inputs
                if(usart_keypad_data_ready)
                {
                    log_msg("[IO] Keypad interrupt received\n\r");
                    usart_keypad_receive_action();
                }
                if(usart_pc_data_ready)
                {
                    log_msg("[IO] PC interrupt received\n\r");
                    usart_pc_receive_action();
                }    
                if(usart_ui_message_ready) //If a message ready from the user interface, process it
                {
                    log_msg("[IO] Processing keypad interrupt\n\r");
                    ui_parse_message(playing_sound);
                }
                */

                if (usart_keypad_data_ready) {
                    // Process interrupts
                    usart_keypad_receive_action();
                }

                if (usart_ui_message_ready) {
                    msg_len = usart_ui_received_packet[2];
                    calc_crc = ui_calculate_crc((unsigned char*)usart_ui_received_packet);
                    msg_crc = usart_ui_received_packet[msg_len - 2] << 8 | 
                                            usart_ui_received_packet[msg_len - 1];
                    if (msg_crc == calc_crc) {
                        switch (usart_ui_received_packet[5]) {
                            // Deal with volume here
                            case UI_CMD_VOLU:
                                vs1053_increase_vol();
                                usart_ui_message_ready = false;
                                break;
                            case UI_CMD_VOLD:
                                vs1053_decrease_vol();
                                usart_ui_message_ready = false;
                                break;
                            default:
                                // Handle in main loop
                                playing_sound = 0;
                                return 0;
                                break;
                        }
                    }
                    else {
                        log_msg("[IO] CRC failed on interrupt");
                        usart_ui_message_ready = false;
                    }
                }
            }

            if(end_of_file) { 
                playing_sound = false;
                return 0;
            }

        }

        cluster = get_set_next_cluster (cluster, GET, 0);
        if (cluster == 0) {
            playing_sound = false;
            usart_transmit_string_to_pc_from_flash(PSTR("Error in getting cluster")); 
            return 0;
        }
    }

    playing_sound = false;
    return 0;
}

/**
 * @brief  This function plays a beep from EEPROM files, until:
 *          1. The files reach the end of file
 *          2. Stop playing command issued from the controller
 * @return unsigned char - return 0 on success
 */

unsigned char play_beep(){

    unsigned int iAudioByteCnt;

    unsigned char StringOfData[] = {0xff ,0xe3 ,0x18 ,0xc4 ,0x00 ,0x0b ,0x70 ,0x92 ,0xdd ,0x19 ,0x46 ,0x00 ,0x02 ,0xe0 ,0x89 ,0x24,
    0x80 ,0x0b ,0xbb ,0xbb ,0xbb ,0xbf ,0x11 ,0x11 ,0x11 ,0x11 ,0xff ,0xef ,0x7f ,0xff ,0x9c ,0xf0,
    0x70 ,0x18 ,0x0c ,0x06 ,0x4e ,0xdc ,0x20 ,0x04 ,0x3f ,0x3e ,0x50 ,0x10 ,0x94 ,0x77 ,0x7f ,0x02,
    0x3b ,0x28 ,0x0f ,0xf0 ,0x70 ,0xe6 ,0x87 ,0xf4 ,0x78 ,0x7f ,0xa1 ,0x73 ,0xe5 ,0xe3 ,0x5d ,0x15,
    0x79 ,0x89 ,0x44 ,0x86 ,0x0f ,0xc9 ,0x24 ,0xa3 ,0xff ,0xe3 ,0x18 ,0xc4 ,0x0d ,0x0f ,0xc0 ,0xde,
    0xd8 ,0x01 ,0x93 ,0x58 ,0x00 ,0x2f ,0x19 ,0xe1 ,0xba ,0x08 ,0xc8 ,0x0a ,0x18 ,0x81 ,0x17 ,0x46,
    0x38 ,0x59 ,0x5e ,0x0c ,0x10 ,0xa4 ,0xd4 ,0x94 ,0x52 ,0x0d ,0x8d ,0x9a ,0xd6 ,0xff ,0xf9 ,0xb8,
    0xba ,0x40 ,0x8e ,0x05 ,0x44 ,0x53 ,0xdd ,0x02 ,0x82 ,0x36 ,0xa4 ,0x0c ,0x4a ,0x57 ,0xf2 ,0x8e,
    0x51 ,0x76 ,0xea ,0xd7 ,0xfd ,0x0e ,0x97 ,0x4c ,0x41 ,0x4d ,0x45 ,0x33 ,0x2e ,0x39 ,0x39 ,0x2e,
    0xff ,0xe3 ,0x18 ,0xc4 ,0x09 ,0x00 ,0x00 ,0x03 ,0x48 ,0x01 ,0xc0 ,0x00 ,0x00 ,0x33 ,0x55 ,0x55,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55}; //generated beep sequence in hex format

    playing_sound = true;

    while(1) {
 
        for (iAudioByteCnt = 0 ; iAudioByteCnt < 216 ; iAudioByteCnt ++ ){
            vs1053_write_data(StringOfData[iAudioByteCnt]);    
            usart_transmit_string_to_pc_from_flash (PSTR("Transmitting Beep"));
        }

        iAudioByteCnt = 0;    //reset byte count to zero after each beep
        if (!sd_init()){ //try to initialize audio card after every beep
            playing_sound = false;
            return 0;
        }
    }  
    return 0;
}


/**
 * @brief This function will set up the initial values for the read_dict_file
 * @param file_name - unsigned char *, name of the file we are trying to find
 * @return unsigned char - status of trying to read
 */
unsigned char init_read_dict(unsigned char *file_name){
    unsigned char error;
    struct dir_Structure *dir;

    //@TODO - 300 only works for the current dictionary -need to make different / better
    dict_clusters = calloc(MAX_NUM_CLUSTERS,sizeof(unsigned long));
    dict_cluster_cnt = 0;

    preceeding_word = calloc(MAX_NUM_CLUSTERS, sizeof(char));
    preceeding_word[0] = 0;


    error = convert_dict_file_name (file_name); //convert file_name into FAT format
    if (error)
        return 2;

    dir = find_files(GET_FILE, dict_file_name); //get the file location
    dict_dir = dir;

    if (dir == 0)
        return (0);

    curr_dict_cluster = (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;
    done_rd_dict = false;

    return 0;

}



/**
 * @brief This function will take in a file_name and read the contents 
 *        of this file.  It will also populate the clusters array with a pointer
 *        to each cluster.  It will work ten clusters at a time and finish. when get to end.
 *        dict_file_name - is global that stores FAT32 format name of the dicionary (corpus)
 * @param file_name - unsigned char *, name of the file we are trying to find
 * @return unsigned char - status of trying to read
 */
unsigned char read_dict_file()
{
    unsigned long cluster, first_sector;
    unsigned char j;
    unsigned long i, k;
    bool end_of_file = false;
    int byte_counter = 0;
    int count = 0;
    char curr_char = 0;
    //clusters needs to be an array - I am not sure of the correct size, so will start
    //at 4096
    //@TODO   MUST FREE SOMEWHERE
    // READ in 281 clusters

    cluster = curr_dict_cluster;
    for (k = 0; k < CLUSTERS_PER_RUN; k++) { //60
        dict_clusters[dict_cluster_cnt] = cluster;
        dict_cluster_cnt ++;
        first_sector = get_first_sector(cluster);

        //PUT ++++ at end of file to make sure we were at the end.  
        for (j = 0; j < sector_per_cluster; j++) {
            sd_read_single_dict_block(first_sector + j);
            for(i = 0; i < BUFFER_SIZE; i ++) { //512
                if(dict_buffer[i] =='+')
                    end_of_file = true;
            }

            //last charachter in this buffer
            curr_char = dict_buffer[BUFFER_SIZE - 1];
            //number of bytes read at each sector
            byte_counter += BUFFER_SIZE;

            if(end_of_file) {
                done_rd_dict = true;
                log_msg("read in dictionary");
                TX_NEWLINE_PC;
                return 0;
            }

        }
        count ++; //number of sectors read

        //if the buffer ends in a \n, then this cluster starts on its own word
        if (curr_char == '\n')
            preceeding_word[dict_cluster_cnt] = 0;
        else
            preceeding_word[dict_cluster_cnt] = 1;

        cluster = get_set_next_cluster (cluster, GET, 0);
        curr_dict_cluster = cluster;
        if (cluster == 0) {
            usart_transmit_string_to_pc_from_flash(PSTR("Error in getting cluster")); 
            return 0;
        }
    }
    return 0;
}


/**
 * @brief This function will find the word in the dictionary file
 *        This will find a word accross multiple clusters/sectors.
 *        In a dictionary file, words are seperated by '/n'
 *        dict_file_name - global that stores the converted FAT32
 *           format file_name after initialization
 * @param file_name - unsigned char *, file for dictionary look up
 * @param word - unsigned char *, word you are trying to find 
 * @return bool - returns whether or not you have found word
 * @TODO - NEED TO FIX WORD - IT IS GETTTING CORRUPTED
 * @TODO - NEED TO TAKE EDGE CASES OFF of CLUSTERS- WORDS that overlap clusters
 */
bool bin_srch_dict(unsigned char *word) {
    bool found = false;
    int cluster_cnt = dict_cluster_cnt;
    unsigned long curr_cluster, first_sector;
    int hi = cluster_cnt - 1;
    int lo = 0;
    int cmp_wrd = 0;
    int mid;
    struct dir_Structure *dir;

    dir = dict_dir;
    if (dir == 0)
        return (0);

    file_size = dir->file_size;


    //search for the cluster that contains the word
    while((hi - lo) > 1) {

        mid = (hi + lo) / 2;
        curr_cluster = dict_clusters[mid];
        first_sector = get_first_sector (curr_cluster);

        //store these values into the buffer array 
        sd_read_single_dict_block(first_sector);

        //this should return 0 for found, 1 for less then first, 2 for greater then first
        //2nd argument tells whether or last word in cluster crosses into this cluster
        cmp_wrd = check_first_full_word(word, preceeding_word[mid]);

        if(cmp_wrd == 0) {
            found = true;
            break;
        }

        else if (cmp_wrd == 1)
            hi = mid - 1;

        else if (cmp_wrd == 2)
            lo = mid;

        //if you get any other return value, you know that it is wrong. 
        else
            return false;
    }



    //if you have narrowed it down to the sector that is pointed at by lo
    if (found == false) {
        if (find_word_in_cluster(word, lo))
            return true;
        log_msg("Done with first.");        

        if(find_word_in_cluster(word, hi))
            return true;

        log_msg("Done with second."); 
        //if you get here, that means that you did not have find the word in any part of the cluster
        //it should be in
        return false;
    }

    //this must mean you found it while doing your work in bin search part.  
    return true;
}

/**
 * @brief This should compare/find word in a cluster
 * @param word - unsigned char *, word to compare with the first word
 * @param arr_cluster_index - unsigned long, this is the number of the cluster you
 *        are searching in
 * @return bool - return whether or not you found value in cluster
 */
bool find_word_in_cluster(unsigned char *word, unsigned long arr_cluster_index) {
    unsigned long word_index, sector_index; // index into word, sector
    unsigned long cluster_index;  // Index into cluster

    // First, find out what cluster we need
    // uses global: dict_clusters
    unsigned long first_sector = get_first_sector(dict_clusters[arr_cluster_index]);
    unsigned char* sector_pointer = (unsigned char*)dict_buffer; // Start at the beginning of the buffer
    char overlap = preceeding_word[arr_cluster_index];

    // Read in the first sector to 'buffer'
    // uses global: buffer
    sd_read_single_dict_block(first_sector);

    sector_index = 0;
    word_index = 0;
    // Determine where we should begin scanning
    if (overlap == 1) {
        while (sector_pointer[sector_index] != '\n') sector_index++;
        sector_index++; // Start after newline
    }

    // Loop through all of the sectors in this cluster searching for word
    for (cluster_index = 0; cluster_index < sector_per_cluster; cluster_index++) {
        // Repopulate the buffer with the next sector
        sd_read_single_dict_block(first_sector + cluster_index);



        // Reset the sector index if we overflowed last time
        // We want to avoid resetting after calculating the first word the first time,
        // or if we didn't get to the end of the sector for some other reason
        if (sector_index >= BUFFER_SIZE) sector_index = 0;

        while (sector_index < BUFFER_SIZE) {
            // Check to see if we've successfully found the word
            if (word[word_index] == '\0' && sector_pointer[sector_index] == '\r')
                return true;
            // If we get to the end of the word in any other way, abort
            else if (word[word_index] == '\0')
                return false;
            // Otherwise, check to see if this is a possible match
            else if (word[word_index] == sector_pointer[sector_index]) {
                word_index++;
                sector_index++;
            }
            // Otherwise, the word is not a possible match. Advance until we can try again.
            else {
                word_index = 0;
                while(sector_pointer[sector_index] != '\n' && sector_index < BUFFER_SIZE)
                    sector_index++;
                sector_index++; // skip the '\n' before a new word
            }
        }
    }


    //now we need to check if our word continues on to the next cluster, because overlapping words are our
    //responsibility
    //if we are not the last cluster 
    if (arr_cluster_index != (dict_cluster_cnt -1)) {
        first_sector = get_first_sector(dict_clusters[arr_cluster_index + 1]);
        sd_read_single_dict_block(first_sector);
        sector_index = 0;

        while (sector_index < BUFFER_SIZE) {
            // Check to see if we've successfully found the word
            //@TODO - ALEX, need to change this back to '/0'
            if (word[word_index] == '\0' && sector_pointer[sector_index] == '\r')
                return true;
            // If we get to the end of the word in any other way, abort
            else if (word[word_index] == '\0')
                return false;
            // Otherwise, check to see if this is a possible match
            else if (word[word_index] == sector_pointer[sector_index]) {
                word_index++;
                sector_index++;
            }
            // Otherwise, the word is not a possible match. return false;
            else
                return false;
        }
    }

    // If we went throught the entire cluster and couldn't find the word, word not in cluster
    return false;
}



/**
 * @brief This should compare/find word with the fist word found in buffer
 * @param word - unsigned char *, word to compare with the first word
 * @param overlap - char, 1 if word from previous cluster overlaps with this one
 *                  0 if word from previous cluster does not overlap
 * @return int - 0 if word is same then first word in buffer
 *               1 if word is less then first word in buffer
 *               2 if word is greater then first word in buffer 
 *               -1 error
 */
int check_first_full_word(unsigned char *word, char overlap) {
    int i;
    unsigned char *first_word;
    if (overlap == 1){
        first_word = (unsigned char *)&dict_buffer[0];
        int i = 0;
        //find the start of the first word
        while (i < BUFFER_SIZE){
            if(dict_buffer[i] == '\n') {
                first_word = (unsigned char *)&dict_buffer[i+1];
                break;
            }
            i++;
        }
    }

    else
        first_word = (unsigned char *)&dict_buffer[0];

    i = 0;
    while(1) {
        //if word is greater then first word
        if (word[i] > first_word[i])
            return 2;
        //if word is less then first word
        else if (word[i] < first_word[i])
            return 1;
        //if both words are terminated, null terminated by word and newline for firstword
        else if ((word[i] == 0)  && (first_word[i] == '\n'))
            return 0;

        i++;
    }

    //if you got here, it is an error
    return -1;

}





//***************************************************************************
//Function: to convert normal short file name into FAT format
//Arguments: pointer to the file name
//return: 0, if successful else 1.
//***************************************************************************

/**
 * @brief Converts the input file_name (which is in FAT format) in the following fashion:
 *        <file_name.ext> -----> <file_name[padding to 8 chars]ext>
 *        file_name must be <= 8 chars and ext must be <= 3 chars.
 *        Thus, INT.MP3 becomes [INT     MP3]. Also, capitalizes lowercase files.
 * @param file_name unsigned char* string which contains the file name that needs to be converted
 * @return unsigned char 1 for failure 0 for victory
 * @TODO Currently overwrites the passed in buffer so if you pass in the same buffer twice, it 
 * will have already converted the file_name (specifically removed the '.' which is what the invalid
 * file check is done off of)
 */ 
unsigned char convert_file_name (unsigned char *file_name)
{
    unsigned char file_name_fat[FILE_NAME_LEN];
    unsigned char j, k;

    for(j = 0; j < FILE_NAME_LEN; j++) {
        if (file_name[j] == '.') 
            break;
    }

    if (j >= FILE_NAME_LEN)
        // assume that a string without any dots is already converted
        return 0;

    // 1 = BAD_EXTENSION
    if (j > 8) {
        log_msg("Invalid file name: $s", file_name);
        return 1;
    }

    for(k = 0; k < j; k++) //setting file name
        file_name_fat[k] = file_name[k];

    for(; k <= 7; k++) //filling file name trail with blanks
        file_name_fat[k] = ' ';

    j++;

    for(k = 8; k < FILE_NAME_LEN; k++) { //setting file extention
        if(file_name[j] != 0)
            file_name_fat[k] = file_name[j++];
        else { //end filename
            file_name_fat[k] = 0;
            break;
        }
    }

    for (j = 0; j < FILE_NAME_LEN; j++) { //converting small letters to caps
        if((file_name_fat[j] >= 0x61) && (file_name_fat[j] <= 0x7a))
            file_name_fat[j] -= 0x20;
    }


    for (j = 0; j < FILE_NAME_LEN; j++)
        file_name[j] = file_name_fat[j];

    return 0;
}




/**
 * @brief Converts the input file_name (which is in FAT format) in the following fashion:
 *        <file_name.ext> -----> <file_name[padding to 8 chars]ext>
 *        file_name must be <= 8 chars and ext must be <= 3 chars.
 *        Thus, INT.MP3 becomes [INT     MP3]. Also, capitalizes lowercase files.
 *        NEED TO USE THIS FOR DICTIONARY FILES
 * @param file_name unsigned char* string which contains the file name that needs to be converted
 * @return unsigned char 1 for failure 0 for victory
 * @TODO Currently overwrites the passed in buffer so if you pass in the same buffer twice, it 
 * will have already converted the file_name (specifically removed the '.' which is what the invalid
 * file check is done off of)
 */ 
unsigned char convert_dict_file_name (unsigned char *file_name) {
    unsigned char file_name_dict_fat[FILE_NAME_LEN];
    unsigned char j, k;

    for (j = 0; j < FILE_NAME_LEN; j++) {
        if(file_name[j] == '.')
            break;
    }


    if (j >= FILE_NAME_LEN)
        // assume that a string without any dots is already converted
        return 0;

    // 1 = BAD_EXTENSION
    if (j > 8) {
        usart_transmit_string_to_pc_from_flash(PSTR("Invalid file_name.")); 
        return 1;
    }

    for(k = 0; k < j; k++) //setting file name
        file_name_dict_fat[k] = file_name[k];

    for(k = j; k <= 7; k++) //filling file name trail with blanks
        file_name_dict_fat[k] = ' ';

    j++;

    for(k = 8; k < FILE_NAME_LEN; k++) { //setting file extention
        if (file_name[j] != 0)
            file_name_dict_fat[k] = file_name[j++];
        else { //filling extension trail with blanks
            file_name_dict_fat[k] = file_name[j];
            break;
        }
    }

    for(j = 0; j < FILE_NAME_LEN; j++) { //converting small letters to caps
        if((file_name_dict_fat[j] >= 0x61) && (file_name_dict_fat[j] <= 0x7a))
            file_name_dict_fat[j] -= 0x20;
    }


    for (j = 0; j < FILE_NAME_LEN; j++)
        dict_file_name[j] = file_name_dict_fat[j];

    return 0;
}

/**
 * @brief  reads the data in file_content into buffer and uses the function
 *         sd_write_single_block() to put the value in buffer onto the SD card
 *         Data you are writing in MUST END IN $. If not garbage will be written in. 
 * @param  file_name - unsigned char *, This contains the file to replace data in
 * @param  file_content - unsighed char *, What to put into file_name
 * @return int - returns 0 if the file exists you are trying to overwrite
 *               returns 1 if the file does not exist
 */
int replace_the_contents_of_this_file_with (unsigned char *file_name, unsigned char *file_content) {
    unsigned char j;
    unsigned int i;
    struct dir_Structure *dir;
    unsigned long cluster, first_sector;

    if(read_file (VERIFY, file_name) == 1) {
        cluster = append_start_cluster;
        first_sector = get_first_sector (cluster);
        start_block = get_first_sector (cluster);
        i = 0;
        j = 0;
        while (*file_content != '$') {      
            buffer[i++]=*file_content;
            file_content++;
            if(i >= 512) {        
                i = 0;
                j++;
                if (j == sector_per_cluster) {
                    j = 0;
                    break;
                }
                start_block++; 
            }
        }
        if (*file_content == '$') {
            buffer[i++] = '$';
            for (; i < 512; i++)  //fill the rest of the buffer with 0x00
                buffer[i] = 0x00;
            _delay_ms(100);
        }
        sd_read_single_block(first_sector);    
        _delay_ms(100);
        dir = (struct dir_Structure *) &buffer[0]; 
        dir->file_size = file_size;
        //if we are writing the modes file - the next line rewrites stuff written in
        // above
        sd_write_single_block (first_sector);
        _delay_ms(100);
        return 0;
    }
    else return 1;
}

/**
 * @brief function creates a file in FAT32 format in the root directory if given 
 *        file name does not exists. If it already exists then append data to end
 * @param file_name - unsigned char *, this is the name of the file to write to
 * @return Void
 */
void write_file (unsigned char *file_name) {
    struct dir_Structure *dir;
    unsigned char j, data, file_created_flag = 0;
    unsigned char start = 0, append_file = 0, sector_end_flag = 0, sector = 0;
    unsigned int i, first_cluster_high, first_cluster_low;
    unsigned long cluster, next_cluster, prev_cluster;
    unsigned long first_sector, cluster_count, extraMemory;

    j = read_file(VERIFY, file_name);

    if (j == 1) {
        append_file = 1;
        cluster = append_start_cluster;
        cluster_count=0;
        while (1) {
            next_cluster = get_set_next_cluster (cluster, GET, 0);
            if (next_cluster == FAT32_EOF)
                break;
            cluster = next_cluster;
            cluster_count++;
        }

        sector = (file_size - (cluster_count * sector_per_cluster * bytes_per_sector)) 
            / bytes_per_sector; //last sector number of the last cluster of the file
        start = 1;
    }
    else if (j == 2)
        return; //invalid file name
    else {
        cluster = get_set_free_cluster (NEXT_FREE, GET, 0);
        if (cluster > total_clusters)
            cluster = root_cluster;

        cluster = search_next_free_cluster(cluster);

        // No free cluster
        if (cluster == 0)
            return;

        get_set_next_cluster(cluster, SET, FAT32_EOF);   //last cluster of the file, marked FAT32_FAT32_EOF

        first_cluster_high = (unsigned int) ((cluster & 0xffff0000) >> 16 );
        first_cluster_low = (unsigned int) ( cluster & 0x0000ffff);
        file_size = 0;
    }

    while (1) {
        if (start) {
            start = 0;
            start_block = get_first_sector (cluster) + sector;
            sd_read_single_block (start_block);
            i = file_size % bytes_per_sector;
            j = sector;
        }
        else {
            start_block = get_first_sector (cluster);
            i = 0;
            j = 0;
        }

        do {
            // Special case when the last character in previous sector was '\r'
            if (sector_end_flag == 1) {
                usart_transmit_byte_to_pc ('\n');
                buffer[i++] = '\n'; // Appending 'Line Feed (LF)' character
                file_size++;
            }

            sector_end_flag = 0;

            data = 0;         // receiveByte();
            if (data == 0x08){  //'Back Space' key pressed
                if (i != 0) {
                    i--;
                    file_size--;
                }
                continue;
            }

            buffer[i++] = data;
            file_size++;
            if (data == '\r') { //'Carriege Return (CR)' character
                if (i == 512)
                    sector_end_flag = 1;  //flag to indicate that the appended '\n' char should be put in the next sector
                else {
                    buffer[i++] = '\n'; //appending 'Line Feed (LF)' character
                    file_size++;
                }
            }

            //though 'i' will never become greater than 512, it's kept here to avoid 
            if (i >= 512) {
                i = 0;
                j++;
                if (j == sector_per_cluster) {
                    j = 0;
                    break;
                }
                start_block++;
            }
        }

        while (data != '~');

        if (data == '~') {
            file_size--;  //to remove the last entered '~' character
            i--;
            for (; i < 512; i++)  //fill the rest of the buffer with 0x00
                buffer[i]= 0x00;
            break;
        }

        prev_cluster = cluster;

        // Look for a free cluster starting from the current cluster
        cluster = search_next_free_cluster(prev_cluster);

        // No free cluster
        if (cluster == 0)
            return;

        get_set_next_cluster(prev_cluster, SET, cluster);
        get_set_next_cluster(cluster, SET, FAT32_EOF);   //last cluster of the file, marked FAT32_FAT32_EOF
    }

    get_set_free_cluster (NEXT_FREE, SET, cluster); //update FSinfo next free cluster entry

    if(append_file) { //executes this loop if file is to be appended
        sd_read_single_block (append_file_sector);    
        dir = (struct dir_Structure *) &buffer[append_file_location];
        extraMemory = file_size - dir->file_size;
        dir->file_size = file_size;
        sd_write_single_block (append_file_sector);
        free_memory_update (REMOVE, extraMemory); //updating free memory count in FSinfo sector;
        return;
    }

    // executes following portion when new file is created

    prev_cluster = root_cluster; // root cluster

    while(1) {
        first_sector = get_first_sector (prev_cluster);

        for (sector = 0; sector < sector_per_cluster; sector++) {
            sd_read_single_block (first_sector + sector);

            for(i = 0; i < bytes_per_sector; i += 32) {
                dir = (struct dir_Structure *) &buffer[i];
                if(file_created_flag){   //to mark last directory entry with 0x00 (empty) mark
                                        //indicating end of the directory file list
                    dir->name[0] = 0x00;
                    return;
                }

                // Looking for an empty slot to enter file info
                if((dir->name[0] == EMPTY) || (dir->name[0] == DELETED)) {
                    for (j = 0; j < 11; j++)
                        dir->name[j] = file_name[j];
                    dir->attrib = ATTR_ARCHIVE;       //settting file attribute as 'archive'
                    dir->nt_reserved = 0;             //always set to 0
                    dir->time_tenth = 0;              //always set to 0
                    dir->create_time = 0x9684;        //fixed time of creation
                    dir->create_date = 0x3a37;        //fixed date of creation
                    dir->last_access_date = 0x3a37;   //fixed date of last access
                    dir->write_time = 0x9684;         //fixed time of last write
                    dir->write_date = 0x3a37;         //fixed date of last write
                    dir->first_cluster_hi = first_cluster_high;
                    dir->first_cluster_lo = first_cluster_low;
                    dir->file_size = file_size;

                    sd_write_single_block (first_sector + sector);
                    file_created_flag = 1;

                    // Updating free memory count in FSinfo sector
                    free_memory_update (REMOVE, file_size); 
                }
            }
        }

        cluster = get_set_next_cluster (prev_cluster, GET, 0);

        if(cluster > 0x0ffffff6) {
            // This situation will occur when total files in root is a multiple of 
            // (32 * sector_per_cluster)
            if(cluster == FAT32_EOF) {  
                // Find next cluster for root directory entries
                cluster = search_next_free_cluster(prev_cluster);
                // Link the new cluster of root to the previous cluster
                get_set_next_cluster(prev_cluster, SET, cluster);
                // Set the new cluster as end of the root directory
                get_set_next_cluster(cluster, SET, FAT32_EOF);
            } 
            else return;
        }
        if (cluster == 0)
            return;
        prev_cluster = cluster;
    }
}


/**
 * @brief function searches for the next free cluster in the root directory starting
 *        at specified cluster
 * @param start_cluster - unsinged long, starting cluster
 * @return usigned long, the number of the next free cluster
 */
unsigned long search_next_free_cluster (unsigned long start_cluster) {
    unsigned long cluster, *value, sector;
    unsigned char i;

    start_cluster -=  (start_cluster % 128);   //to start with the first file in a FAT sector
    for (cluster = start_cluster; cluster < total_clusters; cluster += 128) {
        sector = unused_sectors + reserved_sector_count + ((cluster * 4)
                / bytes_per_sector);
        sd_read_single_block(sector);
        for (i = 0; i < 128; i++)  {
            value = (unsigned long *) &buffer[i * 4];
            if(((*value) & 0x0fffffff) == 0)
                return(cluster + i);
        }
    } 

    return 0;
}


/**
 * @brief function displays the total memory and free memory of SD Card using UART
 *        NOTE: this routine can take upto 15sec for 1GB card (@1MHz clock)
 *        it tries to read from SD whether a free cluster count is stored, if it is 
 *        stored then it will return immediately. Otherwise it will count the total 
 *        number of free clusters, which takes time
 * @return Void
 */
void memory_statistics (void) {
    unsigned long free_clusters, total_cluster_count, cluster;
    unsigned long total_memory, free_memory, sector, *value;
    unsigned int i;

    total_memory = total_clusters * sector_per_cluster / 1024;
    total_memory *= bytes_per_sector;

    TX_NEWLINE_PC;
    TX_NEWLINE_PC;

    // Display total memory
    display_memory (HIGH, total_memory);

    free_clusters = get_set_free_cluster (TOTAL_FREE, GET, 0);

    if (free_clusters > total_clusters) {
        free_cluster_count_updated = 0;
        free_clusters = 0;
        total_cluster_count = 0;
        cluster = root_cluster;    

        while(1) {
            sector = unused_sectors + reserved_sector_count 
                + ((cluster * 4) / bytes_per_sector) ;
            sd_read_single_block(sector);

            for (i = 0; i < 128; i++) {
                value = (unsigned long *) &buffer[i*4];
                if(((*value) & 0x0fffffff) == 0) free_clusters++;
                total_cluster_count++;
                if(total_cluster_count == (total_clusters + 2)) break;
            }  

            if (i < 128)
                break;
            cluster += 128;
        } 
    }

    if (!free_cluster_count_updated) {  
        //update FSinfo next free cluster entry
        get_set_free_cluster (TOTAL_FREE, SET, free_clusters); 
    }

    free_cluster_count_updated = 1;  //set flag
    free_memory = free_clusters * sector_per_cluster / 1024;
    free_memory *= bytes_per_sector ;

    // Print free memory
    display_memory (HIGH, free_memory);
}

/**
 * @brief function converts the unsigned long value of memory into text string and
 *        send ot UART
 * @param flag - unsigned char, if HIGH - memory will be displayed in KBytes.
 *               else in Bytes
 * @param memory - unsigned long, memory value 
 * @return Void
 */
void display_memory (unsigned char flag, unsigned long memory) {
    // 19 character long string for memory display
    unsigned char memory_string[] = "              Bytes"; 
    unsigned char i;

    for (i = 12; i > 0; i--) { //converting free_memory into ASCII string
        if(i == 5 || i == 9) {
            memory_string[i - 1] = ',';  
            i--;
        }
        memory_string[i-1] = (memory % 10) | 0x30;
        memory /= 10;

        if (memory == 0)
            break;
    }

    if (flag == HIGH)
        memory_string[13] = 'K';
    usart_transmit_string_to_pc(memory_string);
}


/**
 * @brief function deletes the specified file in the root directory
 * @param file_name - unsigned char *, file name of file that you want deleted
 * @return Void
 */
void delete_file (unsigned char *file_name)
{
    unsigned char error;

    error = convert_file_name (file_name);
    if (error)
        return;

    find_files (DELETE, file_name);
}


/**
 * @brief function updates the free memory count in FSinfo sector. Whenever a file
 *        is deleted or created, this function will be called to ADD or REMOVE 
 *        clusters occupied by teh file
 * @param flag - unsigned char, can be ADD or REMOVE
 * @param size - unsigned long, file size in Bytes
 * @return Void
 */
void free_memory_update (unsigned char flag, unsigned long size) {
    unsigned long free_clusters;
    //convert file size into number of clusters occupied
    if((size % 512) == 0) size = size / 512;
    else size = (size / 512) +1;
    if((size % 8) == 0) size = size / 8;
    else size = (size / 8) +1;

    if(free_cluster_count_updated) {
        free_clusters = get_set_free_cluster (TOTAL_FREE, GET, 0);
        if (flag == ADD)
            free_clusters = free_clusters + size;
        else  //when flag = REMOVE
            free_clusters = free_clusters - size;
        get_set_free_cluster (TOTAL_FREE, SET, free_clusters);
    }
}

/**
 * @brief Checks the SD card, communicates with computer to let know what type of 
 *        card and sets up the rest of the SD card.
 * @param verbose  bool   Determines whether of not you are communicating found data
 *        to the computer
 * @ref   www.dharmanitech.com
 * @return Void
 */
void init_sd_card(bool verbose) {
    unsigned char init = 0;
    unsigned char error;
    card_type = 0;
    error = sd_init(); // try to initialize sd card

    if(verbose) {
        init = vs1053_initialize();

        if (init == 0) {
            usart_transmit_string_to_pc_from_flash(PSTR("VS1053 MP3 chip sucessfully initialized"));
            TX_NEWLINE_PC;  
        }
        else{
            usart_transmit_string_to_pc_from_flash (PSTR("Error initializing VS1053 - CODE "));
            usart_transmit_byte_to_pc(init + 64);
            TX_NEWLINE_PC;
        }

        if (error) {
            if (error == 1) {
                usart_transmit_string_to_pc_from_flash(PSTR("SD card not detected.."));
                TX_NEWLINE_PC;
                play_beep();  
            }
            
            if (error == 2) 
                usart_transmit_string_to_pc_from_flash(PSTR("Card Initialization failed.."));

            while(1);  //wait here forever if error in SD init   
        }

        switch (card_type) {
            case 1:
                usart_transmit_string_to_pc_from_flash(
                        PSTR("Standard Capacity Card (Ver 1.x) Detected"));
                break;
            case 2:
                usart_transmit_string_to_pc_from_flash(
                        PSTR("High Capacity Card Detected"));
                break;
            case 3:
                usart_transmit_string_to_pc_from_flash(
                        PSTR("Standard Capacity Card (Ver 2.x) Detected"));
                break;
            default:
                usart_transmit_string_to_pc_from_flash(
                        PSTR("Unknown SD Card Detected"));
                break; 
        }


        _delay_ms(1);   //some delay

        error = get_boot_sector_data (); //read boot sector and keep necessary data in global variables
        if (error) {
            TX_NEWLINE_PC;
            usart_transmit_string_to_pc_from_flash(
                    PSTR("FAT32 not found!"));  //FAT32 incompatible drive
        }
        else {
            TX_NEWLINE_PC;
            usart_transmit_string_to_pc_from_flash(
                    PSTR("FAT32 file system detected..."));
        }
        TX_NEWLINE_PC;  
     
    }
    else {
        if (error)
            while(1);  //wait here forever if error in SD init   
        _delay_ms(1);   //some delay

        //read boot sector and keep necessary data in global variables
        error = get_boot_sector_data();
        init = vs1053_initialize();
    }
}
