#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

void primary(int argc, char *argv[])
{
    pid_t keepalive_pid = 0;
    pid_t primary_pid = getpid();

    if (argc == 3 && strcmp(argv[1], "primary") == 0)
    {
        keepalive_pid = atoi(argv[2]);
        printf("Primary (PID: %d) monitoring existing Keepalive PID: %d\n", primary_pid, keepalive_pid);
    }
    else
    {
        printf("Primary started (PID: %d). Spawning dummy...\n", primary_pid);
        pid_t dummy_pid = fork();
        if (dummy_pid == 0)
        {
            char pid_str[20];
            snprintf(pid_str, sizeof(pid_str), "%d", primary_pid);
            execl("./keeper", "keeper", "dummy", pid_str, NULL);
            perror("execl");
            exit(1);
        }
        waitpid(dummy_pid, NULL, 0);

        // Read keepalive PID from file
        FILE *fp = NULL;
        int attempts = 0;
        while (attempts++ < 5 && !(fp = fopen("/tmp/keepalive.pid", "r")))
            sleep(1);

        if (!fp)
        {
            fprintf(stderr, "Failed to find Keepalive PID\n");
            exit(1);
        }
        fscanf(fp, "%d", &keepalive_pid);
        fclose(fp);
        printf("Discovered Keepalive PID: %d\n", keepalive_pid);
    }

    while (1)
    {
        if (kill(keepalive_pid, 0) == -1)
        {
            if (errno == ESRCH)
            {
                printf("Keepalive dead. Respawning...\n");
                pid_t new_dummy = fork();
                if (new_dummy == 0)
                {
                    char pid_str[20];
                    snprintf(pid_str, sizeof(pid_str), "%d", primary_pid);
                    execl("./keeper", "keeper", "dummy", pid_str, NULL);
                    perror("execl");
                    exit(1);
                }
                waitpid(new_dummy, NULL, 0);

                FILE *fp = fopen("/tmp/keepalive.pid", "r");
                if (fp)
                {
                    fscanf(fp, "%d", &keepalive_pid);
                    fclose(fp);
                    printf("New Keepalive PID: %d\n", keepalive_pid);
                }
                else
                {
                    fprintf(stderr, "Failed to read new PID\n");
                    exit(1);
                }
            }
        }
        sleep(1);
    }
}