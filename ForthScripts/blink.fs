: pattern1 1 31 DigitalOut 0 32 DigitalOut 1 33 DigitalOut 0 34 DigitalOut ;
: pattern2 0 31 DigitalOut 1 32 DigitalOut 0 33 DigitalOut 1 34 DigitalOut ;
variable flag

0 flag !		\ reset flag to start with

: blink-loop
  flag @ 		\ read the flag value
  0 = 			\ is it equal to zero
  if	
    pattern1		\ if so, time to trun on the LED1 and LED3
    -1 flag !		\ set the flag 
  else
    pattern2		\ turn off LEDs 
     0 flag !
  then
;

1 addTicker blink-loop			\ start blinking 



