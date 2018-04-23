

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
  //int pid_status = 0;
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

int stringToTokens(string command, string* argumentP) {
   int i = 0;
    int current = 0;
    int beginPosition = command.find_first_not_of("\t ");
    int endPosition = command.find_last_not_of("\t ");
     i = (int) beginPosition;
    if (beginPosition == endPosition && command[beginPosition] != ' ' && command[beginPosition] != '\t') {
	argumentP[0] = command[beginPosition];
  
      return -1;
    }
// iterate over string
    while (i < endPosition) {
      cout << i << " tokenizer" << endl;
// increment i until a non-WS char is found
      while ((command[i] == ' ' || command[i] == '\t')) {
	i += 1;
	cout << "still in this space loop" << endl;
	}
// set beginning of token to current pos -- first non-WS char
      int tokenBeginIndex = i;
// increment i until a WS char is found
      while (command[i] != ' ' && command[i] != '\t' && i <= endPosition){
	i += 1;
	cout << i << endl;
	}
// set token end index to current pos -- first WS char
      int tokenEndIndex = i;
      string token = command.substr((unsigned long) tokenBeginIndex, (unsigned long) (tokenEndIndex - tokenBeginIndex));
      //if (isValidToken(token))
// save the token into the token array
      cout << "the token is " << token << endl;
      cout << "index value is " << current << endl;
      argumentP[current] = token;
      cout << "allocation was fine" << endl;
// increment the pointer index
      current += 1;
    }
    current -= 1;
    cout << "at the bottom" << endl;
return current;
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
    for (int i = 0; i < strlen(raw); i++){
      cout << "i value is " << i << endl;
      if (raw[i] != '|'){
	cout << "adding on a value " << raw[i] << endl;
	//command = command + raw[i];
	//command[count][p] = raw[i];
	//command[count] += raw[i];
	//curr[p] += raw[i];
	
	cout << "current curr length is " << curr.length() << endl;
	//cout << command[count] << endl;
	//cout << "here????" << endl;
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
//     if (pipecount == 0){
//       //cout << command[count] << endl;
//       cout << "why am i inside here" << pipecount << endl;
//       //command[count][p] = '\0';
//       //command[count] += '\0';
//       cout << curr[0] << "first character" << curr.length() << endl;
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
      //cout << command[1] << "second command" << endl;
      //tokens[index] = command;
     // cout << "what am i doing?" << "index is " << index << tokens[index] << endl;
    //}

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


int main(int argF, char ** argv) {
  
  ifstream givenFile;
  
  if (argF > 1){
    givenFile.open(argv[1]);
  }
  
  
  char  buff[300];
  //cwd = getcwd(buff,250);
  if (getcwd(buff,250) == NULL){
    cout << "lolll" << endl;
  }
  cout << buff << endl;
 // int t = 0;
  while (1) {
    cout << "ROUND 1" << endl;
   // t = 1;
  // time to read a line of input
  //char * currentLine = inputParsing();
  char currentLine[110];
  string fullLine = "";
  if (argF > 1) {
    char x;
    
    while ( givenFile.get(x) && x != '\n'){
      fullLine += x;
    }
      }/* Exit command detected - exit shell */
//   else {
//     cout << "> ";
//     fullLine = getLine();
//   }
    /* Exit command detected - exit shell */
    if (fullLine == "exit") {
      break;
      }
      
  
   if( argF > 1 && givenFile.eof()){
     exit(0);
   }
/*  
 * 
  if ((fgets(currentLine, sizeof(currentLine), stdin) == NULL) ){
    break;
  }
  
  if (strlen(currentLine) > 100){
    cout << "invalid" << endl;
  }*/
  int y = 0;
  for (y = 0; y < fullLine.length(); y++){
    currentLine[y] = fullLine[y];
  }
  currentLine[y] = '\0';
  
  cout << "before the current commands" << endl;
  
  char ** tokens = parseLine(currentLine);
  cout << pipecount << endl;
  //cout << *tokens[0] << endl;
  cout << tokens[0] << endl;
  cout << "current line is " << currentLine <<endl;
 // cout << tokens[1] << endl;
//   if (pipecount > 0) {
//     pipearray = (int **) malloc(sizeof(int*) * (pipecount));
//     cout << "here??" << endl;
//     for (pipeid = 0; pipeid < pipecount; pipeid++) {
//       pipearray[pipeid] = (int*)malloc(sizeof(int[2]));
//       if (pipe(pipearray[pipeid]) != 0){
// 	perror("pipe()");
//       }
//       }
//   }
  cout << "the next hurtle" << endl;
  //parse the line
  //for each command in the line {
  
  int pipeid = 0;
  int p_id = 0;
 // int current = 0;
  int num_token_groups = pipecount + 1;
  int limit = pipecount + 1;
  
  int pipes[num_token_groups][2];
  
  
  for (int current = 0; current < limit; current++){
    cout << "current command index is " << current << endl;
    
    int fd[2];
    string inputFile;
    string outputFile;
    string tokenized[100];
   // while(num_token_groups > 0){
    char * command = tokens[current];
    std::string strcommand(command);
    cout << "here hahaha" << endl;
    int a = stringToTokens(strcommand, tokenized);
    int size = limit + 1;
    cout << tokenized[0] << "does the tokenizer work?" << endl;
    //strcommand = str(command);
    
    
    for (int j = 0; j < a; j++){
	   if (tokenized[j] == "<") {
	      if (current != 0) {
		cerr << "wrong pipe" << endl;
		}

	   else if (j + 1 < a) {
	      inputFile = tokenized[j + 1];
	    
	           j++;
    
}
	   }
	 else if (tokenized[j] == ">"){
	   if (current != a) {
	      cerr << "error error error " << endl;
	      }

	    else if (j + 1 < a) {
	      outputFile = tokenized[j+1];

	      j++;
	      }
}
	else {}
	  }

    pipe(fd);
    pipes[current][0] = fd[0];
    pipes[current][0] = fd[1];
    
    ///  num_token_groups -= 1;

    pid = fork();
      cout << "after da fork" << endl;
      if (pid == 0) {
	if (pipecount > 0){
	  	if (current == 0){
	  cout << "piping in from startFile" << endl;
	 if (inputFile.length() > 0) {
		int iF = open(inputFile.c_str(), O_RDONLY);
		if (iF == -1) {
		  cerr << strerror(errno) << endl;	
		  
		  } 
		 
		dup2(iF, STDIN_FILENO);
	  
	}
	}
	
	if (current > 0){
	  cout << "second command piper " << command << endl;
	  int the_iF = pipes[current - 1][0];
	  dup2(the_iF,STDIN_FILENO);
	  close(pipes[current-1][1]);
	}
	
	if (current < num_token_groups - 1){
	  cout << command[current] << " before the pipe" << endl;
	  cout << "spitting out output into next pipe" << endl;
	  int the_oF = pipes[current][1];
	  dup2(the_oF,STDOUT_FILENO);
	  close(pipes[current][0]);
	}
	if (current == num_token_groups - 1){
	  if (outputFile.length() > 0){
	    int oF = open(outputFile.c_str(), O_WRONLY|O_CREAT);
	    dup2(oF,STDOUT_FILENO);
	  }
	}
	}
// 	if (pipecount > 0){
// 	  if (num_token_groups > 0){
// 	    cout << "of course piping" << endl;
// 	    if (num_token_groups == pipecount) {
// 	      cout << "first pipe" << endl;
// 	      close(pipearray[pipeid][0]);
// 	      dup2(pipearray[pipeid][1], STDOUT_FILENO);
// 	    }
// 	  
// 	    else {
// 	      close(pipearray[pipeid][0]);
// 	      dup2(pipearray[pipeid][1], STDOUT_FILENO);
// 	      close(pipearray[pipeid - 1][1]);
// 	      dup2(pipearray[pipeid - 1][0], STDIN_FILENO);
// 	    }
// 	    }
// 	else{
// 	  //last token
// 	  close(pipearray[pipeid - 1][1]);
// 	  dup2(pipearray[pipeid - 1][0], STDIN_FILENO);
// 	  
// 	}
// 	}
      //cout << "THE PARSED STRING" << " " << tokens[current] << endl;
     // char * command = tokens[current];
      //cout << command << endl;
      
      //command = trimwhitespace(command);
      //cout << command << endl;
      
      //cout << command << endl;
      // char ** args;
//       	//string deliminator
// 	//char ** part = "";
	char ** args = (char**)malloc(sizeof(char*) *50);
	for (int f = 0; f < limit + 1; f++){
	  args[f] = (char *) malloc(100 * sizeof(char));
	}
	//cout << "first arg, second arg is " << tokenized[0] << " " << tokenized[1] << endl;
	char * argv[limit + 1];
	
	int element = 0;
	char * ready = "";
	for( element = 0; element < size; element++){
	   // argv[element] = tokenized[element].c_str();
	    string ex = tokenized[element];
	    int g = 0;
	    for (g = 0; g < ex.length(); g++){
	      cout << "string value is " << ex[g] << endl;
	      //args[element] += (char) ex[g];
	      args[element][g] = ex[g];
	    }
	   
	    args[element][g] = '\0';
	  //  cout << args[element];
	    argv[element] = args[element];
	   // cout << argv[0] << "current arg" <<  endl;
	   // argv[element] = ready;
	}
	argv[element] =  NULL;
      
// 	int k = 0;
// 	int t = 0;
// 	int cIndicator = 0;
// 	for (int j = 0; j < strlen(command); j++){
// 	  cout << k <<"this is the k value or kth arg" << endl;
// 	  cout << "j balue is " << j << endl;
// 	  
// 	  if (command[j] != ' '){
// 	    //cout << "i mean your right" << endl;
// 	    if(j == 0 && command[j] != '/'){
// 	      int l = 0;
// 	      while(l < 250 && buff[l] != '\0'){
// 		args[k][t] = buff[l];
// 		l = l + 1;
// 		t = t + 1;
// 	      }
// 	      args[k][t] = '/';
// 	      t = t + 1;
// 	    }
// 	    args[k][t] = command[j];
// 	    cout << "added an actual command" << endl;
// 	    t = t + 1;
// 	  }
// 	  else {
// 	    cout << "we are in here because there is a space" << endl;
// 	    bool nomoreSpace = true;
// 	    while(j + 1 < strlen(command) && nomoreSpace){
// 	      cout << "happened once" << endl;
// 	      nomoreSpace = false;
// 	      if(command[j+1] == ' '){
// 		j = j + 1;
// 		nomoreSpace = true;
// 	      }
// 	      else{
// 		k += 1;
// 		t = 0;
// 		args[k][t] = '\0';
// 	      }
// 	    }
// 	    //args[index][argVal] = part;
// 	    args[k][t] = '\0';
// 	    //k += 1;
// 	    //t = 0;
// 	    //part = "";
// 	  }
// 	  
// 	  //args[k][t] = '\0';
// 	  //cout << "before that" << endl;
// 	  //cout << args << endl;
// 	  //cout << "command is " << args[0] << endl;
// 	  
// 	}
//       
//       for (int j = 0; j <= k; j++){
// 	cout << "component is " << args[j] << endl;
// 	
//       }
//       cout << "-------------------" << endl;
// 	k += 2;
//       cout << "k value is " << k << endl;
//       char * argv[k];
//       int w = 0;
//       for (int e = 0; e < k; e++){
// 	
// 	argv[e] = args[e];
// 	w = e;
//       }
//       cout << "w is " << w << endl;
      //argv[element] = NULL;
	//args[k+1] = NULL;
      cout << "out of loop!" << endl;
      cout << "command is " << argv[0] << endl;
//       cout << "second arg is " << args[1] << endl;
//       cout << "third arg is " << args[2] << endl;
//       cout << " " << endl;
//    

      int exVal = execvp(argv[0], argv);
      if (exVal == -1){
	cout << "exec does not even work" << endl;
      }
      else{
      cout << "oh I just execed all right" << endl;
      }
      //execv(tokens[current][0], tokens[current]);
      //current = current + 1;
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
    }
    for (int i = 0; i <= pipecount; i++){
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
    int pid_status[pipecount + 1];
    for (p_id = 0; p_id <= pipecount; p_id++) {
     // pid_status = 0;
      waitpid(pids[p_id], &pid_status[p_id],0);
    //  cerr << "error status" << pid_status << endl;
	}
    for(p_id = 0; p_id <= pipecount; p_id++){
      cout << "Process exited with status " << pid_status[p_id] << endl;
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
  