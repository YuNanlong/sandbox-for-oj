#include "constants.h"
#include "executor.h"
#include "scmp_rules.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void exe_process(struct Config *config){
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    FILE *error_file = NULL;
    struct rlimit max_memory;
    struct rlimit max_running_time;
    struct rlimit max_output_size;
    struct rlimit max_process_number;
    
    max_memory.rlim_cur = max_memory.rlim_max = (rlim_t)(config->max_memory);
    if(setrlimit(RLIMIT_AS, &max_memory) != 0){ //设置运行内存限制
        EXECUTOR_ERROR(SETRLIMIT_ERROR)
    }
    max_running_time.rlim_cur = max_running_time.rlim_max = (rlim_t)(int)(config->max_running_time / 1000 + 1);
    if(setrlimit(RLIMIT_CPU, &max_running_time) != 0){ //设置运行时间限制
        EXECUTOR_ERROR(SETRLIMIT_ERROR)
    }
    max_output_size.rlim_cur = max_output_size.rlim_max = (rlim_t)(config->max_output_size);
    if(setrlimit(RLIMIT_FSIZE, &max_output_size) != 0){ //设置输出文件大小限制
        EXECUTOR_ERROR(SETRLIMIT_ERROR)
    }
    max_process_number.rlim_cur = max_process_number.rlim_max = (rlim_t)(config->max_process_number);
    if(setrlimit(RLIMIT_NPROC, &max_process_number) != 0){ //设置进程数限制
        EXECUTOR_ERROR(SETRLIMIT_ERROR)
    }
    input_file = fopen(config->input_file_path, "r");
    if(input_file == NULL){
        EXECUTOR_ERROR(FOPEN_ERROR)
    }
    if(dup2(fileno(input_file), fileno(stdin)) == -1){ //将标准输入重定向到输入文件
        EXECUTOR_ERROR(DUP2_ERROR)
    }
    output_file = fopen(config->output_file_path, "w");
    if(output_file == NULL){
        EXECUTOR_ERROR(FOPEN_ERROR)
    }
    if(dup2(fileno(output_file), fileno(stdout)) == -1){ //将标准输出重定向到输出文件
        EXECUTOR_ERROR(DUP2_ERROR)
    }
    error_file = fopen(config->error_file_path, "w");
    if(error_file == NULL){
        EXECUTOR_ERROR(FOPEN_ERROR)
    }
    if(dup2(fileno(error_file), fileno(stderr)) == -1){ //将标准错误重定向到错误文件
        EXECUTOR_ERROR(DUP2_ERROR)
    }
    //加载seccomp filter策略
    if(strcmp("c_cpp", config->seccomp_rule_name) == 0){
        c_cpp_rule(config);
    }
    else if(strcmp("general", config->seccomp_rule_name) == 0){
        general_rule(config);
    }
    else{
        EXECUTOR_ERROR(SCMP_RULE_ERROR)
    }
    execve(config->exe_file_path, config->arg, config->env); //执行用户代码
	EXECUTOR_ERROR(EXECVE_ERROR)
}