/*================================================================
 *   Copyright (C) 2014 All rights reserved.
 *
 *   文件名称：GroupModel.cpp
 *   创 建 者：Zhang Yuanhao
 *   邮    箱：bluefoxah@gmail.com
 *   创建日期：2014年12月15日
 *   描    述：
 *
 ================================================================*/

//#include "../DBPool.h"
//#include "../CachePool.h"
#include "DBPool.h"
#include "CachePool.h"

#include "GroupModel.h"
#include "ImPduBase.h"
#include "Common.h"
#include "AudioModel.h"
#include "UserModel.h"
#include "GroupMessageModel.h"
#include "public_define.h"
#include "SessionModel.h"

CGroupModel* CGroupModel::m_pInstance = NULL;

/**
 *  <#Description#>
 */
CGroupModel::CGroupModel()
{
    
}

CGroupModel::~CGroupModel()
{
    
}

CGroupModel* CGroupModel::getInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CGroupModel();
    }
    return m_pInstance;
}

/**
 *  创建群
 *
 *  @param nUserId        创建者
 *  @param strGroupName   群名
 *  @param strGroupAvatar 群头像
 *  @param nGroupType     群类型1,固定群;2,临时群
 *  @param setMember      群成员列表，为了踢出重复的userId，使用set存储
 *
 *  @return 成功返回群Id，失败返回0;
 */
uint32_t CGroupModel::createGroup(uint32_t nUserId, const string& strGroupName, const string& strGroupAvatar, uint32_t nGroupType, set<uint32_t>& setMember)
{
    uint32_t nGroupId = INVALID_VALUE;
    do {
        if(strGroupName.empty()) {
            break;
        }
        if (setMember.empty()) {
            break;
        }
        // remove repeat user
        
        
        //insert IMGroup
        if(!insertNewGroup(nUserId, strGroupName, strGroupAvatar, nGroupType, (uint32_t)setMember.size(), nGroupId)) {
            break;
        }
        bool bRet = CGroupMessageModel::getInstance()->resetMsgId(nGroupId);
        if(!bRet)
        {
            log("reset msgId failed. groupId=%u", nGroupId);
        }
        
        //insert IMGroupMember
        clearGroupMember(nGroupId);
        insertNewMember(nGroupId, setMember);
        
    } while (false);
    
    return nGroupId;
}

bool CGroupModel::removeGroup(uint32_t nUserId, uint32_t nGroupId, list<uint32_t>& lsCurUserId)
{
    bool bRet = false;
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    set<uint32_t> setGroupUsers;
    if(pDBConn)
    {
        string strSql = "select creator from IMGroup where id="+int2string(nGroupId);
        CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
        if(pResultSet)
        {
            uint32_t nCreator;
            while (pResultSet->Next()) {
                nCreator = pResultSet->GetInt("creator");
            }
            
            if(0 == nCreator || nCreator == nUserId)
            {
                //设置群组不可用。
                strSql = "update IMGroup set status=0 where id="+int2string(nGroupId);
                bRet = pDBConn->ExecuteUpdate(strSql.c_str());
            }
            delete  pResultSet;
        }
        
        if (bRet) {
            strSql = "select userId from IMGroupMember where groupId="+int2string(nGroupId);
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    uint32_t nId = pResultSet->GetInt("userId");
                    setGroupUsers.insert(nId);
                }
                delete pResultSet;
            }
        }
        pDBManager->RelDBConn(pDBConn);
    }
    
    if(bRet)
    {
        bRet = removeMember(nGroupId, setGroupUsers, lsCurUserId);
    }
    
    return bRet;
}


void CGroupModel::getUserGroup(uint32_t nUserId, list<IM::BaseDefine::GroupVersionInfo>& lsGroup, uint32_t nGroupType)
{
    list<uint32_t> lsGroupId;
    getUserGroupIds(nUserId, lsGroupId,0);
    if(lsGroupId.size() != 0)
    {
        getGroupVersion(lsGroupId, lsGroup, nGroupType);
    }
}


