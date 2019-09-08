#Assembler
This project implement a assembly code compiler.
The assembler is a CLI tool that take assembly code and compile it into base 64 (only compiles without linker)

##Making the Project
to make the project to be able to run the program just use make.
navigate to the assembler directory and type:
```
make
```
and then use
```
make clean
```
to remove any access files.

##Using the Assembler
to use the assembler you need .as files. for example if you have the file "A.as" you will type into the terminal:
```
./assembler A
```
this is case sensetive, the file have to be .as but when you are running the assembler you dont need to write .as.

The assembler also support multiple files at the same time. for example if you have file "A.as" and "B.as" you can do:
```
./assembler A B 
```
you can compile as many files as you need at the same time.

###Disclaimer
This project was made as required in project number 14 in course that I learned in the Open university in course number 20465.
you cannot copy any part of this project for your own project!!!
COPPYING ON YOUR OWN RISK
