/*
 * Machine Problem #4 - Writing A Simple Shell
 * 
 * CS4414 Operating Systems
 * 
 * Dan Donovan
 * 
 * 
 * up.cpp - c++ file containing functionality for taking input from stdin and determining if it a valid command / list of command and then executes those commands. This support file reidirection and piping
 * Dan Donovan Spring 2018 
 * 
 * The following code display my implementation for parsing commands and redirection, piping and executing different commands within the input. It includes my chosen implementation for various helper methods and pipe usage.
 * 
 * 
 * 	COMPILE: make 
 * 	OBJECTS: up.o
 * 
 * 	it creates the executable ./msh excutable can take stdin or read from a file a list of commands
 * 
 * 	MODIFICATIONS:
 * 		4/16 - simple uni-command file excution without piping or redirection 
 * 		4/17 - more uni-command support and validation for different inputs and incorrectly 	formatted lines of input
 * 		4-20 - file redirection functionality 
 * 		4-23 - pipe functionality and further input validation and execution 
 * 		4-24 - final series of input validation and checking
 * 
 * 
 * 
 */





#include <stdint.h>
#include <stdlib.h>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>



#define null 0;
using namespace std;

/* Global Declarations */
  bool valid; /* boolean valid that holds if a token is validly set up */
  int pipecount; /* number of pipes in the input string */
  int * errorStatus; /* int[] for the various errors and exit codes */
  char ** tokens; /* char 2d array to hold partly sanitized command and args list from input */
  char operators[3] = {'>','<','|'}; /* type of valid operators */
  int pidstatus; /* status of a progress from execvp */
  char * cwd; /* current working directory */
  char ** args; /* optional 2d array to hold args for execvp command */
  pid_t pid; /* value used to iterate through erorstatus array */
  pid_t pids[50] = {0}; /* array of pids initialized all to 0 */

/* helper function to santize each command and its associated arguments, returns NULL if any
 invalid commands*/
int itemizer(string command, string* argumentP) {
  int modified = 0; /* if list has file redirection */
   int i = 0;
    int current = 0;
    int start = command.find_first_not_of("\t "); /* first index of command that isnt a string */
    int end = command.find_last_not_of("\t "); /* last index of command that isnt a string  */
    i = start;
    if ( start == -1){
      return -1;
    }
    if (start == end && command[start] != ' ' && command[start] != '\t') { /* if string is only 1 character long but is valid */
	argumentP[0] = command[start];
  
      return current + 1;
    }
    if (start == end && (command[start] == ' ' || command[start] == '\t')){
      return -1;
    }
    if (command[start] == '|' || command[start] == '>' || command[start] == '<' || command[end] == '|' || command[end] == '>' || command[end] == '<'){
      return -1; /* invalid placement of a pipe or redirection operator */
    }  

    while (i < end) {

      while ((command[i] == ' ' || command[i] == '\t')) {
	i += 1;
	/* get rid of redundant middle spaces */
	}

      int startOfentry = i;

      while (command[i] != ' ' && command[i] != '\t' && i <= end){
	i += 1;
	}
      /*  token has not been indexed */
      int endOfentry = i;
      string token = command.substr(startOfentry,  (endOfentry - startOfentry));
      argumentP[current] = token; /* put that token in the current index of the string array */

      if (token == ">" || token == "<"){
	modified = current; /* the correct amount of operators excluding the redirection */
      }
      current += 1;
    }
    current -= 1;
  return current; /* return the current index of the last command */

}

