# MP3_Tasmota
Alternative Playground-Driver for DFRobot-MP3-Player for Tasmota

  USAGE:
  Replace the original TASMOTA driver with the same name, connect the additional wire (compared to the TX-only TASMOTA-solution)    for RX and select GPIO_RXD

  project aims:
- Support complete feature set of the player  
- Support bidirectional communication, a new pin (MP3 RX) is needed  
- High verbositiy of the driver for further development and exploration of different players/firmware versions  
- Non-blocking design to keep the main loop of TASMOTA as free as possible  
- MQTT-interface: mainly geared towards home automation (i.e. direct short audio feedback)  
- Web-GUI: usable as a music player with state synchronization with the underlying code via AJAX  
- The main intention is NOT the replacement of the current driver in TASMOTA, but more of a development playground and maybe an alternative for certain use cases. Especially the Web-GUI is quite the opposite to the fundamental design principles of the MQTT-centric TASMOTA



 additional notes on the hardware and player firmware:
- It is not known, how many different player types are on the market
- Development was started on a player, which reported firmware version: 8, but even this might not be enough to diffentiate between the models. There is not one document from the vendors, which really covers all commands and return messages of one player.
- The player needs enough power and instabilities while developing where mostly related to power supplement. For instance having an Node-MCU on the USB-Port of a Notebook with or without power cord makes a big difference and both variants are bad ideas.
- The current WebGUI updates via AJAX two times a second and therefore a stable WIFI is absolutely mandatory.
- The player firmware offers only very limited functions to read the content of the storage, one example is the automatic folder scan in the Web-GUI. The user must be aware, how he has to create the SD-card or USB-stick 


 short description of the code:
-  Every 100 ms the driver gets called. If he has a something to execute, he will do this and if not, he will read over serial, if the MP3-Player has something to tell.  
- Commands are executed as tasks, which can be chained and are stored in a simple array. This contains the task (as uint8_t) and the delay (as uint8_t * 100 ms) in every "slot". So every 100 ms the driver will look, if there is a pending task in a slot, then will execute this task, mark this slot as done and cycle through the slots of the array, until all tasks are done. Between these tasks, it will be returned to the TASMOTA run loop. No delays are used and for instance the player response to a command is read in the next loop. A very long example is the reset routine, which lasts several seconds without blocking TASMOTA.  
- The Web-GUI asks via AJAX every 500ms, if there is something new and gets back a JSON-arrray with the current state. So it will be also updated, if a MQTT-command is launched or the player stops a song. 

MQTT-commands:  
-  The main addition for home automation uses should be the use of audio feedbacks. According to the naming conventions of the player documents this is called MP3ADVERT.  
-  The problem is, that these adverts can only be executed, when a music track is currently played. The driver will (hopefully) handle this automatically, but due to the lack of possibilities to read the file structure of the storage device at runtime, it is absolutely neccesary to stick to a strict naming convention:  
    The folder /ADVERT must contain files with the naming scheme 001.mp3, 002.mp3, ... , 999.mp3.  
    The folder /MP3 must contain an exact copy of the content of /ADVERT and will be used as a "backup", if no other mp3 track is running.
    The command MP3ADVERT 1, will insert the audio track /ADVERT/001.mp3 into the current track or falls back to /MP3/001.mp3.
    
Web-GUI:
-  Shows the FW version and (if supported) the date stamp of the player firmware.  
-  Play/Pause-button with color change to see immediatly if something is playing.  
-  Shows number of folders on the storage device. BEWARE: This shows all sorts of folder (i.e. hidden .folders from the mac finder!). So this can be quite misleading.  
-  A folder scan is an attempt to auto-read, if there are folders, which stick to the strict naming scheme of 01, 02, ..., 99. These folders can for instance contain albums or what not. The scan will under the hood check the player response to special query command and this takes a while. An interruption will probably lead to undefined behaviour of the Web-GUI. If succesful a pull down menu with the found folders and a button to play all files of a folder is shown.  
    
    
    
Thanks to mike2nl for a lot of testing!
