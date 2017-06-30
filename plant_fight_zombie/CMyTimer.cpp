#pragma once 
#include "stdafx.h"
#include "CMyTimer.h"


CMyTimer::CMyTimer()
{

}

CMyTimer::~CMyTimer()
{
	ClearTimer();
}

//清除定时器
void CMyTimer::ClearTimer(){
	for (auto it = m_timerList.begin(); it != m_timerList.end();it++)
	{
		it->bDel = true;
	}
}

void CheckTimerRun(void* q)
{
	CMyTimer* pTimer = (CMyTimer*)q;
	if (pTimer == NULL)
	{
		return;
	}
	while (1)
	{
		//进行删除检查
		pTimer->CheckDelTimer();

		//定时器运行
		if (!pTimer->TimerRun())
		{
			_endthread();
		}
		Sleep(20);
	}
}

bool CMyTimer::ChangElapce(int id, unsigned newTime)
{
	for (auto timer = m_timerList.begin(); timer != m_timerList.end(); timer++)
	{
		if ((*timer).id == id)
		{
			(*timer).timeElapce = newTime;
			return true;
		}
	}
	return false;
}

unsigned CMyTimer::GetElapse(int id)
{
	for (auto timer = m_timerList.begin(); timer != m_timerList.end(); timer++)
	{
		if ((*timer).id == id)
		{
			return (*timer).timeElapce;
		}
	}
	return 0;
}

//添加一个定时器,timeMs为定时器执行间隔
void CMyTimer::AddTimer(unsigned timerId, unsigned timeMs, int param, char* p)
{
	if (timeMs == 0)
	{
		return;
	}
	stTimer tempTimer;
	tempTimer.bDel = false;
	tempTimer.id = timerId;
	tempTimer.iParam = param;
	if (p != NULL)
	{
		tempTimer.sParam = p;
	}

	//得到当前时间
	unsigned timeNow = GetTickCount();
	tempTimer.timeCreate = timeNow;
	tempTimer.timeLastRun = timeNow;
	tempTimer.timeElapce = timeMs;
	m_timerList.push_back(tempTimer);
	if (m_timerList.size() == 1)
	{
		//此时为第一个定时器，开启一个线程用于定时器的执行
		_beginthread(CheckTimerRun, 0, this);
	}
}

//删除定时器,但未从容器移除
void CMyTimer::DeleteTimer(int id)
{
	for (auto it = m_timerList.begin(); it != m_timerList.end(); it++)
	{
		if (it->id == id)
		{
			it->bDel = true;
			return;
		}
	}
}


//定时器处理
//int CMyTimer::OnTimer(int id,int Param,string str)


//定时检测删除定时器
void CMyTimer::CheckDelTimer()
{
	for (auto it = m_timerList.begin(); it != m_timerList.end();)
	{
		if (it->bDel)
		{
			it = m_timerList.erase(it);
			continue;
		}
		it++;
	}
}

bool CMyTimer::TimerRun()
{
	if (m_timerList.size() == 0)
	{
		return false;
	}
	unsigned timeNow = GetTickCount();
	for (auto it = m_timerList.begin(); it != m_timerList.end(); it++)
	{
		//执行相应的定时器操作
		if (timeNow - it->timeLastRun >= it->timeElapce)
		{
			it->timeLastRun = timeNow;
			if (OnTimer(it->id, it->iParam, it->sParam) == 0)
			{
				it->bDel = true;
			}
		}
	}
	return true;
}