# Stand Alone Braille Tutor

## Understanding the documentation
The SABT was designed to help blind students learn to write using a slate and stylus.
Historically, visually impaired students have been restricted to learning braille through the outdated slate and stylus method. This method is slow and tedious, has a long feedback cycle, and requires resources that are not always readily available. An affordable technological solution is much needed in this sector.
Although commonplace to us, personal computers are a rare luxury item in developing countries. In addition, electrical power cannot be counted on due to often unstable power grids. The only reasonable option is a battery powered device.
Easy to use and providing immediate feedback, the Stand Alone Braille Tutor is a completely self sufficient package aimed to ease and accelerate the braille learning process. Running on simply four AA batteries and coming with three different skill level interfaces, this device will be able to help a wide variety of users across the globe.

To best understand this project, read the following documents, ideally in this order:
- This Readme
- The programming guide (SABT\_programming\_guide.pdf)
- The doxygen documentation found in the html folder (start with index.html)
- Overview README files placed in project folders
- In-code comments

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
Refer to README.todo under SABT_MainUnit

## Description of SABT main() code
The main() function is located in SABT_MainUnit.c. The rest of the files are spread throughout the SABT_MainUnit Project. You will have to find the files you need. Here are the steps the code follows once it enters main:

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
Refer to README.future_development under SABT_MainUnit 

## Doxygen Documentation  
The most recent Doxygen documentation can be found at cmu-15-239.github.io/SABTSoftware/ 
