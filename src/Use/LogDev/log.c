#include "log.h"

//Debug mode is declared as turned off
int debugMode = 0;
//Size that should be used to format the time
int sizeTime = 0;


/*
    ---------------------------------------------------------------------
                        Preparation to edit
    ---------------------------------------------------------------------
*/

//Choosing the kind of edit which editLog() has been called for
char* typeEdit(int type){
    //Character for default type of message
    char* typeMsg;
    //Comparison of type with
    switch(type){
        //If the edition is for an ERROR
        case 0:
            //Character to symbolise that there is an error
            typeMsg = "ERROR";
            break;

        //If the edition is for warning
        case 1:
            //Character to warn that there may be a mistake
            typeMsg = "WARN";
            break;

        //If the edition is for tracing
        case 2:
            //Character used to trace where the program passes by
            typeMsg = "TRACE";
            break;
        
        //If the edition is for debuging
        case 3:
            //Character used in debug session
            typeMsg = "DEBUG";
            break;

        //If the edition is for fatal
        case 4:
            //Character to symbolise a fatal error
            typeMsg = "FATAL";
            break;

        //If the edition is to keep on editing according to the previous line
        case 5:
            //Character for simply jumping to the next line
            typeMsg = "FOLLOW";
            break;

        //If the edition is about connecting/disconnecting to the file
        case -5:
            //Character for connection message
            typeMsg = "CONNECTION";
            break;
            
        //If the edition is about connecting/disconnecting to the file
        case -6:
            //Character for disconnection message
            typeMsg = "DISCONNECTION";
            break;
            


        
        //If the edition is about connecting/disconnecting to the file
        case -1:
            //Character signaling an information message
            typeMsg = "INFO";
            break;

        //In any other case, the edition will be an info
        default:
            //Character signaling an information message
            typeMsg = "INFO";
    };

    //Returning the chosen message type
    return typeMsg;
}

//Getting and formating the current time into a string
char* getTime(){
    //Getting the current time
    time_t now = time(NULL);
    //Converting that time into the local one
    struct tm *time = localtime(&now);
    //Amount of infos to convert
    const int amountData = 6;
    //Converted integers with the size of amountData
    char* converted[6] = {intToChar(time->tm_mday),intToChar(time->tm_mon + 1),intToChar(time->tm_year + 1900),intToChar(time->tm_hour),intToChar(time->tm_min),intToChar(time->tm_sec),};
    //Index for converted
    int index = 0;
    //If the conversion failed
    if (converted == NULL){
        //Returns NULL
        return NULL;
    }
    //If the size of the formated time is 0
    if (!(sizeTime)){
        // day, month, year, hour, minute, seconde and of the separations between each of them
        sizeTime = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 5;
    }
    //Character for converting month into an integer value
    char* formated = malloc(sizeof(char) * (sizeTime + 1));
    //If the memory allocation failed
    if (formated == NULL){
        //Freeing the whole list of strings
        for (int k = 0 ; k < amountData ; k++){
            //Freeing every string inside of the table
            freeNull((void**)&converted[k]);
        }
        //Returns NULL
        return NULL;
    }
    //Loop over the main string which will end up returned
    for (int indexConv = 0 ; indexConv < amountData ; indexConv++){
        //If the converted integer made an error
        if (converted[indexConv] == NULL){
            //Freeing the whole list of strings
            for (int k = 0 ; k < amountData ; k++){
                //Freeing every string inside of the table
                freeNull((void**)&converted[k]);
            }
            //Freeing the string table
            free(formated);
            //Returning NULL
            return NULL;
        }
        //If the day number is below 10
        else if (converted[indexConv][1] == '\0'){
            //First, placing a 0
            formated[index] = '0';
            //Then, placing the final integer
            formated[index + 1] = converted[indexConv][0];
            //Incrementation of the index (by 2)
            index += 2;
        }
        //If the day number is over or equal to 10
        else{
            //Loop over the string to place
            for (int indexString = 0 ; converted[indexConv][indexString] != '\0' ; indexString++){
                //Placing the whole integer's value
                formated[index] = converted[indexConv][indexString];
                //Incrementation of the main string's index
                index++;
            }
        }
        //Comparing the index of converted
        switch(indexConv){
            //Placing the date
            case 0:
            case 1:
                //Placing a separation between two dates' values
                formated[index] = '/';
                break;
            
            //Placing the time
            case 3:
            case 4:
                //Placing a separation between two times' values
                formated[index] = ':';
                break;

            //Any other case (separations)
            default:
                //Placing an "end" separation
                formated[index] = '\t';
        }
        //Freeing the used string of the converted integer
        freeNull((void**)&converted[indexConv]);
        //Incrementation of the main string's index
        index++;
    }
    //Placing an end to the made string
    formated[sizeTime - 1] = '\0';
    //Returns that string made out
    return formated;
}


