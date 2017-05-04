**1.	Introduction**

This program is a simple C++ implementation of Nintendo’s classic The Legend of Zelda. For those less 
familiar with Zelda, the game revolves around its protagonist, Link, as he navigates the world 
and encounters multiple enemies with the ultimate goal of rescuing Princess Zelda. While size of the 
world has been made significantly smaller due to the time restraint of the project, the program 
includes many of the original game’s features. The user leads Link through the world, helping him 
defeat enemies and collect treasure along the way.


**2.	Game Logic**

The user is to help Link navigate through the world, helping him defeat enemies and collect treasure 
along the way. If link successfully attacks the enemy, it will take damage. Otherwise, Link will take 
damage and if harmed enough, will be defeated. Collecting treasure will aide Link and can repair 
damage done by enemies.


**3.	User Manual**

    **1.	Special Libraries Needed**

    In order to create the necessary graphics, this program uses the SDL 2 graphics library. 
    This program 
was built using the SDL 2 library installed on the University of Notre Dame student machines. If 
attempting to run Zelda on a different machine where the SDL 2 library is not yet installed, please 
follow the download instructions below for the appropriate operating system. It is important to note 
that the responsiveness of SDL 2 is extremely slow over X11 forwarding and unlikely to load, so it is 
highly recommended that Zelda is run on the physical machine.

    SDL 2 Downloading Instructions: http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php

    **2.	Compiling Instructions**

    This program was built using the Linux operating system. All commands below, unless 
    otherwise 
specified, are to be entered in the terminal command line. Commands are subject to change when running 
on other operating systems. 

    To gain access to the necessary files, the user must clone the Git repository from the link 
below. Once the user has the files, he/she can compile the program.

    https://gitlab.com/anemecek/DataStructuresFinalProject

    To compile the program, the user’s paths must be set appropriately to the location of the 
    SDL 2 
graphics library. If on one of the University of Notre Dame student machines, set the paths as 
follows:
    ```
    setenv PATH /afs/nd.edu/user14/csesoft/new/bin:$PATH
    setenv LD_LIBRARY_PATH /afs/nd.edu/user14/csesoft/new/lib:$LD_LIBRARY_PATH
    ```
    Once this is done, the user can simply enter `make` (without quotes) to compile the program 
using the provided Makefile.

    **3.	Game Controls**

    To run the program, please enter `./Zelda` (without quotes) and the program will open in a 
   new 
window. A title screen will initially pop up. To start the game from the title screen, press any 
key. To move Link, use the keyboard’s arrow keys. To make Link attack, use the A key. Once a 
Game Over screen is reached (either after beating the game or dying), reset the game by pressing 
any key after a few seconds.

    **4.	Known Bugs**

    Currently, there are no major bugs of which we are aware. 

    **5.	Credit**

    Graphics:		https://www.spriters-resource.com/nes/legendofzelda/   
    SDL Tutorials:	http://lazyfoo.net/tutorials/SDL/index.php    
    Game Music:		https://www.youtube.com/watch?v=uyMKWJ5e1kg     
					https://www.youtube.com/watch?v=lpEzYEoV9qY     
					https://www.youtube.com/watch?v=IljKQ1BdHKI     
						

