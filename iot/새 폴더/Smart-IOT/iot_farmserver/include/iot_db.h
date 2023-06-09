
/*****************************************************************************
 *
 * File Name: iot_db.h
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/11
 *
 *****************************************************************************/

#ifndef _iot_db_H_
#define _iot_db_H_

extern int iot_connect_to_db(char * db_name);
extern int iot_send_query(char * _query);
extern char * iot_get_history_data_from_db(char  * _query, char * _result);
extern int iot_insert_data_from_db(char * _query);
extern int iot_delete_data_from_db(char * _query);
extern int iot_update_data_from_db(char * _query);
extern int iot_select_data_from_db(char * _query);
extern int iot_disconnect_from_db(void);
extern int iot_count_setting_data_from_db(char * _query);

#endif

