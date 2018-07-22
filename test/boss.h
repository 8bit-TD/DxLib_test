#pragma once
#include "define.h"

class BOSS {
	//座標
	double x, y, prev_x, prev_y;
	//グラフィックハンドル
	int gh_face[2];
	int gh_shot[3];
	//現在の移動パターン
	int move_pattern;
	//現在のショットパターン
	int shot_pattern;
	//弾構造体
	E_SHOT shot[BOSS_SHOTNUM];
	//合計角度と増加量
	int angle, raise;
	//横移動時の増加量
	int raise2;
	int waitcount;
	bool wait;
	//次の移動場所までの距離
	double movex, movey;
	int p3_state;
	//ダメージを負ったかどうかのフラグ
	bool damageflag;
	//生きているかどうかのフラグ
	bool flag;
	//ショットフラグ
	bool shotflag;
	//ショットカウント
	int scount;
	//ショットサウンドフラグ
	bool s_shot;
	int temp_scount;

	int count;

private:
	void Move();
	void Appear();
	void MovePattern1();
	void MovePattern2();
	void MovePattern3();
	int ShotSerch();
	bool ShotOutCheck(int i);
	void MoveInit(double bx, double by, int state);
	
	void Shot();
	void Draw();
public:
	BOSS();
	void SetDamageFlag();
	void SetFlag(bool f);
	bool GetFlag();
	bool GetShotSound();
	bool GetShotPosition(int index, double *x, double *y, int *type);
	bool GetGrazeFlag(int index);
	void SetGrazeFlag(int index);
	void All();
};
