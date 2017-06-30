#include "stdafx.h"
#include "Sun.h"
#include "Context.h"
#define MOVE_SPEED 5

Sun::Sun()
{
	num = 22;
	src = "picture\\Bullet\\Sun\\";
	currentIndex = 0;
	width = 78;
	height = 78;
	srand((unsigned)(time(NULL)));
	moveTime = rand() % 120;
	moveTime += 30;
	p.x = rand() % 1000 + 150;
	p.y = 0;
}


Sun::~Sun()
{
}

POINT Sun::GetLocation()
{
	return p;
}

void Sun::Move()
{
	if (moveTime-->0)
		p.y += MOVE_SPEED;
}

void Sun::Draw(HDC hDc)
{
	auto image = Context::strToLPW(src, currentIndex);
	currentIndex = (currentIndex + 1) % num;

	HDC dcMem = CreateCompatibleDC(hDc);
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, image, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(dcMem, bitmap);

	TransparentBlt(hDc, p.x, p.y, width, height, dcMem, 0, 0, width, height, RGB(255, 255, 255));

	delete image;
	DeleteObject(dcMem);
	DeleteObject(bitmap);
}