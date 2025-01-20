# piano-reader
make learning sight reading easier by modifying keyboards to support communication with computer

# wip notes 

* hard to do programatically because some of the inputs have some bad noise 
that seems to be dealt with in the firmware or on pcb. this causes false "presses" on middle C to name one of many. 
    * it seems to mostly be an issue with the 7th pin on ribbon #2, 
    which meant if you kept that off then you can avoid the falses 
    and figure out that certain parts of the grid weren't hitting.

* after wasting a three hours on saturday trying to print out a live arrangement of the grid over serial,
as part of the grid discovery process I realized creating a map with this noise on random pins is a waste 
of time and just started manually checking continuity with a dmm
    * this was a lot faster and then the pattern was pretty clear. 
    it just goes straight down the keyboard starting at 1.7 (header 1, pin 7) and 2.7 (header 2, pin 7). 
    * discovered it is definitely an 8x8, with three unused
    * i'll add a script to print out the grid now that that is done, 
    and will use esp32 to avoid needing to mux further

* map is done and the grid is done too (there's a grid()function).
the double press noise is unavoidable without pull up resistors for noise,
so there will need to be a breadboard involved. sample of the signal leakage and grid 
```
    Key Press Grid:
    0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 1 <- I didn't press G#
    0 0 0 0 0 0 0 1 <- middle C
    0 0 0 0 0 0 0 0 
    0 0 0 0 0 0 0 0 
```

* after some trial and error, I settled on using 2k pull up resistors. 
it is very accurate now and allows for multiple keys to be pressed without problems.