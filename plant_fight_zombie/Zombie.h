#pragma once
#include "Context.h"
class Zombie
{
private:
	POINT p;                                     //僵尸坐标
	PZombieInfo zombieInfo;                      //僵尸信息
	STATE_TYPE lastType;                         //上个状态
	STATE_TYPE currentType;                      //当前状态
	unsigned lastAttack;                         //上次攻击时间
	int currentIndex;                            //当前帧
	int currentHp;                               //当前生命值
	PStateInfo GetStateInfo();                   //得到当前状态资源
	
public:
	Zombie();                        //根据僵尸信息创造僵尸
	~Zombie();
	POINT& GetLocation();            //得到坐标
	PZombieInfo GetInfo();           //得到僵尸信息
	bool Draw(HDC hDc);              //绘制僵尸
	bool Move();                     //僵尸移动
	bool BeAttacked(PBulletInfo);    //被攻击
	int Attack(bool);                //攻击函数
	STATE_TYPE GetState();           //得到当前状态
};
typedef Zombie *PZombie;

