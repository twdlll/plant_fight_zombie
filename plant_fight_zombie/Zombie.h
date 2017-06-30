#pragma once
#include "Context.h"
class Zombie
{
private:
	POINT p;                                     //��ʬ����
	PZombieInfo zombieInfo;                      //��ʬ��Ϣ
	STATE_TYPE lastType;                         //�ϸ�״̬
	STATE_TYPE currentType;                      //��ǰ״̬
	unsigned lastAttack;                         //�ϴι���ʱ��
	int currentIndex;                            //��ǰ֡
	int currentHp;                               //��ǰ����ֵ
	PStateInfo GetStateInfo();                   //�õ���ǰ״̬��Դ
	
public:
	Zombie();                        //���ݽ�ʬ��Ϣ���콩ʬ
	~Zombie();
	POINT& GetLocation();            //�õ�����
	PZombieInfo GetInfo();           //�õ���ʬ��Ϣ
	bool Draw(HDC hDc);              //���ƽ�ʬ
	bool Move();                     //��ʬ�ƶ�
	bool BeAttacked(PBulletInfo);    //������
	int Attack(bool);                //��������
	STATE_TYPE GetState();           //�õ���ǰ״̬
};
typedef Zombie *PZombie;

