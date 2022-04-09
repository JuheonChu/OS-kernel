/*header file for userlib.c*/

int printString(char * str);
int readChar();
int readString(char * str);
char* readStringHelper(char * str, int limit);
int readfile(char * filename, char * str);
void type(char * command);
int executeProgram(char * command, int segment);
void terminate();
void clear(char * buffer);
int delete(char * fname);
int readSector(char * filename, int absSector);
void writeFileHelper(char * dest, char * buffer, int sectors);
int writeFile(char * fname, char * buffer, int sectors);
void dir();

