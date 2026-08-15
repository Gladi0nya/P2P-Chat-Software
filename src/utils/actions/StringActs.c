#include "StringActs.h"




/*
    ---------------------------------------------------------------------
                                Informations
    ---------------------------------------------------------------------
*/

//Getting the size of a char*
int length(char* charact){
    //Initialisation of the integer following the size of the char* given
    int size = 0;
    //If the given pointer is not NULL
    if (charact != NULL){
        //Loop over the char* given to the function
        while (charact[size] != '\0'){
            //Incrementation of k
            size++;
        }
    }
    //Return the size found
    return size;
}

//Comparing two char*s (returns 1 if they are the same)
int comparison(char* one,char* two){
    //If both char* provided as arguments are usable
    if (one != NULL && two != NULL){
        //Integer about the index of the char*
        int index = 0;
        //Loop over both char*s
        while (one[index] != '\0' || two[index] != '\0'){
            //If both char*s are different for the current index 
            if (one[index] != two[index]){
                //Returns 0 (=false)
                return 0;
            }
            //Incrementation of the index
            index++;
        }
        //If the final index is the same for both char*
        if (one[index] == two[index]){
            //Returns 1 (=true)
            return 1;
        }
        //Returns 0
        return 0;
    }
    //Returns -1
    return -1;
}

//Getting the first index where the character provided is
long int findChar(char* string,char searchingFor,long int start){
    //If the given arguments are usable
    if (string != NULL && start >= 0){
        //Loop over the string
        for (long int i = start ; string[i] != '\0' ; i++){
            //If the i-element of string is the one provided
            if (string[i] == searchingFor){
                //Returns the index found
                return i;
            }
        }
        //Returns -1
        return -1;
    }
    //Returns -2
    return -2;
}

//Getting the first index where one of the characters provided is
long int findOneAmongList(char* string,char* searchingFor,long int start){
    //If the given arguments are usable
    if (string != NULL && searchingFor != NULL && start >= 0){
        //Loop over the string
        for (long int indexString = start ; string[indexString] != '\0' ; indexString++){
            //Loop over the characters searching to find
            for (long int indexSearch = 0 ; searchingFor[indexSearch] != '\0' ; indexSearch++){
                //If the indexString-element of string is the one provided
                if (string[indexString] == searchingFor[indexSearch]){
                    //Returns the index found
                    return indexString;
                }
            }
        }
        //Returns -1, as nothing was found
        return -1;
    }
    //Returns -2, as at least one of the arguments provided is not usable
    return -2;
}

/*
//Getting the first index where the string provided is (Boyer-Moore algorithm)
int findString(char* string,char* searchingFor){
    //If the given arguments are usable
    if (string != NULL && searchingFor != NULL){
        //Length of the string trying to find
        int lenSearchedFor = length(searchingFor);
        //Initialising the index for searchingFor
        int indexSearch;
        //Loop over the string
        for (int indexString = 0 ; string[indexString] != '\0' ; indexString++){
            //Loop over the characters searching to find
            for (indexSearch = 0 ; searchingFor[indexSearch] != '\0' ; indexSearch++){
                //If one of the characters is different
                if (string[indexString + indexSearch] != searchingFor[indexSearch]){
                    //Breaking out of the loop
                    break;
                }
            }
            //If the loop went until the end
            if (searchingFor[indexSearch] == '\0'){
                //Returns the position
                return indexString;
            }
        }
        //Returns -1
        return -1;
    }
    //Returns -2
    return -2;
}
*/



/*
    ---------------------------------------------------------------------
                                Conversion
    ---------------------------------------------------------------------
*/

//Converting an integer into a character
char* intToChar(int toConvert) {
    //If the given integer == 0
    if (toConvert == 0){
        //Returns the string of "0"
        return createInitString("0");
    }
    //Integer used to count the amount of decimals inside toConvert
    int nbDigits = 0;
    //Copying the integer to convert
    int temp = toConvert;
    //Loop over the integer to convert
    while (temp > 0){
        //Incrementation of the amount of decimals
        nbDigits++;
        //Dividing the integer by 10
        temp /= 10;
    }
    //Allocating memory for the character which will be returned
    char* result = malloc(sizeof(char) * (nbDigits + 1));
    //If the memory allocation failed
    if (result == NULL){
        //Returns NULL
        return NULL;
    }
    //Placing an end to the char created
    result[nbDigits] = '\0';
    //Loop over the amount of decimals
    for (int index = (nbDigits - 1) ; index >= 0 ; index--){
        //Converting the decimal integer into a character
        result[index] = (toConvert % 10) + '0';
        //Dividing the integer by 10
        toConvert /= 10;
    }
    //Return the created character
    return result;
}




/*
    ---------------------------------------------------------------------
                            Creating char*
    ---------------------------------------------------------------------
*/

//Creates and initialise with the given value a new string
char* createInitString(char* fillIn){
    //Initialise a new string
    char* new = malloc(1);
    //If the string provided is not NULL
    if (fillIn != NULL){
        //Fill it with what was provided as an argument, if it failed
        if (!(placeInString(&new,fillIn,0))){
            //Returns NULL
            return NULL;
        }
    }
    //Returns the character made out
    return new;
}


