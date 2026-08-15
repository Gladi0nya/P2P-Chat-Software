#include "Config.h"


//Initialisation of a Congif*
Config* initConfig(FILE* log){
    //Takes place inside of the memory
    Config* init = malloc(sizeof(Config));
    //If the memory allocation failed (not really usefull but shows I thougth about it)
    if (init != NULL){
        //Initialisation of the name of the database in use
        init->database = NULL;
        //Initialisation of the user used to open the database
        init->user = NULL;
        //Initialisation of the password associated to that user
        init->password = NULL;
        //Initialisation of the host in use
        init->host = NULL;
        //Returns the Config* made out
        return init;
    }
    //Printing an error message inside of the log file
    editLog(log,"initConfig",0,"Memory allocation failed for Config*");
    //Returns NULL
    return NULL;
}

//Free the given Config*
void freeConfig(Config** toFree){
    //If the given Config* is not NULL
    if (toFree != NULL && *toFree != NULL){
        //If database's name is not NULL
        if ((*toFree)->database != NULL){
            //Freeing the database's name
            freeNull((void**)&(*toFree)->database);
        }
        //If host's name is not NULL
        if ((*toFree)->host != NULL){
            //Freeing the host's name
            freeNull((void**)&(*toFree)->host);
        }
        //If username is not NULL
        if ((*toFree)->user != NULL){
            //Freeing the username
            freeNull((void**)&(*toFree)->user);
        }
        //If the password is not NULL
        if ((*toFree)->password != NULL){
            //Freeing the password
            freeNull((void**)&(*toFree)->password);
        }
        //Free the provided Config and put it to NULL
        freeNull((void**)toFree);
    }
}



//Read the configuration file's data
int getConfig(FILE* log,char* nameFile,Config* cfg){
    //If the Config* provided is not NULL
    if (cfg != NULL && nameFile != NULL){
        //Openning that provided file in reading mode
        FILE* configFile = openFile(log,nameFile,"r",NULL);
        //If the file openning failed
        if (configFile == NULL){
            //Printing an error message inside of the log file
            editLog(log,"getConfig",0,"Failed to open the given Config file");
            //Returns 0
            return 0;
        }
        //Getting the size of the file
        long int sizeConfigFile = sizeFile(log,configFile);
        //Getting the content of the configuration file
        char* contentConfig = contentFile(log,configFile,sizeConfigFile);
        //If getting the file content failed
        if (contentConfig == NULL){
            //Printing an error message inside of the log file
            editLog(log,"getConfig",0,"Failed to get the content of the file");
            //Returns 0
            return 0;
        }
        //Closing the configuration file
        closeFile(configFile);

        //String containing the name of each part of the configuration file
        char* tempString = NULL;
        char* temporary;
        //Temporary index
        long int tempIndex;
        //Getting the return of the comparison function
        int compared;
        //Defining a list of all the characters searched
        char searched[] = {'=','\n','\0'};
        //List of all of the key words of the configuration file
        char* keyWords[4] = {"database","host","user","password"};
        //Size of the keyWords table
        const int sizeKeyWords = 4;
        //Loop over the file's content
        for (long int indexConfigFile = 0 ; indexConfigFile < sizeConfigFile ; ){
            //If the line begins with a usefull character
            if (contentConfig[indexConfigFile] != '#' && contentConfig[indexConfigFile] != '[' && contentConfig[indexConfigFile] != '\n' && contentConfig[indexConfigFile] != '='){
                //Taking the index of one of characters researched
                tempIndex = findOneAmongList(contentConfig,searched,indexConfigFile);
                //If the finding worked
                if (tempIndex >= 0){
                    //If the temporary string is NULL
                    if (tempString == NULL){
                        //Copying the part selected of the string
                        tempString = takePartLen(contentConfig,indexConfigFile,tempIndex - 1,sizeConfigFile);
                        //Incrementation of the main index
                        indexConfigFile = tempIndex;
                    }
                    //If the temporary string has a value
                    else{
                        //Loop over the table of key words
                        for (int indexKey = 0 ; indexKey < sizeKeyWords ; indexKey++){
                            //Comparing the made temporary string
                            compared = comparison(tempString,keyWords[indexKey]);
                            //If the comparison worked and that we have an equal symbol (=) at the very next index
                            if (compared == 1){
                                temporary = takePartLen(contentConfig,indexConfigFile,tempIndex - 1,sizeConfigFile);
                                //If currently editing the database
                                if (indexKey == 0){
                                    //Copying the part selected of the string
                                    cfg->database = temporary;
                                }
                                //If currently editing the host
                                else if (indexKey == 1){
                                    //Copying the part selected of the string
                                    cfg->host = temporary;
                                }
                                //If currently editing the user
                                else if (indexKey == 2){
                                    //Copying the part selected of the string
                                    cfg->user = temporary;
                                }
                                //If currently editing the password
                                else if (indexKey == 3){
                                    //Copying the part selected of the string
                                    cfg->password = temporary;
                                }
                                //Incrementation of the main index
                                indexConfigFile = tempIndex;
                                //Breaking out of the loop
                                break;
                            }
                            //If the comparison failed because one of the elements is NULL
                            else if (compared == -1){
                                //Breaking out of the loop
                                break;
                            }
                        }
                        //Placing the pointer back to NULL
                        freeNull((void**)&tempString);
                    }
                }
                //If the arguments searching for the special characters of the line ('\n' and '=') were invalid
                else if (tempIndex == -2){
                    //Printing an error message inside of the log file
                    editLog(log,"getConfig",0,"Arguments provided to findOneAmongList() were incorrect");
                }
                //If these characters could not be found
                else{
                    //Printing an error message inside of the log file
                    editLog(log,"getConfig",0,"None of the searched characters were found");
                }
            }
            //If the line begins with a character which won't be usefull
            else if (contentConfig[indexConfigFile] == '#' || contentConfig[indexConfigFile] == '['){
                //Printing an error message inside of the log file
                editLog(log,"getConfig",0,"Current line has no use");
                //Going to the next line
                indexConfigFile = findChar(contentConfig,'\n',indexConfigFile);
                //If the finding failed
                if (indexConfigFile == -1){
                    //Printing an error message inside of the log file
                    editLog(log,"getConfig",0,"End of the file, no more lines are available");
                    //Breaking out of the loop
                    break;
                }
                //If the arguments provided are not usable
                if (indexConfigFile == -2){
                    //Printing an error message inside of the log file
                    editLog(log,"getConfig",0,"At least one of the arguments provided is NULL (to findChar())");
                    //Breaking out of the loop
                    break;
                }
                //Incremetation of the index
                indexConfigFile++;
            }
            //If the line begins with a '\n' or a '='
            else{
                //Printing an error message inside of the log file
                editLog(log,"getConfig",0,"Current character has no use");
                //Incremetation of the index
                indexConfigFile++;
            }
        }
        //Freeing every variables but the config one
        freeNull((void**)&tempString);
        freeNull((void**)&contentConfig);

        //If each part of the config made are initialised
        if (cfg->database != NULL && cfg->host != NULL && cfg->user != NULL && cfg->password != NULL){
            //Returns 1
            return 1;
        }
        //Placing every part to NULL
        freeNull((void**)&cfg->database);
        freeNull((void**)&cfg->host);
        freeNull((void**)&cfg->user);
        freeNull((void**)&cfg->password);
        //Returns 0
        return 0;
    }
    //Returns -1
    return -1;
}