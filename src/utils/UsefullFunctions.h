/*

    This file is the main import file used to include all of the functions that may be used in any projects

*/



//Check according to the one part chosen to include
#ifdef Inputs
//Verification avoiding multiple includes
#ifndef checkInputs
//Definition of variable avoiding multiple includes
#define checkInputs
//Including the chosen file
#include "Inputs/UserInputs.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif

//Check according to the one part chosen to include
#ifdef File
//Verification avoiding multiple includes
#ifndef checkFile
//Definition of variable avoiding multiple includes
#define checkFile
//Including the chosen file
#include "Actions/FileActs.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif

//Check according to the one part chosen to include
#ifdef Integer
//Verification avoiding multiple includes
#ifndef checkIntegers
//Definition of variable avoiding multiple includes
#define checkIntegers
//Including the chosen file
#include "Actions/IntegerActs.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif

//Check according to the one part chosen to include
#ifdef Pointer
//Verification avoiding multiple includes
#ifndef checkPointer
//Definition of variable avoiding multiple includes
#define checkPointer
//Including the chosen file
#include "Actions/PointerActs.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif

//Check according to the one part chosen to include
#ifdef String
//Verification avoiding multiple includes
#ifndef checkString
//Definition of variable avoiding multiple includes
#define checkString
//Including the chosen file
#include "Actions/StringActs.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif


//Check according to the one part chosen to include
#ifdef LogFile
//Verification avoiding multiple includes
#ifndef checkLogFile
//Definition of variable avoiding multiple includes
#define checkLogFile
//Including the chosen file
#include "LogDev/log.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif

//Check according to the one part chosen to include
#ifdef ConfigurationFile
//Verification avoiding multiple includes
#ifndef checkConfigurationFile
//Definition of variable avoiding multiple includes
#define checkConfigurationFile
//Including the chosen file
#include "Configurations/Config.h"
//End mark for the previous if
#endif
//End mark for the previous if
#endif
