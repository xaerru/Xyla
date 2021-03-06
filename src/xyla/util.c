#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

char *
read_file (const char *path)
{
    FILE *file = fopen (path, "rb");
    if (file == NULL) {
        fprintf (stderr, "Couldn't open file \"%s\".\n", path);
        exit (EX_IOERR);
    }

    fseek (file, 0L, SEEK_END);
    size_t fileSize = ftell (file);
    rewind (file);

    char *buffer = (char *)malloc (fileSize + 1);
    if (buffer == NULL) {
        fprintf (stderr, "Not enough memory to read \"%s\".\n", path);
        exit (EX_IOERR);
    }

    size_t bytesRead = fread (buffer, sizeof (char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf (stderr, "Could not read file \"%s\".\n", path);
        exit (EX_IOERR);
    }

    buffer[bytesRead] = '\0';

    fclose (file);
    return buffer;
}

bool
is_digit (char c)
{
    return c >= '0' && c <= '9';
}

bool
is_alpha (char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
