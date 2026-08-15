#include "logger.h"
#include <stdio.h>

#define MODULE_NAME "BOOTSTRAP"

int main(void) {

  if (startLogger())
    return 1;
  
  logMsg(INFO,    MODULE_NAME, "Test");  
  logMsg(WARNING, MODULE_NAME, "Test");
  logMsg(ERROR,   MODULE_NAME, "Test");
  logMsg(DEBUG,   MODULE_NAME, "Test");

  if (stopLogger())
    return 1;
  
  return 0;
}
