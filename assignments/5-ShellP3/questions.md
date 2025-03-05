1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

I save the pid of each child in an array. Then, I loop through the array and call waitpid() on each pid in the array. This ensures that the parent process doesn't end until it has waited for each of the children. If I forgot to call waitpid() on, the parent process might end before one or more of the child processes. This would lead to undefined behavior in my program.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

If I don't close the original pipes after copying with dup2(), I have introduced a descriptor leak. This could cause my program to run out of available file descriptors. File descriptors aren't an infinite resource. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd needs to change the directory of every process. If it was only passed in a child process, the other child processes wouldn't take place in the new directory. The parent's directory would also not be changed. It is important to change the parent's directory as all child processes will inherit the parent's directory. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would use malloc to dynamically allocate memory.
