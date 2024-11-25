#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main() {
    FILE *task = fopen("./task.txt", "r");
    FILE *result = fopen("./result.txt", "w");
    FILE *status = fopen("./status.txt", "r+");
    int ch;
    char isReady[6];

    while (1) {
        rewind(status);
        fgets(isReady, sizeof(isReady), status);
        if (!(strcmp(isReady, "READY"))) {
            break;
        }
    }

    fseek(task, 0, SEEK_END);
    size_t taskFileSize = ftell(task);
    rewind(task);
    int num1, num2, res;
    char op;

    while (fscanf(task, "%d %c %d", &num1, &op, &num2) == 3) {
        fseek(task, 1, SEEK_CUR);
        switch (op) {
            case ('+') :
                res = num1 + num2;
                break;
            case ('-') :
                res = num1 - num2;
                break;
            case ('*') :
                res = num1 * num2;
                break;
            case ('/') :
                if (num2 == 0) {
                    puts("Dividing by zero detected (core dump)");
                    fclose(task);
                    fclose(result);
                    rewind(status);
                    ftruncate(fileno(status), 0);
                    fprintf(status, "DONE");
                    fclose(status);
                    raise(SIGFPE);
                    exit(EXIT_FAILURE);

                }
                res = num1 / num2;
                break;
            default :
                puts("Operation not found");
                fclose(task);
                fclose(result);                    
                rewind(status);
                ftruncate(fileno(status), 0);
                fprintf(status, "DONE");
                fclose(status);
                exit(EXIT_FAILURE);
        }

        fprintf(result, "%d\n", res);
    }
    rewind(status);
    ftruncate(fileno(status), 0);
    fprintf(status, "DONE");

    fclose(task);
    fclose(result);
    fclose(status);
    return 0;   
}