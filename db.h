#ifndef DB_H
#define DB_H
#include <mysql/mysql.h>
int conn_db(MYSQL *conn_ptr, const char* db_ip, const char* db_user, const char* db_pw, const char* db_name);
void close_db(MYSQL *conn_ptr);
int insert_db(MYSQL* conn_ptr, const char* sql);

#endif//DB_H
