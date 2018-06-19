#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int mysys(char *command) {
    pid_t pid;
    if (command == NULL)
        return 1;
    else if ((pid = fork()) < 0)
        return -1;
    else if (pid == 0) {

        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(127);
    } else {
        pid_t wait_pid;
        while ((wait_pid = wait(NULL)) < 0) {
            if (errno = EINTR)
                return 0;
        }
    }
    return 0;
}

void mypipe(char *command) {
    char *p = strtok(command, "|");
    int pid, fd[2];
    pipe(fd);
    char buf[128];
    pid = fork();
    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execl("/bin/sh", "sh", "-c", p, NULL);
        _exit(127);
    }
    wait(NULL);
    p = strtok(NULL, "|");
    while (p != NULL) {
        printf("running %s...\n", p);
        pid = fork();
        if (pid == 0) {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execl("/bin/sh", "sh", "-c", p, NULL);
            _exit(127);
        }
        wait(NULL);
        p = strtok(NULL, "|");
    }
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);

    printf("Receive:\n");
    return;
}
void redirection(char *command) {
    int pid;
    char *flag = strchr(command, '>');
    if (flag == NULL) {
        char *p = strtok(command, " ");
        p = strtok(NULL, ">");
        printf("%s\n", p);
    } else {
        pid = fork();
        if (pid == 0) {
            char *p = strtok(command, ">");
            p = strtok(NULL, ">");
            char *contents = strtok(command, " ");
            contents = strtok(NULL, ">");
            while (*p == ' ')
                p++;
            int fd = open(p, O_CREAT | O_RDWR, 0666);
            dup2(fd, 1);
            close(fd);
            /* write(1, "", 0); */
            puts(contents);
            exit(0);
        }
        wait(NULL);
    }
    return;
}
int main() {
    int mysys(char *command);
    void redirection(char *command);
    while (1) {
        char command[100];
        printf("\033[34m%s\033[0m", getcwd(NULL, 0));
        printf("\033[32m$\033[0m");
        // fgets(command,99,stdin);
        int index = 0;
        command[index] = getchar();
        while (command[index] != '\n') {
            index++;
            command[index] = getchar();
        }
        if (index == 0)
            continue;
        command[index] = '\0';
        if (strcmp(command, "exit") == 0)
            break;
        else {
            char temp[100];
            strcpy(temp, command);
            char *p = strtok(temp, " ");
            if (strchr(command, '|') != NULL) {
                mypipe(command);
            } else if (strcmp(p, "cd") == 0) {
                p = strtok(NULL, " ");
                chdir(p);
            } else if (strcmp(p, "pwd") == 0) {
                printf("\033[34m%s\033[0m\n", getcwd(NULL, 0));
            } else if (strcmp(p, "echo") == 0) {
                redirection(command);
            } else if (strcmp(p, "ls") == 0) {
                if (strcmp(p, command) == 0) {
                    mysys(strcat(strcat(command, " "), getcwd(NULL, 0)));
                } else
                    mysys(command);
            } else
                mysys(command);
        }
    }
    return 0;
}
