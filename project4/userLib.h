int printString(char * str);
int readChar();
int readString(char * str);
int readfile(char * filename, char * str);
void type(char * command);
int executeProgram(char * command, int segment);
void terminate();
int compareTo(char * str1, char * str2);
void clear(char * buffer);
int delete(char * fname);
int readSector(char * filename, int absSector);
void writeFileHelper(char * dest, char * buffer, int sectors);
int writeFile(Char * fname, char * buffer, int sectors);
void dir();

