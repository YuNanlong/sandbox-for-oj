#ifndef KILLER_H
#define KILLER_H

#include <stdio.h>
#include <sys/types.h>

//kill_process函数被子线程调用时传递参数用的结构
struct KillerArgs{
    pid_t pid;
    long int max_running_time;
};

void *kill_process(void *args);

#endif /* KILLER_H */
