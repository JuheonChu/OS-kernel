

main(){

  enableInterrupts();
     interrupt(0x21, 0x00, "Something different! uprog2!\r\n\0", 0, 0);

     //makes a terminate system call
     interrupt(0x21, 0x05, 0, 0, 0);
}

