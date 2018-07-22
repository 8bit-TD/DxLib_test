#pragma once
class ITEM {
private :
	//グラフィックハンドル
	int gh[2], ghs[2];
	//どっちのアイテムかを示す変数
	int type;

	//座標
	double x, y;
	double prev_y;

	//回転角度
	double rad;

	//カウント
	int count;

	//アイテムが落下し始めるフラグ
	bool fall_flag;

	//出現中かどうかのフラグ
	bool flag;

private:
	void Move();
	void Draw();
public:
	ITEM();
	void SetFlag(double x, double y, int type);
	bool GetFlag();
	void Delete();
	void GetPosition(double *x, double *y);
	int GetType();
	void All();
};