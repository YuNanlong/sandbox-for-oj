#include "killer.h"
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

//确保子进程被终止
void *kill_process(void *args){
    pid_t pid = ((struct KillerArgs *)args)->pid;
    long int max_running_time = ((struct KillerArgs *)args)->max_running_time;
    
    if(pthread_detach(pthread_self()) != 0){
        kill(pid, SIGKILL);
        return NULL;
    }
    sleep((unsigned int)(max_running_time / 1000 + 1)); //监控线程等待限制的运行时间
    kill(pid, SIGKILL); //超过限制的运行时间后强行终止子进程
    return NULL;
}
