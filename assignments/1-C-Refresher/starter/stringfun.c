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
    while (n < len){
	if (n > 0 && ( *user_str == ' ' || *user_str == '\t' )){
		if (*(user_str - 1) == ' ' || *(user_str - 1) == '\t'){
			while ( *user_str == ' ' || *user_str == '\t' ){
				user_str++;
			}
		}
    	}
	if (*user_str == '\0'){
		*buff = '.';	
		count = n;
	}
	else{
		*buff = *user_str;
		user_str++;
	}
	buff++;
	n++;
    }
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
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
	}
	return count;
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
        exit(2);
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
		    while (*pointer != '.'){
			   pointer++;
		    }
		    printf("Reversed String:");
		    while (pointer != buff){
			   pointer--;
			    printf("%c", *pointer);
		    } 
		    break;

		case 'w':
		    	int count = 0;
			while (*(pointer) != '.'){
				if ( (*(pointer) == ' ') ||  (*(pointer) == '\t')){
					if ( count>0 ){
						printf(" (%d) \n", count);	
						count = 0;
					}
				}
				else{
					count++;
					printf("%c", *pointer);
				}
				pointer++;
			}
				printf(" (%d) \n", count);	
			break;

		case 'x':
			if ( argc < 5 ){
				exit(1);
			}
			char* old = argv[3];
			char* new = argv[4];
			while (*pointer != '.'){
				if ( *pointer == *old ){
					char* ref = pointer;
					char* reset = old;
					bool match = true;
					while ( *old != '\0' ){
						if ( *pointer != *old ){
							match = false;
							break;
						}
						// printf( " %c %c ", *pointer, *old);
						pointer++;
						old++;
					}
					if ( match == true ){
						// printf("success");
						while( *new != '\0'){
							printf( "%c", *new); 
							new++;
						}
						old = reset;
					}
					else{
						pointer = ref;
						old = reset;
					}
				}
				printf( "%c", *pointer );
				pointer++;
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
    exit(0);
}
//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
