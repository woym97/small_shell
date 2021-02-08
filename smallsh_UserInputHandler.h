
char* expandVariable(const char* argStr) {
	/*
		Expand the $$ variable into the Pid of the parent process
		Code adapted from: https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
	*/
	char* expandedArg;
	int i, varCount = 0;

	// Get the pid of the current process and convert it to a string
	int pid = getpid();
	const char* parentPid = malloc(6);
	sprintf(parentPid, "%d", pid);
	
	int pidStrLen = strlen(parentPid);
	int varLength = strlen(PID_VAR);

	// Count the number of times the variable occurs in the argument
	for (i = 0; argStr[i] != '\0'; i++) {
		// If the variable is found in the substring then
		if (strstr(&argStr[i], PID_VAR) == &argStr[i]) {
			// Add to the count and jump to the index after the variable
			varCount++;
			i += varLength - 1;
		}
	}

	// Create a new string with enough space for the expanded variable
	expandedArg = (char*)malloc(i + varCount * (pidStrLen - varLength) + 1);

	i = 0;
	// Loop through the original string
	while (*argStr) {
		// If the variable is found then replace it
		if (strstr(argStr, PID_VAR) == argStr) {
			strcpy(&expandedArg[i], parentPid);
			i += pidStrLen;
			argStr += varLength;
		}
		// Else copy the string to the new string
		else {
			expandedArg[i++] = *argStr++;
		}	
	}

	expandedArg[i] = '\0';
	return expandedArg;
}


void checkForBackgroundFlag(struct usrCmd* curCommand, int argCount) {
	/*
		Check the last argument of the array and see if the background flag must be set
		Sets flag by default if the FG_ONLY_MODE flag is set
	*/
	if (strcmp(curCommand->argArray[argCount - 1], BG_FLAG) == 0) {
		// Only set the flag if the shell is NOT in FG only mode
		if (FG_ONLY_MODE == 0) {
			curCommand->foreground = 0;
		}
		else {
			curCommand->foreground = 1;
		}
		// Remove & from the arg list regardless of mode
		curCommand->argArray[argCount - 1] = NULL;
	}
	else {
		curCommand->foreground = 1;
	}
}


void parseInput(struct usrCmd* curCommand) {
	/*
		Parse the input to a linked list of arguments
	*/

	// Set up linked list and variables
	char* savePtr; 
	char* inputOutput;
	int argCount = 0;

	// Reset input and output file
	curCommand->inputFile = NULL;
	curCommand->outputFile = NULL;

	// Handle first case
	char* token = strtok_r(curCommand->unparsedInput, " ", &savePtr);

	while (token != NULL) {
		// If the token is a < or > then grab the file names into the struct
		if ((strcmp("<", token) == 0) || (strcmp(">", token) == 0)) {
			inputOutput = token;
			token = strtok_r(NULL, " ", &savePtr);
			if (strcmp("<", inputOutput) == 0) {
				curCommand->inputFile = calloc(strlen(token) + 1, sizeof(char));
				strcpy(curCommand->inputFile, token);
			}
			else {
				curCommand->outputFile = calloc(strlen(token) + 1, sizeof(char));
				strcpy(curCommand->outputFile, token);
			}
			token = strtok_r(NULL, " ", &savePtr);
		}
		else {
			curCommand->argArray[argCount] = calloc(strlen(token) + 1, sizeof(char));
			strcpy(curCommand->argArray[argCount], token);
			// Expand the $$ variable into the pid of the current process
			strcpy(curCommand->argArray[argCount], expandVariable(curCommand->argArray[argCount]));
			// Get next Token & increment argument counter
			token = strtok_r(NULL, " ", &savePtr);
			argCount++;
		}
	}

	// Set the last element to NULL to comply with exec command
	curCommand->argArray[argCount] = NULL;

	// Check if background flag needs to be set and set it
	checkForBackgroundFlag(curCommand, argCount);
}

void commandPrompt() {
	/*
	Handles getting and passing command prompts
	*/
	// Get user input and contine to do so until exit is called
	do {

		// Create struct to hold users input
		struct usrCmd* curCommand = malloc(sizeof(struct usrCmd));
		curCommand->unparsedInput = calloc(2049, sizeof(char));

		// Make sure that stdout and stdin is back to the terminal
		//reEstablishIO();

		// Check BG processes and report back
		cleanUpPidList();

		// Prompt user and get input
		fprintf(stdout, ":");
		fflush(stdout);
		fgets(curCommand->unparsedInput, 2048, stdin);

		// Remove trailing newline character from raw input
		strtok(curCommand->unparsedInput, "\n");

		// Continue on blank lines and comments
		if (strcmp(curCommand->unparsedInput, "\n") == 0) { continue; }
		if (curCommand->unparsedInput[0] == 35) { continue; }

		// Parse the raw input
		parseInput(curCommand);

		// Check for built in commands and execute them
		if (checkForBuiltIn(curCommand) == 1) { continue; }

		execOtherCmd(curCommand);

		free(curCommand->unparsedInput);
		free(curCommand);
		
	} while (1);
}