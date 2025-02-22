1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Fork creates a new process by duplicating the current process. If execvp was called without fork, the current process would be replaced by the new program. This means that you wouldn't have access to the original program anymore. The shell is the main process, and we don't want to lose access to it. The shell/parent process should be able to control the child processes. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  My implementation handles this scenario by using a control flow of if statements to handle the cases in which the return code is less than zero. It exits the program in these cases.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It checks to see if the argument passed is a path or bare command. If the argument is a bare command, it looks in the "PATH" environment variable to execute it. If its a path, it searches the directory the path gives. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose is so that the parent process will wait for the child process to finish before continuing the parent process. If it wasn't called, the shell prompt, or whatever the shell is coded to do next, would be executed before the child process was finished. Also, resources allocated are cleaned up with wait. If the parent process doesn't wait for the child to finish executing, the allocated resources won't be returned. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It provides the exit status code that the child process called when exiting. It is important for debugging and determining the sucesss of your overall program. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I created a bool variable to check if the current position of the pointer was in a quote. If true, the null terminator is added after the pointer encounters a quote. Otherwise, a null terminator is added a space is encountered. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The main changes I added to the parsing logic were handling quotes arguments. There weren't any unexpected challenges as the lecture provided the way to do this.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose is to send notifcations to processes. Other forms of IPC are more for delivering, sharing, ,transfering, coordinating, and processing data. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGSEGV is informs a process of a segmentation error, and it is sent when a process attempts to access memory out of the allocated range or when dereferencing a null pointer. SIGCHILD informs a process of when its child process terminates, and this is useful for parent processes to call wait or other functions to deallocate resources. SIGSTOP is sent to stop a process, and this is useful to stop processes when necessary.  

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  The process is suspended. It cannot be caught or ignored like SIGINT because it is a system level control process. 
