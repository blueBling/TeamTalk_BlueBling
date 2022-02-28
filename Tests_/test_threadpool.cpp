//
//  test_threadpool.cpp
//  test_threadpool
//
//  Created by blueBling on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "ThreadPool.h"

#include<iostream>

#include <unistd.h> //sleep


using std::cout;
using std::endl;


typedef void (*task_handler_t)(uint32_t task_uuid);
class TestTask:public CTask
{
public:
	TestTask(uint32_t task_uuid, task_handler_t task_handler){
		m_task_handler = task_handler;
		m_task_uuid = task_uuid;
	}
	virtual ~TestTask(){};

	virtual void run(){
		if(m_task_handler){
			m_task_handler(m_task_uuid);
		}
	}
private:
	task_handler_t m_task_handler;
	uint32_t m_task_uuid;
};

void task_process(uint32_t task_uuid){
	cout << "task_process, task_uuid is " << task_uuid << endl;
} 

int test_threadpool(){
	CThreadPool g_thread_pool;
	g_thread_pool.Init(2);

	CTask* pTask = new TestTask(6118, task_process);

	g_thread_pool.AddTask(pTask);

	while(1){
		sleep(1);
	}
	
	return 0;
}

int main(){
	test_threadpool();
	return 0;
}