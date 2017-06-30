#pragma once
#include "stdafx.h"
//���tinyxml.h��ʹ��XML������
#include "tinyxml.h"
#include <vector>
#define FILE_PATH  "data.xml"            //xml�ļ���λ��
#define BULLET_NOT_SUN  -1
using namespace std;

//���ڱ�ʶ�����ں���״̬��״̬�������������ж���
enum STATE_TYPE{
	STATE_NONE,                      //�������κ���ȷ��״̬
	STATE_MOVE,                      //�ƶ�״̬
	STATE_STAND,                     //վ��״̬
	STATE_DIE,                       //����װ��
	STATE_ATTACK,                    //����״̬
	STATE_BEATTACK                   //������״̬
};

//��ʬ������
enum ZOMBIE_TYPE{
	ZOMBIE_NONE,                      //���ǽ�ʬ
	ZOMBIE_NORMAL,                    //��ͨ��ʬ
	ZOMBIE_FLAG,                      //���ӽ�ʬ
	ZOMBIE_CONEHEAD,                  //Բ׶ͷ��ʬ
	ZOMBIE_BUCKETHEAD                 //��Ͱ��ʬ
};

//ֲ�������
enum PLANT_TYPE{
	PLANT_NONE,                       //����ֲ��
	PLANT_SUNNER,                     //���տ�
	PLANT_PEASHOOTER,                 //�㶹����
	PLANT_WALLNUT,                    //���ǽ
	PLANT_REPEATER                    //��������
};

//�ӵ�������
enum BULLET_TYPE{
	BULLET_NONE,                      //�����ӵ�
	BULLET_SUN,                       //���⣬����Ϊ���ӵ���һ��
	BULLET_NORMAL,                    //��ͨ�ӵ�
	BULLET_TWO                        //˫���ӵ�
};
//���涯����Դ��һЩ��Ҫ��Ϣ
typedef struct stActionInfo{
	STATE_TYPE type;                 //״̬
	int width;                       //ͼƬ���
	int height;                      //ͼƬ�߶�
	int num;                         //����֡��
	string src;                      //ͼƬ��Դ·��
	stActionInfo(STATE_TYPE _type,int _width,int _height,int _num,string _src)
	{
		type = _type;
		width = _width;
		height = _height;
		num = _num;
		src =_src;
	}
}StateInfo,*PStateInfo;

//��ʬ��ȫ����Ϣ
typedef struct stZombieInfo{
	ZOMBIE_TYPE type;                //��ʬ����
	int hpValue;                     //����ֵ
	int moveSpeed;                   //�ƶ��ٶ�
	int attackValue;                 //������
	int attackSpeed;                 //�����ٶ�
	int defenceValue;                //������
	vector<PStateInfo>* container;  //��Դ����
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

//ֲ���ȫ����Ϣ
typedef struct stPlantInfo{
	PLANT_TYPE type;                 //ֲ������
	int hpValue;                     //����ֵ
	int attackSpeed;                 //�����ٶ�
	BULLET_TYPE bulletType;          //�ӵ����
	int defenceValue;                //ֲ��ķ�����
	string icon;                     //ֲ��ͼ��
	int price;                       //ֲ��ļ۸�
	vector<PStateInfo>* container;  //��Դ����
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

//�ӵ���ȫ����Ϣ
typedef struct stBulletInfo{
	BULLET_TYPE type;                 //�ӵ�����
	int moveSpeed;                    //�ƶ��ٶ�
	int attackValue;                  //�����ٶ�
	string src;                       //ͼƬ·��
	int num;                          //ͼƬ����
	int survival;                     //����ʱ�䣬�������տ�������
	int width;                        //ͼƬ���
	int height;                       //ͼƬ�߶�
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
	static Context* GetContext();                     //Ψһ�õ�Context����
	~Context();                                       //������������Ҫ�ͷ��ڴ�
	bool Init();                                      //��ʼ��
	static LPWSTR strToLPW(string str,int i);         //���ַ���str��֡��ת��ΪLPWSTR
	static LPWSTR strToLPW(string str);               //���ַ���strת��ΪLPWSTR
	void DrawMenu(HDC hDc, int, int, int, int);       //���Ʋ˵�
	PZombieInfo GetZombieInfo(ZOMBIE_TYPE a);         //�õ�ָ�����͵Ľ�ʬ��Ϣ
	PPlantInfo GetPlantInfo(PLANT_TYPE);              //�õ�ָ����ֲ����Ϣ
	PBulletInfo GetBulletInfo(BULLET_TYPE);           //�õ�ָ�����ӵ���Ϣ
	void DrawMouseItem(HDC hDc, POINT p, PLANT_TYPE); //��������ϵ�ֲ��
private:
	static Context* context;
	Context();                           //˽�й��캯��
	vector<PZombieInfo>* zombieCon;      //��ʬ��Ϣ�б�
	vector<PPlantInfo>* plantCon;        //ֲ����Ϣ�б�
	vector<PBulletInfo>* bulletCon;      //�ӵ���Ϣ�б�
	bool InitZombie(TiXmlDocument*);     //��ʼ����ʬ��Ϣ�б�
	bool InitPlant(TiXmlDocument*);      //��ʼ��ֲ����Ϣ�б�
	bool InitBullet(TiXmlDocument*);     //��ʼ���ӵ���Ϣ�б�
	bool InitOther(TiXmlDocument*);      //��ʼ��������Դ
};
