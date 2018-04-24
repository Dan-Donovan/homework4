

#include "up.h"
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
#include <regex>


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


int stringToTokens(string command, string* argumentP) {
  cout << " the command is " << command << endl;
  int modified = 0;
   int i = 0;
    int current = 0;
    int beginPosition = command.find_first_not_of("\t ");
    int endPosition = command.find_last_not_of("\t ");
     i = (int) beginPosition;
    if (beginPosition == endPosition && command[beginPosition] != ' ' && command[beginPosition] != '\t') {
	argumentP[0] = command[beginPosition];
  
      return -1;
    }

    while (i < endPosition) {

      while ((command[i] == ' ' || command[i] == '\t')) {
	i += 1;

	}

      int tokenBeginIndex = i;

      while (command[i] != ' ' && command[i] != '\t' && i <= endPosition){
	i += 1;
	}

      int tokenEndIndex = i;
      string token = command.substr(tokenBeginIndex,  (tokenEndIndex - tokenBeginIndex));
      cout << token << " the current token" << endl;
      argumentP[current] = token;

      if (token == ">" || token == "<"){
	modified = current;
      }
      current += 1;
    }
    current -= 1;

//cout << "number of tokens is " << current << endl;
if (modified != 0){
  //cout << "ACTUAL NUMBER WITH REDIRECT " << modified << endl;

}
  return current;

}

char ** parseLine(char * raw){

  valid = true;
  int index = 0;
  int argVal = 0;
  int size = 100;
  char **tokens = (char **)malloc(size *sizeof(char*));

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
    cout << "INVALID STRING" << endl;
    return null;

  }
  

    string curr = "";
    for (int i = 0; i < strlen(raw); i++){
      
      if (raw[i] != '|'){
	if(p == 0 && raw[i] == ' ' ){
	  p--;
	}
	else {
	  curr += raw[i];
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
	
	curr += '\0';
	
	command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
	for ( int i = 0; i < curr.length() - 1;i++){
	  
	  command[count][i] = curr[i];
	}
	p = 0;
	count += 1;
	curr = "";

	cout << "here i am out of the for loop!" << endl;
	
	valid = true;
	hasOut = 0;
	hasIn = 0; 
	starting = 0;
	previousOut = 0;
      }
    }

      curr += '\0';
      cout << "curr value is " << curr << endl;
      command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
      
	for ( int i = 0; i < curr.length() - 1;i++){
	  command[count][i] = curr[i];
      
	}
	cout << command[count] << " THIS SHOULD BE THE SAME " << endl;
    return command;
  
}

    
  


