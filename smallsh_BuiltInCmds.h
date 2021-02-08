void builtInExit() {
	/*
		Loop through the BG processes and if they are still running
		kill them
	*/
	int* childStatus;
	int pidStatus;
	// Loop through the list of background processes
	for (int i = 0; i < 50; i++) {
		if (BG_PIDS[i] != 0) {
			// If the process has not terminated
			if (waitpid(BG_PIDS[i], &childStatus, WNOHANG) != 0) {
				// Kill the process and remove it from the list
				kill(BG_PIDS[i], SIGTERM);
				BG_PIDS[i] = 0;
			}
		}
		else {
			// If it has already terminated then just remove it from the list
			BG_PIDS[i] = 0;
		}
	}
	// Exit the shell
	exit(0);
}


void builtInCd(struct usrCmd* curCommand) {
	/*
		Changes the working directory of smallsh
	*/

	char* pathtoChangeTo;

	// Initially set the variable to path of home
	strcpy(pathtoChangeTo, getenv("HOME"));

	// Check if there were any arguments given, if so change to that argument
	if (curCommand->argArray[1] != NULL) {
		strcpy(pathtoChangeTo, curCommand->argArray[1]);
	}

	// Change the directory
	chdir(pathtoChangeTo);
}


void builtInStatus() {
	/*
		Prints out either the exit status or the terminating signal of the last foreground process
	*/

	waitpid(PID_LAST_FG_PROCESS, &STAT_LAST_FG_PROCESS, 0);
	// If it was terminated by a signal then print that
	if (WIFSIGNALED(STAT_LAST_FG_PROCESS)) {
		fprintf(stdout, "Terminated by signal %d\n", WTERMSIG(STAT_LAST_FG_PROCESS));
		fflush(stdout);
	}
	// If the process exited then print the exit status
	else {
		printf("Exit value: %d\n", WEXITSTATUS(STAT_LAST_FG_PROCESS));
	}
}


int checkForBuiltIn(struct usrCmd* curCommand) {
	/*
		Check for built in functions and execute them
		Return 1 if function is built in, 0 if not, 2 if exit case
	*/
	if (strcmp(curCommand->argArray[0], "exit") == 0) { builtInExit(); return 1; }
	else if (strcmp(curCommand->argArray[0], "cd") == 0) { builtInCd(curCommand); return 1; }
	else if (strcmp(curCommand->argArray[0], "status") == 0) { builtInStatus(); return 1; }
	return 0;
}