#include "PointerActs.h"




/** ------------------------------------------------------------------------------------------- *
 *  freeNull                                                                                   *
 *                                                                                              *
 *  Free any type of pointer and places NULL as its new value                                   *
 *                                                                                              *
 *  @param log      File used to make logs                                                      *
 *  @param toFree   The Config* to be freed                                                     *
 *  @param cfg      The Config* which will be edited with the given file's value                *
 *                                                                                              *
 *  @retval -1  error in arguments                                                              *
 *  @retval 0   error                                                                           *
 *  @retval 1   done                                                                            *
 *  ------------------------------------------------------------------------------------------- **/
//
void freeNull(void** toFree){
    //If the provided pointer is not already NULL
    if (toFree != NULL && *toFree != NULL){
        //Freeing that pointer
        free(*toFree);
        //Giving a NULL value to that pointer since wont ever be used again
        *toFree = NULL;
    }
}

//Free pointer of char pointer
void freePP(char*** pointer){
    //If the pointer provided is not NULL
    if (pointer != NULL && (*pointer) != NULL){
        //Looping over the event while k-elt is not NULL
        for (int k = 0 ; (*pointer)[k] != NULL ; k++){
            //If the k-elt of the pointer provided is not NULL
            if ((*pointer)[k] != NULL){
                //Freeing the k-elt
                free((*pointer)[k]);
            }
        }
        //Freeing the pointer and passing it to NULL
        freeNull((void**)pointer);
    }
}

//Freeing a gigantic char***
void freePPP(char**** pointer){
    //If the pointer provided is not NULL
    if (pointer != NULL && (*pointer) != NULL){
        //Loop over pointer
        for (int k = 0 ; (*pointer)[k] != NULL ; k++){
            //If pointer[k] is not NULL
            if ((*pointer)[k] != NULL){
                //Freeing that char**
                freePP(&(*pointer)[k]);
            }
        }
        //Freeing pointer and passing it to NULL
        freeNull((void**)pointer);
    }
}






/*
    ---------------------------------------------------------------------
                        Getting pointers larger
    ---------------------------------------------------------------------
*/

//Getting the provided char pointer size times larger and returns the new size
int largerPointer(char** pointer,int size){
    //Creating a temporary pointer with the old pointer reallocation
    char* temp = realloc((*pointer),sizeof(char) * size);
    //If the memory allocation failed
    if (temp == NULL){
        //Freeing the old pointer and placing it to NULL
        freeNull((void**)pointer);
        //Returns 0 as there is no more pointer
        return 0;
    }
    //Otherwise, setting the value of the temporary pointer to the one provided to the function
    (*pointer) = temp;
    //Returns the new size of the pointer given
    return size;
}
