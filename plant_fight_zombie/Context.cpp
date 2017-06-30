#include "stdafx.h"
#include "Context.h"
#include "tinyxml.h"
using namespace std;
//构造函数：初始化数据仓库
Context::Context()
{
	zombieCon = new vector<PZombieInfo>();
	plantCon = new vector<PPlantInfo>();
	bulletCon = new vector<PBulletInfo>();
	Init();
}
//类外初始静态变量
Context* Context::context = NULL;

//这是得到数据仓库的唯一途径
Context* Context::GetContext()
{
	if (context == NULL)
	{
		context = new Context();
	}
	return context;
}

Context::~Context()
{
	//释放僵尸信息列表
	for (auto it = zombieCon->begin(); it != zombieCon->end();it++)
	{
		if (*it != NULL)
		{
			auto con = (*it)->container;
			for (auto STATEIt = con->begin(); STATEIt != con->end(); STATEIt++)
			{
				delete (*STATEIt);
			}
			delete con;
			delete (*it);
		}
	}
	delete zombieCon;

	//释放植物信息列表
	for (auto it = plantCon->begin(); it != plantCon->end();it++)
	{
		if (*it != NULL)
		{
			auto con = (*it)->container;
			for (auto STATEIt = con->begin(); STATEIt != con->end(); STATEIt++)
			{
				delete (*STATEIt);
			}
			delete con;
			delete (*it);
		}
	}
	delete plantCon;

	//释放子弹信息列表
	for (auto it = bulletCon->begin(); it != bulletCon->end();it++)
	{
		if (*it != NULL)
		{
			delete (*it);
		}
	}
	delete bulletCon;
}

bool Context::Init()
{
	TiXmlDocument* doc=new TiXmlDocument(FILE_PATH);
	if (!doc->LoadFile())
	{
		printf("不能打开文件：%s;错误为：%s", FILE_PATH, doc->ErrorDesc());
		exit(1);
	}
	
	InitZombie(doc);
	InitPlant(doc);
	InitBullet(doc);
	InitOther(doc);

	doc->Clear();
	delete doc;
}
//初始化僵尸列表
bool Context::InitZombie(TiXmlDocument* doc)
{
	TiXmlElement* root = doc->RootElement();
	TiXmlElement* zombies = root->FirstChildElement("zombies");
	for (TiXmlElement* zombie = zombies->FirstChildElement();
		zombie != 0; zombie = zombie->NextSiblingElement())
	{
		ZOMBIE_TYPE type = (ZOMBIE_TYPE)atoi(zombie->Attribute("type"));
		int hpValue = atoi(zombie->Attribute("hpValue"));
		int moveSpeed = atoi(zombie->Attribute("moveSpeed"));
		int attackValue = atoi(zombie->Attribute("attackValue"));
		int attackSpeed = atoi(zombie->Attribute("attackSpeed"));
		int defenceValue = atoi(zombie->Attribute("defenceValue"));
		vector<PStateInfo>* container = new vector<PStateInfo>();

		for (TiXmlElement* STATE = zombie->FirstChildElement();
			STATE != 0; STATE = STATE->NextSiblingElement())
		{
			STATE_TYPE type = (STATE_TYPE)atoi(STATE->Attribute("type"));
			int width = atoi(STATE->Attribute("width"));
			int height = atoi(STATE->Attribute("height"));
			string src = string(STATE->Attribute("src"));
			int num = atoi(STATE->Attribute("num"));
			PStateInfo ai = new StateInfo(type, width, height, num, src);
			container->push_back(ai);
		}
		ZombieInfo* zi = new ZombieInfo(type, hpValue, moveSpeed, attackValue, attackSpeed, defenceValue, container);
		zombieCon->push_back(zi);
	}
	return true;
}

bool Context::InitPlant(TiXmlDocument* doc)
{
	TiXmlElement* root = doc->RootElement();
	TiXmlElement* plants = root->FirstChildElement("plants");
	for (TiXmlElement* plant = plants->FirstChildElement();
		plant != 0; plant = plant->NextSiblingElement())
	{
		PLANT_TYPE type = (PLANT_TYPE)atoi(plant->Attribute("type"));
		int hpValue = atoi(plant->Attribute("hpValue"));
		int attackSpeed = atoi(plant->Attribute("attackSpeed"));
		int defenceValue = atoi(plant->Attribute("defenceValue"));
		string icon = string(plant->Attribute("icon"));
		BULLET_TYPE bulletType = (BULLET_TYPE)atoi(plant->Attribute("bulletType"));
		int price = atoi(plant->Attribute("price"));
		vector<PStateInfo>* container = new vector<PStateInfo>();

		for (TiXmlElement* STATE = plant->FirstChildElement();
			STATE != 0; STATE = STATE->NextSiblingElement())
		{
			STATE_TYPE type = (STATE_TYPE)atoi(STATE->Attribute("type"));
			int width = atoi(STATE->Attribute("width"));
			int height = atoi(STATE->Attribute("height"));
			string src = string(STATE->Attribute("src"));
			int num = atoi(STATE->Attribute("num"));
			PStateInfo ai = new StateInfo(type, width, height, num, src);
			container->push_back(ai);
		}
		PlantInfo* zi = new PlantInfo(type, hpValue, attackSpeed,bulletType,defenceValue,icon,container,price);
		plantCon->push_back(zi);
	}
	return true;
}

