I.	Introduction

This program is a simple C++ implementation of Nintendo’s classic The Legend of Zelda.  For those less 
common with Zelda, the game revolves around its protagonist, Link, as he navigates the world and 
encounters multiple enemies with the ultimate goal of rescuing Princess Zelda. While size of the world 
has been made significantly smaller due to the time restraint of the project, the program includes 
many of the original game’s features. The user leads Link through the world, helping him defeat 
enemies and collect treasure along the way.

II.	User Manual

A.	Special Libraries Needed
In order to create the necessary graphics, this program uses the SDL 2 graphics library. This program 
was built using the SDL 2 library installed on the University of Notre Dame student machines. If 
attempting to run Zelda on a different machine where the SDL 2 library is not yet installed, please 
follow the download instructions here for the appropriate operating system. It is important to note 
that the responsiveness of SDL 2 is extremely slow over X11 forwarding and unlikely to load, so it is 
highly recommended that Zelda is run on the physical machine.

B.	Compiling Instructions
This program was built using the Linux operating system. All commands below, unless otherwise 
specified, are to be entered in the terminal command line. Commands are subject to change when running 
on other operating systems. 

To compile the program, the user’s paths must be set appropriately to the location of the SDL 2 
graphics library. If on one of the University of Notre Dame student machines, set the paths as 
follows:

setenv PATH /afs/nd.edu/user14/csesoft/new/bin:$PATH
setenv LD_LIBRARY_PATH /afs/nd.edu/user14/csesoft/new/lib:$LD_LIBRARY_PATH

