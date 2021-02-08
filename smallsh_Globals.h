const char PID_VAR[] = "$$";	// Variable which is expanded to pid of parent process
const char BG_FLAG[] = "&";		// If the last argument is this variable then run the command in BG
int PID_LAST_FG_PROCESS = 0; // Holds the pid of last foreground process run
int STAT_LAST_FG_PROCESS = 0;
int BG_PIDS[50];
int SHELL_PID;
int FG_ONLY_MODE;	// If 1 then BG mode is initialized, if 0 then not

struct usrCmd {
	/*
		Used to house the details of the current input command
	*/
	char* unparsedInput;
	int foreground; // 1 if foreground, 0 if background
	char* argArray[513];
	char* inputFile;
	char* outputFile;
};