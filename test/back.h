#pragma once
class BACK {
	//座標
	double x, y;
	//グラフィックハンドル
	int gh;

private:
	void Draw();
public:
	void All();
	void Move();
	BACK();
};