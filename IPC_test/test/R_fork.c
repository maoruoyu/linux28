#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void create_subprocesses(int level) {
    if (level > 5) {
        return; // 递归终止条件
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // 子进程
        printf("This is level %d, process ID: %d, parent ID: %d\n", level, getpid(), getppid());
        create_subprocesses(level + 1); // 递归调用，创建更多的子进程
        exit(0); // 子进程完成任务后退出
    } else {
        // 父进程
        wait(NULL); // 等待子进程结束
    }
}

int main() {
    create_subprocesses(1); // 从第1层开始创建子进程

    // 原始父进程结束时的提示
    printf("Original parent process %d is ending.\n", getpid());

    return 0;
}