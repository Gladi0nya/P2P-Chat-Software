#include "FileActs.h"




/*
    ---------------------------------------------------------------------
                        Connection preparations
    ---------------------------------------------------------------------
*/

//Checking if the character given is from the ASCII characters
int asciiVal(char val){
    //Returns 1 if it is. Otherwise, 0
    return ('a' <= val && val <= 'z') || ('A' <= val && val <= 'Z') || ('0' <= val && val <= '9');
}

//Checking whether the name provided is correct for a file or not
int checkName(char* name){
    //If the provided name is NULL
    if (name == NULL){
        //Returns 0
        return 0;
    }
    //Counter for all of the points '.' of the name
    int countPoint = 0;
    //Looping over the name provided to the function
    for (int index = 0 ; name[index] != '\0' ; index++){
        //If the current character is a point
        if (name[index] == '.'){
            //Incrementation of the counter of points
            countPoint++;
            //If the amount of points is over 1
            if (countPoint > 1){
                //Returns 0
                return 0;
            }
        }
        //If the current character is not one of the ASCII values
        else if (!(asciiVal(name[index]))){
            //Returns 0
            return 0;
        }
    }
    //Then, the name is valid. Returning 1
    return 1;
}


/*
    ---------------------------------------------------------------------
                            Open/Close file
    ---------------------------------------------------------------------
*/

//Truly openning the given file (no checking of the name)
FILE* trueOpenFile(FILE* log,char* nameFile,char* typeOpen){
    //Openning that provided file in reading mode
    FILE* file = fopen(nameFile,typeOpen);
    //If the file openning failed
    if (file == NULL){
        //Printing an error message inside of the log file
        editLog(log,"trueOpenFile",0,"Failed to open the given file");
        //Closing the opened file
        closeFile(file);
        //Returns NULL
        return NULL;
    }
    //Returns the file openned
    return file;
}

//Openning the file corresponding to the name provided
FILE* openFile(FILE* log,char* nameFile,char* typeOpen,char* securedName){
    //If the arguments are right
    if (nameFile != NULL){
        //If the provided name is NULL or not valid
        if (!(checkName(nameFile))){
            //If the name here in case is not usable
            if (securedName == NULL){
                //Printing an error message inside of the log file
                editLog(log,"openFile",0,"Name given is not valid");
                //Returning NULL, there won't be a file opened
                return NULL;
            }
            //If the name here in case is usable
            else{
                //If the provided name is NULL or not valid
                if (!(checkName(securedName))){
                    //Printing an error message inside of the log file
                    editLog(log,"openFile",0,"The security name given is not valid");
                    //Returning NULL, there won't be a file opened
                    return NULL;
                }
                //If the name here in case is usable
                else{
                    //Openning the file linked to the provided name and returning it
                    return trueOpenFile(log,securedName,typeOpen);
                }
            }
        }
        //If the name provided is valid
        else{
            //Openning the file linked to the provided name and returning it
            return trueOpenFile(log,nameFile,typeOpen);
        }
    }
    //Returns NULL
    return NULL;
}
//Close the given file
void closeFile(FILE* toClose){
    //If the given file is not NULL
    if (toClose != NULL){
        //Closing the file
        fclose(toClose);
    }
}




/*
    ---------------------------------------------------------------------
                Actions without modifications of the file
    ---------------------------------------------------------------------
*/

//Get the size of the opened file
long int sizeFile(FILE* log,FILE* file){
    //Initialisation of a long integer to contain the size of the file provided
    long int length = -1;
    //If the provided arguments are usable
    if (file != NULL){
        //Going to the end of the file (moved by 0)
        fseek(file,0,SEEK_END);
        //Getting the amount of characters the given file has
        length = ftell(file);
        //Going to the beginning of the file (moved by 0)
        fseek(file,0,SEEK_SET);
    }
    //If the provided file is not usable
    else{
        //Printing an error message inside of the log file
        editLog(log,"sizeFile",0,"File provided does not exists");
    }
    //Returns the size of the file
    return length;
}

//Reading the whole file content
char* contentFile(FILE* log,FILE* file,long int len){
    //If the provided arguments are right
    if (file != NULL && len > 0){
        //Allocating memory to a new string
        char* content = malloc(sizeof(char) * (len + 1));
        //If the memory allocation failed
        if (content == NULL){
            //Freeing the content char*
            free(content);
            //Printing an error message inside of the log file
            editLog(log,"contentFile",0,"Memory allocation failed");
            //Returns NULL
            return NULL;
        }
        //Reading the content of the file and placing (reading characters of 1 byte)
        fread(content,1,len,file);
        //Placing an end to the string made out of the file's content
        content[len] = '\0';
        //Returns the content found out of the file
        return content;
    }
    //Printing an error message inside of the log file
    editLog(log,"contentFile",0,"At least one of the provided arguments is not right");
    //Returns NULL
    return NULL;
}



/*
    ---------------------------------------------------------------------
                        Actions modifying the file
    ---------------------------------------------------------------------
*/
