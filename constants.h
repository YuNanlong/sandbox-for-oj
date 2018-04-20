#ifndef CONSTANTS_H
#define CONSTANTS_H

#define KBYTES 1024
#define ARG_SIZE 256
#define ENV_SIZE 256
#define TRUE 0
#define FALSE 1

//用户代码运行结果标识
enum{
    SUCCESS = 0,
    WRONG_ANSWER = 1,
    TIME_LIMIT_EXCEEDED = 2,
    MEMORY_LIMIT_EXCEEDED = 3,
    RUNTIME_ERROR = 4,
    SYSTEM_ERROR = 5,
};

//error_code
enum{
    SETRLIMIT_ERROR = 1,
    FOPEN_ERROR = 2,
    DUP2_ERROR = 3,
    SCMP_RULE_ERROR = 4,
    EXECVE_ERROR = 5,
    FORK_ERROR = 6,
    PTHREAD_ERROR = 7,
    WAITPID_ERROR = 8,
    CONFIG_ERROR = 9,
};

#endif /* CONSTANTS */
