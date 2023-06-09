
/*****************************************************************************
 *
 * File Name: src/farm/query.h
 *
 * Copyright (C) 2014 HUINS,Inc.
 * Programmed by Kim suhak
 * email : shkim@huins.com
 * file creation : 2014/12/29
 *
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define QUERY_CREATE_TABLE_SENSOR_VALUE "CREATE TABLE IF NOT EXISTS tfarmsensorvalue (id INT AUTO_INCREMENT PRIMARY KEY, date DATE, time TIME, temperature FLOAT, humid FLOAT, light INT)"
#define QUERY_CREATE_TABLE_SENSOR_CHECK "CREATE TABLE IF NOT EXISTS tfarmsensorcheck (id INT AUTO_INCREMENT PRIMARY KEY, date DATE, time TIME, temperature INT, humid INT, light INT)"
#define QUERY_CREATE_TABLE_ACTURATOR_VALUE "CREATE TABLE IF NOT EXISTS tfarmactuoperate (id INT AUTO_INCREMENT PRIMARY KEY, date DATE, time TIME, pump INT, fan INT, dcmotor INT, rgbled INT)"
#define QUERY_CREATE_TABLE_ACTURATOR_CHECK "CREATE TABLE IF NOT EXISTS tfarmactucheck (id INT AUTO_INCREMENT PRIMARY KEY, date DATE, time TIME, pump INT, fan INT, rgbled INT)"
#define QUERY_CREATE_TABLE_SETTING "CREATE TABLE IF NOT EXISTS tfarmsetting (id INT AUTO_INCREMENT PRIMARY KEY, hour INT, min INT, period INT)"
#define QUERY_SELECT_SENSOR_DATA "SELECT date, time, temperature, humid FROM tfarmsensorvalue WHERE time >= DATE_ADD(NOW(), INTERVAL-1 HOUR)"
#define QUERY_INSERT_SENSOR_DATA "INSERT INTO  tfarmsensorvalue values (null ,now(), now(), %f, %f, %d)"
#define QUERY_INSERT_SENSOR_CHECK "INSERT into tfarmsensorcheck values (null, now(), now(), %f, %f, %d)"
#define QUERY_INSERT_ACTUATOR_VALUE "INSERT into tfarmactuoperate values (null, now(), now(), %d, %d, %d, %d)"
#define QUERY_INSERT_ACTUATOR_CHECK "INSERT into tfarmactucheck values (null, now(), now(), %d, %d, %d)"
#define QUERY_INSERT_SETTING "INSERT into tfarmsetting values (null, %d, %d, %d)"
#define QUERY_UPDATE_SETTING "UPDATE tfarmsetting SET hour=%d,min=%d,period=%d WHERE id = 1"
#define QUERY_SELECT_SETTING "SELECT hour,min, period from tfarmsetting WHERE id =1"

#define QUERY_SELECT_COUNT_SETTING "SELECT COUNT(*) FROM tfarmsetting"

#ifdef __cplusplus
}
#endif

