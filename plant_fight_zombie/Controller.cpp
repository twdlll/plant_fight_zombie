#include "stdafx.h"
#include "Controller.h"
#include "Bullet.h"
#include "Context.h"
#include "Sun.h"
#include <windows.h>
#define ICON_WIDTH 50
#define ICON_HEIGHT 70
#define CREATE_ZOMBIE 10000
#define CREATE_ZOMBIE_MAX 20000

Controller::Controller()
{
	data = Data::GetData();
	context = Context::GetContext();
}

void Controller::GameOver()
{
	//������ж�ʱ��
	ClearTimer();
	MessageBox(m_hWnd, _T("���Ѿ�ʧ����"), _T("��Ϸ���"), MB_OK);
	//������Ϣ����
	PostQuitMessage(0);
	DestroyWindow(m_hWnd);
}

void Controller::Begin(HWND hWnd)
{
	m_hWnd = hWnd;
	AddTimer(TIMER_DRAW, 50);                        //���ƶ�ʱ��
	AddTimer(TIMER_CREATE_ZOMBIE, CREATE_ZOMBIE);    //������ʬ�Ķ�ʱ��
	AddTimer(TIMER_MOVE, 50);                        //�����ƶ��Ķ�ʱ��
	AddTimer(TIMER_SHOOT, 1000);                     //���Ʒ���
	AddTimer(TIMER_CREATE_SUN, 10000);               //������������
}

//���Ʊ���
void Controller::DrawBackground(HDC hDc)
{
	HDC dcMem;
	dcMem = CreateCompatibleDC(hDc);
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, _T("picture\\map.bmp"), IMAGE_BITMAP, 1280, 720, LR_LOADFROMFILE);
	SelectObject(dcMem, bitmap);

	BitBlt(hDc, 0, 0, 1280, 720, dcMem, 0, 0, SRCCOPY);
	
	DeleteObject(dcMem);
	DeleteObject(bitmap);
}

//�����Ϸ���ֲ��ͼ��
void Controller::DrawMenu(HDC hDc)
{
	context->DrawMenu(hDc, 210, 32, ICON_WIDTH, ICON_HEIGHT);
}

//������������
void Controller::DrawSunText(HDC hDc)
{
	//�����Զ�������
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = 30;
	logFont.lfWeight = FW_MEDIUM;
	HFONT hFont = CreateFontIndirect(&logFont);
	SelectObject(hDc, hFont);

	//�������ֱ���Ϊ͸��
	SetBkMode(hDc, TRANSPARENT);
	SetTextColor(hDc, RGB(228, 228, 0));
	TCHAR strText[100] = { 0 };
	wsprintf(strText, _T("���⣺%d"), data->totalSun);
	TextOut(hDc, 46, 80, strText, lstrlen(strText));
	DeleteObject(hFont);
}

//����ֲ��ͽ�ʬ
void Controller::DrawPlantAndZombie(HDC hDc)
{
	auto plantList = data->plantList;
	plantLock.lock();
	for (auto it = plantList->begin(); it != plantList->end(); it++)
		(*it)->draw(hDc);
	plantLock.unlock();

	auto zombieList = data->zombieList;
	for (auto it = zombieList->begin(); it != zombieList->end();)
	if (!(*it)->Draw(hDc))
	{
		it = zombieList->erase(it);
		continue;
	}
	else
		it++;
}

void Controller::DrawMouseItem(HDC hDc)
{
	if (data->mouseType == PLANT_NONE)
		return;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_hWnd, &p);
	context->DrawMouseItem(hDc, p, data->mouseType);
}

void Controller::DrawBullet(HDC hDc)
{
	auto bulletList = data->bulletList;
	bulletLock.lock();
	for (auto it = bulletList->begin(); it != bulletList->end(); it++)
	{
		(*it)->Draw(hDc);
	}
	bulletLock.unlock();
}

void Controller::Draw()
{
	//����豸������
	HDC hDc = GetWindowDC(m_hWnd);
	if (hDc == NULL)
	{
		return;
	}
	
	//�����ڴ��豸
	HDC dcMem = CreateCompatibleDC(NULL);
	HBITMAP hBitMap = CreateCompatibleBitmap(hDc, 1280, 720);
	SelectObject(dcMem, hBitMap);

	//�Ȼ��Ƶ�dcMem
	//��һ�������Ʊ���
	DrawBackground(dcMem);
	//�ڶ��������Ƶ�ͼ�Ϸ���ֲ��ͼ��
	DrawMenu(dcMem);
	//��������������������
	DrawSunText(dcMem);
	//���Ĳ�������ֲ��ͽ�ʬ
	DrawPlantAndZombie(dcMem);
	//���岽����������ϵ�ֲ��
	DrawMouseItem(dcMem);
	//�������������ӵ�
	DrawBullet(dcMem);

	if (data->sun != NULL)
	{
		sunLock.lock();
		if (data->sun!=NULL)
			data->sun->Draw(dcMem);
		sunLock.unlock();
	}
	
	BitBlt(hDc, 0, 0, 1280, 720, dcMem, 0, 0, SRCCOPY);
	DeleteObject(dcMem);
	DeleteObject(hBitMap);
	ReleaseDC(m_hWnd, hDc);
}


