void handleChildProcessIO(struct usrCmd* curCommand) {
	/*
		Before EXEC acts on the command re route the STDIN and STDOUT
		to the specified paths
		If no file is specified then re route to DEV NULL only if the 
		child process is being run in the background
	*/

	char* inputFileName = NULL;
	char* outputFileName = NULL;
	char* devNullPath = "/dev/null";
	int result = 0;

	// If the process is being run in the FG and no input or output files 
	// have been specified then exit the function
	if (curCommand->inputFile != NULL && curCommand->outputFile != NULL
		&& curCommand->foreground == 1) {
		return;
	}

	// If the input file has been specified then adopt it
	if (curCommand->inputFile != NULL) {
		inputFileName = curCommand->inputFile;
		//strcpy(inputFileName, curCommand->inputFile);
	}
	// Else if the command is being run in the BG adopt dev null
	else if (curCommand->foreground == 0){
		inputFileName = devNullPath;
		//strcpy(inputFileName, devNullPath);
	}

	// If the output file has been specified then adopt it
	if (curCommand->outputFile != NULL) {
		outputFileName = curCommand->outputFile;
		//strcpy(outputFileName, curCommand->outputFile);
	}
	// Else if the command is being run in the BG adopt dev null
	else if (curCommand->foreground == 0) {
		outputFileName = devNullPath;
		//strcpy(outputFileName, devNullPath);
	}

	// If the input needs to be redirected...
	if (inputFileName != NULL) {
		// Open input file read only
		int inputFD = open(inputFileName, O_RDONLY);
		// If the file cannot be opened then print the error and set exit=1	
		if (inputFD == -1) {
			perror("Cannot Open File");
			exit(1);
		}

		// Redirect stdin to source file
		result = dup2(inputFD, 0);
		if (result == -1) {
			perror("Cannot Open File");
			exit(1);
		}
	}

	// If the output needs to be redirected...
	if (outputFileName != NULL) {
		// Open output file write only
		int targetFD = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (targetFD == -1) {
			perror("Cannot Open File");
			exit(1);
		}

		// Redirect stdout to target file
		result = dup2(targetFD, 1);
		if (result == -1) {
			perror("Cannot Open File");
			exit(1);
		}
	}

}