/*utility library functions*/

typedef char byte;

struct dirEntry{
  char name[6]; 
  byte sectors[26]; //26 sectors
};

struct directory{
  struct dirEntry entries[16]; // notice that we have 16 entries
};


int printString(char * str){
  return interrupt(0x21, 0x00, str, 0, 0);
}

int readString(char * str){
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

void copyFile(char * line){
  char src[6];
  char dest[6];
  int numSectors;
  char buffer[13312];
  struct directory diskDir;

  int i,j,k,x,z,sectors;

  for(z = 0; z < 6; z++){
    src[z] = '\0';
    dest[z] = '\0';
  }

   i = 5;
   j = 7;
   k = 0;
   x = 0; // additional variable to keep track of the length of the file

    while(line[i] != ' ' && k < 6){
      /* read contents of <file> on screen.*/
      src[k] = line[i];
      i++;
      k++;
    }

    src[k] = '\0';

    printString(line);

    
    i++;
    k = 0;
    
    while(line[i] != '\0' && k < 6){
      dest[k] = line[i];
      i++;
      k++;
    }

  dest[k] = '\0';
    
  numSectors = 0;

  numSectors = readfile(src, buffer); //read src file
  
  writeFileHelper(dest, buffer, numSectors);

  
}

int deleteFile(char * fname){
  return interrupt(0x21, 0x07, fname, 0, 0);
}

int writeFile(char * fname, char * buffer, int sectors){
  return interrupt(0x21, 0x08, fname, buffer, sectors);
}

int readSector(char * buffer, int absSector){
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


void dir(){
  struct directory diskDir;
  char fileName[20];
  int sectorBuffer[20];

  int i,j,k,cnt;

  i = 0;
  j = 0;
  k = 0;
  cnt = 0;
  
  readSector(&diskDir, 2);



  for(i = 0; i < 16; i++){
    if(diskDir.entries[i].name[0] != 0x00){
      cnt = 0; //counting for number of sectors
      printString("h\0");
      for(j = 0; j < 6 && diskDir.entries[i].name[j] != 0x00; j++){
	fileName[j] = diskDir.entries[i].name[j];
      }
      fileName[j] = ' ';

      for(k = i; i < 26 && diskDir.entries[i].sectors[k] != 0x00; k++){
	cnt++;
      }


      if(cnt > 9){
	sectorBuffer[0] = cnt / 10;
	sectorBuffer[1] = MOD(cnt,10);
	fileName[j+1] = '\0';
	//printString(fileName);



	//printString("\n\0");
	 

      }else{
	sectorBuffer[0] = cnt;
	fileName[j+1] = '\0';

	//printString(fileName);

	
	//printString("\n\0");
      }

      
    }
  }
  

  
  // I cannot read the sector
}

