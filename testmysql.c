#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
 
 
void connection(MYSQL *conn, const char* host, const char* user, const char* password, const char* database) {
    mysql_init(conn); // 注意取地址符&
 
    if (mysql_real_connect(conn, host, user, password, database, 0, NULL, 0)) {
        printf("Connection success!\n");
    } else {
        fprintf(stderr, "Connection failed!\n");
        if (mysql_errno(conn)) {
            fprintf(stderr, "Connection error %d: %s\n", mysql_errno(conn), mysql_error(conn));
        }
        exit(EXIT_FAILURE);
    }
}
 
void insert(MYSQL *conn) {
	int res = mysql_query(conn, "INSERT INTO T_term_update_info(terminalID, userCert, userSID, updateTime, terminalCltVer, terminalType, updateStatus, terminalTypeName, terminalOS, terminalHD, vpnGateID, res1, res2) VALUES ('123', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1')");

    if (!res) {
        printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn));
    } else {
        fprintf(stderr, "Insert error %d: %s\n", mysql_errno(conn), mysql_error(conn));
    }
}
 
#if 0
void update() {
    int res = mysql_query(&conn, "UPDATE student SET student_name='Anna' WHERE student_no='123465'");
    if (!res) {
        printf("Update %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
    } else {
        fprintf(stderr, "Update error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
    }
}
 
void delete() {
    int res = mysql_query(&conn, "DELETE from student WHERE student_no='123465'");
    if (!res) {
        printf("Delete %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
    } else {
        fprintf(stderr, "Delete error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
    }
}
#endif
 
int main (int argc, char *argv[]) {
 
	MYSQL conn;
    connection(&conn, "192.168.54.251", "root", "123456", "vpnwebdb");
    insert(&conn);
    mysql_close(&conn);
    exit(EXIT_SUCCESS);
}
