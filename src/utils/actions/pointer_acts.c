#include "pointer_acts.h"

#include <stdlib.h>




/** ------------------------------------------------------------------------------- *
 *  freeNull                                                                        *
 *                                                                                  *
 *  Free any type of pointer and places NULL as its new value                       *
 *                                                                                  *
 *  @param toFree     The pointer which will be freed and assigned to NULL          *
 * -------------------------------------------------------------------------------- **/
/*
void freeNull(void* toFree)
{
    if (toFree != NULL && *toFree != NULL)
    {
        free(toFree);
        toFree = NULL;
    }
}
*/

/** ------------------------------------------------------------------------------- *
 *  freePP                                                                          *
 *                                                                                  *
 *  Free pointer of char pointer                                                    *
 *                                                                                  *
 *  @param pointer     The pointer which will be freed and assigned to NULL         *
 * -------------------------------------------------------------------------------- **/

void freePP(char** restrict const pointer)
{
    if (pointer != NULL)
    {
        for (uint32_t k = 0 ; pointer[k] != NULL ; k++)
        {
            if (pointer[k] != NULL)
            {
                free(pointer[k]);
            }
        }
        free(pointer);
    }
}


/** ------------------------------------------------------------------------------- *
 *  freePPP                                                                         *
 *                                                                                  *
 *  Free pointer of pointer of char pointer                                         *
 *                                                                                  *
 *  @param pointer     The pointer which will be freed and assigned to NULL         *
 * -------------------------------------------------------------------------------- **/

void freePPP(char*** restrict const pointer)
{
    if (pointer != NULL)
    {
        for (uint32_t k = 0 ; pointer[k] != NULL ; k++)
        {
            if (pointer[k] != NULL)
            {
                freePP(pointer[k]);
            }
        }
        free(pointer);
    }
}




/** ------------------------------------------------------------------------------- *
 *  largerPointer                                                                   *
 *                                                                                  *
 *  Getting the provided char pointer size times larger and returns the new size    *
 *                                                                                  *
 *  @param pointer     The pointer which will be freed and assigned to NULL         *
 *  @param size        Futur size of the pointer given                              *
 *                                                                                  *
 *  @retval 0        error                                                          *
 *  @retval size     new size of the pointer                                        *
 * -------------------------------------------------------------------------------- **/
/*
uint64_t largerPointer(char* pointer,uint64_t size)
{
    pointer = realloc(pointer,sizeof(char) * size);
    if (pointer == NULL)
    {
        return 0;
    }
    return size;
}
*/