//�ƶ�������ֻ���ӵ��ͽ�ʬ���ƶ�
void Controller::Move()
{
	auto bulletList = data->bulletList;
	bulletLock.lock();
	for (auto it = bulletList->begin(); it != bulletList->end(); )
	{
		if (!(*it)->Move())
		{
			delete (*it);
			it = bulletList->erase(it);
			continue;
		}
		it++;
	}
	bulletLock.unlock();

	auto zombieList = data->zombieList;
	for (auto it = zombieList->begin(); it != zombieList->end(); )
	{
		if (!(*it)->Move())
		{
			GameOver();
			break;
		}
		it++;
	}

	if (data->sun != NULL)
	{
		sunLock.lock();
		if (data->sun!=NULL)
			data->sun->Move();
		sunLock.unlock();
	}
}

void Controller::CheckAttack()
{
	auto bulletList = data->bulletList;
	auto zombieList = data->zombieList;
	bulletLock.lock();
	for (auto itb = bulletList->begin(); itb != bulletList->end();)
	{
		bool hit = false;
		for (auto itz = zombieList->begin(); itz != zombieList->end(); itz++)
		{
			POINT bp = (*itb)->GetLocation();
			POINT zp = (*itz)->GetLocation();
			//����ʵ�ʲ����õ�����ֵ
			if (bp.x + 49 >= zp.x + 80 && bp.x + 49 < zp.x+141&&bp.y >= zp.y + 27 && bp.y <= zp.y + 143)
			{
				PBulletInfo bulletInfo = context->GetBulletInfo((*itb)->GetBulletType());
				(*itz)->BeAttacked(bulletInfo);
				hit = true;
				break;
			}
		}
		if (hit)
		{
			delete (*itb);
			itb = bulletList->erase(itb);
		}
		else
			itb++;
	}
	bulletLock.unlock();
}

void Controller::CheckAttacked()
{
	auto plantList = data->plantList;
	auto zombieList = data->zombieList;
	bool flag;
	int value ;
	for (auto zombie = zombieList->begin(); zombie != zombieList->end();zombie++)
	{
		flag = false;
		value = 0;
		POINT pz = (*zombie)->GetLocation();
		plantLock.lock();
		for (auto plant = plantList->begin(); plant != plantList->end(); plant++)
		{
			POINT pp = (*plant)->GetLocation();
			if (pp.x + 72 >= pz.x + 78 && pp.x + 72 < pz.x + 138
				&& pp.y >= pz.y&&pp.y < pz.y + 142)
			{
				//�õ���ʬ�Ĺ�����
				value = (*zombie)->Attack(true);
				if (!(*plant)->BeAttacked(value))
				{
					delete (*plant);
					plantList->erase(plant);
					flag = true;
					break;
				}
			}
		}
		plantLock.unlock();
	}
}

void Controller::GameSuccess()
{
	//������ж�ʱ��
	ClearTimer();
	MessageBox(m_hWnd, _T("���Ѿ��ɹ���"), _T("��Ϸ���"), MB_OK);
	//������Ϣ����
	PostQuitMessage(0);
	DestroyWindow(m_hWnd);
}

void Controller::CreateZombie()
{
	while (true)
	{
		unsigned add = rand() % 10;
		add *= 1000;
		unsigned newTime = add + CREATE_ZOMBIE;
		if (newTime < CREATE_ZOMBIE_MAX)
		{
			ChangElapce(TIMER_CREATE_ZOMBIE,newTime);
			break;
		}
	}
	PZombie pZombie = new Zombie();
	data->zombieList->push_back(pZombie);
	data->zombieTotal--;

	if (data->zombieTotal <= 0)
		GameSuccess();
}

void Controller::CheckState()
{
	auto zombieList = data->zombieList;
	auto plantList = data->plantList;
	for (auto zombie = zombieList->begin(); zombie != zombieList->end(); zombie++)
	{
		if ((*zombie)->GetState() == STATE_ATTACK)
		{
			POINT pz = (*zombie)->GetLocation();
			bool flag = false;
			plantLock.lock();
			for (auto plant = plantList->begin(); plant != plantList->end(); plant++)
			{
				POINT pp = (*plant)->GetLocation();
				if (pp.x + 72 >= pz.x&& pp.x + 72 < pz.x + 138
					&& pp.y >= pz.y&&pp.y < pz.y + 142)
				{
					flag = true;
					break;
				}
			}
			plantLock.unlock();
			if (!flag)
				(*zombie)->Attack(false);
		}
	}
}

