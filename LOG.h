#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define LOG_NAME "log.txt"
#ifndef LOG_NAME
    #define LOG_NAME "log.txt"
#endif

FILE* startLog(FILE* logFile);

void endLog(FILE* logFile);

void printTime(FILE* file);