bool Context::InitBullet(TiXmlDocument* doc)
{
	TiXmlElement* root = doc->RootElement();
	TiXmlElement* bullets = root->FirstChildElement("bullets");
	for (TiXmlElement* bullet = bullets->FirstChildElement();
		bullet != 0; bullet = bullet->NextSiblingElement())
	{
		BULLET_TYPE type = (BULLET_TYPE)atoi(bullet->Attribute("type"));
		int moveSpeed = atoi(bullet->Attribute("moveSpeed"));
		int attackValue = atoi(bullet->Attribute("attackValue"));
		string src = string(bullet->Attribute("src"));
		int num = atoi(bullet->Attribute("num"));
		int survival = atoi(bullet->Attribute("survival"));
		int width = atoi(bullet->Attribute("width"));
		int height = atoi(bullet->Attribute("height"));
		BulletInfo* bulletInfo = new BulletInfo(type, moveSpeed, attackValue, src, num, survival,width,height);
		bulletCon->push_back(bulletInfo);
	}
	return true;
}

bool Context::InitOther(TiXmlDocument* doc)
{
	return true;
}

LPWSTR Context::strToLPW(string str)
{
	size_t size = str.length();
	wchar_t *buffer = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, buffer, size*sizeof(wchar_t));
	buffer[size] = '\0';
	return buffer;
}

LPWSTR Context::strToLPW(string str,int i)
{
	str = str + to_string(i)+string(".bmp");
	size_t size = str.length();
	wchar_t *buffer = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, buffer, size*sizeof(wchar_t));
	buffer[size] = '\0';
	return buffer;
}

void Context::DrawMenu(HDC hDc, int x, int y,int width,int height)
{
	HDC dcMem;
	dcMem = CreateCompatibleDC(hDc);

	for (auto it = plantCon->begin(); it != plantCon->end(); it++)
	{
		LPWSTR src = Context::strToLPW((*it)->icon);
		HBITMAP bitmap = (HBITMAP)LoadImage(NULL, src, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
		SelectObject(dcMem, bitmap);

		BitBlt(hDc, x, y, width, height, dcMem, 0, 0, SRCCOPY);
		x += width+20;
		DeleteObject(bitmap);
	}
	DeleteObject(dcMem);
}

PZombieInfo Context::GetZombieInfo(ZOMBIE_TYPE type)
{
	for (auto it = zombieCon->begin(); it != zombieCon->end(); it++)
	{
		if ((*it)->type == type)
			return (*it);
	}
	return NULL;
}

PPlantInfo Context::GetPlantInfo(PLANT_TYPE type)
{
	for (auto it = plantCon->begin(); it != plantCon->end(); it++)
	{
		if ((*it)->type == type)
			return (*it);
	}
	return NULL;
}

PBulletInfo Context::GetBulletInfo(BULLET_TYPE type)
{
	for (auto it = bulletCon->begin(); it != bulletCon->end(); it++)
	{
		if ((*it)->type == type)
			return (*it);
	}
	return NULL;
}

void Context::DrawMouseItem(HDC hDc, POINT p, PLANT_TYPE type)
{
	LPWSTR src;
	for (auto beg = plantCon->begin(); beg != plantCon->end(); beg++)
	{
		if ((*beg)->type == type)
		{
			auto con = (*beg)->container;
			for (auto it = con->begin(); it != con->end(); it++)
			{
				if ((*it)->type == STATE_STAND)
				{
					src = Context::strToLPW((*it)->src, 0);
					HDC dcMem = CreateCompatibleDC(hDc);
					int width = (*it)->width;
					int height = (*it)->height;
					HBITMAP bitmap = (HBITMAP)LoadImage(NULL, src, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
					SelectObject(dcMem, bitmap);
					TransparentBlt(hDc, p.x, p.y, width, height, dcMem, 0, 0, width, height, RGB(255, 255, 255));

					delete src;
					DeleteObject(bitmap);
					DeleteObject(dcMem);
					return;
				}
			}
		}
	}
}