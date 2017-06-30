#pragma once
#include "Context.h"
class Bullet
{
private:
	POINT p;                            //�ӵ�����
	PBulletInfo bulletInfo;             //�ӵ���Ϣ
	unsigned lastAction;                //�ʼ��ʱ��
	int currentIndex;                   //��ǰ֡
public:
	Bullet(POINT p,PBulletInfo info);   //�����ӵ�
	~Bullet();
	POINT& GetLocation();               //�õ��ӵ�����
	bool Move();                        //�ӵ��ƶ�����
	void Draw(HDC hDc);                 //����ͼ��
	BULLET_TYPE GetBulletType();        //�õ��ӵ�����
};
typedef Bullet *PBullet;

