// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Memory Blocks for Pixels:
// Screen address = 16384
// 256 rows od 512 px = 131072 / 16 (memory block size) = 8192
	@clr
	M=0
(RESTART)
	@i
	M=0
(LOOP)
	@i
	D=M
	@8192
	D=D-A
	@RESTART
	D;JEQ
	@KBD
	D=M
	@WHITE
	D;JEQ
	@BLACK
	0;JMP
(INC)
	@i
	M=M+1
	@LOOP
	0;JMP
(WHITE)
	@clr
	D=M
	@STILLWHITE
	D;JEQ
	@i
	M=0
(STILLWHITE)
	@i
	D=M
	@SCREEN
	A=D+A
	M=0
	@clr
	M=0
	@INC
	0;JMP
(BLACK)
	@clr
	D=M
	@STILLBLACK
	D;JNE
	@i
	M=0
(STILLBLACK)
	@i
	D=M
	@SCREEN
	A=D+A
	M=-1
	@clr
	M=-1
	@INC
	0;JMP