void CGroupModel::getGroupInfo(map<uint32_t,IM::BaseDefine::GroupVersionInfo>& mapGroupId, list<IM::BaseDefine::GroupInfo>& lsGroupInfo)
{ 
   if (!mapGroupId.empty())
    {
       CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            string strClause;
            bool bFirst = true;
            for(auto it=mapGroupId.begin(); it!=mapGroupId.end(); ++it)
            {
                if(bFirst)
                {
                    bFirst = false;
                    strClause = int2string(it->first);
                }
                else
                {
                    strClause += ("," + int2string(it->first));
                }
            }
            string strSql = "select * from IMGroup where id in (" + strClause  + ") order by updated desc";
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    uint32_t nGroupId = pResultSet->GetInt("id");
                    uint32_t nVersion = pResultSet->GetInt("version");
               if(mapGroupId[nGroupId].version() < nVersion)
                    {
                        IM::BaseDefine::GroupInfo cGroupInfo;
                        cGroupInfo.set_group_id(nGroupId);
                        cGroupInfo.set_version(nVersion);
                        cGroupInfo.set_group_name(pResultSet->GetString("name"));
                        cGroupInfo.set_group_avatar(pResultSet->GetString("avatar"));
                        IM::BaseDefine::GroupType nGroupType = IM::BaseDefine::GroupType(pResultSet->GetInt("type"));
                        if(IM::BaseDefine::GroupType_IsValid(nGroupType))
                        {
                            cGroupInfo.set_group_type(nGroupType);
                            cGroupInfo.set_group_creator_id(pResultSet->GetInt("creator"));
                            lsGroupInfo.push_back(cGroupInfo);
                        }
                        else
                        {
                            log("invalid groupType. groupId=%u, groupType=%u", nGroupId, nGroupType);
                        }
                    }
                }
                delete pResultSet;
            }
            else
            {
                log("no result set for sql:%s", strSql.c_str());
            }
            pDBManager->RelDBConn(pDBConn);
       if(!lsGroupInfo.empty())
            {
                fillGroupMember(lsGroupInfo);
            }
        }
        else
        {
            log("no db connection for teamtalk_slave");
        }
    }
    else
    {
        log("no ids in map");
    }
}

//根据nType进行添加删除操作，同时将相应的群组版本加1
bool CGroupModel::modifyGroupMember(uint32_t nUserId, uint32_t nGroupId, IM::BaseDefine::GroupModifyType nType, set<uint32_t>& setUserId, list<uint32_t>& lsCurUserId)
{
    bool bRet = false;
    if(hasModifyPermission(nUserId, nGroupId, nType))
    {
        switch (nType) {
            case IM::BaseDefine::GROUP_MODIFY_TYPE_ADD:
                bRet = addMember(nGroupId, setUserId, lsCurUserId);
                break;
            case IM::BaseDefine::GROUP_MODIFY_TYPE_DEL:
                bRet = removeMember(nGroupId, setUserId, lsCurUserId);
                removeSession(nGroupId, setUserId);
                break;
            default:
                log("unknown type:%u while modify group.%u->%u", nType, nUserId, nGroupId);
                break;
        }
        //if modify group member success, need to inc the group version and clear the user count;
        if(bRet)
        {
            incGroupVersion(nGroupId);
            for (auto it=setUserId.begin(); it!=setUserId.end(); ++it) {
                uint32_t nUserId=*it;
                CUserModel::getInstance()->clearUserCounter(nUserId, nGroupId, IM::BaseDefine::SESSION_TYPE_GROUP);
            }
        }
    }
    else
    {
        log("user:%u has no permission to modify group:%u", nUserId, nGroupId);
    }    return bRet;
}

