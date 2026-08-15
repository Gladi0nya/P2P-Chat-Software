#include "IntegerActs.h"



/*
    ---------------------------------------------------------------------
                                Conversions
    ---------------------------------------------------------------------
*/

//Converting a char* into an integer    -   
unsigned long int charToInt(char* toConvert,int maxLength){
    //Create an integer out of the char*
    unsigned long int createdInt = 0;
    //Max length for an unsigned integer
    const int maxSize = 24;
    //If the provided char* is not NULL
    if (toConvert != NULL && maxLength < maxSize){
        //Getting the length of the current char
        int len = length(toConvert);
        //Loop over the char*
        for (int sizeUInt = 0 ; sizeUInt < len ; sizeUInt++){
            //If the unit of the number made out is inside of the maximum size for an unsigned integer
            if (sizeUInt < maxLength){
                //Comparing every part of the char*
                switch (toConvert[sizeUInt]){
                    //With '0'
                    case '0':
                    //With '1'
                    case '1':
                    //With '2'
                    case '2':
                    //With '3'
                    case '3':
                    //With '4'
                    case '4':
                    //With '5'
                    case '5':
                    //With '6'
                    case '6':
                    //With '7'
                    case '7':
                    //With '8'
                    case '8':
                    //With '9'
                    case '9':
                        //Changing the integer made to be returned
                        createdInt = createdInt * 10 + (toConvert[sizeUInt] - '0');

                    
                    //In any other case
                    default:
                        //Going out of the switch
                        break;
                }
            }
            //If the unit of the current number is out of the maximum size for an unsigned integer
            else{
                break;
            }
        }
    }
    //Returns the integer made out of the char* given
    return createdInt;
}




/*
    ---------------------------------------------------------------------
                            Tables actions
    ---------------------------------------------------------------------
*/

//Making the sum of all elements of the table given
int sumTable(int* tableInt,int sizeTable){
    //Sum of all the elements of the table of integers
    int sum = 0;
    //If the arguments are valid
    if (tableInt != NULL && sizeTable > 0){
        //Loop over the table
        for (int index = 0 ; index < sizeTable ; index++){
            //Incrementation of the sum by the amount of the table's element
            sum += tableInt[index];
        }
    }
    //Returns the sum made out
    return sum;

}
