#include "container.h"
#include "constants.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {
    struct Config config;
    struct Result result;

    config.max_running_time = 1000;
    config.max_memory = 16 * KBYTES * KBYTES;
    config.max_process_number = 2;
	config.max_output_size = 16 * KBYTES * KBYTES;
    config.exe_file_path = "/home/parallels/Desktop/a.out";
    config.input_file_path = "/dev/stdin";
    config.output_file_path = "/home/parallels/Desktop/wm";
    config.error_file_path = "/dev/stderr";
    config.arg[0] = NULL;
    config.env[0] = NULL;
    config.seccomp_rule_name = "c_cpp";
    config.uid = 1000;
    config.gid = 1000;
    run(&config, &result);
    printf("%ld %ld %d %d", result.real_running_time, result.real_memory, result.exit_code, result.result);
    return 0;
}