//向mysql IMGroup插入群组信息
bool CGroupModel::insertNewGroup(uint32_t nUserId, const string& strGroupName, const string& strGroupAvatar, uint32_t nGroupType, uint32_t nMemberCnt, uint32_t& nGroupId)
{
    bool bRet = false;
    nGroupId = INVALID_VALUE;
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    if (pDBConn)
    {
        string strSql = "insert into IMGroup(`name`, `avatar`, `creator`, `type`,`userCnt`, `status`, `version`, `lastChated`, `updated`, `created`) "\
        "values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        
        CPrepareStatement* pStmt = new CPrepareStatement();
        if (pStmt->Init(pDBConn->GetMysql(), strSql))
        {
            uint32_t nCreated = (uint32_t)time(NULL);
            uint32_t index = 0;
            uint32_t nStatus = 0;
            uint32_t nVersion = 1;
            uint32_t nLastChat = 0;
            pStmt->SetParam(index++, strGroupName);
            pStmt->SetParam(index++, strGroupAvatar);
            pStmt->SetParam(index++, nUserId);
            pStmt->SetParam(index++, nGroupType);
            pStmt->SetParam(index++, nMemberCnt);
            pStmt->SetParam(index++, nStatus);
            pStmt->SetParam(index++, nVersion);
            pStmt->SetParam(index++, nLastChat);
            pStmt->SetParam(index++, nCreated);
            pStmt->SetParam(index++, nCreated);
            
            bRet = pStmt->ExecuteUpdate();
            if(bRet) {
                nGroupId = pStmt->GetInsertId();
            }
        }
        delete pStmt;
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_master");
    }
    return bRet;
}

//向mysql IMGroupMember id为nGroupId的群组里插入setUsers里的所有userId，并更新IMGroup里userCnt；
//若已经在setUsers中则更新状态，同时更新一份到redis group_member库中,key为group_member_nGroupId，field为setUsers里的userId，value为时间戳
bool CGroupModel::insertNewMember(uint32_t nGroupId, set<uint32_t>& setUsers)
{
    bool bRet = false;
    uint32_t nUserCnt = (uint32_t)setUsers.size();
    if(nGroupId != INVALID_VALUE &&  nUserCnt > 0)
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            uint32_t nCreated = (uint32_t)time(NULL);
            // 获取 已经存在群里的用户
            string strClause;
            bool bFirst = true;
            for (auto it=setUsers.begin(); it!=setUsers.end(); ++it)
            {
                if(bFirst)
                {
                    bFirst = false;
                    strClause = int2string(*it);
                }
                else
                {
                    strClause += ("," + int2string(*it));
                }
            }
            string strSql = "select userId from IMGroupMember where groupId=" + int2string(nGroupId) + " and userId in (" + strClause + ")";
            CResultSet* pResult = pDBConn->ExecuteQuery(strSql.c_str());
            set<uint32_t> setHasUser;
            if(pResult)
            {
                while (pResult->Next()) {
                    setHasUser.insert(pResult->GetInt("userId"));
                }
                delete pResult;
            }
            else
            {
                log("no result for sql:%s", strSql.c_str());
            }
            pDBManager->RelDBConn(pDBConn);
            
            pDBConn = pDBManager->GetDBConn("teamtalk_master");
            if (pDBConn)
            {
                CacheManager* pCacheManager = CacheManager::getInstance();
                CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
                if (pCacheConn)
                {
                    // 设置已经存在群中人的状态
                    if (!setHasUser.empty())
                    {
                        strClause.clear();
                        bFirst = true;
                        for (auto it=setHasUser.begin(); it!=setHasUser.end(); ++it) {
                            if(bFirst)
                            {
                                bFirst = false;
                                strClause = int2string(*it);
                            }
                            else
                            {
                                strClause += ("," + int2string(*it));
                            }
                        }
                        
                        strSql = "update IMGroupMember set status=0, updated="+int2string(nCreated)+" where groupId=" + int2string(nGroupId) + " and userId in (" + strClause + ")";
                        pDBConn->ExecuteUpdate(strSql.c_str());
                    }
                    strSql = "insert into IMGroupMember(`groupId`, `userId`, `status`, `created`, `updated`) values\
                    (?,?,?,?,?)";
                    
                    //插入新成员
                    auto it = setUsers.begin();
                    uint32_t nStatus = 0;
                    uint32_t nIncMemberCnt = 0;
                    for (;it != setUsers.end();)
                    {
                        uint32_t nUserId = *it;
                        if(setHasUser.find(nUserId) == setHasUser.end())
                        {
                            CPrepareStatement* pStmt = new CPrepareStatement();
                            if (pStmt->Init(pDBConn->GetMysql(), strSql))
                            {
                                uint32_t index = 0;
                                pStmt->SetParam(index++, nGroupId);
                                pStmt->SetParam(index++, nUserId);
                                pStmt->SetParam(index++, nStatus);
                                pStmt->SetParam(index++, nCreated);
                                pStmt->SetParam(index++, nCreated);
                                pStmt->ExecuteUpdate();
                                ++nIncMemberCnt;
                                delete pStmt;
                            }
                            else
                            {
                                setUsers.erase(it++);
                                delete pStmt;
                                continue;
                            }
                        }
                        ++it;
                    }
                    if(nIncMemberCnt != 0)
                    {
                        strSql = "update IMGroup set userCnt=userCnt+" + int2string(nIncMemberCnt) + " where id="+int2string(nGroupId);
                        pDBConn->ExecuteUpdate(strSql.c_str());
                    }
                    
                    //更新一份到redis中
                    string strKey = "group_member_"+int2string(nGroupId);
                    for(auto it = setUsers.begin(); it!=setUsers.end(); ++it)
                    {
                        pCacheConn->hset(strKey, int2string(*it), int2string(nCreated));
                    }
                    pCacheManager->RelCacheConn(pCacheConn);
                    bRet = true;
                }
                else
                {
                    log("no cache connection");
                }
                pDBManager->RelDBConn(pDBConn);
            }
            else
            {
                log("no db connection for teamtalk_master");
            }
        }
        else
        {
            log("no db connection for teamtalk_slave");
        }
    }
    return bRet;
}

