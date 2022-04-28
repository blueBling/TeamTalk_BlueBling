/*================================================================
*     Copyright (c) 2014年 lanhu. All rights reserved.
*   
*   文件名称：CacheManager.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2014年12月02日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include <list>
#include <map>

#include "ostype.h"
#include "Lock.h"
#include "Condition.h"
#include "ImPduBase.h"
#include "public_define.h"
#include "IM.BaseDefine.pb.h"

class CSyncCenter
{
public:
    static CSyncCenter* getInstance();
    
    uint32_t getLastUpdate() {
               CAutoLock auto_lock(&last_update_lock_);
                return m_nLastUpdate;
           }
      uint32_t getLastUpdateGroup() {
                CAutoLock auto_lock(&last_update_lock_);
                return m_nLastUpdateGroup;
           }
    string getDeptName(uint32_t nDeptId);
    void startSync();
    void stopSync();
    void init();
    void updateTotalUpdate(uint32_t nUpdated);

private:
    void updateLastUpdateGroup(uint32_t nUpdated);
    
    CSyncCenter();
    ~CSyncCenter();
    static void* doSyncGroupChat(void* arg);
    
private:
    void getDept(uint32_t nDeptId, DBDeptInfo_t** pDept);
    DBDeptMap_t* m_pDeptInfo;        // 存储部门信息

    static CSyncCenter* m_pInstance; // 同步中心实例
    uint32_t    m_nLastUpdateGroup;  // 上次同步群组信息时间
    uint32_t    m_nLastUpdate;

    CCondition* m_pCondGroupChat;    // 同步超时控制
    CLock*      m_pLockGroupChat;
    static bool        m_bSyncGroupChatRuning;
    bool m_bSyncGroupChatWaitting;   // 同步等待标记，true继续同步，false停止同步
#ifdef _WIN32
    DWORD		m_nGroupChatThreadId;
#else
    pthread_t	m_nGroupChatThreadId;
#endif
    CLock last_update_lock_;


};


#endif /*defined(__CACHEMANAGER_H__) */
