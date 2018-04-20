#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "container.h"
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define EXECUTOR_ERROR(error_code)\
{\
printf("ERROR\nerrno: %s, error_code: "#error_code"\n", strerror(errno));\
fclose(input_file);\
fclose(output_file);\
fclose(error_file);\
raise(SIGUSR1);\
exit(1);\
}\

void exe_process(struct Config *config);

#endif /* EXECUTOR_H */
