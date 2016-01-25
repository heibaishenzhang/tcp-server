#include<stdio.h>
#include<time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>

#include "trace.h"

#define TRACELOG(...)  track(__FUNCTION__,__LINE__,__VA_ARGS__)

pthread_mutex_t g_TrackMutex;
int     isMutexInit = 0;
int     isLogged = 0;

void track(const char* fun_name, int fun_line, const char *format,... )
{

	va_list args;
	struct tm *pstTm;
	FILE      *pFILE = NULL;; 
	long    lClock;
	char    cMsg[256] = {'\0'};
	char    cString[128] = {'\0'};

	if(0 == isLogged)
		return;

	if(0 == isMutexInit) {
		isMutexInit = 1;
		pthread_mutex_init(&g_TrackMutex,NULL);		
	}

	pthread_mutex_lock(&g_TrackMutex);

	lClock = time((long *)0);
	pstTm = localtime( &lClock );
	sprintf( cMsg, "%04d-%02d-%02d %02d:%02d:%02d %s(%d): ", 
			pstTm->tm_year+1900, pstTm->tm_mon+1, pstTm->tm_mday,
			pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec,
			fun_name, fun_line);



	const char* FILENAME_PATH_DEFAULT =  "/tmp/userv.log";
	sprintf( cString,FILENAME_PATH_DEFAULT);

	pFILE = fopen( cString, "a" );
	if( pFILE == NULL ) {
		pthread_mutex_unlock (&g_TrackMutex);
		return;
	}
	fputs( cMsg, pFILE );

	va_start(args,format);
	vfprintf(pFILE ,format, args ) ;
	va_end(args);

	fputs( "\n", pFILE );

	fclose( pFILE );
	pthread_mutex_unlock (&g_TrackMutex);
}



#if 0
static pthread_t server_pid;
static volatile int32_t server_exit = 0;

static pthread_t server_pid1;
static volatile int32_t server_exit1 = 0;

static void *hi_thd(void *param)
{
    /*Read buffer from list and parse them by xml*/
	int i = 0;
        while(1) {
              
              const char *hi = "I'm the first say hi";
		TRACELOG("No=%d, Content=%s", i++, hi);
               
                usleep(100);
        }

    return NULL;
}

int hi_srv_init()
{
    int ret = 0;

    pthread_mutexattr_t mutex_attr;

    /* init server mutex */
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_destroy(&mutex_attr);

    ret = pthread_create(&server_pid, NULL, hi_thd, NULL);
    if (ret != 0) {
        printf("failed to create the MRU server thread.\n");
        return -1;
    }

    return 0;
}

static void *hello_thd(void *param)
{
    /*Read buffer from list and parse them by xml*/
        int i = 0;
        while(1) {
              const char *hi = "I'm the first say hello";
		TRACELOG("No=%d, Content=%s", i++, hi);
                usleep(1000);
        }

    return NULL;
}


int hello_srv_init()
{
    int ret = 0;

    pthread_mutexattr_t mutex_attr;

    /* init server mutex */
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_destroy(&mutex_attr);

    ret = pthread_create(&server_pid1, NULL, hello_thd, NULL);
    if (ret != 0) {
        printf("failed to create the MRU server thread.\n");
        return -1;
    }

    return 0;
}




int main(int argc, char* argv)
{
	const char* hi = "I'm here to say hi";
        int i = 0;


hello_srv_init();
int hi_srv_init();

	while(1) {
  		i++;
		TRACELOG("Main log is, num is %d, content is %s", i, hi);
		sleep(1);
	}
return 0;
}
#endif
