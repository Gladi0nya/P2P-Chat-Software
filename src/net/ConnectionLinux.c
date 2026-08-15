#include "ConnectionLinux.h"


{
    //Initialisation of a socket with address's family = /communication = TCP/Protocol = the most adapted
    int createdSocket = socket(,SOCK_STREAM,0);
    //If the creation failed
    if (createdSocket == -1){
        
    }
}