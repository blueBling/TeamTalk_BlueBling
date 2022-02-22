//
//  test_dbpool.cpp
//  test_dbpool
//
//  Created by blueBling on 22-2-21.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "DBPool.h"

#include<iostream>


using std::cout;
using std::endl;


int test_dbpool(){

	CDBManager* pDBManager = CDBManager::getInstance();
	if (!pDBManager) {
		cout << "DBManager init failed" << endl;
		return -1;
	}

	CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");

    if(pDBConn) {
        string strSql = "select * from IMAdmin";
        CResultSet* pResult = pDBConn->ExecuteQuery(strSql.c_str());
        if(pResult) {
            while (pResult->Next()) {
                uint32_t nId = pResult->GetInt("id");
                string name = pResult->GetString("uname");

				cout << "id:" << nId << "  name:" << name << endl;
				
            }
            delete pResult;
        }

		pDBManager->RelDBConn(pDBConn); //回收连接
    }
    else
    {
        cout << "no db connection for teamtalk_slave" << endl;
    }

	//CDBPool析构会释放mysql连接池，但是连接池是通过指针存储的，动态内存需要手动释放，teamtalk未提供释放所有连接池的接口，因为所有连接池所有连接在服务器正常运行时都得重复使用
	//释放方法，遍历m_dbpool_map所有连接池，逐一delete
//	map<string, CDBPool*>::iterator it = pDBManager->m_dbpool_map.find("teamtalk_master");
//	if (it != pDBManager->m_dbpool_map.end()) {
//		delete it->second;
//	}
//
//	it = pDBManager->m_dbpool_map.find("teamtalk_slave");
//	if (it != pDBManager->m_dbpool_map.end()) {
//		delete it->second;
//	}

	delete pDBManager;
	pDBManager = NULL;
//	CDBManager::s_db_manager = NULL;

	mysql_server_end();
	
	return 0;
}

int test_stmt() {

	CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");

	if (pDBConn)
    {
    	string strSql = "insert into IMUser(`salt`,`sex`,`nick`,`password`,`domain`,`name`,`phone`,`email`,`avatar`,`sign_info`,`departId`,`status`,`created`,`updated`) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

		CPrepareStatement* stmt = new CPrepareStatement();
        if (stmt->Init(pDBConn->GetMysql(), strSql))
        {
            srand((unsigned)time(NULL));
	        uint32_t nSalt = 1001;
            uint32_t nNow = (uint32_t) time(NULL);
            uint32_t index = 0;
			string strOutPass = "";
			string strSalt = "";
            uint32_t nGender = 1;
            uint32_t nStatus = 0;

            string strUserPass = "123456";
            char strUserPassMd5[33] = "49ba59abbe56e057";
        	//CMd5::MD5_Calculate(strUserPass.c_str(), strUserPass.length(), strUserPassMd5); // 计算MD5后的用户密码
            
			string strInPass = strUserPassMd5 + int2string(nSalt);  // 计算MD5后的用户密码 + 混淆码
        	char szMd5[33] = "e0508c0a42b001d7";
        	//CMd5::MD5_Calculate(strInPass.c_str(), strInPass.length(), szMd5);
        	strOutPass = szMd5;
			strSalt = int2string(nSalt);


		    string strNick = "blueBling";	// 花名
		    string strDomain = "bb";		// 花名拼音
		    string strName = "coderlei";	// 真名
		    string strTel = "110";			// 手机号码
		    string strEmail = "123@qq.com"; // Email
		    string strAvatar = "";			// 头像
		    string sign_info = "day day up";//个性签名
		    int nDepartId = 1;
			

            stmt->SetParam(index++, strSalt);
            stmt->SetParam(index++, nGender);
            stmt->SetParam(index++, strNick);
			stmt->SetParam(index++, strOutPass);
            stmt->SetParam(index++, strDomain);
            stmt->SetParam(index++, strName);
            stmt->SetParam(index++, strTel);
            stmt->SetParam(index++, strEmail);
            stmt->SetParam(index++, strAvatar);
            stmt->SetParam(index++, sign_info);
            stmt->SetParam(index++, nDepartId);
            stmt->SetParam(index++, nStatus);
            stmt->SetParam(index++, nNow);
            stmt->SetParam(index++, nNow);
            bool bRet = stmt->ExecuteUpdate();
            
            if (!bRet)
            {
                cout << "insert user failed:" << strSql.c_str() << endl;
                sleep(1);
            }
			else
			{
				uint32_t nId =  stmt->GetInsertId();
				cout << "insert id is " << nId << endl;
			}
        }else {
            cout << "smt Init failed" << endl;
        }
        delete stmt;
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        cout << "no db connection!" << endl;
    }

	//CDBPool析构会释放mysql连接池，但是连接池是通过指针存储的，动态内存需要手动释放，teamtalk未提供释放所有连接池的接口，因为所有连接池所有连接在服务器正常运行时都得重复使用
	//释放方法，遍历m_dbpool_map所有连接池，逐一delete
//	map<string, CDBPool*>::iterator it = pDBManager->m_dbpool_map.find("teamtalk_master");
//	if (it != pDBManager->m_dbpool_map.end()) {
//		delete it->second;
//	}
//
//	it = pDBManager->m_dbpool_map.find("teamtalk_slave");
//	if (it != pDBManager->m_dbpool_map.end()) {
//		delete it->second;
//	}

	delete pDBManager;
	pDBManager = NULL;
//	CDBManager::s_db_manager = NULL;

	mysql_server_end();

	return 0;
}

int main(){
	test_dbpool();
	test_stmt();
	return 0;
}