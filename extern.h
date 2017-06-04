#ifndef _EXTERN_H_
#define _EXTERN_H_

#define KEY 2987
#define AVG_LEN 3
const char *name = "/tmp/s207209_lab5";

typedef struct {
		pid_t pid;
		uid_t uid;
		gid_t gid;
		time_t diff;
		double avg[AVG_LEN];
} info_t;

typedef struct {
		long type;
		info_t info;
} msg_t;

#endif
