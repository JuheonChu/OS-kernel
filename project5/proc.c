#include "proc.h"

/**
 * This function initializes all of the global variables and 
 * structures defined in "proc.h"
 * 
 * @author John Chu & Amir Zawad & Adia Wu
 */
void initializeProcStructures(){
  //1. All entries in the memory map should be marked as FREE
  int cursor = 0;

  for(cursor = 0; cursor < 8; cursor++){
    memoryMap[cursor] = FREE; //FREE = 0x00
    pcbPool[cursor].name[0] = NULL; //first character of the name in the PCB in the PCB pool should be NULL
    pcbPool[cursor].state = DEFUNCT; //state in the PCB in the PCB pool set to DEFUNCT
    pcbPool[cursor].segment = 0x0000; //segment in the PCB in the PCB pool set to 0x0000
    pcbPool[cursor].stackPointer = 0x0000; //stackPointer in the PCB in the PCB pool set to 0x0000
  }

  /*The PCB for the idle process should be initialized with name 'IDLE', state 'Ready', and segments & stackpointers set to 0x000 */ 
  idleProc.name[0] = 'I';
  idleProc.name[1] = 'D';
  idleProc.name[2] = 'L';
  idleProc.name[3] = 'E';
  idleProc.name[4] = '\0';

  idleProc.state = READY; //idle Process in the ready state

  idleProc.segment = 0x0000; //idle Process segment: 0x0000
  idleProc.stackPointer = 0x0000; //idle Process stackPointer: 0x0000

  // running refers to the address of the idle Process
  running = &idleProc;

  //Ready Queue should be empty
  readyHead = NULL;
  readyTail = NULL;
}

/**
 * Returns the index of the first free memory segment or -1 if 
 * all of the memory segments are used.  The returned memory segment
 * should be marked as used.
 * 
 * @author John Chu & Amir Zawad & Adia Wu
 * @return freeMemoryIndex
 */
int getFreeMemorySegment(){
  int i = 0;

  int freeMemoryIndex = -1;
  
  for(i = 0; i < 8; i++){
    if(memoryMap[i] == FREE){
      freeMemoryIndex = i;
      break;
    }
  }

  memoryMap[freeMemoryIndex] = USED; // returned memory segment should be marked as "USED"

  return freeMemoryIndex;
}

/**
 * Release the memory segment indicated by the given index into
 * the memory map.
 *
 * @author John Chu & Amir Zawad & Adia Wu
 */ 
void releaseMemorySegment(int seg){
  int releaseSegment;
  int segIdx;
  
  releaseSegment = seg;
  
  segIdx = (releaseSegment / 4096) -2;

  memoryMap[segIdx] = FREE;
  
}

/**
 * Return a pointer to an available PCB from the PCB pool.  All of the
 * available PCBs should be marked as DEFUNCT.  The returned PCB should
 * be marked as STARTING. Return NULL if there are no PCBs available.
 *
 * @author John Chu & Amir Zawad & Adia Wu
 **/
struct PCB *getFreePCB(){

  int i;

  for(i = 0; i < 8;i++){
    if(pcbPool[i].state == DEFUNCT){
      pcbPool[i].state = STARTING;
	return pcbPool+i;
    }
  }

  return NULL;
}

/**
 * Release the provided PCB by setting its state to DEFUNCT, setting
 * its next and prev pointers to NULL and setting the first character
 * of its name to 0x00.
 *
 * @author John Chu & Amir Zawad & Adia Wu
 */
void releasePCB(struct PCB *pcb){

  pcb->state = DEFUNCT; //set the state to DEFUNCT
  pcb->next = NULL; //set the next PCB to NULL
  pcb->prev = NULL; //set the previous PCB to NULL
  pcb->name[0] = 0x00; //set the first character of the PCB's name to 0x00
  pcb->segment = 0x00;
  pcb->stackPointer = 0x00;
}

/**
 * Add the provided PCB to the tail of the ready queue.
 *
 * @author John Chu & Amir Zawad & Adia Wu
 */
void addToReady(struct PCB *pcb){

  //When the ready queue is empty;
  if(readyHead == NULL){
    readyHead = pcb;
    readyTail = pcb;
    pcb->next = NULL;
    pcb->prev = NULL;
  }else{ //When Ready queue already have PCB
    readyTail->next = pcb;
    pcb->prev = readyTail;
    pcb->next = NULL;
    readyTail = pcb;
  }
}

/**
 * Remove the PCB at the head of the ready queue and return a 
 * pointer to it.
 * 
 * @author John Chu & Amir Zawad & Adia Wu
 */
struct PCB *removeFromReady(){
  struct PCB *removePCB;

  //Empty Ready queue
  if(readyHead == NULL){
    return NULL;
  }else if(readyHead == readyTail){ //When there is only one PCB in ready queue

    removePCB = readyHead;
    readyHead = NULL;
    readyTail = NULL;
    return removePCB;
    
  }else{ //general case
    removePCB = readyHead;
    readyHead = readyHead->next;//remove the PCB at the head of the ready queue
    readyHead->prev = NULL;  //remove the reference of original ready Head's prev pointer
    return removePCB;
  }

}
