#pragma once
#include "CMyTimer.h"
#include "Bullet.h"
#include "Data.h"
#include "Context.h"
using namespace std;

//定时器的类型
enum TIMER_TYPE{
	TIMER_DRAW,                //用于绘图
	TIMER_CREATE_ZOMBIE,       //用于创建僵尸
	TIMER_MOVE,                //用于控制移动
	TIMER_SHOOT,               //用于发射子弹
	TIMER_CREATE_SUN           //创造自由阳光
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
	void Begin(HWND hWnd);            //游戏开始
	void LButtonUp();                 //鼠标左键放开
	void RButtonUp();                 //鼠标右键放开
private:
	void Draw();                      //绘制函数
	void DrawBackground(HDC hDc);     //绘制背景
	void DrawMenu(HDC hDc);           //绘制植物图标
	void DrawSunText(HDC hDc);        //绘制阳光数(文字)
	void DrawPlantAndZombie(HDC hDc); //绘制植物和僵尸
	void DrawMouseItem(HDC hDc);      //绘制鼠标上的植物
	void DrawBullet(HDC hDc);         //绘制子弹
	void CreateZombie();              //创建僵尸
	void Move();                      //控制僵尸移动和子弹移动
	void CheckAttack();               //检测子弹是否攻击到僵尸
	void CheckAttacked();             //检测植物是否被攻击
	void CheckState();                //检测僵尸状态是否恢复
	void Shoot();                     //发射子弹
	void GameOver();                  //游戏失败
	void GameSuccess();               //游戏成功
	bool ObtainSun();                 //获取阳光
	PLANT_TYPE GetInRect();           //得到对应位置的植物
	bool CanPut(POINT&);              //是否可在该位置放置植物
	void CreateSun();                 //创建自由阳光
};