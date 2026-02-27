#include "file.h"

#include <stdio.h>

int write_file(const char *filename, const char *string)
{
    FILE *fptr = fopen(filename, "w");
    if(!fptr)
        return -1;

    if(string)
        fputs(string, fptr);

    fclose(fptr);
    return 0;
}

int read_file(const char *filename, char *string)
{
    FILE *fptr = fopen(filename, "r");
    if (!fptr)
        return -1;

    char buffer[100];

    while(fgets(buffer, 100, fptr))
    {
        printf("%s\n", buffer);
    }

    fclose(fptr);
    (void)string;
    return 0;
}