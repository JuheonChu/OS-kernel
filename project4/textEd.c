#include "userlib.h"

/**
 * textEditor program
 * Enter the filename and contents of that file
 * @author John Chu & Amir Zawad & Adia Wu
 * 
 **/
void main(){

        int rv = 0;
        char file[13312];
	char *fileOffset;
	char line[81];
	char filename[7];
	int sectors;
	
	print("Enter filename: ");
	read(filename, 6);
	println("\n\r\0");

	println(filename);
	println("Do ctrl-D + Enter on its own line to quit\0");

	
	fileOffset = &file[0];
	
	while((fileOffset - file) < 13312) {
	  
	        rv = read(line,80); //read a line up to 80 characters
		
		 if (line[0] == 0x04) {	// ctrl-D to save the file
			break;
		}
		
		println("\0");  // go to the next line.
		
		strCpy(line, fileOffset, rv);
		fileOffset = fileOffset + rv;
		fileOffset[0] = '\n';
		fileOffset[1] = '\r';
		fileOffset = fileOffset + 2;
	}
	
	sectors = ((fileOffset - file) / 512) + 1;
	
	writeFile(filename, file, sectors);
	
	println("\0");
	
	exit(); //terminate the textEdit program
}
 

