#pragma once
#include "CMyTimer.h"
#include "Bullet.h"
#include "Data.h"
#include "Context.h"
using namespace std;

//��ʱ��������
enum TIMER_TYPE{
	TIMER_DRAW,                //���ڻ�ͼ
	TIMER_CREATE_ZOMBIE,       //���ڴ�����ʬ
	TIMER_MOVE,                //���ڿ����ƶ�
	TIMER_SHOOT,               //���ڷ����ӵ�
	TIMER_CREATE_SUN           //������������
};

class Controller:public CMyTimer
{
private:
	HWND m_hWnd;
	Data* data;
	Context* context;
	mutex bulletLock;
	mutex plantLock;
	mutex sunLock;
public:
	Controller();
	~Controller(){}
	int OnTimer(int id, int iParam, string str);
	void Begin(HWND hWnd);            //��Ϸ��ʼ
	void LButtonUp();                 //�������ſ�
	void RButtonUp();                 //����Ҽ��ſ�
private:
	void Draw();                      //���ƺ���
	void DrawBackground(HDC hDc);     //���Ʊ���
	void DrawMenu(HDC hDc);           //����ֲ��ͼ��
	void DrawSunText(HDC hDc);        //����������(����)
	void DrawPlantAndZombie(HDC hDc); //����ֲ��ͽ�ʬ
	void DrawMouseItem(HDC hDc);      //��������ϵ�ֲ��
	void DrawBullet(HDC hDc);         //�����ӵ�
	void CreateZombie();              //������ʬ
	void Move();                      //���ƽ�ʬ�ƶ����ӵ��ƶ�
	void CheckAttack();               //����ӵ��Ƿ񹥻�����ʬ
	void CheckAttacked();             //���ֲ���Ƿ񱻹���
	void CheckState();                //��⽩ʬ״̬�Ƿ�ָ�
	void Shoot();                     //�����ӵ�
	void GameOver();                  //��Ϸʧ��
	void GameSuccess();               //��Ϸ�ɹ�
	bool ObtainSun();                 //��ȡ����
	PLANT_TYPE GetInRect();           //�õ���Ӧλ�õ�ֲ��
	bool CanPut(POINT&);              //�Ƿ���ڸ�λ�÷���ֲ��
	void CreateSun();                 //������������
};