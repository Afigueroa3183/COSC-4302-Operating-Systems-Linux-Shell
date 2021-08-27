Kaden Carter, Alejandro Figueroa, Jarrett Lumsden
COSC 4302
Bo Sun
7/30/2021

This project is a simple shell designed in C-language for use on the Linux OS.
This was developed as a group for our Operating Systems semester project.

How to run:
In a linux environment, ensure you are in the same directory as the projectshell.c file.

Compile the projectshell.c file. If using gcc, use the command below.
gcc -o projectshell projectshell.c

You can then run the newly made executable using the command below.
./projectshell

Once it is running you may type in linux commands and use the shell just like you normally would.
In order to exit the shell, you may use the command "exit", or "quit".
In the event of a recurring process, such as ping, you can use ctrl+z to exit the shell.
You should avoid file names with spaces in them for this shell's use.

The program will parse your command and search all PATHs for it.
If found, the command will execute, if not, the shell will inform you the command could not be found.

As an example,
ping google.com -c 5
Will ping the google servers 5 times and output the results of these pings.
You may also run executables in the same directory through the shell as shown below.
./helloWorld