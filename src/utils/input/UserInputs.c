#include "UserInputs.h"




/*
    ---------------------------------------------------------------------
                            stdin's actions
    ---------------------------------------------------------------------
*/

//Clearing stdin from everything the user may have entered
void clearStdin(){
    //Declaration of a variable to contain every value that is inside of stdin
    int stdVal;
    //Loop while stdin is not empty
    while ((stdVal = getchar()) != '\n' && stdVal != EOF){
        //Doing nothing of it
    }
}

//Letting user choose his action
int getUserInt(char* quest){
    //Creating an integer which will be returned
    int returnVal;
    //Printing that question
    printf("%s",quest);
    //Getting user's value (if the user's value was not placed inside of returnVal)
    if (!(scanf("%i",&returnVal))){
        //Clearing stdin
        clearStdin();
        //Returning -10
        return -10;
    }
    //Clearing stdin
    clearStdin();
    //Returns the integer value made out of user's interaction
    return returnVal;
}

//Letting user choose his action
char getUserChar(char* quest){
    //Printing that question
    printf("%s",quest);
    //Creating a char which will be returned
    char returnVal = getchar();
    //Clearing stdin
    clearStdin();
    //Returns the integer value made out of user's interaction
    return returnVal;
}

//Letting user choose his action
char* getUserString(char* quest){
    //Creating a string which will be returned
    char* returnVal = createInitString(NULL);
    //Size of the previoulsy defined string
    int size = 0;
    //Getting every variable from the user
    char stdVal;
    //Printing that question
    printf("%s",quest);
    //Getting user's response if it is not too big
    while (((stdVal = getchar()) != '\n') && size < maxUserStringSize){
        //Getting the pointer larger
        size = largerPointer(&returnVal,size + 1);
        //If the placement failed
        if (size == 0){
            //Clearing stdin
            clearStdin();
            //Freeing the main string
            freeNull((void**)&returnVal);
            //Returns NULL
            return NULL;
        }
        //Placing down the character inside of the main string
        returnVal[size - 1] = stdVal;
    }
    //Getting the pointer larger
    size = largerPointer(&returnVal,size + 1);
    //If the placement failed
    if (size == 0){
        //Clearing stdin
        clearStdin();
        //Freeing the main string
        freeNull((void**)&returnVal);
        //Returns NULL
        return NULL;
    }
    //Placing an end to the main string
    returnVal[size - 1] = '\0';
    //Returns the integer value made out of user's interaction
    return returnVal;
}