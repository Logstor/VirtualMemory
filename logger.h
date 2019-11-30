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

double timeIt(void a())
{
    // Initialize
    clock_t start, end;
    double timeTaken;

    // Catch time
    start = clock();

    // Run function
    a();

    // Catch time
    end = clock();

    // Calc and return
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}