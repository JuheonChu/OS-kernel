main(){

        enableInterrupts();
	interrupt(0x21, 0x00, "WooHoo! I'm a user program!\r\n\0", 0, 0);

	//while(1);
	interrupt(0x21, 0x05, 0, 0, 0);
	
}
