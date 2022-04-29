
#define MAIN

#include "proc.h"

//define the prototypes of OS kernel functions
void putChar(int row, int column, char ch, char color);
void putStr(int row, int column, char * str, char color);
int interrupt (char irq, int ax, int bx, int cx, int dx);
int printString(char * str);
int readChar();
void backSpace(char * buf, int index, char ch);
int readString(char * buf, int limit);
int readSector(char * buf, int absSector);
int handleInterrupt21(int ax, int bx, int cx, int dx);
char* printInt(int x);
int readfile(char * filename, char * buf);
int kStrcpy(char * src, char * dest);
int executeProgram(char * fname);
void terminate();
int writeSector(char * buffer, int sector);
int deleteFile(char * fname);
int writeFile(char * fname, char * buffer, int sectors);
void handleTimerInterrupt(int segment, int stackPointer);

typedef char byte;

struct dirEntry{
  char name[6]; 
  byte sectors[26]; //26 sectors
};

struct directory{
  struct dirEntry entries[16]; // notice that we have 16 entries
};

void main(){

 
  /*Testing for writeFile*/
 
 
  /*Testing shell.c*/

  initializeProcStructures();

    makeInterrupt21();

    
  
    handleInterrupt21(0x04,"shell\0",0x2000,0);
    makeTimerInterrupt();

  
  
  while(1); //infinite loop
}


/** print out the character at (row, column) in our video memory
 *  @param int row, int column, char ch, char color
 */
void putChar(int row, int column, char ch, char color){
  putInMemory(0xB000, 0x8000 + (row *160) + (column * 2), ch);
  putInMemory(0xB000, 0x8000 + (row *160) + (column * 2) + 1, color);

}


/** print out the string at (row, column) in our video memory
 *  @param int row, int column, char * str, char color
 */

void putStr(int row, int column, char * str, char color){
  while(*str != '\0'){
    putChar(row, column, *str, color);
    ++str;
    column++;
  }
}


/** 
 * This will print out the string returning how many characters in string we * have in &str
 * 
 * @author John Chu & Adia Wu
 * @param char * str: the character to be printed out 
 * @return count (return the number of characters in the str)
 */
int printString (char * str){
  int count; //local variable to keep track of how many character in the strings we have

  count = 0;
  
  while(*str != '\0'){ //loop until the null termination pointer 
    interrupt(0x10, 0x0E*256 + *str, 0,0,0); //'Q'
    ++str;
    count++;
  }
  return count;
}


/** 
 * read a single character from what user typed through the keyboard
 * @return ch (character that we read from the user-typed keyboard charcter)
 */
int readChar(){
  char ah = 0x00;
  int  ch = interrupt(0x16, ah*256, 0, 0, 0);
  return ch;
}


/**
 * helper method to make a backspace interrupt
 * @param char * buf, int index, char ch
 */
void backSpace(char * buf, int index, char ch){
        interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	index--;
	buf[index]=' ';
	//interrupt(0x10, 0x0E*256 + ch, 0, 0, 0);
	interrupt(0x10, 0x0E*256+buf[index], 0, 0, 0);
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	readChar();
}

/** read a series of characters from what user type through the keyboard
 * @param char * buf, int limit
 * char * buf is the string characters that user type through the keyboard.
 * int limit parameter is used for a bonus feature to set the threshold of the amount of character   s to be printed
 */
