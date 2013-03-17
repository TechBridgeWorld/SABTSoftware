/**
 * @file FAT32.h
 * @brief FAT32 implementation of SD card
 * ********************************************************
 * **** ROUTINES FOR FAT32 IMPLEMATATION OF SD CARD *****
 * ********************************************************
 * Controller: ATmega32 (Clock: 8 Mhz-internal)
 * Compiler  : AVR-GCC (winAVR with AVRStudio)
 * Version   : 2.3
 * Author  : CC Dharmani, Chennai (India)
 *         www.dharmanitech.com
 * Date    : 08 May 2010
 * ********************************************************
 * @author CC Dharmani, Chennai (India)
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 *
 * Link to the Post: 
 *  http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html
 */

#ifndef _FAT32_H_
#define _FAT32_H_

//Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure{
unsigned char nothing[446];        //ignore, fill the gap in the structure
unsigned char partitionData[64];   //partition records (16x4)
unsigned int signature;            //0xaa55
};

//Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure{         
unsigned char status;              //0x80 - active partition
unsigned char headStart;          //starting head
unsigned int cylSectStart;         //starting cylinder and sector
unsigned char type;                //partition type 
unsigned char headEnd;             //ending head of the partition
unsigned int cylSectEnd;           //ending cylinder and sector
//total sectors between MBR & the first sector of the partition
unsigned long firstSector;    
unsigned long sectorsTotal;        //size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure{
  unsigned char jumpBoot[3];        //default: 0x009000EB
  unsigned char OEMName[8];
  unsigned int bytesPerSector;      //deafault: 512
  unsigned char sectorPerCluster;
  unsigned int reservedSectorCount;
  unsigned char numberofFATs;
  unsigned int rootEntryCount;
  unsigned int totalSectors_F16;    //must be 0 for FAT32
  unsigned char mediaType;
  unsigned int FATsize_F16;         //must be 0 for FAT32
  unsigned int sectorsPerTrack;
  unsigned int numberofHeads;
  unsigned long hiddenSectors;
  unsigned long totalSectors_F32;
  unsigned long FATsize_F32;        //count of sectors occupied by one FAT
  unsigned int extFlags;
  unsigned int FSversion;           //0x0000 (defines version 0.0)
  unsigned long rootCluster;        //first cluster of root directory (=2)
  unsigned int FSinfo;              //sector number of FSinfo structure (=1)
  unsigned int BackupBootSector;
  unsigned char reserved[12];
  unsigned char driveNumber;
  unsigned char reserved1;
  unsigned char bootSignature;
  unsigned long volumeID;
  unsigned char volumeLabel[11];    //"NO NAME "
  unsigned char fileSystemType[8];  //"FAT32"
  unsigned char bootData[420];
  unsigned int bootEndSignature;    //0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
  unsigned long leadSignature; //0x41615252
  unsigned char reserved1[480];
  unsigned long structureSignature; //0x61417272
  unsigned long freeClusterCount; //initial: 0xffffffff
  unsigned long nextFreeCluster; //initial: 0xffffffff
  unsigned char reserved2[12];
  unsigned long trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure{
  unsigned char name[11];
  unsigned char attrib; //file attributes
  unsigned char NTreserved; //always 0
  unsigned char timeTenth; //tenths of seconds, set to 0 here
  unsigned int createTime; //time file was created
  unsigned int createDate; //date file was created
  unsigned int lastAccessDate;
  unsigned int firstClusterHI; //higher word of the first cluster number
  unsigned int writeTime; //time of last write
  unsigned int writeDate; //date of last write
  unsigned int firstClusterLO; //lower word of the first cluster number
  unsigned long fileSize; //size of file in bytes
};

//buffer variable
#define BUFFER_SIZE 512

=======
>>>>>>> f013637c3ed95f16fee1d41d1b448961cc2f64da
//Attribute definitions for file/directory
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0f

#define DIR_ENTRY_SIZE     0x32
#define EMPTY              0x00
#define DELETED            0xe5
#define GET                0
#define SET                1
#define READ               0
#define VERIFY             1
#define ADD                0
#define REMOVE             1
#define LOW                0
#define HIGH               1
#define TOTAL_FREE         1
#define NEXT_FREE          2
#define GET_LIST           0
#define GET_FILE           1
#define DELETE             2
#define EOF                0x0fffffff

// Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure
{
  unsigned char nothing[446];           // ignore, fill the gap in the structure
  unsigned char partition_data[64];     // partition records (16x4)
  unsigned int signature;               // 0xaa55
};

// Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure
{         
  unsigned char status;                 // 0x80 - active partition
  unsigned char head_start;             // starting head
  unsigned int cyl_sect_start;          // starting cylinder and sector
  unsigned char type;                   // partition type
  unsigned char head_end;               // ending head of the partition
  unsigned int cyl_sect_end;            // ending cylinder and sector
  // total sectors between MBR & the first sector of the partition
  unsigned long first_sector;    
  unsigned long sectors_total;          // size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure
{
  unsigned char jump_boot[3];           // default: 0x009000EB
  unsigned char oem_name[8];
  unsigned int bytes_per_sector;        // default: 512
  unsigned char sector_per_cluster;
  unsigned int reserved_sector_count;
  unsigned char number_of_fats;
  unsigned int root_entry_count;
  unsigned int total_sectors_f16;       // must be 0 for FAT32
  unsigned char media_type;
  unsigned int fat_size_f16;            // must be 0 for FAT32
  unsigned int sectors_per_track;
  unsigned int number_of_heads;
  unsigned long hidden_sectors;
  unsigned long total_sectors_f32;
  unsigned long fat_size_f32;           // count of sectors occupied by one FAT
  unsigned int ext_flags;
  unsigned int fs_version;              // 0x0000 (defines version 0.0)
  unsigned long root_cluster;           // first cluster of root directory (=2)
  unsigned int fs_info;                 // sector number of FSinfo structure (=1)
  unsigned int backup_boot_sector;
  unsigned char reserved[12];
  unsigned char drive_number;
  unsigned char reserved1;
  unsigned char boot_signature;
  unsigned long volume_id;
  unsigned char volume_label[11];       // "NO NAME"
  unsigned char file_system_type[8];    // "FAT32"
  unsigned char boot_data[420];
  unsigned int boot_end_signature;      // 0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
  unsigned long lead_signature;         // 0x41615252
  unsigned char reserved1[480];
  unsigned long structure_signature;    // 0x61417272
  unsigned long free_cluster_count;     // initial: 0xffffffff
  unsigned long next_free_cluster;      // initial: 0xffffffff
  unsigned char reserved2[12];
  unsigned long trail_signature;        // 0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure
{
  unsigned char name[11];
  unsigned char attrib;                 // file attributes
  unsigned char nt_reserved;            // always 0
  unsigned char time_tenth;             // tenths of seconds, set to 0 here
  unsigned int create_time;             // time file was created
  unsigned int create_date;             // date file was created
  unsigned int last_access_date;
  unsigned int first_cluster_hi;        // higher word of the first cluster number
  unsigned int write_time;              // time of last write
  unsigned int write_date;              // date of last write
  unsigned int first_cluster_lo;        // lower word of the first cluster number
  unsigned long file_size;              // size of file in bytes
};


//************* external variables *************
volatile unsigned long first_data_sector, root_cluster, total_clusters;
volatile unsigned int  bytes_per_sector, sector_per_cluster, reserved_sector_count;
unsigned long unused_sectors, append_file_sector;
unsigned long append_file_location, file_size, append_start_cluster;

// Global flag to keep track of free cluster count updating in FSinfo sector
unsigned char free_cluster_count_updated;

//for Text files track all clusters that they contain
unsigned long *dict_clusters;
unsigned int dict_cluster_cnt;

//************* functions *************
unsigned char get_boot_sector_data(void);
unsigned long get_first_sector(unsigned long cluster_number);
unsigned long get_set_free_cluster(unsigned char tot_or_next, 
                                   unsigned char get_set, 
                                   unsigned long fs_entry);
struct dir_Structure* find_files(unsigned char flag, unsigned char *file_name);
unsigned long get_set_next_cluster(unsigned long cluster_number,
                                   unsigned char get_set,
                                   unsigned long cluster_entry);
unsigned char read_file(unsigned char flag, unsigned char *file_name);
unsigned char read_and_retrieve_file_contents(unsigned char *file_name,
                                              unsigned char *data_string);
unsigned char play_mp3_file(unsigned char *file_name);
unsigned char convert_file_name(unsigned char *file_name);
int replace_the_contents_of_this_file_with(unsigned char *file_name,
                                           unsigned char *file_content);
void write_file(unsigned char *file_name);
void append_file(void);
unsigned long search_next_free_cluster(unsigned long start_cluster);
void memory_statistics(void);
void display_memory(unsigned char flag, unsigned long memory);
void delete_file(unsigned char *file_name);
void free_memory_update(unsigned char flag, unsigned long size);
void init_sd_card(bool verb);

int check_first_full_word(unsigned char *word);

#endif /* _FAT32_H_ */
