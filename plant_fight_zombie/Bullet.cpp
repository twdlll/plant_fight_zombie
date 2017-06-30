#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet(POINT _p,PBulletInfo info)
{
	p = _p;
	bulletInfo = info;
	currentIndex = 0;
	lastAction = GetTickCount();
}


Bullet::~Bullet()
{
}

bool Bullet::Move()
{
	if (bulletInfo->type == BULLET_SUN)
	{
		if (GetTickCount() >= lastAction + bulletInfo->survival)
			return false;
		return true;
	}
	p.x+=bulletInfo->moveSpeed;
	if (p.x >= 1280 + 10)
		return false;
	return true;
}

void Bullet::Draw(HDC hDc)
{
	HDC dcMem = CreateCompatibleDC(hDc);
	auto str = bulletInfo->src;
	LPWSTR src;
	if (bulletInfo->type == BULLET_SUN)
	{
		src = Context::strToLPW(str, currentIndex);
		currentIndex = (currentIndex + 1) % bulletInfo->num;
	}
	else
	{
		src = Context::strToLPW(str);
	}
	int width = bulletInfo->width;
	int height = bulletInfo->height;
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, src, IMAGE_BITMAP, width,height, LR_LOADFROMFILE);
	SelectObject(dcMem, bitmap);

	TransparentBlt(hDc, p.x, p.y, width, height, dcMem, 0, 0, width, height, RGB(255, 255, 255));
	delete src;
	DeleteObject(bitmap);
	DeleteObject(dcMem);
}

BULLET_TYPE Bullet::GetBulletType()
{
	return bulletInfo->type;
}

POINT& Bullet::GetLocation()
{
	return p;
}