#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "clist.h"
#include "buff_list.h"
#include "error.h"

struct list_head buff_list = LIST_HEAD_INIT(buff_list);
pthread_mutex_t buff_list_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    struct list_head node;
    char buff[2048];
} buff_list_t;

void push_buff_list(const char* buff)
{
    buff_list_t *pList = (buff_list_t*)malloc(sizeof( buff_list_t));
    if(!pList) {
        //tracelog("Error, no more memory");
	return;
    } 
    memset(pList, 0x00, sizeof(buff_list_t));
    memcpy(pList->buff, buff, strlen(buff));
    pthread_mutex_lock(&buff_list_lock);
    list_add_tail(&pList->node, &buff_list);
    pthread_mutex_unlock(&buff_list_lock);
}

void pop_buff_list(char* buff)
{
    struct list_head *pos = NULL, *tmp = NULL;
    buff_list_t *pList = NULL;

    pthread_mutex_lock(&buff_list_lock);
    list_for_each_safe(pos, tmp, &buff_list) {
        pList = list_entry(pos, buff_list_t, node);
        if(strlen(pList->buff) > 0) {
            strncpy(buff, pList->buff, sizeof(pList->buff) - 1);
            printf("%s:buff=%s\n", __FUNCTION__, buff); 
            list_del(pos);
            free(pList);
            pList = NULL;
        }
    }
    pthread_mutex_unlock(&buff_list_lock);    
}