/*
    ---------------------------------------------------------------------
                        Libraries' actions
    ---------------------------------------------------------------------
*/

//Function used to start a connection with the log file 
FILE* connectLog(char** nameFile,int len,char* typeOpen,int debug){
    //Declaration for the returned variable
    FILE* opened;
    //Openning the file with the provided name
    opened = openFile(NULL,*nameFile,typeOpen,"logFile.log");
    //Chaging the value of the debug mode to the one chosen by the editor
    debugMode = debug;
    //Editing the log file with an information about an initialisation of the logging program
    editLog(opened,NULL,-5,(*nameFile));
    //Freeing the file name
    freeNull((void**)nameFile);
    //Returns the opened file (=NULL on errors)
    return opened;
}
//Disconnection from the log file (same as fclose but used to keep the same norm of write)
int disconnectLog(FILE* usedFile){
    //If the provided file is not NULL
    if (usedFile != NULL){
        //Editing the log file with an information about disconnecting from the logging program
        editLog(usedFile,NULL,-6,NULL);
        //Closing that file
        closeFile(usedFile);
        //Returns 1
        return 1;
    }
    //Returns 0
    return 0;
}

//Function used to edit that log file
int editLog(FILE* toEdit,char* nameFunc,int type,char* msg){
    //If the provided file is not NULL
    if (toEdit != NULL){
        //Getting the formated current time
        char* time = getTime();
        //Taking the kind of edit which has to be done
        char* typeMsg = typeEdit(type);
        //If the message is used to symbolise a connection
        if (type == -5){
            //Printing a connection message
            fprintf(toEdit,"-----------------------------------------------------------------------------------------------\n%s\tConnection to \"%s\" file\n---------------------------------------\n",time,msg);
        }
        //If the message is used to symbolise a connection
        else if (type == -6){
            //Printing a disconnection message
            fprintf(toEdit,"---------------------------------------\n%s\tDisconnection from the log file\n-----------------------------------------------------------------------------------------------\n",time);
        }
        //If the message type is not to follow the previous message written and that the debug mode is on
        else if (type != 5 && debugMode){
            //If the function's name is not NULL
            if (nameFunc != NULL){
                //If the message here is not NULL
                if (msg != NULL){
                    //Printing the message in the log file
                    fprintf(toEdit,"%s\t%s\t%s\tMsg : %s\n",time,typeMsg,nameFunc,msg);
                }
                //If the message is NULL
                else{
                    //Printing the message in the log file
                    fprintf(toEdit,"%s\t%s\t%s\n",time,typeMsg,nameFunc);
                }
            }
            //If the function's name is NULL
            else{
                //If the message here is not NULL
                if (msg != NULL){
                    //Printing the message in the log file
                    fprintf(toEdit,"%s\t\t%s\tMsg : %s\n",time,typeMsg,msg);
                }
                //If the message is NULL
                else{
                    //Printing the message in the log file
                    fprintf(toEdit,"%s\t\t%s\n",time,typeMsg);
                }
            }
        }
        //If the message to be written is to follow the previous message
        else if (type == 5){
            //If the message string is not NULL
            if (msg != NULL){
                //Write it with an indentation
                fprintf(toEdit,"\t\t\t\t%s\n",msg);
            }
            //If the message string is NULL
            else{
                //Freeing the formated time
                free(time);
                //Returns 0
                return 0;
            }
        }
        //If the debug mode is off
        else{
            //Freeing the formated time
            free(time);
            //Returns 0
            return 0;
        }
        //Push what fprintf wrote in the log file
        fflush(toEdit);
        //Freeing the formated time
        free(time);
        //Returns 1
        return 1;
    }
    //Returns 0
    return 0;
}
