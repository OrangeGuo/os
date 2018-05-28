#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
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

void quit(char *command) {
    char *flag = strchr(command, '>');
    if (flag == NULL) {
        char *p = strtok(command, " ");
        p = strtok(NULL, ">");
        printf("%s\n", p);
    } else {
        puts(command);

        char *p = strtok(command, ">");
        p=strtok(NULL, ">");
		puts(p);

        char *contents = strtok(command, " ");
        contents = strtok(NULL, ">");
        int fd = open(p, O_CREAT | O_RDWR, 0666);
        dup2(fd, 1);

        write(1, contents, 6);
        close(fd);

		
    }
    return;
}
int main() {
    int mysys(char *command);
    void quit(char *command);
    while (1) {
        char command[100];
        printf("$");
        // fgets(command,99,stdin);
        int index = 0;
        command[index] = getchar();
        while (command[index] != '\n') {
            index++;
            command[index] = getchar();
        }
        command[index] = '\0';
        if (strcmp(command, "exit") == 0)
            break;
        else {
            char temp[100];
            strcpy(temp, command);
            char *p = strtok(temp, " ");
            if (strcmp(p, "cd") == 0) {
                p = strtok(NULL, " ");
                chdir(p);
            } else if (strcmp(p, "pwd") == 0) {
                printf("current working directory: %s\n", getcwd(NULL, 0));
            } else if (strcmp(p, "echo") == 0) {
                quit(command);
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
