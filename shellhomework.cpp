

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
  int count = 0;
  int p = 0;
  char ** command = (char **)malloc(strlen(raw) *sizeof(char*));
  if (raw[p] == '|' || raw[p] == '>' || raw[p] == '<'  || raw[strlen(raw)-1] == '|' || raw[strlen(raw)-1] == '>' || raw[strlen(raw)-1] == '<'){
    valid = false;
    return null;
    //cannot end with a operations or begin with an operation
    
  }
  
    cout << "before the for loop!" << endl;
    cout << strlen(raw) << endl;
    string curr = "";
    for (int i = 0; i < strlen(raw) -1; i++){
      cout << "i value is " << i << endl;
      if (raw[i] != '|'){
	cout << "adding on a value " << raw[i] << endl;
	//command = command + raw[i];
	//command[count][p] = raw[i];
	//command[count] += raw[i];
	//curr[p] += raw[i];
	curr += raw[i];
	cout << "current curr length is " << curr.length() << endl;
	//cout << command[count] << endl;
	cout << "here????" << endl;
	if(p == 0 && raw[i] == ' ' ){
	  p--;
	}
	p++;
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
	cout << "after this part" << endl;
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

	//command[count] += '\0';
	//command[count][p] = '\0';
	//p = p +1;
	curr += '\0';
	
	command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
	for ( int i = 0; i < curr.length() - 1;i++){
	  command[count][i] = curr[i];
	}
	p = 0;
	count += 1;
	cout << "---------------------------------" << endl;
	curr = "";
	//tokens[index][argVal] = command;
	cout << "here i am out of the for loop!" << endl;
	//tokens[index] = command;
	//index += 1;
	
	//command = "";
	valid = true;
	hasOut = 0;
	hasIn = 0; 
	starting = 0;
	previousOut = 0;
      }
    }
    cout << "after the loop" << endl;
    if (pipecount == 0){
      //cout << command[count] << endl;
      cout << "why am i inside here" << pipecount << endl;
      //command[count][p] = '\0';
      //command[count] += '\0';
      cout << curr[0] << "first character" << curr.length() << endl;
      curr += '\0';
      command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
	for ( int i = 0; i < curr.length() - 1;i++){
	  command[count][i] = curr[i];
      
	}
      //command[count] = (char *) (strlen(curr) + 1 * sizeof(char));
      //command[count] = curr;
      
      cout << command[0][0] << " first" << endl;
      cout << "it does equal 0!" << endl;
      cout << command[0] << " that was the command" << endl;
      //tokens[index] = command;
     // cout << "what am i doing?" << "index is " << index << tokens[index] << endl;
    }
    cout << "here i am out of the for loop for real!" << endl;
    //return tokens;
    return command;
  
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
  char ** tokens = parseLine(currentLine);
  cout << pipecount << endl;
  cout << *tokens[0] << endl;
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
      cout << "num groups left" << num_token_groups << endl;
      num_token_groups -= 1;
      cout << "before the pid " << endl;
      pid = fork();
      cout << "after da fork" << endl;
      if (pid == 0) {
	cout << "inside ie child" << endl;
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
      cout << "gotta be here" << tokens[current] << endl;
      char * command = tokens[current];
      //cout << command << endl;
      
      //command = trimwhitespace(command);
      //cout << command << endl;
      
      //cout << command << endl;
      //char * args;
      	//string deliminator
	//char ** part = "";
      args = (char**)malloc(sizeof(char*) *50);
      for (int f = 0; f < 50; f++){
	args[f] = (char *) malloc(100 * sizeof(char));
      }
      
      int k = 0;
	int t = 0;
	for (int j = 0; j < strlen(command); j++){
	  cout << k <<"this is the k value or kth arg" << endl;
	  cout << "j balue is " << j << endl;
	  
	  if (command[j] != ' '){
	    //cout << "i mean your right" << endl;
	    if(j == 0 && command[j] != '/'){
	      int l = 0;
	      while(l < 250 && buff[l] != '\0'){
		args[k][t] = buff[l];
		l = l + 1;
		t = t + 1;
	      }
	      args[k][t] = '/';
	      t = t + 1;
	    }
	    args[k][t] = command[j];
	    cout << "added an actual command" << endl;
	    t = t + 1;
	  }
	  else {
	    cout << "we are in here because there is a space" << endl;
	    bool nomoreSpace = true;
	    while(j + 1 < strlen(command) && nomoreSpace){
	      cout << "happened once" << endl;
	      nomoreSpace = false;
	      if(command[j+1] == ' '){
		j = j + 1;
		nomoreSpace = true;
	      }
	      else{
		k += 1;
		t = 0;
		args[k][t] = '\0';
	      }
	    }
	    //args[index][argVal] = part;
	    args[k][t] = '\0';
	    //k += 1;
	    //t = 0;
	    //part = "";
	  }
	  
	  //args[k][t] = '\0';
	  //cout << "before that" << endl;
	  //cout << args << endl;
	  //cout << "command is " << args[0] << endl;
	  
	}
      
      for (int j = 0; j <= k; j++){
	cout << "component is " << args[j] << endl;
	
      }
      cout << "-------------------" << endl;
	k += 2;
      cout << "k value is " << k << endl;
      char * argv[k];
      int w = 0;
      for (int e = 0; e < k; e++){
	
	argv[e] = args[e];
	w = e;
      }
      cout << "w is " << w << endl;
      argv[w] = NULL;
	//args[k+1] = NULL;
      cout << "out of loop!" << endl;
      cout << "command is " << args[0] << endl;
//       cout << "second arg is " << args[1] << endl;
//       cout << "third arg is " << args[2] << endl;
//       cout << " " << endl;
//    

      execvp(argv[0], argv);
      //execv(tokens[current][0], tokens[current]);
	
      }
    else{
      cout << "the parent" << endl;
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
  