#pragma once
#include "pch.h"

class EFFECT_PDEAD {
private:
	//座標
	double x, y;
	//画像サイズ
	int width[3];
	int height[3];
	//グラフィックハンドル
	int gh[3];
	//拡大率
	double rate;
	//透明度
	int alpha;
	//パーティクルエフェクト構造体
	PEFFECT_EXPAND expand[EFFECT_PDEADNUM];
	//回転させる画像用の角度
	double rota;
	//カウント
	int count;
	//実行中かどうかのフラグ
	bool flag;

private:
	void Move();
	void Draw();
public:
	EFFECT_PDEAD();
	void SetFlag(double x, double y);
	bool GetFlag();
	void All();
};