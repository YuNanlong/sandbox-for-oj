#include "scmp_rules.h"
#include "constants.h"
#include <seccomp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//加载C和C++的seccomp filter策略
void c_cpp_rule(struct Config *config){
    int i;
    //系统调用白名单
    int syscall_whitelist[] = {SCMP_SYS(read), SCMP_SYS(fstat),
                                SCMP_SYS(mmap), SCMP_SYS(mprotect),
                                SCMP_SYS(munmap), SCMP_SYS(uname),
                                SCMP_SYS(arch_prctl), SCMP_SYS(brk),
                                SCMP_SYS(access), SCMP_SYS(exit_group),
                                SCMP_SYS(close), SCMP_SYS(readlink),
                                SCMP_SYS(sysinfo), SCMP_SYS(write),
                                SCMP_SYS(writev), SCMP_SYS(lseek)};
    int length = (int)sizeof(syscall_whitelist);
    scmp_filter_ctx ctx = NULL;
    
    if((ctx = seccomp_init(SCMP_ACT_KILL)) != NULL){ //初始化seccomp filter
        SCMP_RULE_ERROR(SCMP_INIT_ERROR)
    }
    //加载系统调用白名单
    for(i = 0; i < length; i++){
        if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscall_whitelist[i], 0) != 0){
            SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
        }
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)(config->exe_file_path))) != 0){ //只能运行用户代码生成的可执行文件
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0){ //禁止"w"和"rw"
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_load(ctx) != 0){ //加载策略到内核
        SCMP_RULE_ERROR(SCMP_LOAD_ERROR)
    }
    seccomp_release(ctx);
}

//加载通用的seccomp filter策略
void general_rule(struct Config *config){
    int i;
    //系统调用黑名单
    int syscalls_blacklist[] = {SCMP_SYS(clone),
                                SCMP_SYS(fork), SCMP_SYS(vfork),
                                SCMP_SYS(kill), 
#ifdef __NR_execveat
                                SCMP_SYS(execveat)
#endif
                               };
    int length = (int)sizeof(syscalls_blacklist);
    scmp_filter_ctx ctx = NULL;

    if((ctx = seccomp_init(SCMP_ACT_ALLOW)) != NULL){
        SCMP_RULE_ERROR(SCMP_INIT_ERROR)
    }
    //加载系统调用黑名单
    for(i = 0; i < length; i++){
        if(seccomp_rule_add(ctx, SCMP_ACT_KILL, syscalls_blacklist[i], 0) != 0){
            SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
        }
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)(config->exe_file_path))) != 0){ //只能运行用户代码生成的可执行文件
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0){ //禁止"w"和"rw"
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0){ //禁止"w"和"rw"
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0) != 0){
        SCMP_RULE_ERROR(SCMP_RULE_ADD_ERROR)
    }
    if(seccomp_load(ctx) != 0){ //加载策略到内核
        SCMP_RULE_ERROR(SCMP_LOAD_ERROR)
    }
    seccomp_release(ctx);
}
