#include "stdafx.h"
#include "Data.h"
#define ZOMBIE_TOTAL 10

Data::Data()
{
	totalSun = 50;
	plantList = new list<PPlant>();
	zombieList = new list<PZombie>();
	bulletList = new list<PBullet>();
	sun = NULL;
	zombieTotal = ZOMBIE_TOTAL;
}

Data::~Data()
{
	for (auto it = plantList->begin(); it != plantList->end();it++)
	{
		if ((*it) != NULL)
			delete *it;
	}
	delete plantList;

	for (auto it = zombieList->begin(); it != zombieList->end();it++)
	{
		if ((*it) != NULL)
			delete *it;
	}
	delete zombieList;

	for (auto it = bulletList->begin(); it != bulletList->end();it++)
	{
		if ((*it) != NULL)
			delete *it;
	}
	delete bulletList;
}

Data* Data::data = NULL;

Data* Data::GetData()
{
	if (data == NULL)
	{
		data = new Data();
	}
	return data;
}