void Controller::Shoot()
{
	auto plantList =data->plantList;
	plantLock.lock();
	for (auto plant = plantList->begin(); plant != plantList->end(); plant++)
	{
		PBullet bullet = (*plant)->Shoot();
		if (bullet)
		{
			bulletLock.lock();
			data->bulletList->push_back(bullet);
			bulletLock.unlock();
		}
	}
	plantLock.unlock();
}

//����������������ֲ������
PLANT_TYPE Controller::GetInRect()
{
	PLANT_TYPE type=PLANT_NONE;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	//�ж��Ƿ���ֲ��ͼ��������
	if (pt.x < 220 || pt.y<32 || pt.y>32 + 70)
		return PLANT_NONE;
	//�ж϶�Ӧ��ֲ������
	if (pt.x >= 220 && pt.x < 220 + 50)
	{
		type=PLANT_SUNNER;
	}
	else if (pt.x > 290 && pt.x < 290+50)
	{
		type=PLANT_PEASHOOTER;
	}
	else if (pt.x>360 && pt.x < 360 + 50)
	{
		type = PLANT_WALLNUT;
	}
	else if (pt.x>430 && pt.x < 430 + 50)
	{
		type = PLANT_REPEATER;
	}
	//���������������Ƿ�����
	if (type != PLANT_NONE)
	{
		int price = context->GetPlantInfo(type)->price;
		if (data->totalSun >= price)
			return type;
	}
	return PLANT_NONE;
}


bool Controller::CanPut(POINT&temp)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	//x,y��ȥ���ڻ����ڵĲ���
	pt.x -= 82;
	pt.y -= 130;
	if (pt.x<0 || pt.y<0 || pt.x>111 * 10 || pt.y>160 * 3)
	{
		return false;
	}

	int row = pt.y / 160;            //�ڼ��У���ʵ������1
	int col = pt.x / 111;            //�ڼ��У���ʵ������1
	temp.x = col * 111 + 82 + 20;    //Ҫ���Ƶ�x����
	temp.y = row * 160 + 130 + 40;   //Ҫ���Ƶ�y����,ֲ�ﲻ�ܹ��ڱ�Ե���ã�����+40
	auto plantList = data->plantList;
	plantLock.lock();
	for (auto it = plantList->begin(); it != plantList->end(); it++)
	{
		POINT p = (*it)->GetLocation();
		if (p.x == temp.x && p.y == temp.y)
		{
			plantLock.lock();
			return false;
		}
	}
	plantLock.unlock();
	return true;
}

bool Controller::ObtainSun()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_hWnd, &p);
	auto bulletList = data->bulletList;
	bulletLock.lock();
	for (auto it = bulletList->begin(); it != bulletList->end(); it++)
	{
		POINT pt = (*it)->GetLocation();
		BULLET_TYPE type = (*it)->GetBulletType();
		if (type != BULLET_SUN)
		{
			continue;
		}
		BulletInfo* info = context->GetBulletInfo(type);
		int width = info->width;
		int height = info->height;
		if (p.x > pt.x&&p.x<pt.x + width&&p.y>pt.y&&p.y < pt.y + height)
		{
			data->totalSun += 50;
			delete (*it);
			bulletList->erase(it);
			bulletLock.unlock();
			return true;
		}
	}
	bulletLock.unlock();

	if (data->sun != NULL)
	{
		POINT pt = data->sun->GetLocation();
		if (p.x > pt.x&&p.x<pt.x + 78 && p.y>pt.y&&p.y < pt.y + 78)
		{
			data->totalSun += 50;
			sunLock.lock();
			delete data->sun;
			data->sun = NULL;
			sunLock.unlock();
			return true;
		}
	}


	return false;
}

void Controller::LButtonUp()
{
	if (data->mouseType == PLANT_NONE)
	{
		data->mouseType = GetInRect();
		if (data->mouseType == PLANT_NONE)
			ObtainSun();
		return;
	}

	POINT p;
	if (CanPut(p))
	{
		PPlant pPlant = new Plant(p, context->GetPlantInfo(data->mouseType));
		plantLock.lock();
		data->plantList->push_back(pPlant);
		plantLock.unlock();
		int price = context->GetPlantInfo(data->mouseType)->price;
		data->mouseType = PLANT_NONE;
		data->totalSun -= price;
	}
}

void Controller::RButtonUp()
{
	data->mouseType = PLANT_NONE;
}

void Controller::CreateSun()
{
	if (data->sun == NULL)
	{
		sunLock.lock();
		data->sun = new Sun();
		sunLock.unlock();
	}
}

int Controller::OnTimer(int id, int iParam, string str)
{
	switch (id)
	{
	case TIMER_DRAW:
		Draw();
		break;
	case TIMER_CREATE_ZOMBIE:
		CreateZombie();
		break;
	case TIMER_MOVE:
		Move();
		CheckAttack();
		CheckAttacked();
		CheckState();
		break;
	case TIMER_SHOOT:
		Shoot();
		break;
	case TIMER_CREATE_SUN:
		CreateSun();
		break;
	}
	return 1;
}