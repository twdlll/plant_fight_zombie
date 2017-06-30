#include "stdafx.h"
#include "Plant.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Context.h"
#include "Sun.h"
using namespace std;

class Data{
private:
	static Data * data;
	Data();
	//它的唯一工作是在析构函数中删除Data的实例
	class Attend{
	public:
		~Attend(){
			if (Data::data != NULL)
				delete data;
		}
	};
	static Attend attent;     //用来析构掉单例
public:
	~Data();                            //析构函数
	static Data* GetData();             //得到单例
	list<PPlant>* plantList;            //当前植物列表
	list<PZombie>* zombieList;          //当前僵尸列表
	list<PBullet>* bulletList;          //当前子弹列表
	int totalSun;                       //当前阳光数
	PLANT_TYPE mouseType;               //当前鼠标状态
	int zombieTotal;                    //僵尸总数
	Sun* sun;                           //出现在屏幕的自由阳光
};