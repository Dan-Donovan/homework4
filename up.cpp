

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
// iterate over string
    while (i < endPosition) {
     // cout << i << " tokenizer" << endl;
// increment i until a non-WS char is found
      while ((command[i] == ' ' || command[i] == '\t')) {
	i += 1;
//	cout << "still in this space loop" << endl;
	}
// set beginning of token to current pos -- first non-WS char
      int tokenBeginIndex = i;
// increment i until a WS char is found
      while (command[i] != ' ' && command[i] != '\t' && i <= endPosition){
	i += 1;
//	cout << i << endl;
	}
// set token end index to current pos -- first WS char
      int tokenEndIndex = i;
      string token = command.substr((unsigned long) tokenBeginIndex, (unsigned long) (tokenEndIndex - tokenBeginIndex));
      cout << token << " the current token" << endl;
      //if (isValidToken(token))
// save the token into the token array
  //    cout << "the token is " << token << endl;
   //   cout << "index value is " << current << endl;
      argumentP[current] = token;
    //  cout << "allocation was fine" << endl;
// increment the pointer index
      if (token == ">" || token == "<"){
	modified = current;
      }
      current += 1;
    }
    current -= 1;
   // cout << "at the bottom" << endl;
cout << "number of tokens is " << current << endl;
if (modified != 0){
  cout << "ACTUAL NUMBER WITH REDIRECT " << modified << endl;
  //return (modified -1);
}
  return current;

}