//Creates a string out of a given part of another one looping over the whole string (except higher index than the lastCopy's one)
char* takePart(char* bigString,long int firstCopy,long int lastCopy){
    //If the arguments given are usable
    if (bigString != NULL && firstCopy >= 0 && lastCopy > firstCopy){
        //Getting the length of the string used to copy
        int len = lastCopy - firstCopy + 1;
        //Creating a string of the size which should be used
        char* smallString = malloc(sizeof(char) * (len));
        //If the memory allocation worked
        if (smallString != NULL){
            //Placing an end to that string
            smallString[len - 1] = '\0';
            //Loop over the part to copy
            for (int index = 0 ; (bigString[index] != '\0' && index < lastCopy) ; index++){
                //If the current index has passed through the one of the first one to copy
                if (index >= firstCopy){
                    //Place the current index's character inside of the smaller string
                    smallString[index - firstCopy] = bigString[index];
                }
            }
        }
        //Returns the made pointer
        return smallString; //Is NULL when malloc() fails
    }
    //Returns NULL
    return NULL;
}

//Creates a string out of a given part of another one only looping over the part to copy
char* takePartLen(char* bigString,long int firstCopy,long int lastCopy,long int lenBigString){
    //If the arguments given are usable
    if (bigString != NULL && firstCopy > 0 && firstCopy < lenBigString && lastCopy >= firstCopy && firstCopy < lenBigString){
        //Getting the length of the string used to copy
        int len = lastCopy - firstCopy + 1;
        //Creating a string of the size which should be used
        char* smallString = malloc(sizeof(char) * (len + 1));
        //If the memory allocation worked
        if (smallString != NULL){
            //Placing an end to that string
            smallString[len] = '\0';
            //Loop over the part to copy
            for (int index = firstCopy ; index <= lastCopy ; index++){
                //Place the current index's character inside of the smaller string
                smallString[index - firstCopy] = bigString[index];
            }
        }
        //Returns the made pointer
        return smallString; //Is NULL when malloc() fails
    }
    //Returns NULL
    return NULL;
}


/*
    ---------------------------------------------------------------------
                            String modifications
    ---------------------------------------------------------------------
*/

//Modifying the char*
char* modifyChar(char* instruction,char** toPlace,char toReplace){
    //If the arguments of the function are not NULL
    if (instruction != NULL && toPlace != NULL){
        //Initialise a char* to copy and modify instruction
        char* cpInstruction = createInitString("0");
        //If the memory allocation worked
        if (cpInstruction == NULL){
            //Returning the NULL pointer
            return NULL;
        }
        //Initialise integers following where we are in toPlace (the second char**)
        int indexToPlace = 0;
        //Length of the char* in use
        int lenToPlace;
        //Position of the index to be placed
        int eltCp = 0;
        //Loop over the size of the main char*
        for (int elt = 0 ; instruction[elt] != '\0' ; elt++){
            //If the elt-elt of instruction is a toReplace (to modify) and the indexToPlace-elt is not NULL
            if ((instruction[elt] == toReplace) && (toPlace[indexToPlace] != NULL)){
                //Length of the new char* in use
                lenToPlace = length(toPlace[indexToPlace]);
                //Reallocate the memory with the size +lenToPlace
                eltCp = largerPointer(&cpInstruction,(eltCp + lenToPlace));
                //If the memory reallocation did not work
                if (!(eltCp)){
                    //Returning the NULL pointer
                    return NULL;
                }
                //Loop over the size of the stuff to place
                for (int k = 0 ; k < lenToPlace ; k++){
                    //Modify the element with the indexToPlace, k element of toPlace
                    cpInstruction[eltCp - (lenToPlace - k)] = toPlace[indexToPlace][k];
                }
                //Incrementation of indexToPlace
                indexToPlace++;
            }
            //If the k-element is not one to be modifyed
            else{
                //Reallocate the memory with the size +1
                eltCp = largerPointer(&cpInstruction,eltCp + 1);
                //If eltCp == 0. In other words, the memory reallocation did not work
                if (!(eltCp)){
                    //Returning the NULL pointer
                    return NULL;
                }
                //Placing the char into the copy of the char* to modify
                cpInstruction[eltCp - 1] = instruction[elt];
            }
        }
        //Reallocate the memory with the size +1
        if (!(largerPointer(&cpInstruction,eltCp + 1))){
            //Returning the NULL pointer
            return NULL;
        }
        //Placing an end to the char*
        cpInstruction[eltCp] = '\0';
        //Returns the made char*
        return cpInstruction;
    }
    //Returns NULL
    return NULL;
}

//Place down the second provided char* at the (size-1) index inside a char*
int placeInString(char** string,char* toPlace,int currentIndex){
    //If the currentIndex provided is valid for the function or the string to place in string or the main string are not NULL
    if (currentIndex >= 0 && toPlace != NULL && string != NULL){
        //Variable about the current index used
        int copyIndex = currentIndex;
        //Reallocation of the string given and incrementation of the size linked to it
        currentIndex = largerPointer(string,currentIndex + length(toPlace) + 1);
        //If the memory reallocation did not work
        if (!(currentIndex)){
            //Returns 0
            return 0;
        }
        //Loop over the elements of current
        for (int index = 0 ; toPlace[index] != '\0' ; index++){
            //Placing the index-element of current
            (*string)[copyIndex + index] = toPlace[index];
        }
        //Place an end of string
        (*string)[currentIndex - 1] = '\0';
        //Returns the current index of the string (after placing the second char*)
        return currentIndex;//It now is the size
    }
    //Returns an impossible length for a string
    return -1;
}
