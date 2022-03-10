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
//#include "public_define.h"

//+++ start +++ add by blueBling
#define     GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX    "_im_group_msg"
#define     GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX     "_im_user_group"


typedef struct DBUserInfo_t
{
    uint32_t nId;//用户ID
    uint8_t nSex;// 用户性别 1.男;2.女
    uint8_t nStatus; // 用户状态0 正常， 1 离职
    uint32_t nDeptId;// 所属部门
    string strNick;// 花名
    string strDomain;// 花名拼音
    string strName;// 真名
    string strTel;// 手机号码
    string strEmail;// Email
    string strAvatar;// 头像
    string sign_info;//个性签名
    DBUserInfo_t& operator=(const DBUserInfo_t& rhs)
    {
        if(this != &rhs)
        {
            nId = rhs.nId;
            nSex = rhs.nSex;
            nStatus = rhs.nStatus;
            nDeptId = rhs.nDeptId;
            strNick = rhs.strNick;
            strDomain = rhs.strDomain;
            strName = rhs.strName;
            strTel = rhs.strTel;
            strEmail = rhs.strEmail;
            strAvatar = rhs.strAvatar;
            sign_info = rhs.sign_info;
        }
        return *this;
    }
    
} DBUserInfo_t;

//typedef hash_map<uint32_t, DBUserInfo_t*> DBUserMap_t;
//+++ end +++ add by blueBling


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
