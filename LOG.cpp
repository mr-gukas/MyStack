#include "LOG.h"

FILE* startLog(FILE* logFile)
{
    logFile = fopen(LOG_NAME, "a");
    setvbuf (logFile, NULL, _IONBF, 0);

    fprintf(logFile, "---------------------------Started logging---------------------------------\n");
    printTime(logFile);

    return logFile;
}

void printTime(FILE* file)
{
    time_t t = time(NULL);

    struct tm tm = *localtime(&t);
    
    fprintf(file, "DATE: %d-%02d-%02d\n"
                  "TIME: %02d:%02d:%02d\n",
                 tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

}

void endLog(FILE* logFile)
{
    fprintf(logFile, "END OF LOG!\n");
    fclose (logFile);
    logFile = nullptr;
}
