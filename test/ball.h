#pragma once
#ifndef _BALL
#define _BALL

class BALL {
private:
	//座標
	double x, y;
	//グラフィックハンドル
	int gh;
	//一時フラグ
	bool toggle;
	//sin波に使う角度の増加量
	int raise;
	//角度
	double angle;

private:
	void Move(double px, double py);
	void Draw();
public:
	BALL();
	void All(double px, double py);
	double GetPosition();
};

#endif