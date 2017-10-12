\ This Forth script simulates device control unit on the 
\ mbed reconfigurable computing platform

\ Let us assume that 3 LEDs on board are 3 physical devices
\ LED1 - TV, LED2 - AC, LED3 - lights

\ first, we define words that control the devices

: tv-on  1 31 DigitalOut ;
: tv-off 0 31 DigitalOut ;

: ac-on  1 32 DigitalOut ;
: ac-off 0 32 DigitalOut ;

: light-on 1 33 DigitalOut ;
: light-off 0 33 DigitalOut ;

\ Define flags indicating status of each device
variable tv-state 
variable ac-state
variable light-state

0 tv-state !

20 90 1 crt_btn "tv" "Toggle" ctl-tv
Drop					\ drop succes/error code
90 90 2 crt_btn "ac" "Toggle" ctl-ac
Drop
160 90 3 crt_btn "lt" "Toggle" ctl-lt
Drop

90 200 10 crt_btn "quit" "Quit" quit-app
Drop

0 1 35 5 4 crt_st_txt "tv" "TV is:"
Drop
0 1 105 5 5 crt_st_txt "ac" "AC is:"
Drop
0 1 175 5 6 crt_st_txt "lt." "Lt. is:"
Drop

0 1 35 60 7 crt_st_txt "tv-st" "Off"
Drop
0 1 105 60 8 crt_st_txt "ac-st" "Off"
Drop
0 1 175 60 9 crt_st_txt "lt-st" "Off"
Drop

\ let us turn off all devices before we begin
tv-off ac-off light-off 
\ turn off 4th LED
0 34 DigitalOut

: ctl-tv
	tv-state @ 	        \ get the current TV state
	0 =
	If			\ off, turn it on
		tv-on
		1 tv-state !    \ let us set tv-state variable 1 == on
		7 set_st_txt ." on"
		1 0 7 set_st_clr
		Drop
	else
		tv-off
		0 tv-state !
		7 set_st_txt ." off"
		0 1 7 set_st_clr
		Drop
	then
;

: ctl-ac
	ac-state @
	0 =
	If
		ac-on
		1 ac-state !   \ ac on
		8 set_st_txt ." on"
		1 0 8 set_st_clr 
		Drop
	else
		ac-off
		0 ac-state !
		8 set_st_txt ." off"
		0 1 8 set_st_clr
		Drop
	then
;

: ctl-lt
	light-state @
	0 =
	If
		light-on
		1 light-state !   \ ac on
		9 set_st_txt ." on"
		1 0 9 set_st_clr 
		Drop
	else
		light-off
		0 light-state !
		9 set_st_txt ." off"
		0 1 9 set_st_clr
		Drop
	then
;

: quit-app
	exit_ml
;

show

ml

clr_gui

