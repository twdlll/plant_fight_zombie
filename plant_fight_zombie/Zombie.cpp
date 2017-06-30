#include "stdafx.h"
#include "Context.h"
#include "Zombie.h"

Zombie::Zombie()
{
	srand((unsigned)(time(NULL)));
	ZOMBIE_TYPE type = ZOMBIE_NONE;
	int select = (rand() % 10 + 1);
	if (select < 4)
	{
		type = ZOMBIE_NORMAL;
	}
	else if (select < 7)
	{
		type = ZOMBIE_FLAG;
	}
	else if (select <9)
	{
		type = ZOMBIE_CONEHEAD;
	}
	else
	{
		type = ZOMBIE_BUCKETHEAD;
	}
	zombieInfo = Context::GetContext()->GetZombieInfo(type);
	//从上到下第几行
	int index = rand() % 3;
	//僵尸坐标：从右往左
	p.x = 1280;
	p.y = 100 + 160 * index;
	currentHp = zombieInfo->hpValue;
	//当前状态设置为：移动状态
	currentType = STATE_MOVE;
	currentIndex = 0;
	lastType = STATE_MOVE;
	//与所给的僵尸信息链接起来
	lastAttack = 0;
}


Zombie::~Zombie()
{
}

PZombieInfo Zombie::GetInfo()
{
	return zombieInfo;
}

//获得僵尸坐标
POINT& Zombie::GetLocation()
{
	return p;
}

//被攻击函数，僵尸死亡返回false，活着返回true
bool Zombie::BeAttacked(PBulletInfo info)
{
	currentHp -= info->attackValue-zombieInfo->defenceValue;
	if (currentHp <= 0)
	{
		currentType = STATE_DIE;
		if (zombieInfo->type == ZOMBIE_CONEHEAD || zombieInfo->type == ZOMBIE_BUCKETHEAD)
		{
			currentType = lastType;
			zombieInfo = Context::GetContext()->GetZombieInfo(ZOMBIE_NORMAL);
			currentHp = zombieInfo->hpValue;
		}
		return false;
	}
	return true;
}

STATE_TYPE Zombie::GetState()
{
	return currentType;
}

//通过flag判定僵尸是否攻击
//若攻击则返回攻击力，否则返回0
int Zombie::Attack(bool flag)
{
	if (flag == false)
	{
		currentType = STATE_MOVE;
		return 0;
	}
	unsigned currentTime = GetTickCount();
	if (lastAttack + zombieInfo->attackSpeed<=currentTime)
	{
		currentType = STATE_ATTACK;
		lastAttack = currentTime;
		return zombieInfo->attackValue;
	}
	return 0;
}

//僵尸移动，当移动出界时或死亡动画完成时，返回false
bool Zombie::Move()
{
	switch (currentType)
	{
	case STATE_ATTACK:
		break;
	case STATE_MOVE:
		p.x -= zombieInfo->moveSpeed;
		break;
	case STATE_DIE:
		int num = GetStateInfo()->num;
		if (currentIndex == num)
			return false;
		break;
	}

	if (p.x+60 <= 0)
		return false;

	return true;
}

PStateInfo Zombie::GetStateInfo()
{
	auto con = zombieInfo->container;
	for (auto beg = con->begin(); beg != con->end(); beg++)
	{
		if ((*beg)->type == currentType)
			return (*beg);
	}
	return NULL;
}

bool Zombie::Draw(HDC hDc)
{
	bool flag = true;
	HDC dcMem = CreateCompatibleDC(hDc);
	PStateInfo info = GetStateInfo();
	if (info == NULL)
		return flag;
	int width = info->width;
	int height = info->height;
	int num = info->num;
	if (currentType != lastType)
	{
		lastType = currentType;
		currentIndex = 0;
	}
	auto src = Context::strToLPW(info->src, currentIndex);
	
	//对于不同状态，显然帧数的增加是不同的
	if (currentType == STATE_DIE)
	{
		currentIndex = currentIndex < num - 1 ? currentIndex + 1 : currentIndex;
		if (currentIndex == num - 1)
			flag = false;
	}
	else
	{
		currentIndex = (currentIndex + 1) % num;
	}
	
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, src, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(dcMem, bitmap);
	TransparentBlt(hDc, p.x, p.y, width, height, dcMem, 0, 0, width, height, RGB(255, 255, 255));

	delete src;
	DeleteObject(bitmap);
	DeleteObject(dcMem);
	return flag;
}