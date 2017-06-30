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
	//����Ψһ������������������ɾ��Data��ʵ��
	class Attend{
	public:
		~Attend(){
			if (Data::data != NULL)
				delete data;
		}
	};
	static Attend attent;     //��������������
public:
	~Data();                            //��������
	static Data* GetData();             //�õ�����
	list<PPlant>* plantList;            //��ǰֲ���б�
	list<PZombie>* zombieList;          //��ǰ��ʬ�б�
	list<PBullet>* bulletList;          //��ǰ�ӵ��б�
	int totalSun;                       //��ǰ������
	PLANT_TYPE mouseType;               //��ǰ���״̬
	int zombieTotal;                    //��ʬ����
	Sun* sun;                           //��������Ļ����������
};