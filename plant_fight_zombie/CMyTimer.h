#pragma once
#include "string"
#include "list"
using namespace std;

struct stTimer
{
	unsigned timeElapce;   //��ʱ���������ʱ��
	unsigned timeCreate;   //��ʱ������ʱ��
	unsigned timeLastRun;  //�ϴζ�ʱ��ִ��ʱ��
	unsigned id;           //��ʱ��Id
	int iParam;            //Ԥ������
	string sParam;         //Ԥ������
	bool bDel;             //�Ƿ�ɾ��

	stTimer()
	{
		timeCreate = 0;
		timeLastRun = 0;
		id = 1;
		iParam = 0;
		bDel = false;
	}
};

typedef list<stTimer> TimerList;

class CMyTimer{
public :
	CMyTimer();
	virtual ~CMyTimer();
	//��Ӷ�ʱ��
	void AddTimer(unsigned timerId, unsigned timeMs, int param = 0, char* p = NULL);
	//ɾ����ʱ��
	void DeleteTimer(int id);
	//��ʱ������
	virtual int OnTimer(int id, int iParam, string str) = 0;
	//��ⶨʱ������
	bool TimerRun();
	//��ö�ʱ����ʱ����
	unsigned GetElapse(int id);
	//�ı䶨ʱ����ʱ����
	bool ChangElapce(int id, unsigned newTime);
	//������ж�ʱ��
	void ClearTimer();
	//��ʱ���ɾ����ʱ��,��������ɾ����ʱ��
	void CheckDelTimer();

private:
	TimerList m_timerList;
};