#pragma once
class GRAZE {
private:
	//グラフィックハンドル
	int gh;
	//座標
	double x, y;
	//角度
	double rad;
	//アルファ値
	int alpha;
	//拡大率
	double rate;
	//カウント
	int count;
	//実行中かどうかのフラグ
	bool flag;

private:
	void Move();
	void Draw();
public:
	GRAZE();
	void SetFlag(double x, double y);
	bool GetFlag();
	void All();
};