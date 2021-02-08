void cleanUpPidList() {
    /*
        Check which background Pids are still running and if they are done
        print them to the terminal
    */
    int* childStatus;
   
    for (int i = 0; i < 50; i++) {
        // Find an index that is still 'running'
        if (BG_PIDS[i] != 0) {
            // If the process has stopped running
            if (waitpid(BG_PIDS[i], &childStatus, WNOHANG) != 0) {
                // Print how it exited
                if (WIFEXITED(childStatus)) {
                    fprintf(stdout, "Background pid %d exited with status %d\n", BG_PIDS[i],
                        WEXITSTATUS(childStatus));
                    fflush(stdout);  
                }
                else {
                    fprintf(stdout, "Background pid %d terminated by signal %d\n", 
                        BG_PIDS[i], WTERMSIG(STAT_LAST_FG_PROCESS));
                    fflush(stdout);
                }
                // Remove it from the list
                BG_PIDS[i] = 0;
            }
        }
    }
}

void addPidToList(int pid) {
    /*
        Add the passed pid to the running list of pids
    */
    fprintf(stdout, "Background pid is %d\n", pid);
    fflush(stdout);
    // Loop through the array and add the background to the next slot avaliable
    for (int i = 0; i < 50; i++) {
        if (BG_PIDS[i] == 0) {
            BG_PIDS[i] = pid;
            break;
        }
    }
}

void execOtherCmd(struct usrCmd* curCommand) {
    /*
        Executes non built in commands
    */

    // Fork a new process
    pid_t spawnPid = fork();
    PID_LAST_FG_PROCESS = spawnPid;

    switch (spawnPid) {
    // Case where fork fails
    case -1:
        perror("fork()\n");
        exit(1);
        break;
    // Case where child process executes
    case 0:
        handleChildProcessIO(curCommand);
        // Execute the given command in the child process
        execvp(curCommand->argArray[0], curCommand->argArray);
        // Provide error if there is one to provide
        perror(curCommand->argArray[0]);
        exit(1);
        break;
    // Case where this is still the parent process
    default:
        // Wait for child's termination if the foreground 
        // flag is set in the command
        if (curCommand->foreground == 1) {
            // Update the PID of the last foreground process
            waitpid(PID_LAST_FG_PROCESS, &STAT_LAST_FG_PROCESS, 0);
            // Check the exit status of the child process 
            // If it was terminated by a signal then print that
            if (WIFSIGNALED(STAT_LAST_FG_PROCESS)) {
                fprintf(stdout, "\nTerminated by signal %d\n", WTERMSIG(STAT_LAST_FG_PROCESS));
                fflush(stdout);
            }
        }
        // Else add the pid of the background process to the running list 
        else {
            addPidToList(spawnPid);
        }
        break;
    }
}