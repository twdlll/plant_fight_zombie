#pragma once
#include "stdafx.h"
//添加tinyxml.h，使用XML解析器
#include "tinyxml.h"
#include <vector>
#define FILE_PATH  "data.xml"            //xml文件的位置
#define BULLET_NOT_SUN  -1
using namespace std;

//用于标识对象处于何种状态，状态类型适用于所有对象
enum STATE_TYPE{
	STATE_NONE,                      //不处于任何明确的状态
	STATE_MOVE,                      //移动状态
	STATE_STAND,                     //站立状态
	STATE_DIE,                       //死亡装填
	STATE_ATTACK,                    //攻击状态
	STATE_BEATTACK                   //被攻击状态
};

//僵尸的种类
enum ZOMBIE_TYPE{
	ZOMBIE_NONE,                      //不是僵尸
	ZOMBIE_NORMAL,                    //普通僵尸
	ZOMBIE_FLAG,                      //旗子僵尸
	ZOMBIE_CONEHEAD,                  //圆锥头僵尸
	ZOMBIE_BUCKETHEAD                 //铁桶僵尸
};

//植物的种类
enum PLANT_TYPE{
	PLANT_NONE,                       //不是植物
	PLANT_SUNNER,                     //向日葵
	PLANT_PEASHOOTER,                 //豌豆射手
	PLANT_WALLNUT,                    //坚果墙
	PLANT_REPEATER                    //连续射手
};

//子弹的种类
enum BULLET_TYPE{
	BULLET_NONE,                      //不是子弹
	BULLET_SUN,                       //阳光，被认为是子弹的一种
	BULLET_NORMAL,                    //普通子弹
	BULLET_TWO                        //双重子弹
};
//保存动作资源的一些必要信息
typedef struct stActionInfo{
	STATE_TYPE type;                 //状态
	int width;                       //图片宽度
	int height;                      //图片高度
	int num;                         //动画帧数
	string src;                      //图片资源路径
	stActionInfo(STATE_TYPE _type,int _width,int _height,int _num,string _src)
	{
		type = _type;
		width = _width;
		height = _height;
		num = _num;
		src =_src;
	}
}StateInfo,*PStateInfo;

//僵尸的全部信息
typedef struct stZombieInfo{
	ZOMBIE_TYPE type;                //僵尸类型
	int hpValue;                     //生命值
	int moveSpeed;                   //移动速度
	int attackValue;                 //攻击力
	int attackSpeed;                 //攻击速度
	int defenceValue;                //防御力
	vector<PStateInfo>* container;  //资源容器
	stZombieInfo(ZOMBIE_TYPE _type,int _hpValue,int _moveSpeed,
		int _attackValue,int _attackSpeed,int _defenceValue,vector<PStateInfo>* _container)
	{
		type = _type;
		hpValue = _hpValue;
		moveSpeed = _moveSpeed;
		attackSpeed = _attackSpeed;
		attackValue = _attackValue;
		defenceValue = _defenceValue;
		container = _container;
	}
}ZombieInfo,*PZombieInfo;

//植物的全部信息
typedef struct stPlantInfo{
	PLANT_TYPE type;                 //植物类型
	int hpValue;                     //生命值
	int attackSpeed;                 //攻击速度
	BULLET_TYPE bulletType;          //子弹类别
	int defenceValue;                //植物的防御力
	string icon;                     //植物图标
	int price;                       //植物的价格
	vector<PStateInfo>* container;  //资源容器
	stPlantInfo(PLANT_TYPE _type, int _hpValue, int _attackSpeed,
		BULLET_TYPE _bulletType,int _defenceValue,string _icon,vector<PStateInfo>* _container,int _price)
	{
		type = _type;
		hpValue = _hpValue;
		attackSpeed = _attackSpeed;
		bulletType = _bulletType;
		defenceValue = _defenceValue;
		container = _container;
		icon = _icon;
		price = _price;
	}
}PlantInfo, *PPlantInfo;

//子弹的全部信息
typedef struct stBulletInfo{
	BULLET_TYPE type;                 //子弹类型
	int moveSpeed;                    //移动速度
	int attackValue;                  //攻击速度
	string src;                       //图片路径
	int num;                          //图片数量
	int survival;                     //生存时间，用于向日葵产阳光
	int width;                        //图片宽度
	int height;                       //图片高度
	stBulletInfo(BULLET_TYPE _type, int _moveSpeed, int _attackValue,
		string _src, int _num,int _survival,int _width,int _height)
	{
		type = _type;
		moveSpeed = _moveSpeed;
		attackValue = _attackValue;
		src = _src;
		num = _num;
		survival = _survival;
		width = _width;
		height = _height;
	}
}BulletInfo, *PBulletInfo;

class Context
{
public:
	static Context* GetContext();                     //唯一得到Context对象
	~Context();                                       //析构函数，主要释放内存
	bool Init();                                      //初始化
	static LPWSTR strToLPW(string str,int i);         //将字符串str和帧数转化为LPWSTR
	static LPWSTR strToLPW(string str);               //将字符串str转化为LPWSTR
	void DrawMenu(HDC hDc, int, int, int, int);       //绘制菜单
	PZombieInfo GetZombieInfo(ZOMBIE_TYPE a);         //得到指定类型的僵尸信息
	PPlantInfo GetPlantInfo(PLANT_TYPE);              //得到指定的植物信息
	PBulletInfo GetBulletInfo(BULLET_TYPE);           //得到指定的子弹信息
	void DrawMouseItem(HDC hDc, POINT p, PLANT_TYPE); //画出鼠标上的植物
private:
	static Context* context;
	Context();                           //私有构造函数
	vector<PZombieInfo>* zombieCon;      //僵尸信息列表
	vector<PPlantInfo>* plantCon;        //植物信息列表
	vector<PBulletInfo>* bulletCon;      //子弹信息列表
	bool InitZombie(TiXmlDocument*);     //初始化僵尸信息列表
	bool InitPlant(TiXmlDocument*);      //初始化植物信息列表
	bool InitBullet(TiXmlDocument*);     //初始化子弹信息列表
	bool InitOther(TiXmlDocument*);      //初始化其他资源
};