int readString(char * buf, int limit){


  //Bonus #1 Feature & Backspace
  int index = 0; //cursor to keep track of the character in char * buf
  char ch;
  char readCh[2]; //helper character array to store the inputs.

  readCh[1] = 0x00; // notice that every character or string has to finish with null pointer
  ch = 0x00;
  ch = readChar(); // a call to read the character.
  //ch = readChar(); is also another way.
  
  while(ch != 0x0D){ //we continue getting the inputs until ENTER key is hit.
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

 
  buf[index]=0x00; //added null termination
  
  return index;
  
}


/* Reading a sector from Disk via the Bios (Interrupt 0x13).
 * We will read a message from message.txt and print it out to the screen. 
 * @param char * buf, int absSector
 * char * buf indicates the address of the buffer into which the data in the sector should be placed.
 * int absSector indicates the aboslute sector number of the disk to be read.
 * return 1
 */
int readSector(char * buf, int absSector){
  //For a 3.5 inch floppy disk,
  int relSector;
  int head;
  int track;
  relSector = MOD(absSector, 18)+1;
  head = MOD(DIV(absSector,18),2);
  track = DIV(absSector, 36);
  interrupt(0x13, 0x2*256+0x1, buf, track*256+relSector, head*256+0);
  return 1;
}


/* helper Function to return the remainder for modulus operation.
 * @param int sector, int divisor
 * @return remainder 
 */
int MOD(int sector, int divisor){
    int remainder;
    while(sector >= divisor){
      sector = sector - divisor;
    }
    remainder = sector;
    return remainder;
}


/* helper function to return quotient for division operation
 * @param int absSector, int divisor
 * @return quotient 
 */
int DIV(int absSector, int divisor){ //ex. 32 / 4
    int quotient = 0;

    while((quotient + 1)*divisor <= absSector){
      quotient++;
    }
    
    return quotient;

    
  }


/**
 * Creating the System Call Interface.
 * Depending on the value of ax, handleInterrupt21 will perform a diferent s * ystem call.
 * @param int ax, int bx, int cx, int dx
 * ax will be used to indicate which service is being requested, and bx,cx,  * and dx are used as the address of a string to print, a buffer to fill o * r the number of the sector to be read)
 */
int handleInterrupt21(int ax, int bx, int cx, int dx){

  //testing Code 
  //printString("Quit Interrupting!\0"); //commented pop bx; from handleInterrupt function in kernel.asm to prevent from the message twice.
  //return 0;

  
  
  char * buf = bx; //character that we will read from the user
  
  //Creating printString, readCharacter, readString()
  if(ax == 0x00){ //print a null terminated string at the current cursor location
    return printString(buf);
  }else if(ax == 0x11){ //read a character from the keyboard
    char ch = readChar();
    buf[0]=ch;
    printString(ch+"\0");
    return 1;
  }else if(ax == 0x01){ //read characters from the keyboard until ENTER is pressed.
    return readString(buf, 80);
  }else if(ax == 0x02){
    return readSector(bx,cx);
  }
  else if(ax == 0x03){ //read in the Disk Directory from sector 2 and search it for indicated filename.
   return readfile(bx, cx);
  }else if(ax == 0x04){
    return executeProgram(bx);
  }else if(ax == 0x05){
    terminate();
  }else if(ax == 0x07){
    return deleteFile(buf);
  }else if(ax == 0x08){
    return writeFile(bx,cx,dx);
  }
  else{
    printString("There is an Error!\0"); //prints out the error message
    return -1;
  }
  
}


/* helper function to keep track of how many digits does integer x have
 * @param int x
 * @return digit
 */
int countDigit(int x){
  int digit = 0;
  
  do{ 
    x = DIV(x,10); //ex) 4000, 400, 40, 4, 0
    digit++; //ex) 1, 2, 3, 4, 5
    
  }while(x != 0);

  
  return digit;
}


/* reverse the character orders
 * @param char * str, int length
 */
void reverse(char * str, int length){
  int start = 0;
  int end = length -1;

  for(start = 0; start < end; start++){
    int temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
  }
}


/* convert the input integer into the string whose length is 5
 * @param int num, int length
 * @return str
 */
char* itoa(int num, int length){ //x: 5, length: 1
  int index = 0;
  int isNegative = 0; // false
  char * digits = "0\0 1\0 2\0 3\0 4\0 5\0 6\0 7\0 8\0 9\0"; //char * that stores all the possible modulus values
  char readCh [2]; //helper variable to read the character
  char * str; //the string value that contains the converted string value from integer value.

  readCh[1] = '\0';
  
  if(num == 0){ //Case1 : when x = 0
    str[index] = '0';
    index++;
    str[index] = '\0'; //append the null termination. 
    printString("00000\0"); // print 00000 if we want to print 0
    return str;
  }
  
   //we only want to print the decimal within the length of 5
  if(length > 5){
    printString("Sorry, it's more than a limit\0");
    return "\0";
  }

  //for negative integer, set the boolean isNegative value to true.
  else if(num < 0){
    isNegative = 1;
    num = -num; //we will add the sign later
  }

  //we are putting every digit number into the string array in a reversed order
  while(num != 0){ 
   
    int mod = MOD(num, 10); //notice that we are printing out the decimal
    readCh[0] = digits[mod*3]; // ex) if the modulus is 5, the index for 5 is 15. 
    //printString(&readCh[0]); debugging purpose
    str[index++] = readCh[0];
    str[index]='\0'; //append the null termination.
    num = num / 10;
  }

  //filling out with 0's 
  if(index <= 4){
    while(index < 5){
      if(index == 3 && isNegative == 1){ //for negative four digit number, we dont' have to do.
	break;
      }    
      str[index] = '0'; //otherwise, we have to fill out with 0's whose digit is less than 5.
      index++;
    }
  }

  //if the number we originally passed is negative then we append the sign
  if(isNegative == 1){ 
    str[index++] = '-';
  }
  str[index] = '\0'; //append the null termination

  //at this point, we will have a string that is written in reverse order.
  //So, we have to reverse the reversed the order now so that we can print out our original number
  reverse(str, index);

  printString(str); 
  
  return str;
}


/**
 * Bonus Feature #2
 * print the Integer on the screen
 * taken a single integer argument, we will convert this interger to string and print out
 * @param int x
 * @return str
 */
char* printInt(int x){
  //
  char * str; // char * that we will eventually print on the screen.
  
  int length = countDigit(x); //count how many digits that the x has
  int i = 0;
 
  if(x < 0){ //if x is a negative number then we can include sign when counting a length of the digits
    length++; // - sign
  }
  
  str = itoa(x, length); //change the integer to the string form 
  //printString(itoa(x, str, length));
  
  //printString(str); // print the String-formatted integer
  return str;
}


int searchFile(struct directory * diskDir, char * filename){
  int i =0;
  int j = 0;
  for (i=0; i<16; i++){ //traverse 16 entries
    for (j=0; j<6; j++){ //traverse the first 6 bytes to compare the filename
      if (diskDir->entries[i].name[j] != filename[j]) {
	break;
      }
      if(j == 5){
	return i;
      }
    }
  }

  return -1;
}


/**
 * Reads in Disk Directory from Sector 2 and searches for indiciated filename.
 * If filename found, reads each sector of the file and store into buffer.
 *
 * @param filename:  Null terminated string indicating name of the file to be read.
 * @param buffer:    buffer where contents of the file are to be placed.
 * @return number of sectors read OR -1 if filename not found.
 *
 * @author: John Chu, Amir Zawad, Adia Wu
 */
int readfile(char * filename, char * buffer){
  int i;
  int j;
  int sectorCursor = 0;
  char sectorBuffer[512];
  int count = 0;

  struct directory diskDir;

  int entry = -1;
  int index = 0;
 
  readSector(&diskDir,2);  // read Disk Directory from sector 2.

  entry = searchFile(&diskDir, filename);

  if (entry != -1){ //when file is found
    
    while(diskDir.entries[entry].sectors[sectorCursor] != 0x00 && sectorCursor <= 25){

      /* read each sector from file*/
      readSector(buffer + count, diskDir.entries[entry].sectors[sectorCursor]);

      sectorCursor++;
      count+=512;
    }
    //entry = sectorCursor;
    return sectorCursor;   // number of sectors read.
  }else{
    printString("File not found!\0");
    return -1;
  }
}

/**
 * A helper function to copy the string from src to dest
 * 
 * @param char * src, char * dest
 * @return 1 
 */
int kStrcpy(char * src, char * dest) {
  int i=0;
  
	while(src[i] != '\0') {
	  putInMemory(0x1000, dest+i, src[i]);
	  i++;
     	}

	dest[i] = 0x00;
	return 1;
}


/**
 * Uses readFile function to load executable file (given by char * name)
 * from disk into buffer. Uses putInMemory function from kernel.asm to
 * transfer executable program from buffer into mempry segment to be executed.
 *
 * @param name:    name of program to execute.
 * @param segment: segment into which program should be loaded.
 *
 * @return -1 if program was not found OR -2 if segment is not valid.
 * @return 1 if program cannot be executed.
 * @author John Chu, Amir Zawad, Adia Wu
 */
int executeProgram(char * fname){

  int index;
  char buffer[13312]; //maximum size of the program file
  int numSectors; //number of sectors read from the file "fname"
  int segIdx;
  int segment; // a place where we will load PCB

  struct PCB *process; 

  index = 0;
  
  numSectors = readfile(fname, buffer); 

  if(numSectors == -1){ //when file is not found
    printString("program was not found \0");
    return -1;
  }

  setKernelDataSegment();
  
  segIdx = getFreeMemorySegment();

  restoreDataSegment();
  
  if(segIdx == -1){
    printString("Invalid segment!\0");
    return -2;
  }

  segment = 0x2000 + (segIdx * 0x1000); //0x1000 = 4096

  // Obtain a DEFUNCT PCB 

  setKernelDataSegment();
  process = getFreePCB(); //get the available process
  restoreDataSegment();
  
  kStrcpy(fname, process->name);

  //printString(fname); shell
  //printString(proces->name); shell
  
  process->state = STARTING; //set the process state to STARTING
  process->stackPointer = 0xFF00; // set the stack pointer to 0xFF00


  
  while(index < (numSectors * 512)){
     putInMemory(segment, index, buffer[index]);
     index++;
  }

   
  //initializeProgram(segment);

  launchProgram(segment);
  
  return 1; 
}


/**
 * terminate the currently executing program & run the shell program
 *
 * @return void
 * @author John Chu, Amir Zawad, Adia Wu 
*/
void terminate(){

  /*reset segment registers and the stack pointer to memory segment containing kernel (0x1000)*/

  setKernelDataSegment();
  

  
  //free the memory segment that is using
  releaseMemorySegment(running);


  //free the PCB that is using
  releasePCB(running);



  //set the state of the running PCB to DEFUNCT
  running->state = DEFUNCT;

  restoreDataSegment();

  while(1);
}

/**
* @function writeSector: write on sector for writeFile functionality.
*                        Accomplished using the same BIOS call as 
*                        readSector (interrupt 0x13), with AH == 3.
* 
* @param char * buffer:  address of the buffer in which the data 
*                        in the sector should be written.
* @param int sector:     sector to which the buffer should be written.
* @return 1 
* @author John Chu & Amir Zawad & Adia Wu
*/
int writeSector(char * buffer, int sector){
  int relSector = MOD(sector, 18) + 1;
  int op = DIV(sector, 18);
  int head = MOD(op,2);

  int track = DIV(sector,36);

  interrupt(0x13, 3 * 256 + 1, buffer, track*256 + relSector, head*256+0);

  return 1;
}

/**
* @function deleteFile: Function to delete file using a two step process:
*                       1) Sectors allocated to the file are marked free in Disk Map (0x00)
*                       2) First character of the filename in Disk Directory is set to 0x00.
*
* @param char * fname:  Name of the file to be deleted.
* @return 1:            if file is successfully deleted
* @return -1:           if file is not found
* @author John Chu & Amir Zawad & Adia Wu
*/
int deleteFile(char * fname){
  
  int i;
  int j;
  int k;
  int sectorCursor = 0;
  int sectors[27];
  int sector;

  char map[512];
  struct directory diskDir;

  int entry = -1;
 

  sector = 0;

  printString("delete in kernel\0");

  /*read the sectors from the directory and map*/
  readSector(map,1);  // read Disk Directory from sector 1
  readSector(&diskDir, 2);

  //search the disk directory and fine the fname
  entry = searchFile(&diskDir, fname);

  if (entry != -1){ //when file is found

    for(i = 0 ; i < 6; i++){
      diskDir.entries[entry].name[i] = 0x00;
    }

    for(j = 0; j < 26; j++){
      sectors[j] = diskDir.entries[entry].sectors[j];
      diskDir.entries[entry].sectors[j] = 0x00;
      sectorCursor++;
    }

    sectors[26] = 0x00;

    for(k = 0; k < sectorCursor; k++){
      sector = sectors[k];
      if(sector == 0){
	break;
      }

      map[sector]= 0x00;
    }
    
    // write Disk Map and Disk Sector back to disk to save changes.
    writeSector(map,1);
    writeSector(&diskDir,2);

    return 1; // Successfully deleted
  }else{ 
    printString("File not found!\0");
    return -1; 
  }
}

/**
* @function writeFile:  Function to write into a free sector on the disk.
*                       Modified Disk Directory and Disk Map written back
*                       to disk to save changes. Writes sectors * 512 bytes
*                       of data from buffer into file (fname). Overwrites the file
*                       if file fname exists. Max number of sectors written is 26.
*
* @param char * fname:  name of file to be written.
* @param char * buffer: pointer to buffer which is written into file fname.
* @return sector:       the number of sectors
* @return -1:           no Disk Directory available, so file not written.
* @return -2:           no. of sectors in Disk Map < written sectors,
*                       so write as many sectors.
* @author John chu & Amir Zawad & Adia Wu 
*/

int writeFile(char * fname, char * buffer, int sectors){

  char map[512];
  struct directory diskDir;

  int directoryEntry, i,j,k,idx, residue;
  int fileNameLength = 0;

  int sectorNum;

  char helperFileBuffer[512];
  char subBuffer[512];

  int flag = -1; // to see if it found the free entry

  int val;

  
  /*Read the Map & Directory from Sectors*/
  readSector(map,1);
  readSector(&diskDir,2);
  
 /*Find a free directory entry*/
  for(directoryEntry = 0; directoryEntry < 16; directoryEntry++){
    //Found the free entry
    if(diskDir.entries[directoryEntry].name[0] == 0x00){
      flag = 1;
      break;
    }
  }

  /*If there is no free directory*/
  if(flag == -1){
    printString("No Empty location available for the file!\0");
    return -1;
  }

  /*Count the length of the filename*/
  while(fname[fileNameLength] != '\0' && fname[fileNameLength] != 0x00){
    fileNameLength++;
  }

  /*Assign the filename into the directory free entry*/
  for(j = 0; j < fileNameLength; j++){
    diskDir.entries[directoryEntry].name[j] = fname[j];
  }

  if(fileNameLength < 6){
    residue = 6 - fileNameLength;

    for(j = 0; j < residue; j++){
      //fill the residues with 0x00
      diskDir.entries[directoryEntry].name[j+fileNameLength] = 0x00; 
    }
  }

  /*Write the file contents into the sectors consisting of the file*/
  for(k = 0; k < sectors; k++){
    sectorNum = 0;

    while(map[sectorNum] != 0x00){
      sectorNum++;
    }

    //No free sectors to write the file
    if(sectorNum == 26){
      printString("Not enough space in the directory!\0");
      return -2;
    }

    map[sectorNum] = 0xFF;

    diskDir.entries[directoryEntry].sectors[k] = sectorNum;

    /*Store the file sectors that the buffer is holding*/
    for(j = 0; j < 512; j++){
      val = k+1;
      helperFileBuffer[j] = buffer[j*val];
    }

    writeSector(helperFileBuffer, sectorNum);
  }

  /*Write the Map and Directory to the disk*/
  writeSector(map,1);
  writeSector(&diskDir,2);

  //return the number of sectors to be written
  return sectorNum;
}

/**
 * OS schedules processes using round-robin scheduling.
 * handleTimerInterrupt(segment, sp) will occur each time a timer interrupt occurs.
 * segment and stackpointer of the process that was interrupted will be passed to save
 * the running process.
 * 
 * @param int segment, int sp
 * @author John Chu & Amir Zawad & Adia Wu
 */
void handleTimerInterrupt(int segment, int sp) {

  int i;
  
  //struct PCB process;

  struct PCB * removeHead;

  int newSegment, newSp;

  setKernelDataSegment();

 
  //STEP 1: Save the current process
  if(running != NULL){
    running->segment = segment;
    running->stackPointer = sp;
    running->state = READY;
   
  }

  restoreDataSegment();


  setKernelDataSegment(); 

  // STEP 2: Check if the running process is the idle process
  if(running == &idleProc){
    running == NULL;
  }else{
    //We cannot add Defunct running process to the ready queue
    if(running->state != DEFUNCT){
      addToReady(running);
    }
  }

  restoreDataSegment();
  
  // If ready queue is empty, then we should schedule an idle process.
  if(readyHead == NULL){
    idleProc.state = RUNNING;
    running = &idleProc; //idle Process scheduled
  }



  setKernelDataSegment();
  //STEP 4: remove the head PCB of the ready queue (RR algorithm)
  removeHead = removeFromReady();

  restoreDataSegment();
  
  if(removeHead == NULL){
    removeHead = &idleProc;
  }


  removeHead->state = RUNNING;


  newSegment = removeHead->segment;

  newSp = removeHead->stackPointer;

  running = removeHead;


  returnFromTimer(newSegment,newSp);
}


