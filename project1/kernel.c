void putChar(char c, int row, int col, int color);
void putStr(char str[], int row, int col, int color);
void main(){
	putInMemory(0xB000, 0x8140, 'H');
	putInMemory(0xB000, 0x8141, 0x0F);	
	putInMemory(0xB000, 0x8142, 'e');
	putInMemory(0xB000, 0x8143, 0x0F);
	putInMemory(0xB000, 0x8144, 'l');
	putInMemory(0xB000, 0x8145, 0x0F);
	putInMemory(0xB000, 0x8146, 'l');
	putInMemory(0xB000, 0x8147, 0x0F);
	putInMemory(0xB000, 0x8148, 'o');
	putInMemory(0xB000, 0x8149, 0x0F);
	putInMemory(0xB000, 0x814A, 32);
	putInMemory(0xB000, 0x814B, 0x0F);
	putInMemory(0xB000, 0x814C, 'W');
	putInMemory(0xB000, 0x814D, 0x0F);
	putInMemory(0xB000, 0x814E, 'o');
	putInMemory(0xB000, 0x814F, 0x0F);
	putInMemory(0xB000, 0x8150, 'r');
	putInMemory(0xB000, 0x8151, 0x0F);
	putInMemory(0xB000, 0x8152, 'l');
	putInMemory(0xB000, 0x8153, 0x0F);
	putInMemory(0xB000, 0x8154, 'd');
	putInMemory(0xB000, 0x8155, 0x0F);

	putChar('H', 13, 35, 0x4F);
	putChar('e', 13, 36, 0x4F);
	putChar('l', 13, 37, 0x4F);
	putChar('l', 13, 38, 0x4F);
	putChar('o', 13, 39, 0x4F);
	putChar(' ', 13, 40, 0x4F);
	putChar('W', 13, 41, 0x4F);
	putChar('o', 13, 42, 0x4F);
	putChar('r', 13, 43, 0x4F);
	putChar('l', 13, 44, 0x4F);
	putChar('d', 13, 45, 0x4F);
	
	putStr("CompOS", 14, 35, 0x4F);
	while(1){
	}
}
void putChar(char c, int row, int col, int color){
  int offset = 0x8000+80*(row-1)*2+(col-1)*2;
  putInMemory(0xB000, offset, c);
  putInMemory(0xB000, offset+1, color);
}
void putStr(char str[], int row, int col, int color){
  int i = 0;
  while(str[i]!='\0'){
    putChar(str[i], row, col, color);
    i++;
    col++;
  }
}
