#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int n = 0;  
    int count;
    bool found = false;
    while (n < len+1){
	if (*user_str == ' ' || *user_str == '\t'){
		char ref = *user_str;
		while ( *user_str == ' ' || *user_str == '\t' ){
					user_str++;
		}
		if (n != 0 && *user_str != '\0'){							// Handle cases where space(s) aren't the first character
			*buff = ref;
			buff++;
			n++;
		}
	}	
	if (*user_str == '\0'){
		*buff = '.';	
		count = n;
		found = true;
	}
	else{
		*buff = *user_str;
		user_str++;
	}
	buff++;
	n++;
    }
    if (found == false){
	    return -1;
    }
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    printf("[");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
	int n = 0;
	int count = 0;
	while (n<str_len){
		if ( (*(buff+1) == ' ') ||  (*(buff+1) == '\t')){
			count++;
		}
		if (*(buff+1) == '.'){
			count++;
			break;
		}	
		buff++;
		n++;
		if (n>len){
			break;
		}
	}
	return count;
}

int replace_string(char* old, char*new, int len, char* buff){
	// Creates a copy of the buffer for the existing buffer to reference once a word has been matched. 
	// Once the buffer has copied the new word, it can referecing its copy for the remaining words of the orignal string. 
	int n = 0;
	char* buff_copy = (char*)malloc(len); 
	if (buff_copy == NULL) {
        	printf("Memory allocation failed!\n");
        	exit (99);
    	}
	bool match = NULL;
	memcpy(buff_copy, buff, len);
	while (n<len+1){
		if (*buff == *old){
			char* ref = buff;
			char* reset = old;
			match = true;
			while (*old != '\0'){
				if (*buff != *old){
					match = false;
					break;
				}
				buff++;
				old++;
			}
			if ( match == true ){
				buff = ref;
				old = reset;
				while( *new != '\0'){
					*buff = *new;
					new++;
					buff++;
					n++;
				}
				int overflow = 0;
				while( *buff_copy != ' ' && *buff_copy != '\t' && *buff_copy != '.' ){
					buff_copy++;
					// handles edge case
					overflow++;
					if (n+overflow>len-1){
						break;
					}

				}
				n -= overflow;
				while (n<len){
					*buff = *buff_copy;
					buff++;
					buff_copy++;
					n++;
				}
			}
			else{
				buff = ref;
				old = reset;
			}
		}
		if (n>len){
			return -1;
		}
		buff++;
		buff_copy++;
		n++;
	}
	if (match == true){
		return 0;
	}
	return -1;
}
//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING 
    //      The code checks if at least 2 arguments exist. In OR statements, if the first conditional returns false, the second
    //      conditonal wouldn't be checked. If argc is at least 2, argv[1] will always exist, so accessing argv[1] is always safe.  
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      If argc is less than 3, it means that a string is not provided. "stringfun" should be the first argument. An operation
    //      should be the second argument. A string the operation performs on is the third argument. 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*)malloc(BUFFER_SZ);
    if (buff == NULL){
	    printf("L");
	exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(-1);
    }

    char* pointer = buff;
    switch (opt){

	    case 'c':
		    rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
		    if (rc < 0){
			printf("Error counting words, rc = %d", rc);
			exit(2);
		    }
		    printf("Word Count: %d\n", rc);
		    break;

	    case 'r':
		    // Creates a new buffer. Pointer iterates towards the end of the existing buffer. When the existing buffer is iterated backwards,
		    // it adds each character to the new buffer. 
		    int len = BUFFER_SZ;
		    char* buff_copy = (char*)malloc(len);
		    if (buff_copy == NULL) {
			printf("Memory allocation failed!\n");
			exit (-1);
		    }
		    char* head = buff_copy;
		    int n  = 0;
		    while (*buff != '.' && (n<len)){
			   buff++;
			   n++;
		    }
		    if ( *buff == '.' ){
			    buff--;
		    }
		    while (n>0){
			   *buff_copy = *buff; 
			   buff--;
			   buff_copy++;
			   n--;
		    } 
		    buff = head;
		    break;

		case 'w':
		    	printf( "Word Print\n----------\n");
		    	int count = 0;
			int order = 1;
			while (*(pointer) != '.'){
				if ( (*(pointer) == ' ') ||  (*(pointer) == '\t')){
					if ( count>0 ){
						printf("(%d)\n", count);	
						count = 0;
					}
				}
				else{
					count++;
					if (count == 1){
						printf("%d. ", order);
						order++;
					}
					printf("%c", *pointer);
				}
				pointer++;
			}
			printf("(%d) \n", count);	
			printf("\nNumber of words returned: %d\n", order-1);
			break;

		case 'x':
			if ( argc < 5 ){
				exit(-1);
			}
			char* old = argv[3];
			char* new = argv[4];
			int check = replace_string(old, new, BUFFER_SZ, buff);
			if (check != 0){
				exit (-1);
			}
			break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}
//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          Accessing data to an address beyond the allocated memory range can results in undefined or unexpected behavior.
//          Your code could trigger segmentation faults, and it might be hard to find the line of code at fault. Providing both the pointer
//          and the length can make sure that you don't access beyond the allocated memory range, helping you to avoid segmentation faults 
//          that are hard to trace. It prevents buffer overflow by helping you check bounds for the allocated range. Finally, it can maintain
//          clean coding practices for other software developers who might need to build upon or reference your codee.  