/* helper function that takes the raw input and does various input validation, and splits the line on "|" and places result, if valid, in a 2D array */
char ** parseLine(char * raw){
  /* various  indexes, pipecount and file descriptor counters */
  valid = true;
  int index = 0;
  int argVal = 0;
  int size = 100;
  int hasOut = 0;
  int hasIn = 0; 
  int starting = 0;
  int previousOut = 0;
  pipecount = 0;
  int count = 0;
  int p = 0;
  char ** command = (char **)malloc(strlen(raw) *sizeof(char*));
  if (raw[p] == '|' || raw[p] == '>' || raw[p] == '<'  || raw[strlen(raw)-1] == '|' || raw[strlen(raw)-1] == '>' || raw[strlen(raw)-1] == '<'){
    /* if the first/last character of a string is a pipe or redirection return invalid string */
    valid = false;
    return NULL;

  }
  

    string curr = "";
    /* for each character in the string  */
    for (int i = 0; i < strlen(raw); i++){
      /* if the character is not a pipe  */
      if (raw[i] != '|'){
	if(p == 0 && raw[i] == ' ' ){
	  p--;
	}
	else {
	  /* add the character to the current string  */
	  curr += raw[i];
	}
	p++;
	
	if(raw[i] == '>'){
	  hasOut += 1; 
	  if (hasOut > 1){
	    return NULL;
	  }
	  previousOut = 1;
	}
	if(raw[i] == '<'){
	  hasIn += 1;
	  if (hasIn > 1){
	    return NULL;
	  }
	  /* if file redirection is in wrong place */
	  if (pipecount != 0) {
	    return NULL;
	    valid =false;
	  }
	}
      }
      /* otherwise the character is a pipe character */
      if(raw[i] == '|'){
	  if (raw[i-1] != ' '){
	    /* if the previous character is not a string then it is invalid input */
	    valid = false;
	    return NULL;
	  }
	  if (raw[i+1] != ' '){
	    valid = false;
	    return NULL;
	  }
	  pipecount += 1;
	  /* if there are two many file redirections  */
	  if (hasOut > 1 || hasIn > 1 || (hasOut > 0 &&  hasIn > 0)){
	    valid = false;
	    return NULL;
	  }
	  /* if file redirection from output is incorrect it is invalid */
	  if (previousOut == 1){
	    valid = false;
	    return NULL;
	  }
	/* null terminate the string */
	
	curr += '\0';
	/* for the current index of the 2d array allocate an accurate amount of memory and copy from the current string into this index */
	command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
	for ( int i = 0; i < curr.length();i++){
	  
	  command[count][i] = curr[i];
	}

	/* recent most of the flags, the curr string, and add 1 to the current count */
	p = 0;
	count += 1;
	curr = "";
	valid = true;
	hasOut = 0;
	hasIn = 0; 
	starting = 0;
	previousOut = 0;
      }
    }

      /* if there is no pipes or it is last command */      
       if (curr.length() < 1){
 	return NULL;
       }
      
      command[count] = (char *) malloc((curr.length() + 1) * sizeof(char));

      int m = 0;
      /* malloc space in the 2d char array for this last command */
	for ( m = 0; m < curr.length();m++){
	  command[count][m] = curr[m];

	}
	command[count][m] = '\0';
    /* return the 2d char array */
    return command;
  
}

    
/* main method -- can take in a input file an argument if one does not want to use standard in */
int main(int argF, char ** argv) {
  
  ifstream givenFile;
  /* if a file was passed as an argument for the input */
  if (argF > 1){
    givenFile.open(argv[1]);
  }
  
  /* set up a buffer and get the name of the current working directory */
  char  buff[300];
  if (getcwd(buff,250) == NULL){

  }
  
  /* start of do while loop  */
  commands:
  do  {
  /* set up the current line and clear it each time the loop is called  */
  char currentLine[110];
  memset(&currentLine[0],0,110);
  string fullLine = "";
  if (argF > 1) {
    char x;
    /* if reading from file, get each character before a new line  */
    while ( givenFile.get(x) && x != '\n'){
      fullLine += x;
    }
      }

   else {
         /* otherwise, read from standard in  */
	getline(cin, fullLine);
   }
    if (cin.eof() == true){
      break;
    }
    /* if the line reads "exit" then escape the shell  */
    if (fullLine == "exit") {
      pipecount = 0;
      break;
      }
      
      
  
   if( argF > 1 && givenFile.eof()){
     break;
     exit(0);
   }
   /* if the length of the line is greater than 100 characters the line is too long and is considered invalid input  */
  if (fullLine.length() > 100 ){
    cout << "invalid input" << endl;
    goto commands;
  }
  
  
  int length = fullLine.length();
  int y = 0;
  for (y = 0; y < fullLine.length(); y++){
    /* get the length of the line and then null terminate it*/
    currentLine[y] = fullLine[y];
  }
  currentLine[y] = '\0';
  
  /* call and return the parseLine helper function  */
  char ** tokens = parseLine(currentLine);
  if (tokens == NULL){
    cout << "invalid input" << endl;
    goto commands;
  }

  /* set the initial p_id index */
  int p_id = 0;
  int num_token_groups = pipecount + 1; /* number of distinct tokens groups  */
  int limit = pipecount + 1;
  int pipes[2]; /* set up the pipes array */

   int pid_status[pipecount + 1]; /* set up the pid status array */
   int in = 0;
   /* for each distrinct token group in the command  */
  for (int current = 0; current < limit; current++){

    /* initialized the inputfile and outputfile strings */
    string inputFile = "";
    string outputFile = "" ;
    string tokenized[101];
    /* get and convert the current token group from the 2d array */
    char * command = tokens[current];
    std::string strcommand(command);

    /* call the second helper function to further sanitize and separate input into token groups */
    int a = itemizer(strcommand, tokenized);
    if(a == -1){
      /* if invalid then go to next input line and output invalid input */
      cout << "invalid input" << endl;
      goto commands;
    }

    int size = a+1;
    int realSize = 0;
    /* pipe the pipes array */
    pipe(pipes);

    /* for each character in the current token group  */
    for (int j = 0; j < size; j++){
	  /* check to make sure a token is one of the valid perscribed tokens allowed */
	   if(tokenized[j] != ">" && tokenized[j] != "<"){
	   bool contains_not_approved = (tokenized[j].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
	   /* if it is not, exit */
	   if(contains_not_approved == false){
	     cout << "invalid input" << endl;
	     goto commands;
	   }
	   
	   }

	   if (j == 0){
	     /* get the first character and if it is not "/" that it must be 
	      a relative command, and the cwd needs to be prepended to it*/
	     string command = tokenized[j];
	     if (command[0] != '/'){
	       int l = 0;
	       string cwd = "";
	       while(l < 250 && buff[l] != '\0'){
		    cwd = cwd + buff[l];
	
		    l = l + 1;   
		    }
		    command = cwd + "/" + command;
		    tokenized[j] = command;
		   }
	   }
	   /* if redirection to std in, make sure it is valid and file name is correct */
	   if (tokenized[j] == "<") { 
	      if (current != 0) {
		cout << "invalid input" << endl;
		}

	      if (j + 1 < size) {
		if (realSize == 0) {
		realSize = j;
		}
		inputFile = tokenized[j + 1];
		  if (inputFile[0] != '/'){
		  int l = 0;
		  string cwd = "";
		  while(l < 250 && buff[l] != '\0'){
		    cwd = cwd + buff[l];
	
		    l = l + 1;   
		    }
		    inputFile = cwd + "/" + inputFile;
		   }
		    bool inputfile_contains_not_approved = (inputFile.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
		    
	   if(inputfile_contains_not_approved == false){
	     cout << "invalid input" << endl;
	     goto commands;
	   } 
		  j++;
    
		}
	
	   }
	   /* if it is output redirection, make sure it is valid and file name is correct and valid as well  */
	 else if (tokenized[j] == ">"){
	   if (current !=  limit - 1) {
	      cout << "invalid input" << endl;
	      }

	   if (j + 1 < size) {
	     if(realSize == 0){
	     realSize = j;
	     }
	      outputFile = tokenized[j+1];
	      if (outputFile[0] != '/'){
	      int l = 0;
	      string cwd = "";
 	      while(l < 250 && buff[l] != '\0'){
		cwd = cwd + buff[l]; 
 		
	        l = l + 1;   
	      }
	      bool outputfile_contains_not_approved = (outputFile.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
	      
	   if(outputfile_contains_not_approved == false){
	     cout << "invalid input" << endl;
	     goto commands;
	   }
	      outputFile = cwd + "/" + outputFile;
	      
	      }
	      
	      j++;
	      }
}
	else {}
	  }
	  
	  /* if there was redirection then I have to modify the number of tokens in this group */
      if (realSize != 0){
	size = realSize;
      }

    /* fork */
    pid = fork();
      /* if child process */
      if (pid == 0) {

	 /* set up the args array for the execvp command for later */
	char ** args = (char**)malloc(sizeof(char*) *50);
	for (int f = 0; f < size + 1; f++){
	  args[f] = (char *) malloc(100 * sizeof(char));
	}
	
	char * argv[size + 1];
	int element = 0;
	char * ready = "";
	
	for( element = 0; element < size; element++){

	    string ex = tokenized[element];
	    if (ex == ">" || ex == "<" ){
	      
	      break;
	    }
	    int g = 0;
	    for (g = 0; g < ex.length(); g++){
	      args[element][g] = ex[g];
	    }
	   
	    args[element][g] = '\0';
	    argv[element] = args[element];
	}
	argv[element] =  '\0';
	
	/* if it is the first token in the group */
	if (current == 0){ 
	  /* if there is redirection */
	 if (inputFile.length() > 0) {
		
		int iF = open(inputFile.c_str(), O_RDONLY);
		if (iF == -1) {
		 // cerr << strerror(errno) << endl;	  
		  } 
		  /* make sure to take input from this file  */
		dup2(iF, STDIN_FILENO);
		
	}
	
	}
	/* if there are pipes we need to handling piping */
	if (pipecount > 0){
	  	
	/* if not the first command in the series  */
	if (current > 0  ){
	  /* get input from the previous pipe */
	  dup2(in, STDIN_FILENO);

	  close(pipes[0]);
	}
	/* if not the last command in the series  */
	if ( current < num_token_groups - 1){
	  /* write the output the write end of the pipe */
	  dup2(pipes[1], STDOUT_FILENO);
	  close(pipes[1]);

	}
	
	}
	/* if it is the last input in the series */
	if (current == num_token_groups - 1){
	  /* if there is an output file */
	  if (outputFile.length() > 0){
	    int we_oF = open(outputFile.c_str(), O_WRONLY|O_CREAT);
	    if (we_oF == -1) {
		//  cerr << strerror(errno) << endl;	  
		  } 
		  /* output into that file  */
	    dup2(we_oF,STDOUT_FILENO);
	  }
	  


	}
      
      int exVal = -1;
    
      int h = 0;
      /* exec the command with the given arguments that were formatted above */
      h = execvp(argv[0], argv);
      /* exit if there is an error */
      exit(h);
      }
    else{
      /* parent process */
      
      /* close the write end of pipe and get the read end of pipe to connect to next iteration through loop if needed  */
      close(pipes[1]);
      in = pipes[0];
      
      /* call wait id to wait for the child process to finish */
      waitpid(pid, &pid_status[p_id],0);
      p_id++;
    }
  }
     /* print out each of the valid commands exit codes */
    for(int d = 0; d <= pipecount; d++){
      cerr << pid_status[d] << endl;
    }
  
    free(tokens); /* free up the 2d char array */
  } while(1);
  /* end of method main */
  return 0;
}
      