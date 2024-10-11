#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int max_depth = 5; // 设定最大递归深度
    int i = 0;

    // 创建第一个子进程
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) {
        // 在子进程中递归创建子进程
        for (i = 1; i < max_depth; i++) {
            pid = fork();
            if (pid == -1) {
                perror("fork error");
                exit(-1);
            } else if (pid == 0) {
                // 打印子进程信息
                printf("I am child process at depth %d,parent id：%d, pid=%d\n", i, getppid(),getpid());
                continue; // 子进程继续循环
            } else {
                // 父进程等待子进程结束
                wait(NULL);
                break; // 父进程退出循环
            }
        }
    } else {
        // 父进程等待所有子进程结束
        while (wait(NULL) > 0); // 等待所有子进程
        printf("I am the parent process, pid=%d, exiting now.\n", getpid());
    }

    printf("------------------end of file \n");

    return 0;
}