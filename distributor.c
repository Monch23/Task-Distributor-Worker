#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

int main() {
    FILE *task = fopen("./task.txt", "w+");
    FILE *status = fopen("./status.txt", "w+");
    FILE *result = fopen("./result.txt", "w+");
    int num1, num2, stat, res;
    char op;
    char isDone[5] = {0};
    
    pid_t pid = fork();

    if (pid > 0) { // Distributor
        while (1) {
            puts("Input number 1 ([q] for exit)");
            if (!scanf("%d", &num1)) { break; }
            fflush(NULL);

            puts("[+ - * /] ([q] for exit)");
            if (!scanf("%c", &op)) { break; }
            fflush(NULL);
            
            puts("Input number 2 ([q] for exit)");
            if (!scanf("%d", &num2)) { break; }
            fflush(NULL);
            
            fprintf(task, "%d %c %d\n", num1, op, num2);
            system("clear");
        }   

        rewind(task);
        fprintf(status, "READY");
        rewind(status);
        sync();

        while (1) {
            rewind(status);
            fgets(isDone, sizeof(isDone), status);
            if (!(strcmp(isDone, "DONE"))) {
                break;
            }
        }
        wait(&stat);

        if (WIFEXITED(stat)) {
            while (fscanf(task, "%d %c %d", &num1, &op, &num2) == 3) {
                fseek(task, 1, SEEK_CUR);
                fscanf(result, "%d", &res);
                printf("%d %c %d = %d \n", num1, op, num2, res);
            }
        } else if (WIFSIGNALED(stat)) {
            printf("Child process terminated with signal <%d> \n", WTERMSIG(stat));
        }
    } else if (pid == 0) { // Worker
        execl("./worker", "worker", NULL);
        exit(EXIT_FAILURE);
    }

    fclose(task);
    fclose(result);
    fclose(status);
    return 0;
}