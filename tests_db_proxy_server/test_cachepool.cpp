//
//  test_cachepool.cpp
//  test_cachepool
//
//  Created by blueBling on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "CachePool.h"

#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

int test_cachepool(){

	CacheManager* pCacheManager = CacheManager::getInstance();
	if (!pCacheManager) {
		cout << "CacheManager init failed" << endl;
		return -1;
	}

    CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread"); //从unread缓存池空闲链表中获取一个连接
    if (pCacheConn)
    {
        string strTotalUpdate = pCacheConn->get("total_user_updated");

        string strLastUpdateGroup = pCacheConn->get("last_update_group");

		if(strTotalUpdate != "")
        {
            uint32_t nLastUpdate = string2int(strTotalUpdate);
			cout << "nLastUpdate:" << nLastUpdate << endl;
        }
        else
        {
           	cout << "total_user_updated is empty" << endl;

	        string strUpdated = int2string(time(NULL));
	        pCacheConn->set("total_user_update", strUpdated);
        }
		
        if(strLastUpdateGroup.empty())
        {
            uint32_t nLastUpdateGroup = string2int(strLastUpdateGroup);
			cout << "nLastUpdateGroup:" << nLastUpdateGroup << endl;
        }
        else
        {
            cout << "strLastUpdateGroup:" << strLastUpdateGroup << endl;
        }

		pCacheManager->RelCacheConn(pCacheConn); //还回连接
    }
    else
    {
        cout << "no cache connection to get total_user_updated" << endl;
    }

	//CachePool析构会释放缓存池，但是缓存池是通过指针存储的，动态内存需要手动释放，teamtalk未提供释放所有缓存池的接口，因为所有缓存池所有连接在服务器正常运行时都得重复使用
	//释放方法，遍历m_cache_pool_map所有缓存池，逐一delete
//	map<string, CachePool*>::iterator it = pCacheManager->m_cache_pool_map.find("unread");
//	if (it != pCacheManager->m_cache_pool_map.end()) {
//		delete it->second;
//	}
//	it = pCacheManager->m_cache_pool_map.find("group_set");
//	if (it != pCacheManager->m_cache_pool_map.end()) {
//		delete it->second;
//	}
//	it = pCacheManager->m_cache_pool_map.find("token");
//	if (it != pCacheManager->m_cache_pool_map.end()) {
//		delete it->second;
//	}
//	it = pCacheManager->m_cache_pool_map.find("sync");
//	if (it != pCacheManager->m_cache_pool_map.end()) {
//		delete it->second;
//	}
//	it = pCacheManager->m_cache_pool_map.find("group_member");
//	if (it != pCacheManager->m_cache_pool_map.end()) {
//		delete it->second;
//	}
	
	return 0;
}

int main(){
	test_cachepool();
	return 0;
}