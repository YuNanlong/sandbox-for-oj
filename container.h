#ifndef CONTAINER_H
#define CONTAINER_H

#include "constants.h"
#include <stdio.h>
#include <sys/types.h>

#define CONTAINER_ERROR(error_code)\
{\
printf("ERROR\nerrno: %s, error_code: "#error_code"\n", strerror(errno));\
raise(SIGUSR1);\
exit(1);\
}\

//用户代码运行环境设置
struct Config{
    long int max_running_time; //用户代码运行时间限制
    long int max_memory; //用户代码运行内存限制
    int max_process_number; //用户代码运行进程数限制
    long int max_output_size; //用户代码输出大小限制
    char *exe_file_path; //用户代码生成的可执行文件路径
    char *input_file_path; //用户代码标准输入文件路径
    char *output_file_path; //用户代码输出文件路径
    char *error_file_path; //用户代码错误文件路径
    char *arg[ARG_SIZE]; //用户代码运行参数
    char *env[ENV_SIZE]; //用户代码环境变量设置
    char *seccomp_rule_name;
    uid_t uid;
    gid_t gid;
};

//用户代码运行结果
struct Result{
    long int real_running_time; //运行时间
    long int real_memory; //运行内存
    int exit_code; //退出状态码
    int result; //用户代码的运行结果
};

void init_result(struct Result *result);
int verify_config(struct Config *config);
void run(struct Config *config, struct Result *result);

#endif /* CONTAINER_H */
