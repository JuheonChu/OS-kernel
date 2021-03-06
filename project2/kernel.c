#define MAX_LIMIT 10 //maximum number of characters to be printed out for readString()

//define the prototypes of OS kernel functions
void putChar(int row, int column, char ch, char color);
void putStr(int row, int column, char * str, char color);
int interrupt (char irq, int ax, int bx, int cx, int dx);
int printString(char * str);
int readChar();
int readString(char * buf);
int readSector(char * buf, int absSector);
int handleInterrupt21(int ax, int bx, int cx, int dx);
char* printInt(int x);

void main(){
  

  //printString("Hello World\0");  testing code for printString(char * str);

  //char * buf = "*****\0";
  //char ch;

  //testing for read character
  //printString("Type a char: \0");
  //ch = readChar();
  // buf[2] = ch;

  //printString("Read: \0");
  //printString(buf);
  //printString("\n\r\0");

  // testings for readString(char * buf)
  //char line[20];
  //printString("Enter a line : \0");
  //readString(line);
  //printString("\n\0");
  //printString(line);

  //testings for readSector
  //char buffer[512];
  //readSector(buffer,30);
  //printString(buffer);

  //testings for handleInterrupt21();
  //makeInterrupt21();
  //interrupt(0x21, 0x00,0,0,0);

  //testing for Creating printString, readChar, and readString System Calls
  //char line[80];
  //char ch[1];

  //makeInterrupt21(); //setup ISR for interrupt 0x21
  //interrupt(0x21, 0x00, "Type>\0", 0, 0); //display prompt
  //interrupt(0x21, 0x11, ch, 0, 0); //read character
  //line[0] = ch[0];
  //line[1] = 0x00;
  //interrupt(0x21, 0x00, line, 0, 0); //print string

  //Testing for Bonus Feature #1
  //char line [MAX_LIMIT];
  //printString("Enter a line : \0");
  //readString(line, MAX_LIMIT);
  //printString("\n\0");
  //printString(line);


  //Testing for Bonus Feature #2
  //int x =  24000;
  //printInt(x); //
  
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


/** this will print out the string returning how many characters in string we have in &str
 * @param char * str: the character to be printed out 
 * @return count (return the number of characters in the str)
 */
int printString (char * str){
  int count = 0; //local variable to keep track of how many character in the strings we have
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
	interrupt(0x10, 0x0E*256+buf[index], 0, 0, 0);
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
	ch = readChar();
}

/* read a series of characters from what user type through the keyboard
 * @param char * buf, int limit
 * char * buf is the string characters that user type through the keyboard.
 * int limit parameter is used for a bonus feature to set the threshold of the amount of character   s to be printed
 */
int readString(char * buf, int limit){

  //Before Bonus Feature #1.
  /*int index = 0;
  char ch;
  char readCh[2]; //helper character array to read the chrarcter

  readCh[1] = 0x00; // notice that every character or string has to finish with null pointer
  ch = 0x00;
  ch = readChar(); //call of non-function error
  
  while(ch != 0x0D){ //we continue getting the inputs until ENTER key is hit.

    buf[index] = ch; assign the input into our buf array.
    readCh[0] = ch; //read the input character
    printString(readCh); // it has to print readChar, not buf because buf is not terminated with null pointer yet.
    index++; // move to the next cursor
    ch = readChar(); //get another input from the user
  }

  buf[index]=0x00; //added null termination 
  return index; // return how many string characters are in char * buf
  */

  //Bonus #1 Feature & Backspace
  int index = 0; //cursor to keep track of the character in char * buf
  char ch;
  char readCh[2]; //helper character array to store the inputs.

  readCh[1] = 0x00; // notice that every character or string has to finish with null pointer
  ch = 0x00;
  ch = interrupt(0x16, 0, 0, 0, 0); // a call to read the character.
  //ch = readChar(); is also another way.
  
  while(ch != 0x0D){ //we continue getting the inputs until ENTER key is hit.
    if(index >= limit){ //if we are at the cursor position ahead of where we initially set as a limit
      //buf[index]=0x00; //since we cannot assign any further input character, we can add a null termination.
      ch = readChar();
      if(ch == 0x08){ //even if our cursor hits the limit, we still need to allow the Backspace operation works
	backspace(buf, index, ch); //backspace key operation
      }else{
	continue;
      }
    
    }else if(ch == 0x08){ //Backspace/Delete key entered
      if(index > 0){ //any index greater than 0 should allow the user to be able to delete the character that he/she previously typed.
	backspace(buf,index, ch); //backspace key operation
      }else{ //we don't have to do anything when there is no character in buf
	continue;
      }
   
    }else{ //normal situation for typing characters with the keyboard
      buf[index] = ch;
      readCh[0] = ch;
      printString(readCh); // it has to print readChar, not buf because buf is not terminated with null pointer yet.
      index++; //move the cursor to the next index
      ch = readChar(); //get another input from the user
    }
    
    
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


/* Creating the System Call Interface.
 * Depending on the value of ax, handleInterrupt21 will perform a diferent system call.
 * @param int ax, int bx, int cx, int dx
 * ax will be used to indicate which service is being requested, and bx,cx, and dx are used as the * address of a string to print, a buffer to fill or the number of the sector to be read)
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
    return readString(buf);
  }else{
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


/*
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
