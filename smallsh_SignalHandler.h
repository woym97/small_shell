void handle_SIGINT(int signo) {
	/*
		Catch the SIGINT signal:
		- Terminate the foreground child process that is running 
		- Ignore signal in all other processes 
	*/
	// Check the pid of the current process, if it is the same as
	// the PID of the last foreground process then terminate it
	if (getpid() == PID_LAST_FG_PROCESS) {
		raise(SIGTERM);
	}
}

void handle_SIGTSTP(int signo) {
	/*
		Catch the SIGSTP signal:
		- Ignore in child FG processes
		- Ignore in child BG processes
		- In parent if in BG only mode:
			- Exit BG only mode
		- In parent if not in BG only mode:
			- Enter FG only mode
		[FG Only Mode] 
			Shell enters a state where subsequent commands can no longer be run in the background.
			In this state, the & operator should is ignored
	*/
	char* message;
	// Check the pid of the current process, if it is the original then this is the parent 
	if (getpid() == SHELL_PID) {
		// Handle the case where the shell is already in FG only mode
		if (FG_ONLY_MODE == 1) {
			message = "Exiting foreground-only mode\n";
			FG_ONLY_MODE = 0;
		}
		// Handle the case where the shell is not in FG only mode
		else {
			message ="Entering foreground-only mode (& is now ignored)\n";
			FG_ONLY_MODE = 1;
		}
		write(STDOUT_FILENO, message, strlen(message));
		write(STDOUT_FILENO, ":", 1);
	}
}

void installSigHandlers() {
	/*
		Install the custom signal handlers for the shell
		Code adapted from [Exploration: Signal Handling API] CS_344
	*/

	// Initialize SIGINT_action struct to be empty
	struct sigaction SIGINT_action = { 0 };

	// Apply the correct function to handle the signal
	SIGINT_action.sa_handler = handle_SIGINT;
	// Block the other signals while this is exectuing
	sigfillset(&SIGINT_action.sa_mask);
	// Set no flags
	SIGINT_action.sa_flags = SA_RESTART;

	// Install the handler
	sigaction(SIGINT, &SIGINT_action, NULL);

	// Install the SIGSTP handler the same way
	struct sigaction SIGTSTP_action = { 0 };
	SIGTSTP_action.sa_handler = handle_SIGTSTP;
	sigfillset(&SIGTSTP_action.sa_mask);
	SIGTSTP_action.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}