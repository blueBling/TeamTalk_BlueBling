//
//  test_lock.cpp
//  test_lock
//
//  Created by blueBling on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "Thread.h"
#include "Lock.h"

#include<iostream>

#include <unistd.h> //sleep


using std::cout;
using std::endl;

static uint32_t s_cnt = 0;
static CLock* g_pLock = new CLock();
static CRWLock *g_pRWDeptLock = new CRWLock();

class MyThread: public CThread
{
public:
	MyThread(){m_bRunning = false;};
	virtual ~MyThread(){StopThread();}
    
	void OnThreadRun(void);
	
	void StartThread(){
		m_bRunning = true;
		CThread::StartThread();
	}

	void StopThread(){m_bRunning = false;}
	bool IsRunning() { return m_bRunning; }
private:
	bool 		m_bRunning;
};

void MyThread::OnThreadRun(void){
	for(uint32_t i = 0; i < 100000 && m_bRunning; i++){

		//CAutoLock autoLock(g_pLock);               //type 1
		CAutoRWLock autoLock(g_pRWDeptLock, 0);      //type 2
		//g_pLock->lock();                           //type 3
		//g_pRWDeptLock->wlock();                    //type 4
		s_cnt++; 
		//g_pLock->unlock();
		//g_pRWDeptLock->unlock();
		usleep(10);
	}
}

int test_lock(){

	MyThread mythread_1;
	mythread_1.StartThread();

	MyThread mythread_2;
	mythread_2.StartThread();

	while(1){
		sleep(10);
		cout << "s_cnt is:" << s_cnt << endl;
	}
	
	return 0;
}

int main(){
	test_lock();
	return 0;
}