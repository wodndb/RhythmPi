# RhythmPi
Rhythm game for raspberry pi (KOREATECH term project)

Notice : I stopped this project because i returned raspberry pi to Dept. of CSE, KOREATECH. I have not raspberry pi, so I can't test this project. I will restart when I buy my own raspberry pi.

New Notice (2018-03-07) : I get raspberry pi 3 B Model, but I can't run this project because of segmentation fault

If you want to run this program using raspberry pi 3 B Model, please excute below commands.

sudo apt-get update

sudo apt-get upgrade

sudo rpi-update

sudo apt-get install libgles2-mesa libsdl2-dev

libgles2-mesa, libsdl2-dev are needed for using OpenGL ES

(ref : https://www.raspberrypi.org/forums/viewtopic.php?t=191857)


[REF for studying]

libncursed5-dev libncursedw5-dev are needed for handling keyboard input event.

(ref : hrrps://stackoverflow.com/questions/35215692/how-to-ncurses-on-raspberry-pi)
