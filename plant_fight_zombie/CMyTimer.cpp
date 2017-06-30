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

//�����ʱ��
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
		//����ɾ�����
		pTimer->CheckDelTimer();

		//��ʱ������
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

//���һ����ʱ��,timeMsΪ��ʱ��ִ�м��
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

	//�õ���ǰʱ��
	unsigned timeNow = GetTickCount();
	tempTimer.timeCreate = timeNow;
	tempTimer.timeLastRun = timeNow;
	tempTimer.timeElapce = timeMs;
	m_timerList.push_back(tempTimer);
	if (m_timerList.size() == 1)
	{
		//��ʱΪ��һ����ʱ��������һ���߳����ڶ�ʱ����ִ��
		_beginthread(CheckTimerRun, 0, this);
	}
}

//ɾ����ʱ��,��δ�������Ƴ�
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


//��ʱ������
//int CMyTimer::OnTimer(int id,int Param,string str)


//��ʱ���ɾ����ʱ��
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
		//ִ����Ӧ�Ķ�ʱ������
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