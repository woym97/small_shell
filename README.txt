Program 3 - "smallsh" 
[CS_344] W2021
Madison Woy - woym
-------------------------------------------

[DESCRIPTION]
This program is a small shell applications that can be run throught the command line. It has the following functionality:
	1. Provide a prompt for running commands
		- The built in commands listed below are implemented in the program, all others are implemented via execvp
	2. Handles blank lines and comments, which are lines beginning with the # character
	3. Provide expansion for the variable $$, this variable expands to the PIS of the parent shell
	4. Execute 3 commands exit, cd, and status via code built into the shell
	5. Execute other commands by creating new processes using a function from the exec family of functions
	6. Supports input and output redirection
	7. Supports running commands in foreground and background processes
		- SIGTSTP implemented to enter/exit a "foreground only mode" otherwise the & operator can be used to run processes in the background
			EX: sleep 5 &
	8. Implement custom handlers for 2 signals, SIGINT and SIGTSTP
		- SIGINT terminates the currently running foreground child process 

[TO RUN]
To run this program, compile and enter the program via:

	gcc --std=gnu99 -o smallsh main.c
	./smallsh
