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
	//���ϵ��µڼ���
	int index = rand() % 3;
	//��ʬ���꣺��������
	p.x = 1280;
	p.y = 100 + 160 * index;
	currentHp = zombieInfo->hpValue;
	//��ǰ״̬����Ϊ���ƶ�״̬
	currentType = STATE_MOVE;
	currentIndex = 0;
	lastType = STATE_MOVE;
	//�������Ľ�ʬ��Ϣ��������
	lastAttack = 0;
}


Zombie::~Zombie()
{
}

PZombieInfo Zombie::GetInfo()
{
	return zombieInfo;
}

//��ý�ʬ����
POINT& Zombie::GetLocation()
{
	return p;
}

//��������������ʬ��������false�����ŷ���true
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

//ͨ��flag�ж���ʬ�Ƿ񹥻�
//�������򷵻ع����������򷵻�0
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

//��ʬ�ƶ������ƶ�����ʱ�������������ʱ������false
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
	
	//���ڲ�ͬ״̬����Ȼ֡���������ǲ�ͬ��
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