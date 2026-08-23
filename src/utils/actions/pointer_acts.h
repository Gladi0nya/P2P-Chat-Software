//Protect it from being included twice
#ifndef PointerActsFunctions
#define PointerActsFunctions

#include <stdint.h>


/*
    ---------------------------------------------------------------------
                            Freeing pointers
    ---------------------------------------------------------------------
*/
//Free any type of pointer and places NULL as its new value
//void freeNull(void* toFree);

//Free pointer of char pointer
void freePP(char** restrict const pointer);

//Freeing a gigantic char**
void freePPP(char*** restrict const pointer);



/*
    ---------------------------------------------------------------------
                        Getting pointers larger
    ---------------------------------------------------------------------
*/
//Getting the provided char pointer (size + 1) times larger and returns the new size
//uint64_t largerPointer(char* pointer,uint64_t size);




//End of the if from the beginning of the file
#endif