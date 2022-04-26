/*================================================================
*     Copyright (c) 2015年 lanhu. All rights reserved.
*   
*   文件名称：UserModel.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2015年01月05日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __USERMODEL_H__
#define __USERMODEL_H__

#include "IM.BaseDefine.pb.h"
#include "ImPduBase.h"
#include "public_define.h"

class CUserModel
{
public:
    static CUserModel* getInstance();
    ~CUserModel();
	//从mysql IMUser获取updated >= nLastTime用户的id
    void getChangedId(uint32_t& nLastTime, list<uint32_t>& lsIds);
	//从mysql IMUser获取id在lsIds的所有用户详情，并存到lsUsers中
    void getUsers(list<uint32_t> lsIds, list<IM::BaseDefine::UserInfo>& lsUsers);
	//从mysql IMUser获取指定nUserId的用户详情，并存到cUser中
    bool getUser(uint32_t nUserId, DBUserInfo_t& cUser);

	//更新mysql IMUser中cUser的用户信息
    bool updateUser(DBUserInfo_t& cUser);
	//向mysql IMUser插入cUser信息
    bool insertUser(DBUserInfo_t& cUser);
//    void getUserByNick(const list<string>& lsNicks, list<IM::BaseDefine::UserInfo>& lsUsers);
    void clearUserCounter(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::SessionType nSessionType);
	//将nUserId和nPeerId对话的平台类型（Windos Mac IOS或ANDROID ）插入到mysql IMCallLog中
	void setCallReport(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::ClientType nClientType);

	//更新mysql IMUser user_id的签名信息
    bool updateUserSignInfo(uint32_t user_id, const string& sign_info);
	//获取mysql IMUser user_id的签名信息
    bool getUserSingInfo(uint32_t user_id, string* sign_info);
	//更新mysql IMUser user_id的勿扰状态为shield_status（0关闭勿扰 1开启勿扰）
	bool updatePushShield(uint32_t user_id, uint32_t shield_status);
	//获取mysql IMUser user_id的勿扰状态
    bool getPushShield(uint32_t user_id, uint32_t* shield_status);

private:
    CUserModel();
private:
    static CUserModel* m_pInstance;
};

#endif /*defined(__USERMODEL_H__) */
