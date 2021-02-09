# nand2tetris
Solved exercises from book "The Elements of Computing Systems"

The solutions proposed are not guaranteed to be free of bugs and probably are not the most efficient ones.

If you have better/different implementations please reach out in order to compare the two and help improve this code.

## About the VM:
The two VM from ex07 and ex08 are different, the one in ex08 is the complete one and also implements few performance improvements.
I did some tests comparing .asm files from other nand2tetris repos and this implementation seems to generate between 30% and 15% faster assembly code.

## Compiling the programs
Every program have a `Makefile` with a `make` rule.

## Debug mode
In order to see verbose output in `assembler.h` and `vmtranslator.h` switch `DEBUG_MODE` to 1.

## Solved exercises:

1. Boolean Logic
2. Boolean Arithmetic
3. Sequencial Logic
4. Machine Language
5. Computer Architecture
6. Assembler
7. Virtual Machine I
8. Virtual Machine II


The tools and initial setup for the exercises can be found at [nand2tetris.org](https://www.nand2tetris.org/)
