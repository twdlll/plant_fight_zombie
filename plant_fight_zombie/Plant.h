#pragma once
#include "Context.h"
#include "Bullet.h"
class Plant{
private:
	POINT p;
	PPlantInfo plantInfo;
	STATE_TYPE lastType;             //上个状态
	STATE_TYPE currentType;          //当前状态
	unsigned lastAction;             //上个动作时间
	int currentIndex;                //当前帧
	int currentHp;                   //当前生命值
	PStateInfo GetStateInfo();       //得到当前状态信息
public:
	POINT& GetLocation();                //得到植物坐标
	void draw(HDC hDc);                  //绘制植物
	Plant(POINT,PPlantInfo);             //构造函数
	PBullet Shoot();                     //发射子弹 
	bool BeAttacked(int);                //被攻击
	PPlantInfo GetPlantInfo();           //得到植物信息
};
typedef Plant *PPlant;