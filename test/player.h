#pragma once

#include "effect_pdead.h"
#include "ball.h"

class PLAYER {
private:
	//x座標,y座標
	double x, y;

	//画像幅
	int width, height;

	//グラフィックハンドル格納用配列
	int gh[12];

	//移動係数
	float move;

	//横方向と縦方向のカウント数。
	int xcount, ycount;
	//添字用変数
	int ix, iy, result;

	//プレイヤーのライフ
	int life;
	bool damageflag;
	bool endflag;

	int power;

	//ダメージ中のカウント
	int dcount;

	//弾
	SHOT shot[PSHOT_NUM];

	//カウント
	int count;

	//サウンド関連フラグ
	//ショット音
	bool s_shot;

	EFFECT_PDEAD *effect_pdead;

	//ボールクラス
	BALL ball;

private:
	void Move();
	void Draw();
	void Shot();
	void Ball();
	void BallShotSet(int index);
	int NearEnemySerch();

public:
	PLAYER();
	bool GetShotSound();
	bool GetShotPosition(int index, double *x, double *y);
	void SetShotFlag(int index, bool flag);
	void GetPosition(double *x, double *y);
	void SetDamageFlag();
	bool GetDamageFlag();
	int GetLife();
	void SetPower(int p);
	int GetPower();
	void All();
};
