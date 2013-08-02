To prepare a new SD card, format it in FAT32 using your disk utility of choce and then copy over all the files from the 'sd_card_files' directory. If it gives you pain, try fiddling with the allocation unit and cluster size parameters while formatting till something works.

On a Mac, the following should work (should work for *IX OSes?):

1. Make sure the SD card is not inserted
2. 'ls /dev/rdisk?' to list current drives
3. Insert SD card
4. 'ls /dev/rdisk?' The new drive is the SD card, let it's number be #
5. 'ls /dev/rdisk#s?' - To list partiions on the disk, let $ be the parition you want (usually the first, and ideally, the only on the SD card)
6. Identify the volume label for the SD card. Check under '/Volumes/'.
7. 'diskutil unmount "VOLUMELABEL"' to unmount the SD card volume.
6. 'sudo newfs_msdos -v "VOLUMELABEL" -F 32 /dev/rdisk#s$'
7. Your SD card should be ready to copy files onto. Mount it again using 'diskutil mount "VOLUMELABEL"'
8. 'cp <path to sd_card_files>/* /Volumes/VOLUMELABEL/'

SD Card
The SD card contains configuration and media files essential to the operation of the SABT. There should be an image for the SD card in the git repo. This image should easily it on a 1 or 2GB SD card. 
File Naming and Hierarchy
The limitations of the file system mean that all primary filenames can be at most 8 characters long, with extensions limited at 3 characters. So, a typical filename will look like SYS_WELC.mp3. Apart from MODES.dat, all files on the SD card are MP3 files and can be accessed through functions defined in the wrapper audio library (audio.h and audio.c).

The suggested naming hierarchy is “AAA_BBB.mp3”, where “AAA_” define the fileset (e.g. MD7_ for mode 7 specific files and HIN_ for Hindi language specific files) and “BBBB” is the actual filename. The audio functions usually allow the developer to specify the fileset and filename as separate parameters which allows reusability of code by simply changing filesets and maintaining filenames (e.g. MD7 allows the developer to simply add a new script and relevant MP3 files and recompile to obtain the letter learning mode in a new language).