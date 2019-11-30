#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME "log.txt"

static unsigned int logCount = 0;

void writeLog(const char* data)
{
    logCount++;
    FILE* file = fopen(FILENAME, "a");

    fprintf(file, "%u: %s", logCount, data);

    fclose(file);
}