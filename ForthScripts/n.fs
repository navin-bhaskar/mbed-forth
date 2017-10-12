\ This is a simple script which prints "You clicked on hello " on serial console
\ when you click on "hello" button on LCD

: hello \ this word prints "You clicked on hello" and a new line
	." You clicked on hello  " Cr 
;
0 0 5 crt_bmp "hi" "test.bmp"
\ create a button and register call back
30 40 1 crt_btn "hello" "Hello" hello
\ word to exit the main loop
: exit_loop exit_ml ;
30 150 2 crt_btn "exit" "Exit" exit_loop
show		    \ show the GUI items
ml                  \ enter the main loop
clr_gui
