#pragma once
#include "pch.h"

#ifndef _ENEMY
#define _ENEMY

class ENEMY {
private:
	//座標とグラフィックハンドル
	double x, y;
	int gh[3];
	//画像サイズ
	int width, height;
	//出現、　停止、　帰還、　発射タイミング
	int in_time, stop_time, out_time, shot_time;

	//敵の種類
	int type;
	//弾の種類
	int stype;
	//移動パターン
	int m_pattern;
	//ショットパターン
	int s_pattern;

	int hp;
	int item;

	int temp;

	//弾のサウンドフラグ
	bool s_shot;

	//敵が出現してからのカウント
	int count;

	//発射した弾数
	int num;
	//発射直後のラジアン
	double rad;

	//敵消滅フラグ
	int deadflag;
	//敵クラス消滅フラグ
	bool endflag;

	//弾構造体
	E_SHOT shot[ENEMY_SNUM];
	//ショットが打てるようになったかのフラグ
	bool sflag;
	//ショットが打てるようになってからのカウント
	int scount;

private:
	void Move();
	void Shot();
	void Draw();
	bool OutCheck();
	bool ShotOutCheck(int i);

public:
	bool All();
	void GetPosition(double *x, double *y);
	bool GetShotPosition(int index, double *x, double *y);
	void SetDeadFlag();
	void SetShotFlag(int index, bool flag);
	void SetGrazeFlag(int index);
	int GetShotType();
	int GetItem();
	bool GetDeadFlag();
	bool GetShotSound();
	bool GetGrazeFlag(int index);
	ENEMY(int type, int stype, int m_pattern, int s_pattern, int in_time, int stop_time, int shot_time, int out_time, int x, int y, int speed, int hp, int item);
};

#endif
