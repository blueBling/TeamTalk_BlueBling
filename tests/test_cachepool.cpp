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

typedef unsigned int	uint32_t;

int test_cachepool(){

	CacheManager* pCacheManager = CacheManager::getInstance();
	if (!pCacheManager) {
		cout << "CacheManager init failed" << endl;
		return -1;
	}

    CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
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

		pCacheManager->RelCacheConn(pCacheConn);
    }
    else
    {
        cout << "no cache connection to get total_user_updated" << endl;
    }
	
	return 0;
}

int main(){
	test_cachepool();
	return 0;
}