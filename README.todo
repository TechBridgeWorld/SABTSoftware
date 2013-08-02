One major bug on the SABT has been random system crashes on button presses. This was usually being dealt with by restarting the device. Preliminary investigation suggests that the bug originates from the USART communication between the primary interface board and the MCU. A temporary fix has been implemented in the form of CRC checks on packets received checked against a checksum sent as the last 2 bytes of the packet and also buffer overflow protection for the packet buffer. A permanent fix would involve implementing byte level parity checks and a two-way communication protocol to ensure dependable packet delivery. Currently, the end user may need to press a key more than once if a CRC fails since there is no way for the interface board to ascertain if a packet is to be resent.

Runtime mode file parsing is currently broken. Ideally, the SABT should read MODES.DAT from the SD card and parse its contents to enable modes. This allows modes to be enabled and disabled without reprogramming the device. However, for some reason, it was not able to handle more than 7 modes. A rewrite of the relevant functions also did not help. So, currently, dynamic mode configuration is disabled in favour of a static configuration that is hard-coded into ui_handle.c’s check_modes() function.

We need a better way to read in the dictionary more quickly. It currently takes 10-15 seconds which is a distracting amount of silence.

In digitalIO.c, in the Primary Board's code base, we use a function called delay().  The reason for this is that we are sending two things in a row in this function. If the delay() function is not used, both transfers will not work.  Look to fix the use of this.

At the end of the dictionary file, you must have a line of ++++++++, so that the dictionary parser knows where to stop.

Volume sound files are not of uniform volume. Some are louder and some are softer then others. Look to standardize these sound files or even change them based off of User testing. 

Dictionary is currently read in all at once before any sound files are played. Each time through the loop a number of dictionary nodes are read in.  This number is stored in the value CLUSTERS_PER_RUN which is set to 60 currently.  The value is set in the header file FAT32.h.
