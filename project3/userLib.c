/*utility library functions*/


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

int executeProgram2(char * filename, int segment){
  return interrupt(0x21, 0x04, "uprog2\0", 0x2000, 0);
}

void clear(char * buffer){
  int i = 0;
  for(i = 0; i < 13312; i++){
    buffer[i] = 0x00;  // clear contents of buffer
  }
  
}
