# Stand Alone Braille Tutor

## Understanding the documentation
[This is What the SABT does]

To best understand this project, read the following documents, ideally in this order:
- This Readme
- The programming guide (SABT\_programming\_guide.pdf)
- The doxygen documentation found in the html folder (start with index.html)
- In code comments

## Setting up the project

To develop the Stand Alone Braille Tutor, follow these steps.

### Required Software and Hardware
- Windows XP or later
- AVR Studio 4. Later versions may also work, but this guide assumes the use of AVR Studio 4. 
- An AVR JTAGICE mkII programmer.
- Two USB type A to USB type B cables.
- Headphones or a speaker system with analog input.
- Git or Github for Windows.

### Downloading the project from GitHub
1. Download and install Github for Windows. This can be obtained from windows.github.com. 
2. Obtain access to the repository at https://github.com/CMU-15-239/SABTSoftware.
3. Click the "Clone in Windows" button and choose a location for the repository. This guide will assume the repository is copied to Documents\SABTSoftware.

### Compiling the software
See the programming guide - SABT\_programming\_guide.pdf.

### Loading the software binaries onto the SABT hardware
See the programming guide - SABT\_programming\_guide.pdf.

## Components
There are four different boards that compose the SABT - the Main Control Unit and
one of three User Interface boards. The Main Control Unit is used in conjunction with one of the other three boards to make an interface suitable for the user's skill level.
### Main Control Unit (MCU)
This folder contains all of the code for the control unit which does all of the processing for the boards and contains the modes. Open SABT\_MainUnit.aps in AVR Studio 4.
The MCU handles the following:
- Provides connection (via USB cable) to a PC for debugging purposes / mode
  selection
- Provides battery power / power from USB
- Has an SD card slot for sound files
- Plays sound files
- Handles different Modes

### Primary User Interface Board (Primary UI)
This folder contains all of the code for the first user interface board - the board with just six large braille dots. Open SABT\_Primary.aps in AVR Studio 4.
- Consists of six large braille dots, essentially one large cell
- Has volume control buttons, mode control buttons, enter buttons
- Transmits signals to MCU when a button is pressed for processing by the MCU

### Intermediate User Interface Board (Intermediate UI)
No work has been done yet in this folder, but it should contain the file SABT\_Intermediate.aps, and handle the intermediate user interface board.
- Consists of three large braille cells and two rows of actual sized braille
  cells (16 cells per row)

### Advanced User Interface Board (Advanced UI)
No work has been done yet in this folder, but it should contain the file SABT\_Advanced.aps, and handle the advanced user interface board.
- Consists of six rows of braille cells (16 cells per row)

## Current issues and pitfalls
- We need a better way to read in the dictionary more quickly. It currently takes 10-15 seconds which is a distracting amount of silence.
- Pressing a volume button while an mp3 file is playing breaks the system.
- We are currently receiving a warning - fixing the warning causes the code to not function, so there must be another way to resolve the warning such that the code can still run. This warning is:
```c
../FAT32.c: In function 'read_and_retrieve_file_contents':
../FAT32.c:360: warning: 'num_bytes_read' may be used uninitialized in this function
```

##Description of SABT MAIN Code
Main function is located in SABT_MainUnit.c.  This is where the function starts, in the main() function. The rest of the files are spread throughout the SABT_MainUnit Project. You will have to find the files you need. Here are the steps the code follows once it enters main:

1. It calls initialize system
  a. This call initializes all inputs form hardware, interrupts, SD card, sound player and global variables
2. The function enters an infinite while loop (functional loop).  
  - It first checks if any inputs are ready to be read (where you determine is something is ready to be read based on an interrupt setting a variable). If it is, it reads values into a buffer
  - It then checks for inputs from the PC
  - If a timer interrupt has fired, reset the timer interrupt
  - Check if the message you are reading in from either the PC or the UI board are ready to be interpreted, and interpret them if they are
  - If there is a sound file that has been queued up, must then play that sound file by reading from the SD card and sending the data to the VS1053 sound file. 
  - The function then checks if you have set one of the modes as the current mode. If you have then you must enter and execute a step in that mode.
  - Step into the mode and take a single step down the mode. This usually involves interpreting any messages that have been received. Using this input to move down the mode. You then queue a sound file to play to tell the user what they have done. 

## Opportunities for further development
- The Intermediate and Advanced user interface boards need to be developed in their entirety.
- Users have expressed interest in two different voices - one for input and one for output. This would involve recording one voice (preferably masculine) for all of the prompts and instructions given by the device, and a second voice (ideally feminine) to echo user input. For example, The instructions for Letter Practice and the letter prompts should be in the first voice, while the voice echoing the dot input should be the second voice.
- Users have expressed interest in a mode that serves as a reference for how to write each letter. In other words, this mode would be Letter Practice without any user input - it would just read out how to form each letter. This could potentially be done by using the mode select buttons to scroll between letters instead of reading out eery letter.
- From user testing, we found that some users entered dots very quickly. It would be very helpful to have either some mechanism to skip MP3s (if a user already knows the prompt) or some way to adjust the speed of the MP3 playback.
- The Animal Game could be expanded to include more animal sounds, and to include a variety of animals such that all of the letters are tested at some point.
- In the Animal Game, users wanted to be able to skip animals.
- A couple of users experessed interested in a "Household Sounds" game - much like the animal game, but the sounds are sounds of everyday life.
- Sentence writing practice - This could be a feature especially for the Intermediate and Advanced boards which have slate rows.
- One player hangman could be set up to use a random word from the entire dictionary as opposed to the fixed current list we provide.

## Doxygen Documentation  
The most recent Doxygen documentation can be found at cmu-15-239.github.io/SABTSoftware/ 
