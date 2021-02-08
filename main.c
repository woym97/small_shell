#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>   // for execv, getpid, fork
#include <sys/wait.h> // for waitpid
#include <signal.h>
#include "smallsh_Globals.h"
#include "smallsh_IOHandler.h"
#include "smallsh_ExecCmdHandler.h"
#include "smallsh_UserInputHandler.h"
#include "smallsh_SignalHandler.h"
#include "smallsh_BuiltInCmds.h"

int main() {
	// Enter normal mode
	FG_ONLY_MODE = 0;
	// Initiate the shell pid global variable
	SHELL_PID = getpid();
	// Install the signal handlers
	installSigHandlers();
	// Run the shell
	commandPrompt();
	return 0;
}