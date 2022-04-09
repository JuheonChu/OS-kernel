/*utility library functions*/
#include "userlib.h"



int printString(char * str){
  return interrupt(0x21, 0x00, str, 0, 0);
}

int readChar(){
  return interrupt(0x16,0, 0, 0, 0);
}

int readString(char * str, int limit){
  return interrupt(0x21, 0x01, str, 0, 0);
}

char* readStringHelper(char * str, int limit){
  char ch = 0x00;
  int index = 0;
  char * buf = 0x00;

  char readCh[2];

  readCh[1] = 0x00;

  ch = 0x00;
  ch = readChar();
  
  while(ch != 0x0D){
    if(index >= limit){ //if we are at the cursor position ahead of where we initially set as a limit
      //buf[index]=0x00; //since we cannot assign any further input character, we can add a null termination.
      ch = readChar();
      if(ch == 0x08){ //even if our cursor hits the limit, we still need to allow the Backspace operation works
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	index--;
	buf[index]=' ';
	//interrupt(0x10, 0x0E*256 + ch, 0, 0, 0);
	interrupt(0x10, 0x0E*256+buf[index], 0, 0, 0);
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	
      }else{
	continue;
      }
    
    }else if(ch == 0x08){ //Backspace/Delete key entered
      if(index > 0){ //any index greater than 0 should allow the user to be able to delete the character that he/she previously typed.
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	index--;
	buf[index]=' ';
	//interrupt(0x10, 0x0E*256 + ch, 0, 0, 0);
	interrupt(0x10, 0x0E*256+buf[index], 0, 0, 0);
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
      }else{ //we don't have to do anything when there is no character in buf
	if(index == 0){
	  ch = readChar();
	}
	continue;
      }
   
    }else{ //normal situation for typing characters with the keyboard
      buf[index] = ch;
      readCh[0] = ch;
      printString(readCh); // it has to print readChar, not buf because buf is not terminated with null pointer yet.
      index++; //move the cursor to the next index
     
    }
    
    ch = readChar();
  }

  buf[index] = 0x00;
  //printString(buf);
  return buf;
}


int readfile(char * filename, char * str){
  return interrupt(0x21, 0x03, filename, str, 0); 
}

void type(char * filename, char * file){
  readfile(filename, file);
  printString(file);
}

int executeProgram(char * filename, int segment){
   return interrupt(0x21, 0x04, filename, 0x2000, 0);
}

void terminate(){
  interrupt(0x21, 0x05, 0,0,0);
}

void clear(char * buffer){
  int i = 0;
  for(i = 0; i < 13312; i++){
    buffer[i] = 0x00;  // clear contents of buffer
  }
}

/**
  * helper method to get the number of sectors to use to write the file
  * @author John chu, Amir Zawad, Adia Wu 
  */
void writeFileHelper(char * dest, char * buffer, int numSectors){
  //printString(dest);
  writeFile(dest+'\0', buffer, numSectors);
  printString(buffer);
  clear(buffer);
}




int deleteFile(char * fname){
  return interrupt(0x21, 0x07, fname, 0, 0);
}

int writeFile(char * fname, char * buffer, int sectors){
  //printString("\r\n\0");
  //printString(fname);
  //printString("\r\n\0");
  //printString(buffer);
  return interrupt(0x21, 0x08, fname, buffer, sectors);
}

int readSector(char * buffer, int absSector){
  // printString("readSector()\0");
  return interrupt(0x21, 0x02, buffer, absSector, 0); 
}


/* helper Function to return the remainder for modulus operation.
 * @param int sector, int divisor
 * @return remainder 
 * @author John Chu, Amir Zawad, Adia Wu
 */
int MOD(int sector, int divisor){
    int remainder;
    while(sector >= divisor){
      sector = sector - divisor;
    }
    remainder = sector;
    return remainder;
}

