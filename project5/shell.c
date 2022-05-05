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
  
  while(1){
    //clear(line);
    //clear(buffer);
    
    printString("Shell > \0");
    readString(line);
    printString("\r\n\0");
    recognizesUserCommand(line, buffer); //recognizes user command.
    printString("\r\n\0");
    
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

   while(line[i] != ' '){ //notice that filename cannot be more than 6 characters
      /* read contents of <file> on screen.*/
      src[k] = line[i];
      i++;
      k++;
    }

    src[k] = '\0';

  
    i++;
    k = 0;
    
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

    char * segments;

    i = 5;

  

    while(line[i] != '\0'){ 
      segments[i-5] = line[i];
      i++;
    }

    kill(segments);
    
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
