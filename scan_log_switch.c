#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include "trace.h"

static pthread_t server_pid;
static volatile int32_t server_exit = 0; 

static void *scan_thd(void *param)
{
	while(1) {
                char enable[2] = {"\0"};
		get_profile_string("/userv/config.ini", "LOG", "log_enable", enable);
                if(strncasecmp(enable,"1") == 0)
            		isLogged = 1;	
		sleep(1);
	}

	return NULL;
}

int scan_log_switch_init()
{
    int ret = 0;

    pthread_mutexattr_t mutex_attr;

    /* init server mutex */
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_destroy(&mutex_attr);

    ret = pthread_create(&server_pid, NULL, scan_thd, NULL);
    if (ret != 0) {
        printf("failed to create the MRU server thread.\n");
        return -1;
    }

    return 0;
}

int scan_log_switch_close()
{
    server_exit = 1;

    pthread_join(server_pid, NULL);

    printf("parse server close.\n");
    return 0;
}
