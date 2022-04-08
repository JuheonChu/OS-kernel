/*utility library functions*/
#include "userlib.h"
#define MAX_LIMIT 80


int printString(char * str){
  return interrupt(0x21, 0x00, str, 0, 0);
}

int readChar(){
  return interrupt(0x16,0, 0, 0, 0);
}

int readString(char * str, int limit){
  return interrupt(0x21, 0x01, str, 0, 0);
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
  return interrupt(0x21, 0x08, fname, buffer, sectors);
}

int readSector(char * buffer, int absSector){
  printString("readSector()\0");
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