char ** parseLine(char * raw){
  //cout << "inside belly of the beast" << endl;
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
    cout << "INVALID STRING" << endl;
    return null;
    //cannot end with a operations or begin with an operation
    
  }
  
  //  cout << "before the for loop!" << endl;
   // cout << strlen(raw) << endl;
    string curr = "";
    for (int i = 0; i < strlen(raw); i++){
      
     // cout << "i value is " << i << endl;
      if (raw[i] != '|'){
//	cout << "adding on a value " << raw[i] << endl;
	//command = command + raw[i];
	//command[count][p] = raw[i];
	//command[count] += raw[i];
	//curr[p] += raw[i];
	
//	cout << "current curr length is " << curr.length() << endl;
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
//	cout << "after this part" << endl;
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
//	cout << "---------------------------------" << endl;
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
  //  cout << "after the loop" << endl;
//     if (pipecount == 0){
//       //cout << command[count] << endl;
//       cout << "why am i inside here" << pipecount << endl;
//       //command[count][p] = '\0';
//       //command[count] += '\0';
//       cout << curr[0] << "first character" << curr.length() << endl;
      curr += '\0';
      cout << "curr value is " << curr << endl;
      command[count] = (char *) malloc(curr.length() + 1 * sizeof(char));
      
	for ( int i = 0; i < curr.length() - 1;i++){
	  command[count][i] = curr[i];
      
	}
	cout << command[count] << " THIS SHOULD BE THE SAME " << endl;
      //command[count] = (char *) (strlen(curr) + 1 * sizeof(char));
      //command[count] = curr;
      
    //  cout << command[0][0] << " first" << endl;
     // cout << "it does equal 0!" << endl;
     // cout << command[0] << " that was the command" << endl;
      //cout << command[1] << "second command" << endl;
      //tokens[index] = command;
     // cout << "what am i doing?" << "index is " << index << tokens[index] << endl;
    //}

  //  cout << "here i am out of the for loop for real!" << endl;
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
    //cout << "lolll" << endl;
  }
 // cout << buff << endl;
 // int t = 0;
  while (1) {
    cout << "Start of loop" << endl;
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
      pipecount = 0;
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
  fullLine = fullLine + " ";
  int y = 0;
  for (y = 0; y < fullLine.length(); y++){
    currentLine[y] = fullLine[y];
  }
  currentLine[y] = '\0';
  
  cout << "before the current commands" << endl;
  
  char ** tokens = parseLine(currentLine);
  //cout << pipecount << endl;
  //cout << *tokens[0] << endl;
  //cout << tokens[0] << endl;
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
  //cout << "the next hurtle" << endl;
  //parse the line
  //for each command in the line {
  
  int p_id = 0;
 // int current = 0;
  int num_token_groups = pipecount + 1;
  int limit = pipecount + 1;
  //int pipes2d[limit][2];
  int pipes[2];
  //int pipes[num_token_groups][2];

   int pid_status[pipecount + 1];
   int in = 0;
   //pipe(pipes);
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
    //cout << tokenized[0] << "does the tokenizer work?" << endl;
    //if (current == 0 ){
    pipe(pipes);
   // }
    //pipes2d[current][0] = pipes[0];
    //pipes2d[current][1] = pipes[1];
	cout << size << " is " << endl;
        cout << "current command index is " << current << endl;
    cout << "pipes 0 and pipes 1 " << pipes[0] << " " << pipes[1] << endl;
    
    for (int j = 0; j < size; j++){
	   cout <<  "WHY ARE WE NOT IN THIS LOOP" << endl;
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
		    //inputFile = buff[l] + inputFile[0];
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
 		//outputFile = buff[l] + outputFile;
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
//     pipe(fd);
//     pipes[current][0] = fd[0];
//     pipes[current][1] = fd[1];
//     
    
    ///  num_token_groups -= 1;

    pid = fork();
      cout << pid << " after da fork" << endl;
      if (pid == 0) {
//	cout << "inside the child " << current << endl;
	char ** args = (char**)malloc(sizeof(char*) *50);
	for (int f = 0; f < size + 1; f++){
	  args[f] = (char *) malloc(100 * sizeof(char));
	}
	//cout << "first arg, second arg is " << tokenized[0] << " " << tokenized[1] << endl;
	char * argv[size + 1];
	int element = 0;
	char * ready = "";
	cout << "size for iterating is " << size << endl;
	for( element = 0; element < size; element++){
//	    cout << "element is " << element << endl;
	   // argv[element] = tokenized[element].c_str();
	    string ex = tokenized[element];
	    if (ex == ">" || ex == "<" ){
	      cout << "SHOULD NEVER SEE THIS" << endl;
	      break;
	    }
	    int g = 0;
	    for (g = 0; g < ex.length(); g++){
//	      cout << "string value is " << ex[g] << endl;
	      //args[element] += (char) ex[g];
	      args[element][g] = ex[g];
	    }
	   
	    args[element][g] = '\0';
	  //  cout << args[element];
	    argv[element] = args[element];
	   // cout << argv[0] << "current arg" <<  endl;
	   // argv[element] = ready;
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
	//	 cout << "hereereer" << endl;
		dup2(iF, STDIN_FILENO);
		cout << "after the dup" << endl;
	}
	
// 	  cout << "pipes[0] is " << pipes[0] << endl;
// 	  //close(pipes[0]);
// 	  int the_oF = pipes[1];
// 	  cout << "the of " << the_oF << endl;
// 	  dup2(pipes[1], STDOUT_FILENO);
// 	  cout << "please " << endl;
// 	  close(pipes[1]);
// 	  cout << "here we GO" << endl;
	  cout << "true love" << endl;
	
	}
      
	if (pipecount > 0){
	  	
	//&& current < num_token_groups - 1
	if (current > 0  ){
	  cout << "second command piper " << command << endl;
// 	  int the_iF = pipes[current - 1][0];
// 	  dup2(the_iF,STDIN_FILENO);
// 	  close(pipes[current-1][1]);
// 	
	  //close(pipes[1]);
	 //close(pipes[1]);
	  dup2(in, STDIN_FILENO);

	  close(pipes[0]);
	  cerr << "LOL" << endl;
// 	  
// 	  //dup2(in, 0);
// 	  //close(in);
// 	  close(pipes2d[current-1][1]);
// 	  dup2(pipes2d[current -1][0], STDIN_FILENO);
// 	  close(pipes2d[current-1][0]);
	}
	
	if ( current < num_token_groups - 1){
	  //cout << "num - 1" << num_token_groups - 1 << endl;
	 // cout << tokenized[current] << " before the pipe" << endl;
	 // cout << "spitting out output into next pipe" << endl;
// 	  int the_oF = pipes[current][1];
// 	  cout << "output doesnt work" << endl;
// 	  int error = dup2(the_oF,STDOUT_FILENO);
// 	  cout << " error from dup is " << error;
// 	  close(pipes[current][0]);
// 	
// 	  cout << "inside this one" << endl;
// 	  
// 	  close(pipes[0]);

	  
	  cerr << "set up write pipe" << endl;
	 // close(pipes[0]);
	  dup2(pipes[1], STDOUT_FILENO);
	  close(pipes[1]);

// 	  close(pipes2d[current][0]);
// 	  cout << "lol" << endl;
// 	  dup2(pipes2d[current][1], STDOUT_FILENO);
// 	  cerr << "was it this?" << endl;
// 	  close(pipes2d[current][1]);
	    cerr << "made it here" << endl;
	  
	  
	  //dup2(pipes[1], 1);
	  //close(pipes[1]);
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
// 	  dup2(in, STDIN_FILENO);
// 	  cout << "made it here" << endl;
// 	  execvp(argv[0], argv);

	}
      cerr << "------------------------------------------------------------------" << endl;
    // if (current < num_token_groups - 1){
      int exVal = -1;
       cerr << "args are " << argv[0] << " " << argv[1] << " "  << argv[2] << " " << argv[3] << endl;
      cerr << "fsdfsf" << endl;
    
      int h = 0;
      
     // close(pipes[0]);
     // close(pipes[1]);
      execvp(argv[0], argv);
      //exit(h);
      if (h == -1 ){
	cerr << ":(" << endl;
      }
      else{
      cerr << "whelp i execed " << endl;
      
      }
      }
    else{
      //close(pipes[1]);
      cout << "pid value is " << p_id << " pid value is " << pid << endl;
      
      //pipeid++;
      //pid[p_id] = pid;

      close(pipes[1]);
      in = pipes[0];
      cout << "the parent" << endl;
      
      waitpid(pid, &pid_status[p_id],0);
      cout << "finished eaiting" << endl;
      p_id++;
    }
	
    for(int d = 0; d <= pipecount; d++){
      cout << "Process " << " exited with status " << pid_status[d] << endl;
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
  