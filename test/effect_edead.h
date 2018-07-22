#pragma once
class EFFECT_EDEAD {
private:
	//座標
	double x, y;
	//グラフィックハンドル
	int gh[3];
	//エフェクト画像の角度
	double rad;
	//拡大率
	double rate;
	//透明度
	int alpha;
	//どの画像を使うかの添字
	int index;
	//カウント
	int count;
	//実行中かどうかのフラグ
	bool flag;
private:
	void Move();
	void Draw();
public:
	EFFECT_EDEAD();
	bool GetFlag();
	void SetFlag(double x, double y);
	void All();
};