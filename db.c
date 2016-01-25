#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>

#include "myconfig.h"
#include "trace.h"

int conn_db(MYSQL *conn_ptr, const char* host, const char* user, const char* password, const char* database) 
{
	mysql_init(conn_ptr);

	if (mysql_real_connect(conn_ptr, host, user, password, database, 0, NULL, 0)) {
		TRACELOG("Database ip=%s, user=%s, database name=%s, Connection success!", host, user, database);
	} else {
		TRACELOG("Connection failed!");
		if (mysql_errno(conn_ptr)) {
			TRACELOG("Connection error %d: %s\n", mysql_errno(conn_ptr), mysql_error(conn_ptr));
		}
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void close_db(MYSQL *conn_ptr)
{
    if(!conn_ptr) {
        mysql_close(conn_ptr);
        conn_ptr = NULL;    
    }
}


int query_db(MYSQL *conn_ptr, const char* sql)
{
    MYSQL_RES *res_ptr;
  	MYSQL_ROW sqlrow;
	MYSQL_FIELD *fd;
	int res, i, j;

	if (conn_ptr) {
		res = mysql_query(conn_ptr, sql); //查询语句
		if (res) {       
			printf("SELECT error:%s\n",mysql_error(conn_ptr));   
		} else {      
			res_ptr = mysql_store_result(conn_ptr);				//取出结果集
			if(res_ptr) {             
				printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr)); 
				j = mysql_num_fields(res_ptr);        
				while((sqlrow = mysql_fetch_row(res_ptr)))  {	//依次取出记录
					for(i = 0; i < j; i++)       
						printf("%s\t", sqlrow[i]); 				//输出
					printf("\n");        
				}            
				if (mysql_errno(conn_ptr)) {                    
					fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));             
				}      
			}      
			mysql_free_result(res_ptr);      
		}
	} else {
		printf("Connection failed\n");
	}
}

static void get_db_svr_params(char* ip, char* port, char* user, char* pw, char* db_name)
{
	get_profile_string("./config.ini", "DB", "db_ip", ip);
	get_profile_string("./config.ini", "DB", "db_port", port);
	get_profile_string("./config.ini", "DB", "db_user", user);
	get_profile_string("./config.ini", "DB", "db_pw", pw);
	get_profile_string("./config.ini", "DB", "db_name", db_name);
}


int insert_db(MYSQL *conn_ptr, const char* sql)
{
	int res;
#if 0
	char ip[20] = {'\0'};
	char port[20] = {'\0'};
	char user[20] = {'\0'};
	char pw[20] = {'\0'};
	char db_name[20] = {'\0'};
        MYSQL conn_ptr;

	//get_db_svr_params( ip, port, user, pw, db_name);
	//conn_db(&conn_ptr, ip, user, pw, db_name); 
    
#endif
	TRACELOG("sql=%s", sql);
	res = mysql_query(conn_ptr, sql); 	
       
	if (!res) {		
		TRACELOG("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));	
	}  else {		
		TRACELOG("Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
	}
	
	//close_db(&conn_ptr);
	return EXIT_SUCCESS;
    
}

int delete_db(MYSQL *conn_ptr, const char* sql)
{
	int res;
    
	if (conn_ptr) {
		res = mysql_query(conn_ptr, sql);
		if (!res) {		//输出受影响的行数
			printf("delete %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));	
		}  else {		//打印出错误代码及详细信息
			fprintf(stderr, "delete error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
		}
		return 0;
	}
}

#if 0 
int main() {
	MYSQL conn_ptr;
	int ret = 0;
	char sql[4096] = "INSERT INTO T_term_update_info(terminalID, userCert, userSID, updateTime, terminalCltVer, terminalType, updateStatus, terminalTypeName, terminalOS, terminalHD, vpnGateID, res1, res2) VALUES ('11', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1')";

	ret = conn_db(&conn_ptr, "192.168.54.251", "root", "123456", "vpnwebdb" );
	if(0 == ret) {
		printf("sql=%s\n", sql);
		insert_db(&conn_ptr, sql);   
	}
	close_db(&conn_ptr);
	return 0;
}
#endif
