#include "container.h"
#include "executor.h"
#include "killer.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

//初始化Result结构
void init_result(struct Result *result){
    result->real_running_time = 0;
    result->real_memory = 0;
    result->exit_code = 0;
    result->result = SUCCESS;
}

//检查config的有效性
int verify_config(struct Config *config){
    if(config->max_memory < 1){
        return FALSE;
    }
    if(config->max_running_time < 1){
        return FALSE;
    }
    if(config->max_output_size < 1){
        return FALSE;
    }
    if(config->max_process_number < 1){
        return FALSE;
    }
    if(config->input_file_path == NULL
       || config->output_file_path == NULL
       || config->error_file_path == NULL){
        return FALSE;
    }
    if(config->seccomp_rule_name == NULL){
        return FALSE;
    }
    if(config->uid == 0 || config->gid == 0){
        return FALSE;
    }
    return TRUE;
}

//运行容器
void run(struct Config *config, struct Result *result){
    pid_t pid;
    int wstatus;
    int signal = 0;
    int tid = 0;
    struct rusage rusage;
    struct KillerArgs args;
	struct timeval start, end;
    
    if(verify_config(config)){
        //CONTAINER_ERROR(CONFIG_ERROR)
    }
    init_result(result);
	gettimeofday(&start, NULL);
    pid = fork(); //fork一个子进程用于运行用户代码
    if(pid < 0){ //fork失败
        CONTAINER_ERROR(FORK_ERROR)
    }
    else if(pid == 0){
        exe_process(config); //运行用户代码
    }
    else{
        args.pid = pid;
        args.max_running_time = config->max_running_time;
        if(pthread_create((pthread_t *)(&tid), NULL, kill_process, (void *)(&args)) != 0){ //确保子进程被终止
            kill(pid, SIGKILL);
            CONTAINER_ERROR(PTHREAD_ERROR)
        }
        if(waitpid(pid, &wstatus, WSTOPPED) == -1){
            kill(pid, SIGKILL);
            CONTAINER_ERROR(WAITPID_ERROR)
        }
		gettimeofday(&end, NULL);
        getrusage(RUSAGE_CHILDREN, &rusage); //获取子进程的资源使用情况
        result->real_running_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        result->real_memory = rusage.ru_maxrss * KBYTES;
        if(WIFEXITED(wstatus)){ //判断子进程是否被正常终止
            result->exit_code = WEXITSTATUS(wstatus); //获取子进程的退出状态
        }
        if(WIFSIGNALED(wstatus)){ //判断子进程是否被信号终止
            signal = WTERMSIG(wstatus); //获取终止子进程的信号
        }
        if(signal == SIGUSR1){ //由系统错误导致子进程被终止
            result->result = SYSTEM_ERROR;
        }
        else if(result->real_memory > config->max_memory){ //子进程运行内存超限
            result->result = MEMORY_LIMIT_EXCEEDED;
        }
        else if(result->real_running_time > config->max_running_time){ //子进程运行时间超限
            result->result = TIME_LIMIT_EXCEEDED;
        }
        else if((signal > 0 && signal < 65) || result->exit_code != 0){ //子进程运行异常
            result->result = RUNTIME_ERROR;
        }
    }
}