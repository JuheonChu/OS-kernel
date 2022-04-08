#define MAX_LIMIT 80

void main(){

    char fileName[20];
    char buf[MAX_LIMIT];
    int numChars = 0;
    int index = 0; //cursor to keep track of the character in char * buf
    char ch;
    char readCh[2]; //helper character array to store the inputs.

  readCh[1] = 0x00; // notice that every character or string has to finish with null pointer
  ch = 0x00;

  printString("Enter a filename: \0");
  numChars = readString();
  printString("\r\n\0");
  
  ch = readChar(); // a call to read the character.
  //ch = readChar(); is also another way.
  
  while(ch != 0x04){ //we continue getting the inputs until ENTER key is hit.
    if(index >= MAX_LIMIT){ //if we are at the cursor position ahead of where we initially set as a limit
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
	interrupt(0x10, 0x0E*256+buf[index], 0, 0, 0);
	interrupt(0x10, 0x0E*256+0x08, 0, 0, 0);
      }else{ //we don't have to do anything when there is no character in buf
	if(index == 0){
	  ch = readChar();
	}
	continue;
      }
   
    }else if(ch == 0x0D){
      if(index >= MAX_LIMIT){
	ch = readChar();
      }else if(index + 1 >= MAX_LIMIT){
	buf[index] = '\n';
	readCh[0] = ch;
	printString("\r\n\0");
	index++;
      }else{

	buf[index] = '\n';
	buf[index+1] = '\r';
	readCh[0] = ch;
	printString("\r\n\0");
	index++;
	index++;
      }
    }

    else{ //normal situation for typing characters with the keyboard
      buf[index] = ch;
      readCh[0] = ch;
      printString(readCh); // it has to print readChar, not buf because buf is not terminated with null pointer yet.
      index++; //move the cursor to the next index
     
    }
    
    ch = readChar();
  }

 
    buf[index]=0x00; //added null termination 


    printString("\r\n\0");
    printString(buf);
    

    writeFile(fileName, buf, 10); //save the file

    printStirng("\r\n\0");

    terminate(); //terminate
     
    
}
