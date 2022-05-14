/**
 * user program to test the sleep(int seconds) functionality.
 * 
 * @author Professor Siddiqui
 * @testedBy John Chu & Amir Zawad & Adia Wu
 */
void main(){
     int i = 0;

     enableInterrupts();

     for(i = 0; i < 5; i++){
       
       sleep(1000);
       print("---- uprog4 ---- \n\r\0");
      }
      exit();
}
