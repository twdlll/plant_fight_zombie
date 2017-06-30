#pragma once
#include "Context.h"
#include "Bullet.h"
class Plant{
private:
	POINT p;
	PPlantInfo plantInfo;
	STATE_TYPE lastType;             //�ϸ�״̬
	STATE_TYPE currentType;          //��ǰ״̬
	unsigned lastAction;             //�ϸ�����ʱ��
	int currentIndex;                //��ǰ֡
	int currentHp;                   //��ǰ����ֵ
	PStateInfo GetStateInfo();       //�õ���ǰ״̬��Ϣ
public:
	POINT& GetLocation();                //�õ�ֲ������
	void draw(HDC hDc);                  //����ֲ��
	Plant(POINT,PPlantInfo);             //���캯��
	PBullet Shoot();                     //�����ӵ� 
	bool BeAttacked(int);                //������
	PPlantInfo GetPlantInfo();           //�õ�ֲ����Ϣ
};
typedef Plant *PPlant;