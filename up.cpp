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



#define null 0;
using namespace std;

/* Global Declarations */
  bool valid; 
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


int itemizer(string command, string* argumentP) {
  int modified = 0;
   int i = 0;
    int current = 0;
    int start = command.find_first_not_of("\t ");
    int end = command.find_last_not_of("\t ");
     i = (int) start;
    if (start == end && command[start] != ' ' && command[start] != '\t') {
	argumentP[0] = command[start];
  
      return current + 1;
    }
    if (command[start] == '|' || command[start] == '>' || command[start] == '<' || command[end] == '|' || command[end] == '>' || command[end] == '<'){
      return -1;
    }  

    while (i < end) {

      while ((command[i] == ' ' || command[i] == '\t')) {
	i += 1;

	}

      int startOfentry = i;

      while (command[i] != ' ' && command[i] != '\t' && i <= end){
	i += 1;
	}

      int endOfentry = i;
      string token = command.substr(startOfentry,  (endOfentry - startOfentry));
      argumentP[current] = token;

      if (token == ">" || token == "<"){
	modified = current;
      }
      current += 1;
    }
    current -= 1;

if (modified != 0){


}
  return current;

}

char ** parseLine(char * raw){

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
    valid = false;
    return NULL;

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
	    return NULL;
	    valid =false;
	  }
	}
      }
      
      if(raw[i] == '|'){
	  if (raw[i-1] != ' '){
	    valid = false;
	    return NULL;
	  }
	  pipecount += 1;
	  if (hasOut > 1 || hasIn > 1 || (hasOut > 0 &&  hasIn > 0)){
	    valid = false;
	    return NULL;
	  }
	  if (previousOut == 1){
	    valid = false;
	    return NULL;
	  }
	
	curr += '\0';
	
	command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
	for ( int i = 0; i < curr.length();i++){
	  
	  command[count][i] = curr[i];
	}
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

      //curr += '\0';
       if (curr.length() < 1){
 	return NULL;
       }
      
      command[count] = (char *) malloc((curr.length() + 1) * sizeof(char));

      int m = 0;
	for ( m = 0; m < curr.length();m++){
	  command[count][m] = curr[m];

	}
	command[count][m] = '\0';
    return command;
  
}

    
  


int main(int argF, char ** argv) {
  
  ifstream givenFile;
  
  if (argF > 1){
    givenFile.open(argv[1]);
  }
  
  
  char  buff[300];
  if (getcwd(buff,250) == NULL){

  }
  commands:
  do  {
  char currentLine[110];
  memset(&currentLine[0],0,110);
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
  
  int length = fullLine.length();
//   if(fullLine[length - 1] != ' '){
//     fullLine = fullLine + " ";
//   }
  int y = 0;
  for (y = 0; y < fullLine.length(); y++){
    currentLine[y] = fullLine[y];
  }
  currentLine[y] = '\0';
  

  char ** tokens = parseLine(currentLine);
  if (tokens == NULL){
    cout << "invalid input" << endl;
    goto commands;
  }

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

    int a = itemizer(strcommand, tokenized);
    if(a == -1){
      cout << "invalid input" << endl;
      goto commands;
    }

    int size = a+1;
    int realSize = 0;
    
    pipe(pipes);


    for (int j = 0; j < size; j++){
	   if(tokenized[j] != ">" && tokenized[j] != "<"){
	   bool contains_not_approved = (tokenized[j].find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
	   if(contains_not_approved == false){
	     goto commands;
	   }
	   }

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
		cout << "invalid input" << endl;
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
		    bool inputfile_contains_not_approved = (inputFile.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234566789-./_") == string::npos);
	   if(inputfile_contains_not_approved == false){
	     cout << "invalid input" << endl;
	     goto commands;
	   } 
		  j++;
    
		}
	
	   }
	 else if (tokenized[j] == ">"){
	   if (current !=  limit - 1) {
	      cout << "invalid input" << endl;
	      }

	   if (j + 1 < size) {
	     realSize = j;
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
      if (realSize != 0){
	size = realSize;
      }


    pid = fork();
      if (pid == 0) {

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
	
	if (current == 0){
	 
	 if (inputFile.length() > 0) {
		
		int iF = open(inputFile.c_str(), O_RDONLY);
		if (iF == -1) {
		  cerr << strerror(errno) << endl;	  
		  } 
		dup2(iF, STDIN_FILENO);
		
	}
	
	}
      
	if (pipecount > 0){
	  	
	
	if (current > 0  ){
	  dup2(in, STDIN_FILENO);

	  close(pipes[0]);
	}
	
	if ( current < num_token_groups - 1){
	  dup2(pipes[1], STDOUT_FILENO);
	  close(pipes[1]);

	}
	
	}
	
	if (current == num_token_groups - 1){
	  if (outputFile.length() > 0){
	    int we_oF = open(outputFile.c_str(), O_WRONLY|O_CREAT);
	    if (we_oF == -1) {
		  cerr << strerror(errno) << endl;	  
		  } 
	    dup2(we_oF,STDOUT_FILENO);
	  }
	  


	}
      
      int exVal = -1;
    
      int h = 0;
      h = execvp(argv[0], argv);
      perror("exec failure");
      exit(h);
      if (h == -1 ){

      }
      else{

      
      }
      }
    else{
      
      close(pipes[1]);
      in = pipes[0];
      
      waitpid(pid, &pid_status[p_id],0);
      p_id++;
    }
  }
    for(int d = 0; d <= pipecount; d++){
      cerr << pid_status[d] << endl;
    }
  
    free(tokens);
  } while(1);
  return 0;
}
      