//从mysql获取成员nUserId所加入的群组
//从mysql IMGroupMember里获取userId为nUserId 状态正常 前nLimited条记录的groupId，存入lsGroupId
void CGroupModel::getUserGroupIds(uint32_t nUserId, list<uint32_t>& lsGroupId, uint32_t nLimited)
{
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
    if(pDBConn)
    {
        string strSql ;
        if (nLimited != 0) {
            strSql = "select groupId from IMGroupMember where userId=" + int2string(nUserId) + " and status = 0 order by updated desc, id desc limit " + int2string(nLimited);
        }
        else
        {
            strSql = "select groupId from IMGroupMember where userId=" + int2string(nUserId) + " and status = 0 order by updated desc, id desc";
        }
        
        CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
        if(pResultSet)
        {
            while(pResultSet->Next())
            {
                uint32_t nGroupId = pResultSet->GetInt("groupId");
                lsGroupId.push_back(nGroupId);
            }
            delete pResultSet;
        }
        else
        {
            log("no result set for sql:%s", strSql.c_str());
        }
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_slave");
    }
}

//从mysql IMGrup里获取id在lsGroupId里所有群组版本信息，存入lsGroup
void CGroupModel::getGroupVersion(list<uint32_t> &lsGroupId, list<IM::BaseDefine::GroupVersionInfo> &lsGroup, uint32_t nGroupType)
{
    if(!lsGroupId.empty())
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if(pDBConn)
        {
            string strClause;
            bool bFirst = true;
            for(list<uint32_t>::iterator it=lsGroupId.begin(); it!=lsGroupId.end(); ++it)
            {
                if(bFirst)
                {
                    bFirst = false;
                    strClause = int2string(*it);
                }
                else
                {
                    strClause += ("," + int2string(*it));
                }
            }
            
            string strSql = "select id,version from IMGroup where id in (" +  strClause  + ")";
            if(0 != nGroupType)
            {
                strSql += " and type="+int2string(nGroupType);
            }
            strSql += " order by updated desc";
            
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while(pResultSet->Next())
                {
                    IM::BaseDefine::GroupVersionInfo group;
                    group.set_group_id(pResultSet->GetInt("id"));
                    group.set_version(pResultSet->GetInt("version"));
                    lsGroup.push_back(group);
                }
                delete pResultSet;
            }
            else
            {
                log("no result set for sql:%s", strSql.c_str());
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            log("no db connection for teamtalk_slave");
        }
    }
    else
    {
        log("group ids is empty");
    }
}

//判断nUserId是否在群组nGroupId里
//通过查找redis group_member库哈希表key为group_member_nGroupId filed为nUserId的字段判断成员是否存在
bool CGroupModel::isInGroup(uint32_t nUserId, uint32_t nGroupId)
{
    bool bRet = false;
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if (pCacheConn)
    {
        string strKey = "group_member_" + int2string(nGroupId);
        string strField = int2string(nUserId);
        string strValue = pCacheConn->hget(strKey, strField);
        pCacheManager->RelCacheConn(pCacheConn);
        if(!strValue.empty())
        {
            bRet = true;
        }
    }
    else
    {
        log("no cache connection for group_member");
    }
    return bRet;
}

