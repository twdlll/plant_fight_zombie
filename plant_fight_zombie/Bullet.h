#pragma once
#include "Context.h"
class Bullet
{
private:
	POINT p;                            //子弹坐标
	PBulletInfo bulletInfo;             //子弹信息
	unsigned lastAction;                //最开始的时间
	int currentIndex;                   //当前帧
public:
	Bullet(POINT p,PBulletInfo info);   //构造子弹
	~Bullet();
	POINT& GetLocation();               //得到子弹坐标
	bool Move();                        //子弹移动函数
	void Draw(HDC hDc);                 //绘制图形
	BULLET_TYPE GetBulletType();        //得到子弹类型
};
typedef Bullet *PBullet;

