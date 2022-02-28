/*
 * DBPool.h
 *
 *  Created on: 2014年7月22日
 *      Author: ziteng
 *  Modify By ZhangYuanhao
 *  2015-01-12
 *  enable config the max connection of every instance
 */

#ifndef DBPOOL_H_
#define DBPOOL_H_

//#include "../base/util.h"
#include "util.h"
#include "ThreadPool.h"
#include <mysql.h>

//+++ start +++ add by blueBling
#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;
//+++ end +++ add by blueBling

#define MAX_ESCAPE_STRING_LEN	10240

class CResultSet {
public:
	CResultSet(MYSQL_RES* res);
	virtual ~CResultSet();

	bool Next(); //用于获取结果集下一行信息来更新m_row
	int GetInt(const char* key); //获取指定列名的值(数值型)
	char* GetString(const char* key); //获取指定列名的值(字符型)
private:
	int _GetIndex(const char* key);

	MYSQL_RES* 			m_res; //保存了sql执行的结果集
	MYSQL_ROW			m_row; //存储了结果集某行信息
	map<string, int>	m_key_map; //存储了结果集各个字段名和编号的映射关系
};

/*
 * TODO:
 * 用MySQL的prepare statement接口来防止SQL注入
 * 暂时只用于插入IMMessage表，因为只有那里有SQL注入的风险，
 * 以后可以把全部接口用这个来实现替换
 */
class CPrepareStatement {
public:
	CPrepareStatement();
	virtual ~CPrepareStatement();

	//用于创建mysql预编译语句并初始化m_param_bind
	bool Init(MYSQL* mysql, string& sql);

	//重载了多个版本便于设置不同类型的输入参数
	void SetParam(uint32_t index, int& value);
	void SetParam(uint32_t index, uint32_t& value);
    void SetParam(uint32_t index, string& value);
    void SetParam(uint32_t index, const string& value);

	bool ExecuteUpdate();      //绑定输入参数并执行预编译语句
	uint32_t GetInsertId(); //返回执行INSERT或UPDATE自动生成的id(AUTO_INCREMENT)
private:
	MYSQL_STMT*	m_stmt;       //执行预编译执行句柄
	MYSQL_BIND*	m_param_bind; //保存各个输入参数的属性
	uint32_t	m_param_cnt;  //记录输入参数的个数
};

class CDBPool;

class CDBConn {
public:
	CDBConn(CDBPool* pDBPool);
	virtual ~CDBConn();
	int Init();

	CResultSet* ExecuteQuery(const char* sql_query); //执行sql语句并返回执行结果集
	bool ExecuteUpdate(const char* sql_query); //执行sql语句根据行rows affected情况返回执行成功或失败
	char* EscapeString(const char* content, uint32_t content_len); //将带特殊字符的语句里特殊字符转义，存到m_escape_string

	uint32_t GetInsertId();

	const char* GetPoolName();
	MYSQL* GetMysql() { return m_mysql; }
private:
	CDBPool* 	m_pDBPool;	// to get MySQL server information
	MYSQL* 		m_mysql;
	//MYSQL_RES*	m_res;
	char		m_escape_string[MAX_ESCAPE_STRING_LEN + 1]; //存储带特殊字符转义后的语句
};

class CDBPool {
public:
	CDBPool(const char* pool_name, const char* db_server_ip, uint16_t db_server_port,
			const char* username, const char* password, const char* db_name, int max_conn_cnt);
	virtual ~CDBPool();

	int Init();
	CDBConn* GetDBConn();
	void RelDBConn(CDBConn* pConn);

	const char* GetPoolName() { return m_pool_name.c_str(); }
	const char* GetDBServerIP() { return m_db_server_ip.c_str(); }
	uint16_t GetDBServerPort() { return m_db_server_port; }
	const char* GetUsername() { return m_username.c_str(); }
	const char* GetPasswrod() { return m_password.c_str(); }
	const char* GetDBName() { return m_db_name.c_str(); }
private:
	string 		m_pool_name;
	string 		m_db_server_ip;
	uint16_t	m_db_server_port;
	string 		m_username;
	string 		m_password;
	string 		m_db_name;
	int			m_db_cur_conn_cnt;
	int 		m_db_max_conn_cnt;
	list<CDBConn*>	m_free_list;
	CThreadNotify	m_free_notify;
};

// manage db pool (master for write and slave for read)
class CDBManager {
public:
	virtual ~CDBManager();

	static CDBManager* getInstance();

	int Init();

	CDBConn* GetDBConn(const char* dbpool_name);
	void RelDBConn(CDBConn* pConn);
public:
	CDBManager();

public:
	static CDBManager*		s_db_manager;
	map<string, CDBPool*>	m_dbpool_map;
};

#endif /* DBPOOL_H_ */
