# Stand Alone Braille Tutor

## Setting up the project

To develop the Stand Alone Braille Tutor, follow these steps.

### Required Software and Hardware
- Windows XP or later
- AVR Studio 4. Later versions may also work, but this guide assumes the use of AVR Studio 4. 
- An AVR JTAGICE mkII programmer.
- Git or Github for Windows.

### Downloading the project from GitHub

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
