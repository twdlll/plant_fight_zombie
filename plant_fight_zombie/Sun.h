#pragma once
using namespace std;
class Sun
{
public:
	Sun();
	~Sun();
	void Move();                  //�ƶ�
	POINT GetLocation();          //�õ�����
	void Draw(HDC hDc);           //��ͼ
private:
	POINT p;                      //����
	int moveTime;                 //�ƶ�����
	int currentIndex;             //��ǰ֡
	int num;                      //ͼƬ����
	string src;                   //ͼƬ��Դ
	int width;                    //ͼƬ���
	int height;                   //ͼƬ�߶�
};

