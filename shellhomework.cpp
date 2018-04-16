

#include "shellhomework.h"
#include <stdint.h>
#include <stdlib.h>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <math.h>
#include <unistd.h>

using namespace std;

/* Global Declarations */
// int * arr; /* Array used to hold all initial inputs, and intermediary and final max values */
// int count; /* Number of inputs N */
// int thread_count; /* Number of threads used */
// 
  bool valid; // is at a value series of commands
  int * pipecount;
  int * errorStatus;
  char ** tokens;
  char operators[3] = {'>','<','|'}
  int pidstatus;
  char * cwd;
  int ** pipearray;
  pid_t pid;
  pid_t pipeid;
char * inputParsing() {
  fflush(stdout)
  int index = 0;
  char * inputbuffer = malloc(sizeof(char) * 100);
  
  char current = "";
  while(True){
    current = getchar();
    
    if (current == EOF || c == '\n' ){
      inputbuffer[index] = '\0';
      inputbuffer = trimwhitespace(inputbuffer)
      return inputbuffer;
    }
    else {
      inputbuffer[index] = current;
      index += 1;
    }
    if (index == 100){
      cerr << "line size exceeds 100" << endl;
      //check this out 
      exit(EXIT_FAILURE);
    }
    
  
}

}

char *trimwhitespace(char *str){
  char *end;
// Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0){ // All spaces?
    return str;

  }// Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)){
    end--;
  }
// Write new null terminator
  *(end+1) = '\0';
  return str;
}

char ** parseLine(char * raw){
  valid = True;
  int index = 0;
  int size = 100;
  char **tokens = malloc(size *sizeof(char*));
  char * currtoken;
  int hasOut = 0;
  int hasIn = 0; 
  int starting = 0;
  int previousOut = 0;
  if (raw[0] == '|' || raw[0] == ">" || raw[0] == "<"  || raw[strlen(raw)-1] == "|" || raw[strlen(raw)-1] == ">" || raw[strlen(raw)-1] == "<"){
    valid = False;
    return null;
    //cannot end with a operations or begin with an operation
    
  }
  

    for (int i = current; i < strlen(raw); ++i){
      
      if (raw[i] != "|"){
	command = command + raw[i];
	if(raw[i] == ">"){
	  hasOut += 1; 
	  previousOut = 1;
	}
	if(raw[i] == "<"){
	  hasIn += 1;
	  if pipecount != 0 {
	    valid =False;
	  }
	}
      }
      
      if(raw[i] == "|"){
	  if (raw[i-1] != " "){
	    valid = False;
	  }
	  pipecount += 1;
	  if (hasOut > 1 || hasIn > 1 || (hasOut > 0 &&  hasIn > 0)){
	    valid = False
	  }
	  if (previousOut == 1){
	    valid = False;
	  }
	tokens[index] = command;
	index += 1;
	string command = "";
	valid = True;
	int hasOut = 0;
	int hasIn = 0; 
	int starting = 0;
	int previousOut = 0;
      }
    }
    
    
    
    
      if raw[i] !
  currtoken = strtok(raw, "|");
  while (currtoken != NULL){
    tokens[index] = currtoken;
    
    index += 1;
    
    currtoken = strtok(NULL, "|");
  
  }
  
  tokens[index] = NULL;
  
  
  
  tokens[index] = '\0';
  return tokens;
    
  }
}
int main() {

  cwd = getcwd();
  
  while (1) {
  // time to read a line of input
  char * currentLine = inputParsing();
  char ** currentCommands = parseLine(currentLine);

  pipe_array = malloc(sizeof(int*) * pipecount);
  for (pipeid = 0; pipeid < pipecount; pipeid++) {
  pipearray[pipeid] = malloc(sizeof(int[2]));
    if (pipe(pipe_array[pipe_idx]) != 0){
      perror("pipe()");
    }
    }
  //parse the line
  //for each command in the line {
  int num_token_groups = pipecount + 1;
  while(num_token_groups > 0){
    num_token_groups -= 1;
    pid = fork();
    if (pid == 0) {
      
      if (num_token_groups == pipecount){
	close pipearray
      }
      
      do redirection stuff
	execve ( command, args , ...);
	oops, why did exec fail?
    } 
    else {
    store pid somewhere
    }
    }
  for each command in the line {
    waitpid (stored pid, &status);
    check return code placed in status;
  } 
  

  
  