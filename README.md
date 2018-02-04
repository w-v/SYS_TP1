# Shell pipes implementation in C
The purpose is to build a shell capable of running commands with parameters as well as commands linked with pipes.

## Description of the files

* `src/`  
 `shell1.c` : running commands without parameters  
 `shell2.c` : running commands with parameters  
 `shell3.c` : running a pipe with 2 commands with parameters, asking for the command separately (without the `|` syntax)  
 `shell4.c` : running a pipe with 2 commands with parameters, written with the `|` syntax  
 `shell5.c` : running a pipe with N commands with parameters, written with the `|` syntax  

* `run` : script for compiling and testing the programs  
 syntax :  

 `run fileNumber numberOfPipes`  

 `fileNumber` : 1 for shell1.c etc.  
 `numberOfPipes` : how many pipes the program is tested with. Max is 6  

## Options

The `shell5` program can be switched into verbose mode by giving it an argument (any argument) 