int main(int argF, char ** argv) {
  
  ifstream givenFile;
  
  if (argF > 1){
    givenFile.open(argv[1]);
  }
  
  
  char  buff[300];
  if (getcwd(buff,250) == NULL){
    //cout << "lolll" << endl;
  }
  commands:
  do  {
    cout << "Start of loop" << endl;
  char currentLine[110];
  string fullLine = "";
  if (argF > 1) {
    char x;
    
    while ( givenFile.get(x) && x != '\n'){
      fullLine += x;
    }
    
      }
   else {
	getline(cin, fullLine);
   }

    if (fullLine == "exit") {
      pipecount = 0;
      break;
      }
      
  
   if( argF > 1 && givenFile.eof()){
     exit(0);
   }
  if (fullLine.length() > 100 ){
    cout << "invalid input" << endl;
    goto commands;
  }
  
  fullLine = fullLine + " ";
  int y = 0;
  for (y = 0; y < fullLine.length(); y++){
    currentLine[y] = fullLine[y];
  }
  currentLine[y] = '\0';
  
  cout << "before the current commands" << endl;
  
  char ** tokens = parseLine(currentLine);
  cout << "current line is " << currentLine <<endl;
 
  int p_id = 0;
 
  int num_token_groups = pipecount + 1;
  int limit = pipecount + 1;
  int pipes[2];

   int pid_status[pipecount + 1];
   int in = 0;
  for (int current = 0; current < limit; current++){


    string inputFile = "";
    string outputFile = "" ;
    string tokenized[101];
    char * command = tokens[current];
    std::string strcommand(command);
    cout << "here hahaha" << endl;
    int a = stringToTokens(strcommand, tokenized);
    cout << "a " <<  a << endl;
    int size = a+1;
    int realSize = 0;
    
    cout << tokenized[0] << " " << tokenized[1] << " " << tokenized[2] << " " << tokenized[3];
    pipe(pipes);

	cout << size << " is " << endl;
        cout << "current command index is " << current << endl;
    cout << "pipes 0 and pipes 1 " << pipes[0] << " " << pipes[1] << endl;
    
    for (int j = 0; j < size; j++){
      
	   bool contains_not_approved = (tokenized[j].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
	   if(contains_not_approved == false){
	     cout << "invalid input" << endl;
	     goto commands;
	   }
	   cout <<  "WHY ARE WE NOT IN THIS LOOP" << endl;
	   if (j == 0){
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
	   if (tokenized[j] == "<") { 
	      if (current != 0) {
		cerr << "wrong pipe" << endl;
		}

	      if (j + 1 < size) {
		realSize = j;
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
		   
		  cout << inputFile << "was it appended correctly" << endl;
		  j++;
    
		}
	
	   }
	 else if (tokenized[j] == ">"){
	   if (current !=  limit - 1) {
	      cerr << "error error error " << endl;
	      }

	   if (j + 1 < size) {
	     realSize = j;
	     cout << "current tokenized value is " << tokenized[j + 1] << endl;
	      outputFile = tokenized[j+1];
	      if (outputFile[0] != '/'){
	      int l = 0;
	      string cwd = "";
 	      while(l < 250 && buff[l] != '\0'){
		cwd = cwd + buff[l]; 
 		
	        l = l + 1;   
	      }
	      cout << "current output file is " << outputFile << endl;
	      outputFile = cwd + "/" + outputFile;
	      cout << outputFile << "was it appended correctly" << endl;
	      }
	      
	      j++;
	      }
}
	else {}
	  }
      if (realSize != 0){
	size = realSize;
      }


    pid = fork();
      cout << pid << " after da fork" << endl;
      if (pid == 0) {

	char ** args = (char**)malloc(sizeof(char*) *50);
	for (int f = 0; f < size + 1; f++){
	  args[f] = (char *) malloc(100 * sizeof(char));
	}
	
	char * argv[size + 1];
	int element = 0;
	char * ready = "";
	cout << "size for iterating is " << size << endl;
	for( element = 0; element < size; element++){

	    string ex = tokenized[element];
	    if (ex == ">" || ex == "<" ){
	      cout << "SHOULD NEVER SEE THIS" << endl;
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
	cout << argv[0] << " sfsfS " << argv[1] << endl;
	
	if (current == 0){
	  cout << "piping in from startFile" << endl;
	 if (inputFile.length() > 0) {
		cerr << "time for some FILE INPUT" << endl;
		int iF = open(inputFile.c_str(), O_RDONLY);
		if (iF == -1) {
		  cerr << strerror(errno) << endl;	  
		  } 
		dup2(iF, STDIN_FILENO);
		cout << "after the dup" << endl;
	}
	  cout << "true love" << endl;
	
	}
      
	if (pipecount > 0){
	  	
	
	if (current > 0  ){
	  cout << "second command piper " << command << endl;
	  dup2(in, STDIN_FILENO);

	  close(pipes[0]);
	  cerr << "LOL" << endl;
	}
	
	if ( current < num_token_groups - 1){
	  cerr << "set up write pipe" << endl;
	  dup2(pipes[1], STDOUT_FILENO);
	  close(pipes[1]);
	    cerr << "made it here" << endl;

	}
	
	}
	
	if (current == num_token_groups - 1){
	  cerr << "inside the belly of the beast" << endl;
	  if (outputFile.length() > 0){
	    cerr << "FILE OUTPUTING TIME" << endl;
	    int we_oF = open(outputFile.c_str(), O_WRONLY|O_CREAT);
	    dup2(we_oF,STDOUT_FILENO);
	  }
	  
	  cerr << " tried to close " << endl;

	}
      cerr << "------------------------------------------------------------------" << endl;
      int exVal = -1;
       cerr << "args are " << argv[0] << " " << argv[1] << " "  << argv[2] << " " << argv[3] << endl;
      cerr << "fsdfsf" << endl;
    
      int h = 0;
      h = execvp(argv[0], argv);
      perror("exec failure");
      exit(h);
      if (h == -1 ){
	cerr << ":(" << endl;
      }
      else{
      cerr << "whelp i execed " << endl;
      
      }
      }
    else{
      cout << "pid value is " << p_id << " pid value is " << pid << endl;
      
      close(pipes[1]);
      in = pipes[0];
      cout << "the parent" << endl;
      
      waitpid(pid, &pid_status[p_id],0);
      cout << "finished eaiting" << endl;
      p_id++;
    }
  }
    for(int d = 0; d <= pipecount; d++){
      cout << "Process " << " exited with status " << pid_status[d] << endl;
    }
  
	  
  } while(1);
  return 0;
}
      