//判断群组是否具有修改权限；
//若群组类型为GROUP_TYPE_TMP且nType为GROUP_MODIFY_TYPE_ADD则返回true;
//当上述条件不满足但creator为nUserId时也返回true;
//其他情况返回false
bool CGroupModel::hasModifyPermission(uint32_t nUserId, uint32_t nGroupId, IM::BaseDefine::GroupModifyType nType)
{
    if(nUserId == 0) {
        return true;
    }
    
    bool bRet = false;
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
    if(pDBConn)
    {
        string strSql = "select creator, type from IMGroup where id="+ int2string(nGroupId);
        CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
        if(pResultSet)
        {
            while (pResultSet->Next())
            {
                uint32_t nCreator = pResultSet->GetInt("creator");
                IM::BaseDefine::GroupType nGroupType = IM::BaseDefine::GroupType(pResultSet->GetInt("type"));
                if(IM::BaseDefine::GroupType_IsValid(nGroupType))
                {
                    if(IM::BaseDefine::GROUP_TYPE_TMP == nGroupType && IM::BaseDefine::GROUP_MODIFY_TYPE_ADD == nType)
                    {
                        bRet = true;
                        break;
                    }
                    else
                    {
                        if(nCreator == nUserId)
                        {
                            bRet = true;
                            break;
                        }
                    }
                }
            }
            delete pResultSet;
        }
        else
        {
            log("no result for sql:%s", strSql.c_str());
        }
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_slave");
    }
    return bRet;
}

bool CGroupModel::addMember(uint32_t nGroupId, set<uint32_t> &setUser, list<uint32_t>& lsCurUserId)
{
    // 去掉已经存在的用户ID
    removeRepeatUser(nGroupId, setUser);
    bool bRet = insertNewMember(nGroupId, setUser);
    getGroupUser(nGroupId,lsCurUserId);
    return bRet;
}

//更新mysql IMGroupMember里groupId为nGroupId， userId在setUser里的成员状态为1(0-正常，1-已退出)；
//将redis group_member库里哈希表key为group_member_nGroupId field在setUser里的字段删除；
//最后调用getGroupUser从redis里获取群组成员存入lsCurUserId
bool CGroupModel::removeMember(uint32_t nGroupId, set<uint32_t> &setUser, list<uint32_t>& lsCurUserId)
{
    if(setUser.size() <= 0)
    {
        return true;
    }
    bool bRet = false;
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    if(pDBConn)
    {
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
        if (pCacheConn)
        {
            string strClause ;
            bool bFirst = true;
            for(auto it= setUser.begin(); it!=setUser.end();++it)
            {
                if (bFirst) {
                    bFirst = false;
                    strClause = int2string(*it);
                }
                else
                {
                    strClause += ("," + int2string(*it));
                }
            }
            string strSql = "update IMGroupMember set status=1 where  groupId =" + int2string(nGroupId) + " and userId in(" + strClause + ")";
            pDBConn->ExecuteUpdate(strSql.c_str());
            
            //从redis中删除成员
            string strKey = "group_member_"+ int2string(nGroupId);
            for (auto it=setUser.begin(); it!=setUser.end(); ++it) {
                string strField = int2string(*it);
                pCacheConn->hdel(strKey, strField);
            }
            pCacheManager->RelCacheConn(pCacheConn);
            bRet = true;
        }
        else
        {
            log("no cache connection");
        }
        pDBManager->RelDBConn(pDBConn);
        if (bRet)
        {
            getGroupUser(nGroupId,lsCurUserId);
        }
    }
    else
    {
        log("no db connection for teamtalk_master");
    }
    return bRet;
}


