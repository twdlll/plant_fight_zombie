#include "stdafx.h"
#include "Plant.h"
#include "Bullet.h"

POINT& Plant::GetLocation()
{
	return p;
}

Plant::Plant(POINT _p,PPlantInfo _pi)
{
	p = _p;
	plantInfo = _pi;
	currentIndex = 0;
	currentType = STATE_STAND;
	currentHp = _pi->hpValue;
	lastType = STATE_STAND;
	lastAction = GetTickCount();
}

PStateInfo Plant::GetStateInfo()
{
	auto con = plantInfo->container;
	for (auto beg = con->begin(); beg != con->end(); beg++)
	{
		if ((*beg)->type = currentType)
			return (*beg);
	}
	return NULL;
}

void Plant::draw(HDC hDc)
{
	auto info = GetStateInfo();
	int width = info->width;
	int height = info->height;
	int num = info->num;
	if (currentType != lastType)
	{
		lastType = currentType;
		currentIndex = 0;
	}
	auto src = Context::strToLPW(info->src, currentIndex);

	currentIndex = (currentIndex + 1) % num;
	HDC dcMem = CreateCompatibleDC(hDc);
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, src, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(dcMem, bitmap);

	TransparentBlt(hDc, p.x, p.y, width, height, dcMem, 0, 0, width, height, RGB(255, 255, 255));

	delete src;
	DeleteObject(dcMem);
	DeleteObject(bitmap);
}

//受到攻击，a为攻击力
bool Plant::BeAttacked(int a)
{
	currentHp -= a;
	if (currentHp <= 0)
		return false;
	return true;
}

PPlantInfo Plant::GetPlantInfo()
{
	return plantInfo;
}

//植物发射子弹；若不发射则返回NULL
PBullet Plant::Shoot()
{
	POINT tp;
	unsigned currentTime = GetTickCount();
	PBullet bullet=NULL;
	PBulletInfo info = info = Context::GetContext()->GetBulletInfo(plantInfo->bulletType);
	switch (plantInfo->type)
	{
	case PLANT_WALLNUT:
		break;
	case PLANT_SUNNER:
	case PLANT_PEASHOOTER:
	case PLANT_REPEATER:
		tp.x = p.x + 35;
		tp.y = p.y;
		if (currentTime >= lastAction + plantInfo->attackSpeed)
		{
			if (plantInfo->type == PLANT_SUNNER)
				lastAction = currentTime + info->survival;
			else
				lastAction = currentTime;
			bullet = new Bullet(tp, info);
		}
		break;
	}
	return bullet;
}


