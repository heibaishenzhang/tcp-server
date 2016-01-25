#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "buff_list.h"
#include "db.h"
#include "parse_xml.h"
#include "trace.h"


static pthread_t server_pid;
static volatile int32_t server_exit = 0;

static void *parse_thd(void *param)
{
    /*Read buffer from list and parse them by xml*/
	while(1) {
		char buff[2048] = {'\0'};
		char sql[4096] = {'\0'};
		pop_buff_list(buff);
		if(strlen(buff) > 0) {
			parse_xml(buff, sql); 
			insert_db((MYSQL*)param, sql); 
		}
		usleep(100);
	}
    
    return NULL;
}

int parse_srv_init(MYSQL* conn)
{
    int ret = 0;

    pthread_mutexattr_t mutex_attr;

    /* init server mutex */
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_destroy(&mutex_attr);

    ret = pthread_create(&server_pid, NULL, parse_thd, conn);
    if (ret != 0) {
        TRACELOG("failed to create the MRU server thread.\n");
        return -1;
    }

    return 0;
}

int parse_srv_close()
{
    server_exit = 1;

    pthread_join(server_pid, NULL);

    TRACELOG("parse server close.\n");
    return 0;
}
