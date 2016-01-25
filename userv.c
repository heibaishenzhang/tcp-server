#include <stdio.h>
#include <signal.h>
#include <mysql/mysql.h>

#include "error.h"
#include "select_tcp.h"
#include "myconfig.h"
#include "db.h"
#include "scan_log_switch.h"
#include "trace.h"

static void signal_trap()
{
	signal(SIGTERM, SIG_DFL);  
	signal(SIGHUP, SIG_DFL); 
	signal(SIGINT, SIG_DFL); 
	signal(SIGQUIT, SIG_DFL); 
}

static void get_svr_params(char* svr_ip, char *svr_port)
{
	get_profile_string("./config.ini", "SERVER", "svr_ip", svr_ip);
	get_profile_string("./config.ini", "SERVER", "svr_port", svr_port);       
}

static void get_db_svr_params(char* ip, char* port, char* user, char* pw, char* db_name)
{
	get_profile_string("./config.ini", "DB", "db_ip", ip);
	get_profile_string("./config.ini", "DB", "db_port", port);
	get_profile_string("./config.ini", "DB", "db_user", user);
	get_profile_string("./config.ini", "DB", "db_pw", pw);
	get_profile_string("./config.ini", "DB", "db_name", db_name);
}


int main(int argc, char* argv[])
{
    MYSQL conn_ptr;
    int ret = 0;

    int port = 0;
    char svr_port[20] = {'\0'};
    char svr_ip[20] = {'\0'};
    const int backlog = 5;

    
    char db_ip[20] = {'\0'};
    char db_port[20] = {'\0'};
    char db_user[20] = {'\0'};
    char db_pw[20] = {'\0'};
    char db_name[20] = {'\0'};
    
    scan_log_switch_init();


    signal_trap();

    get_svr_params(svr_ip, svr_port);
    get_db_svr_params(db_ip, db_port, db_user, db_pw, db_name);
   
    /*Connect to database*/
    ret = conn_db(&conn_ptr, db_ip, db_user, db_pw, db_name);
    if(0 != ret) {
         return ret;   
    }

    /*Get data from client*/    
    port = atoi(svr_port);

    TRACELOG("!!!!!!!!!!!!!!!!Upgrade Server!!!!!!!!!!!!!!!!!!!!!!");
    TRACELOG("!!!!!!!!!!!!!!!!  Start       !!!!!!!!!!!!!!!!!!!!!!");
    TRACELOG("!!!!!!!!!!!!!!!!WonderSoft!!!!!!!!!!!!!!!!!!!!!!");
    TRACELOG("!!!!!!!!!!!!!!!!copyright@2016!!!!!!!!!!!!!!!!!!!!!!");
    TRACELOG("Upgrade Server ip=%s, port=%d\n", svr_ip, port);

    parse_srv_init(&conn_ptr);

    setcp(svr_ip, port, backlog);

    close_db(&conn_ptr);

    parse_srv_close();
    
    scan_log_switch_close();

    return 0;
}
