#ifndef SCMP_RULES_H
#define SCMP_RULES_H

#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define SCMP_RULE_ERROR(error_code)\
{\
printf("ERROR\nerrno: %s, error_code: "#error_code"\n", strerror(errno));\
raise(SIGUSR1);\
exit(1);\
}\

void c_cpp_rule(struct Config *config);
void general_rule(struct Config *config);

#endif /* SCMP_RULES_H */
