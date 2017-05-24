# SIC-Assembler



###	 To Compile :
```
gcc main.c -o sic
```

### To use the Assembler :
```
./sic -i INPUT_FILE_NAME
```
or
```
./sic -i INPUT_FILE_NAME -o OUTPUT_FILE_NAME
```

### Supportive Directive

* START
* END
* RESW
* RESB
* WORD
* BYTE
* EQU
* ORG
* LTORG

### Supported Operations

* LDA
* STA
* ADD
* AND
* COMP
* DIV
* J
* JEQ
* JGT
* JLT
* JSUB
* LDCH
* LDL
* LDX
* MUL
* OR
* RD
* RSUB
* STCH
* STL
* STX
* SUB
* TD
* TIX
* WD