# Stand Alone Braille Tutor

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

TODO: Finish this part on the lab computer
### Compiling the software
1. In AVR Studio 4, open Documents\SABTSoftware\SABT\_MainUnit\...

TODO: Finish this part on the lab computer (binary location)
### Loading the software binaries onto the SABT hardware
1. Obtain the SABT Main Control Unit and one of the SABT User Interface boards. You will also need the JTAGICE mkII programmer, two USB (type A to type B) cables, and headphones or a speaker with an analog input.
2. Remove all of the casing from the two boards.
3. Connect...

## Current Issues and Pitfalls

## Opportunities for further development
- Users have expressed interest in two different voices - one for input and one for output. This would involve recording one voice (preferably masculine) for all of the prompts and instructions given by the device, and a second voice (ideally feminine) to echo user input. For example, The instructions for Letter Practice and the letter prompts should be in the first voice, while the voice echoing the dot input should be the second voice.
- Users have expressed interest in a mode that serves as a reference for how to write each letter. In other words, this mode would be Letter Practice without any user input - it would just read out how to form each letter. This could potentially be done by using the mode select buttons to scroll between letters instead of reading out eery letter.
- From user testing, we found that some users entered dots very quickly. It would be very helpful to have either some mechanism to skip MP3s (if a user already knows the prompt) or some way to adjust the speed of the MP3 playback.
- The Animal Game could be expanded to include more animal sounds, and to include a variety of animals such that all of the letters are tested at some point.
- In the Animal Game, users wanted to be able to skip animals.
- A couple of users experessed interested in a "Household Sounds" game - much like the animal game, but the sounds are sounds of everyday life.
- Sentence writing practice - This could be a feature especially for the Intermediate and Advanced boards which have slate rows.


## Template:

Please provide details for as many sections that apply to your specific project. For sections that do not apply, simply indicate the phrase N/A. 

1. Provide instructions on how to checkout the code from the current repository and where to place it on the local machine (e.g., C:\MyProject).

2. List the platforms on which the project can compiled and built (Windows, Linux, Mac OS).

3. Since the project may be multifaceted, provide a list of the different components included in this project along with a description of each one stating its purpose and how it interacts with the other components.

4. Specify the tools needed to compile and build the project including compilers and/or IDEs. (e.g., Java SDK 1.6, gcc 4.3.2 on the Linux platform, Eclipse Juno, Visual Studio 10, etc...).

5. List any additional libraries that are needed for compiling, building and running the code (e.g., Boost C++ Library 1.53.0, Android API 15).

6. Specify the locations where these tools can be acquired (e.g., URLs, FTP sites, local CMU servers).

7. Indicate the location where these tools need to be installed on the local machine for project compilation, building and execution.

8. Indicate if there are any project config files, what they are used for and provide information on where these files need to be located on the local machine.

9. Specify any settings within these config files for compiling, building and/or execution (e.g., BASEPATH=C:\Users\Bob\myProject\).

10. Provide instructions on how to compile and build the project using the specified tool(s) from item 4 (e.g., startup Eclipse, import project X, right click on file Main and select Run as -> Java Application) and also specify any compiler/build configuration settings (paths of include files and other dependencies).

11. Indicate the target platform on which the binary (or binaries) are intended to run (e.g., Windows, Android Device, Custom hardware).

12. Indicate if the application requires any input files that are read and processed when the application executes (e.g., XML Files, text files, image files, etc).

13. Specify the locations where these input files can be obtained if not already included in the project (e.g., URL, CMU Serves, etc...).

14. Specify the locations where these input files need to reside on the local machine when the application executes.

15. Indicate if the application generates output files.

16. Specify the location of where these output files will be written.

17. If using external hardware or mobile device, provide instructions on how this device needs to be connected to the computer.

18. Regarding mobile devices and/or external hardware, indicate if any of the generated binaries from item 10 need to be installed on this device and provide instructions on how to install these binaries.

19. Specify any settings on the computer for the device (e.g., the COM port).

20. Provide instructions on how to run the application on the external hardware, mobile device and/or computer.

21. If the mobile device requires interaction with the computer, provide instructions on how this interaction should work from an end user standpoint (i.e., first run application A on the laptop computer, then lunch program B on the mobile device, next you should see a screen indicating a connection was made, etc...).

22. List any known bugs, configuration issues or other pitfalls to look out for when compiling, building and/or running the application.
    
23. Provide a list of future features/capabilities that you would like to see included in the existing tool.


## Overview
The Stand Alone Braille Tutor (hereafter SABT) is a device that helps blind
users learn to write braille through audio feedback and accessible controls.

## Current Progress
Currently, the primary board is the only board with significant software. The
MCU and Primary UI boards can communicate between themselves and the PC in order
to play some very basic games.
### Current Games (Primary UI)
- Dot practice
 - The SABT prompts the user to press a numbered dot (1-6). If the button
   pressed is correct, the SABT replies 'Good' and prompts the user to find
   another dot. If incorrect, the board replies 'No' and the user must find the
   correct dot.

## Components
There are two different boards that compose the SABT - the Main Control Unit and
one of three User Interface boards.
### Main Control Unit (MCU)
The MCU handles the following:
- Provides connection (via USB cable) to a PC for debugging purposes / mode
  selection
- Provides battery power / power from USB
- Has an SD card slot for sound files
- Plays sound files
- Handles different Modes

### Primary User Interface Board (Primary UI)
- Consists of six large braille dots, essentially one large cell
- Has volume control buttons, mode control buttons, enter buttons
- Transmits signals to MCU when a button is pressed for processing by the MCU

### Intermediate User Interface Board (Intermediate UI)
- Consists of three large braille cells and two rows of actual sized braille
  cells (16 cells per row)

### Advanced User Interface Board (Advanced UI)
- Consists of six rows of braille cells (16 cells per row)

## Menu System
Because the SABT is designed for blind / visually impaired users, the menu
system provides a design challenge 

## Doxygen Documentation  
Documentation at http://cmu-15-239.github.com/SABTSoftware/
