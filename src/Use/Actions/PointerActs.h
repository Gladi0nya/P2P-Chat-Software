//Protect it from being included twice
#pragma once

/*
    ---------------------------------------------------------------------
                    Including a C standard libraries
    ---------------------------------------------------------------------
*/
//Memory actions (malloc/calloc/free and else)
#include <stdlib.h>



/*
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
----------------------- Separation pre-processor/functions ---------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
*/





/*
    ---------------------------------------------------------------------
                            Freeing pointers
    ---------------------------------------------------------------------
*/
//Free any type of pointer and places NULL as its new value
void freeNull(void** toFree);

//Free pointer of char pointer
void freePP(char*** pointer);

//Freeing a gigantic char***
void freePPP(char**** pointer);



/*
    ---------------------------------------------------------------------
                        Getting pointers larger
    ---------------------------------------------------------------------
*/
//Getting the provided char pointer (size + 1) times larger and returns the new size
int largerPointer(char** pointer,int size);

