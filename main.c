#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void primary(int argc, char *argv[]);
void dummy(int primary_pid);
void keepalive(int primary_pid);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        primary(argc, argv);
    }
    else if (argc == 3 && strcmp(argv[1], "dummy") == 0)
    {
        dummy(atoi(argv[2]));
    }
    else if (argc == 3 && strcmp(argv[1], "keepalive") == 0)
    {
        keepalive(atoi(argv[2]));
    }
    else if (argc == 3 && strcmp(argv[1], "primary") == 0)
    {
        primary(argc, argv);
    }
    else
    {
        fprintf(stderr, "Usage: %s [dummy|keepalive|primary <pid>]\n", argv[0]);
        exit(1);
    }
    return 0;
}