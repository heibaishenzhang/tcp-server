#ifndef PARSE_THREAD_H
#define PARSE_THREAD_H
#include <mysql/mysql.h>
int parse_srv_init(MYSQL* conn_ptr);
int parse_srv_close(); 
#endif//PARSE_THREAD_H