//若setUser里userid在redis里则剔除
void CGroupModel::removeRepeatUser(uint32_t nGroupId, set<uint32_t> &setUser)
{
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if (pCacheConn)
    {
        string strKey = "group_member_"+int2string(nGroupId);
        for (auto it=setUser.begin(); it!=setUser.end();) {
            string strField = int2string(*it);
            string strValue = pCacheConn->hget(strKey, strField);
            pCacheManager->RelCacheConn(pCacheConn);
            if(!strValue.empty())
            {
                setUser.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }
    else
    {
        log("no cache connection for group_member");
    }
}

//若nUserId在群组nGroupId里则向redis group_set库 key为group_set_nGroupId的哈希表插入nUserId的屏蔽状态信息
//0屏蔽 1显示
bool CGroupModel::setPush(uint32_t nUserId, uint32_t nGroupId, uint32_t nType, uint32_t nStatus)
{
    bool bRet = false;
    if(!isInGroup(nUserId, nGroupId))
    {
        log("user:%d is not in group:%d", nUserId, nGroupId);
        return bRet;;
    }
    
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_set");
    if(pCacheConn)
    {
        string strGroupKey = "group_set_" + int2string(nGroupId);
        string strField = int2string(nUserId) + "_" + int2string(nType);
        int nRet = pCacheConn->hset(strGroupKey, strField, int2string(nStatus));
        pCacheManager->RelCacheConn(pCacheConn);
        if(nRet != -1)
        {
            bRet = true;
        }
    }
    else
    {
        log("no cache connection for group_set");
    }
    return bRet;
}

//获取redis group_set库里key为group_set_nGroupId的哈希表里用户在lsUser的屏蔽状态信息，将获取到的信息存入lsPush
void CGroupModel::getPush(uint32_t nGroupId, list<uint32_t>& lsUser, list<IM::BaseDefine::ShieldStatus>& lsPush)
{
    if (lsUser.empty()) {
        return;
    }
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_set");
    if(pCacheConn)
    {
        string strGroupKey = "group_set_" + int2string(nGroupId);
        map<string, string> mapResult;
        bool bRet = pCacheConn->hgetAll(strGroupKey, mapResult);
        pCacheManager->RelCacheConn(pCacheConn);
        if(bRet)
        {
            for(auto it=lsUser.begin(); it!=lsUser.end(); ++it)
            {
                string strField = int2string(*it) + "_" + int2string(IM_GROUP_SETTING_PUSH);
                auto itResult = mapResult.find(strField);
                IM::BaseDefine::ShieldStatus status;
                status.set_group_id(nGroupId);
                status.set_user_id(*it);
                if(itResult != mapResult.end())
                {
                    status.set_shield_status(string2int(itResult->second));
                }
                else
                {
                    status.set_shield_status(0);
                }
                lsPush.push_back(status);
            }
        }
        else
        {
            log("hgetall %s failed!", strGroupKey.c_str());
        }
    }
    else
    {
        log("no cache connection for group_set");
    }
}

//从redis group_member库里哈希表key为group_member_nGroupId的所有字段，将该key的所有filed存入lsUserId
void CGroupModel::getGroupUser(uint32_t nGroupId, list<uint32_t> &lsUserId)
{
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if (pCacheConn)
    {
        string strKey = "group_member_" + int2string(nGroupId);
        map<string, string> mapAllUser;
        bool bRet = pCacheConn->hgetAll(strKey, mapAllUser);
        pCacheManager->RelCacheConn(pCacheConn);
        if(bRet)
        {
            for (auto it=mapAllUser.begin(); it!=mapAllUser.end(); ++it) {
                uint32_t nUserId = string2int(it->first);
                lsUserId.push_back(nUserId);
            }
        }
        else
        {
            log("hgetall %s failed!", strKey.c_str());
        }
    }
    else
    {
        log("no cache connection for group_member");
    }
}

//更新mysql IMGroup表里id为nGroupId的群组lastChated信息
void CGroupModel::updateGroupChat(uint32_t nGroupId)
{
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    if(pDBConn)
    {
        uint32_t nNow = (uint32_t)time(NULL);
        string strSql = "update IMGroup set lastChated=" + int2string(nNow) + " where id=" + int2string(nGroupId);
        pDBConn->ExecuteUpdate(strSql.c_str());
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_master");
    }
}

//bool CGroupModel::isValidateGroupId(uint32_t nGroupId)
//{
//    bool bRet = false;
//    CDBManager* pDBManager = CDBManager::getInstance();
//    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
//    if(pDBConn)
//    {
//        string strSql = "select id from IMGroup where id=" + int2string(nGroupId)+" and status=0";
//        CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
//        if(pResultSet && pResultSet->Next())
//        {
//            bRet =  true;
//            delete pResultSet;
//        }
//        pDBManager->RelDBConn(pDBConn);
//    }
//    else
//    {
//        log("no db connection for teamtalk_slave");
//    }
//    return bRet;
//}


//判断群组nGroupId是否存在(判断是否在redis group_member库里)
bool CGroupModel::isValidateGroupId(uint32_t nGroupId)
{
    bool bRet = false;
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if(pCacheConn)
    {
        string strKey = "group_member_"+int2string(nGroupId);
        bRet = pCacheConn->isExists(strKey);
        pCacheManager->RelCacheConn(pCacheConn);
    }
    return bRet;
}


void CGroupModel::removeSession(uint32_t nGroupId, const set<uint32_t> &setUser)
{
    for(auto it=setUser.begin(); it!=setUser.end(); ++it)
    {
        uint32_t nUserId=*it;
        uint32_t nSessionId = CSessionModel::getInstance()->getSessionId(nUserId, nGroupId, IM::BaseDefine::SESSION_TYPE_GROUP, false);
        CSessionModel::getInstance()->removeSession(nSessionId);
    }
}

//将mysql IMGroup里id为nGroupId的version加1
bool CGroupModel::incGroupVersion(uint32_t nGroupId)
{
    bool bRet = false;
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    if(pDBConn)
    {
        string strSql = "update IMGroup set version=version+1 where id="+int2string(nGroupId);
        if(pDBConn->ExecuteUpdate(strSql.c_str()))
        {
            bRet = true;
        }
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_master");
    }
    return  bRet;
}

//为lsGroups里每项GroupInfo的group_member_list添加相应的成员id
void CGroupModel::fillGroupMember(list<IM::BaseDefine::GroupInfo>& lsGroups)
{
    for (auto it=lsGroups.begin(); it!=lsGroups.end(); ++it) {
        list<uint32_t> lsUserIds;
        uint32_t nGroupId = it->group_id();
        getGroupUser(nGroupId, lsUserIds);
        for(auto itUserId=lsUserIds.begin(); itUserId!=lsUserIds.end(); ++itUserId)
        {
            it->add_group_member_list(*itUserId);
        }
    }
}


//获取成员nUserId加入群组nGroupId时间戳(通过查阅redis group_member库 key为group_member_nGroupId的哈希表获取)
uint32_t CGroupModel::getUserJoinTime(uint32_t nGroupId, uint32_t nUserId)
{
    uint32_t nTime = 0;
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if (pCacheConn)
    {
        string strKey = "group_member_" + int2string(nGroupId);
        string strField = int2string(nUserId);
        string strValue = pCacheConn->hget(strKey, strField);
        pCacheManager->RelCacheConn(pCacheConn);
        if (!strValue.empty()) {
            nTime = string2int(strValue);
        }
    }
    else
    {
        log("no cache connection for group_member");
    }
    return  nTime;
}

//删除mysql IMGroupMember里groupId为nGroupId的记录，
//同时删除redis里group_member库里相关哈希表key为group_member_nGroupId的相关字段
void CGroupModel::clearGroupMember(uint32_t nGroupId)
{
    CDBManager* pDBManager = CDBManager::getInstance();
    CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
    if(pDBConn)
    {
        string strSql = "delete from IMGroupMember where groupId="+int2string(nGroupId);
        pDBConn->ExecuteUpdate(strSql.c_str());
        pDBManager->RelDBConn(pDBConn);
    }
    else
    {
        log("no db connection for teamtalk_master");
    }
    CacheManager* pCacheManager = CacheManager::getInstance();
    CacheConn* pCacheConn = pCacheManager->GetCacheConn("group_member");
    if(pCacheConn)
    {
        string strKey = "group_member_" + int2string(nGroupId);
        map<string, string> mapRet;
        bool bRet = pCacheConn->hgetAll(strKey, mapRet);
        if(bRet)
        {
            for(auto it=mapRet.begin(); it!=mapRet.end(); ++it)
            {
                pCacheConn->hdel(strKey, it->first);
            }
        }
        else
        {
            log("hgetall %s failed", strKey.c_str());
        }
        pCacheManager->RelCacheConn(pCacheConn);
    }
    else
    {
        log("no cache connection for group_member");
    }
}
