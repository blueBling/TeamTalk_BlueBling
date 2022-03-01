/*================================================================
*     Copyright (c) 2015年 lanhu. All rights reserved.
*   
*   文件名称：DepartModel.h
*   创 建 者：Zhang Yuanhao
*   邮    箱：bluefoxah@gmail.com
*   创建日期：2015年03月12日
*   描    述：
*
#pragma once
================================================================*/
#ifndef __DEPARTMODEL_H__
#define __DEPARTMODEL_H__

#include "ImPduBase.h"
#include "IM.BaseDefine.pb.h"

class CDepartModel {
public:
    static CDepartModel* getInstance();
    ~CDepartModel() {}
	//用于从mysql表IMDepart获取updated > nLastTime的部门id,并将id存到list<uint32_t>& lsChangedIds
    void getChgedDeptId(uint32_t& nLastTime, list<uint32_t>& lsChangedIds);
	//用于从mysql表IMDepart获取id在lsDeptIds的部门详情，将这些部门详情信息存入list<IM::BaseDefine::DepartInfo>& lsDepts
    void getDepts(list<uint32_t>& lsDeptIds, list<IM::BaseDefine::DepartInfo>& lsDepts);
	//用于从mysql表IMDepart获取指定nDeptId的部门详情
    void getDept(uint32_t nDeptId, IM::BaseDefine::DepartInfo& cDept);
private:
    CDepartModel(){};
private:
    static CDepartModel* m_pInstance;
};

#endif /*defined(__DEPARTMODEL_H__) */
