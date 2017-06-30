#pragma once
using namespace std;
class Sun
{
public:
	Sun();
	~Sun();
	void Move();                  //移动
	POINT GetLocation();          //得到坐标
	void Draw(HDC hDc);           //绘图
private:
	POINT p;                      //坐标
	int moveTime;                 //移动次数
	int currentIndex;             //当前帧
	int num;                      //图片数量
	string src;                   //图片资源
	int width;                    //图片宽度
	int height;                   //图片高度
};

