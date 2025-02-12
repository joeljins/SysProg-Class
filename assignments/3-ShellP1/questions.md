1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application because it allows you to specify and specific number of bytes to read, allows you to get a single line, as a shell usually accepts a single line with commands and arguments, and allows for simple error handling. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  malloc() is used when the size of the data is unknown at compile time. Because we are storing user input, it is impossible to determine the exact space needed for user input. If necessary, we can increase or decrease the space allocated. Arrays are used for fixed amounts of space, and they cannot be reallocated. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  The shell might not recognize commands with trailing/leading whitespaces. Some commands/arguments might be execute differently when they include trailing/leading whitespace. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: You can create file and redirect standard output to it. A challenge might be creating the file and determining what data or how much data should be redirected. You can redirect standard output to be appeneded to a file.  A challenge might be avoiding deletion of existing data from the file. You can redirect standard error to a file. A challenge might be storing the data in a way that is helpful for debugging and finding the root of the error. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to change the destination of input or output. Piping is about chaining commands together so that the output of one command directly becomes the input of another command. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep these separate in a shell because you don't want to confuse STDERR and STDOUT. Also, if you are using pipes or redirection, you might accidentally redirect or pipe error information instead of expected output. Finally, keeping everything organized is helpful for debugging purposes. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > We should provide a way to merge them, so that it could be easier to see the cause of the errors. Both STDOUT and STDERR could be redirected to a file that contains the STDOUT and the STDERR that comes along with it. 
