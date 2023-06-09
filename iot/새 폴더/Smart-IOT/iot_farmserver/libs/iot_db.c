
/*****************************************************************************
 *
 * File Name: iot_db.c
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/11
 *
 *****************************************************************************/
// #include <mysql.h>
#include <mariadb/mysql.h>
#include <string.h>
#include <stdio.h>
#include "iot_db.h"


#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "root"
//#define DB_NAME "iot"
#define CHOP(x) x[strlen(x) - 1] = ' '

#define IOTDB_DEBUG 1

MYSQL       *connection=NULL, conn;
MYSQL_RES   *sql_result;
MYSQL_ROW   sql_row;
int       query_stat; 


int iot_connect_to_db(char * db_name)
{
    mysql_init(&conn);
    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, db_name, 3306, (char *)NULL, 0);

    if(connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 0;
    }
    else
        return 1;
}

int iot_send_query(char * _query)
{
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 0;
    }
    else
        return 1;

}

int iot_update_data_from_db(char * _query)
{
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 0;
    }
    else
        return 1;
}

int iot_delete_data_from_db(char * _query)
{
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 0;
    }
    else
        return 1;
}

char * iot_get_setting_data_from_db(char  * _query, char * _result)
{
	//char _result[4096] = {0};

#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return NULL;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
		sprintf(_result, "%s,%s,%s", sql_row[0], sql_row[1], sql_row[2]);
		printf("%s : result - %s\n", __func__, _result);
    }

    return _result;
}

char * iot_get_emergency_data_from_db(char  * _query, char * _result)
{
	//char _result[4096] = {0};

#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return NULL;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
		sprintf(_result, "%s", sql_row[0]);
		printf("%s : result - %s\n", __func__, _result);
    }

    return _result;
}

char * iot_get_pet_time_data_from_db(char  * _query, char * _result)
{
	//char _result[4096] = {0};

#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return NULL;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
		sprintf(_result, "%s ,%s", sql_row[0],  sql_row[1]);
		printf("%s : result - %s\n", __func__, _result);
    }

    return _result;
}


char * iot_get_history_data_from_db(char  * _query, char * _result)
{
	//char _result[4096] = {0};

#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return NULL;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        //sprintf(_result + strlen(_result), "%s %s %s℃ %s%,", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
        sprintf(_result + strlen(_result), "%s %s %sdegrees %s%,", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
    }

    return _result;
}

int iot_count_setting_data_from_db(char * _query)
{
	int _ret = 0;
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return -1;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        printf("%s data: %s\n", __func__, sql_row[0]);
		_ret = atoi(sql_row[0]);
        //if(strcmp(sql_row[0], "1"))
				//return 1;
    }

    return _ret;

}

int iot_select_data_from_db(char * _query)
{
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 0;
    }

    sql_result = mysql_store_result(connection);

    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        printf("%s %s %s", sql_row[0], sql_row[1], sql_row[2]);
    }

    return 1;
}


int iot_insert_data_from_db(char * _query)
{
#ifdef IOTDB_DEBUG
	printf("%s : query - %s\n", __func__, _query);
#endif
    query_stat = mysql_query(connection, _query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 0;
    }
    else
        return 1;
}

int iot_disconnect_from_db(void)
{
   mysql_close(connection);

   return 1;
}
