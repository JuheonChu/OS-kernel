#include "userlib.h"

void recognizesUserCommand(char * command);

typedef char byte;

struct dirEntry{
  char name[6]; 
  byte sectors[26]; //26 sectors
};

struct directory{
  struct dirEntry entries[16]; // notice that we have 16 entries
};


void main(){
  
  char * line = "\r\n\0";
  
  char buffer[13312]; //file buffer

  //start every user program with enableInterrupts() for concurrent execution.
  enableInterrupts();

  printString("Butcher Operating system starts \r\n\0");
  printString("-----------------------------------------\r\n\0");
  while(1){

    clear(line);
    //display the user-friendly menu for bonus feature #5
    printString("1. type <file> (display the contents of a <file>)\r\n\0");
    printString("2. execute <program> (execute the <program>)\r\n\0");
    printString("3. Delete <file> (delete the <file>)\r\n\0");
    printString("4. copy <src> <dest> (copy the src file and write to <dest file) \r\n\0");
    printString("5. dir (list all the files in the directory)\r\n\0");
    printString("6. ps (View all the running processes)\r\n\0");
    printString("7. kill <seg> (kill a process with <seg> ID \r\n\0");
    printString("8. whoami (displays the privilege of a currently using account \r\n\0");
    ////////////////////////////////////////////////////////
    printString("Shell > \0");
    readString(line);
    printString("\r\n\0");
    recognizesUserCommand(line, buffer); //recognizes user command.
    printString("\r\n\0");
    yield();
  }
}

void recognizesUserCommand(char * line, char * buffer){

  char errorMessage[25];
  struct directory diskDir;
  int x = 0; 
  int y = 0;

  int flagExecute = 0;
  int flagType = 0;
  int flagDelete = 0;
  int flagWrite = 0;
  char fileName[20];
  char dest[20];
  int i = 5;
  int j = 8;
  int k = 0;
  int z = 0;
  // type command
  if (line[0] == 't' && line[1] == 'y' &&
      line[2] == 'p' && line[3] == 'e' && line[4] == ' '){

    i = 5;
    j = 8;
    k = 0;

    while(line[i] != '\0'){
      /* read contents of <file> on screen.*/
      fileName[k] = line[i];
      i++;
      k++;
    }

    fileName[k] = '\0';

       
   flagType =  readfile(fileName, buffer);

   if(flagType == -1){
     println("File does not exist! \0");
   }else{
     printString(buffer);
   }
    
    
     

    clear(buffer);
  }
  // execute command
  else if(line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c'
	   && line[4] == 'u' && line[5] == 't' && line[6] == 'e' && line[7] == ' '){
    i = 5;
    j = 8;
    k = 0;
    
    while(line[j] != '\0'){
      fileName[k] = line[j];
      k++;
      j++;
    }
    
    fileName[k]='\0';
     
    // load <fileName> into memory and execute.
    flagExecute = executeProgram(fileName, 0x2000);
    
    
    if(flagExecute == -1){ //non-existing program
      printString("Program does not exist!\0");
    }
    
   
    
  }else if(line[0] == 'd' && line[1] == 'e' && line[2] == 'l' && line[3] == 'e'
	   && line[4] == 't' && line[5] == 'e' && line[6] == ' '){

    i = 5;
    j = 7;
    k = 0;

    while(line[j] != '\0'){
      /* read contents of <file> on screen.*/
      fileName[k] = line[j];
      j++;
      k++;
    }

    fileName[k] = '\0';
    //printString(fileName);

    flagDelete = deleteFile(fileName); // remove specified file from Disk

    if(flagDelete == -1){
      printString("file Not Found!\0"); // File was not found on Disk
    }

    clear(fileName);
  }else if(line[0] == 'c' && line[1] == 'o' && line[2] == 'p' && line[3] == 'y' && line[4] == ' '){

  char src[6];
  char dest[6];
  int numSectors;


  int sectors;

  z = 0;
  
  for(z = 0; z < 6; z++){
    src[z] = '\0';
    dest[z] = '\0';
  }

   i = 5;
   j = 7;
   k = 0;

   // stores the name of src file into char [] src.
   while(line[i] != ' '){ //notice that filename cannot be more than 6 characters
      /* read contents of <file> on screen.*/
      src[k] = line[i];
      i++;
      k++;
    }

    src[k] = '\0';

  
    i++;
    k = 0;

    // stores the name of the dest file into char [] dest.
    while(line[i] != '\0'){
      dest[k] = line[i];
      i++;
      k++;
    }

  dest[k] = '\0';
    
  numSectors = 0;

  numSectors = readfile(src, buffer); //read src file
  
  writeFile(dest, buffer, numSectors); //write the file into the directory

  
  clear(buffer);
  
  } else if(line[0] == 'd' && line[1] == 'i' && line[2] == 'r' && line[3] == '\0'){

    int index;

    struct directory diskDir;
    
    int i,j;

    char fname[6];
    char * buf;

    readSector(&diskDir, 2);

    for(i = 0; i < 16; i++){
      if(diskDir.entries[i].name[0] != 0x00){
	buf = diskDir.entries[i].name;
	buf[6] = '\0';
	printString(buf);
	printString("\n\r\0");
      }

    }

    clear(buf);
    

  }else if(line[0] == 'p' && line[1] == 's' && line[2] == '\0'){

    showProcess();

  }else if(line[0] == 'k' && line[1] == 'i' && line[2] == 'l' && line[3] == 'l' && line[4] == ' '){

    int killProcessID = -1;

    char ch = line[5];

  
    switch(ch){
    case '0':
      killProcessID = 0;
      //printString("Zero\0");
      break;
    case '1':
      killProcessID = 1;
      //printString("One\0");
      break;
    case '2':
      killProcessID = 2;
      //printString("Two\0");
      break;
    case '3':
      killProcessID = 3;
      //printString("Three\0");
      break;
    case '4':
      killProcessID = 4;
      //printString("Four\0");
      break;
    case '5':
      killProcessID = 5;
      //printString("Five\0");
      break;
    case '6':
      killProcessID = 6;
      //printString("Six\0");
      break;
    case '7':
      killProcessID = 7;
      //printString("Seven\0");
      break;
    default:
      killProcessID = -1;
      break;
    }
    
    if(killProcessID != -1){
      kill(killProcessID);
    }

    clear(line);
    
  }else if(line[0] == 'w' && line[1] == 'h' && line[2] == 'o' && line[3] == 'a' && line[4] == 'm' && line[5] == 'i'){

    //added for bonus feature #5
    
    char fname [6];

    fname[0] = 'w';
    fname[1] = 'h';
    fname[2] = 'o';
    fname[3] = '\0';

    //executes who user program that simulates "whoami" command from linux.
    executeProgram(fname);
						       
  }
  else{
    errorMessage[0] = 'U';
    errorMessage[1] = 'n';
    errorMessage[2] = 'r';
    errorMessage[3] = 'e';
    errorMessage[4] = 'c';
    errorMessage[5] = 'o';
    errorMessage[6] = 'g';
    errorMessage[7] = 'n';
    errorMessage[8] = 'i';
    errorMessage[9] = 'z';
    errorMessage[10] = 'e';
    errorMessage[11] = 'd';
    errorMessage[12] = ' ';
    errorMessage[13] = 'c';
    errorMessage[14] = 'o';
    errorMessage[15] = 'm';
    errorMessage[16] = 'm';
    errorMessage[17] = 'a';
    errorMessage[18] = 'n';
    errorMessage[19] = 'd';
    errorMessage[20] = '!';
    errorMessage[21] = '\0';
    printString(errorMessage);
  }

  
}
