

#include "shellhomework.h"
#include <stdint.h>
#include <stdlib.h>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define null 0;
using namespace std;

/* Global Declarations */
// int * arr; /* Array used to hold all initial inputs, and intermediary and final max values */
// int count; /* Number of inputs N */
// int thread_count; /* Number of threads used */
// 
  bool valid; // is at a value series of commands
  int pipecount;
  int * errorStatus;
  char ** tokens;
  char operators[3] = {'>','<','|'};
  int pidstatus;
  char * cwd;
  int ** pipearray;
  char ** args;
  pid_t pid;
  pid_t pipeid;
  pid_t pids[50] = {0};
  int pid_status = 0;
// char * inputParsing() {
//   fflush(stdout)
//   int index = 0;
//   char * inputbuffer = malloc(sizeof(char) * 100);
//   
//   char current = "";
//   while(True){
//     current = getchar();
//     
//     if (current == EOF || c == '\n' ){
//       inputbuffer[index] = '\0';
//       inputbuffer = trimwhitespace(inputbuffer)
//       return inputbuffer;
//     }
//     else {
//       inputbuffer[index] = current;
//       index += 1;
//     }
//     if (index == 100){
//       cerr << "line size exceeds 100" << endl;
//       //check this out 
//       exit(EXIT_FAILURE);
//     }
//     
//   
// }
// 
// }

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
  cout << "inside belly of the beast" << endl;
  valid = true;
  int index = 0;
  int argVal = 0;
  int size = 100;
  char **tokens = (char **)malloc(size *sizeof(char*));
  //char * currtoken;
  int hasOut = 0;
  int hasIn = 0; 
  int starting = 0;
  int previousOut = 0;
  pipecount = 0;
  char * command = "";
  if (raw[0] == '|' || raw[0] == '>' || raw[0] == '<'  || raw[strlen(raw)-1] == '|' || raw[strlen(raw)-1] == '>' || raw[strlen(raw)-1] == '<'){
    valid = false;
    return null;
    //cannot end with a operations or begin with an operation
    
  }
  
    cout << "before the for loop!" << endl;
    cout << strlen(raw) << endl;
    for (int i = 0; i < strlen(raw); i++){
      cout << "i value is " << i << endl;
      if (raw[i] != '|'){
	command = command + raw[i];
	if(raw[i] == '>'){
	  hasOut += 1; 
	  previousOut = 1;
	}
	if(raw[i] == '<'){
	  hasIn += 1;
	  if (pipecount != 0) {
	    valid =false;
	  }
	}
      }
      
      if(raw[i] == '|'){
	  if (raw[i-1] != ' '){
	    valid = false;
	  }
	  pipecount += 1;
	  if (hasOut > 1 || hasIn > 1 || (hasOut > 0 &&  hasIn > 0)){
	    valid = false;
	  }
	  if (previousOut == 1){
	    valid = false;
	  }
	
// 	//string deliminator
// 	string part = "";
// 	int k = 0;
// 	for (int j = 0; j < strlen(command); j++){
// 	  if (command[j] != ' '){
// 	    part += command[j];
// 	  }
// 	  else {
// 	    tokens[index][argVal] = part;
// 	    argVal += 1;
// 	    part = "";
// 	  }
// 	  
// 	}
	
	//tokens[index][argVal] = command;
	cout << "here i am out of the for loop!" << endl;
	tokens[index] = command;
	index += 1;
	
	command = "";
	valid = true;
	hasOut = 0;
	hasIn = 0; 
	starting = 0;
	previousOut = 0;
      }
    }
    if (pipecount == 0){
      cout << "it does equal 0!" << endl;
      tokens[index] = command;
    }
    cout << "here i am out of the for loop for real!" << endl;
    return tokens;
}

    
  /*  
    
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
}*/


int main() {
  char  buff[300];
  //cwd = getcwd(buff,250);
  if (getcwd(buff,250) == NULL){
    cout << "lolll" << endl;
  }
  cout << buff << endl;
  
  while (1) {
  // time to read a line of input
  //char * currentLine = inputParsing();
  char currentLine[110];
  if ((fgets(currentLine, sizeof(currentLine), stdin) == NULL) ){
    break;
  }
  
  if (strlen(currentLine) > 100){
    cout << "invalid" << endl;
  }
  cout << "before the current commands" << endl;
  char ** currentCommands = parseLine(currentLine);
  cout << pipecount << endl;
  if (pipecount > 0) {
    pipearray = (int **) malloc(sizeof(int*) * (pipecount));
    cout << "here??" << endl;
    for (pipeid = 0; pipeid < pipecount; pipeid++) {
      pipearray[pipeid] = (int*)malloc(sizeof(int[2]));
      if (pipe(pipearray[pipeid]) != 0){
	perror("pipe()");
      }
      }
  }
  cout << "the next hurtle" << endl;
  //parse the line
  //for each command in the line {
  
  int pipeid = 0;
  int p_id = 0;
  int current = 0;
  int num_token_groups = pipecount + 1;
    while(num_token_groups > 0){
      num_token_groups -= 1;
      cout << "before the pid " << endl;
      pid = fork();
      cout << "after da fork" << endl;
      if (pid == 0) {
	if (pipecount > 0){
	  if (num_token_groups > 0){
	    if (num_token_groups == pipecount) {
	      close(pipearray[pipeid][0]);
	      dup2(pipearray[pipeid][1], STDOUT_FILENO);
	    }
	  
	    else {
	      close(pipearray[pipeid][0]);
	      dup2(pipearray[pipeid][1], STDOUT_FILENO);
	      close(pipearray[pipeid - 1][1]);
	      dup2(pipearray[pipeid - 1][0], STDIN_FILENO);
	    }
	    }
	else{
	  //last token
	  close(pipearray[pipeid - 1][1]);
	  dup2(pipearray[pipeid - 1][0], STDIN_FILENO);
	  
	}
	}
      
      char * command = tokens[current];
      //char * args;
      	//string deliminator
	//char ** part = "";
	int k = 0;
	
	for (int j = 0; j < strlen(command); j++){
	  if (command[j] != ' '){
	    if(j == 0 && command[j] != '/'){
	      int l = 0;
	      while(l < 250 && buff[l] != '\0'){
		args[k] += buff[l];
	      }
	      args[k] += '/';
	    }
	    args[k] += command[j];
	    
	  }
	  else {
	    while(j + 1 < strlen(command)){
	      if(command[j+1] == ' '){
		j = j + 1;
	      }
	    }
	    //args[index][argVal] = part;
	    k += 1;
	    //part = "";
	  }
	  cout << "command is " << args[0] << endl;
	  
	}
      cout << "command is " << args[0] << endl;
      execv(args[0], args);
      //execv(tokens[current][0], tokens[current]);
	
      }
    else{
      pipeid++;
      pids[p_id] = pid;
      p_id++;
      
	//if (
  //       for (j = 0; j < pipe_count; j++) {
  // 	close(pipe_array[j][0]);
  // 	close(pipe_array[j][1]);
  //       }
    }
    for (p_id = 0; p_id <= pipecount; p_id++) {
      pid_status = 0;
      waitpid(pids[p_id], &pid_status,0);
      cerr << "error status" << pid_status << endl;
	}
	  }
  }
  return 0;
}
      
    
    
//       do redirection stuff
// 	execve ( command, args , ...);
// 	oops, why did exec fail?
//     } 
//     else {
//     store pid somewhere
//     }
//     }
//   for each command in the line {
//     waitpid (stored pid, &status);
//     check return code placed in status;
//   } 
//   
// 
//   
  