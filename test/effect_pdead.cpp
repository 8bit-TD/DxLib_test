#include "effect_pdead.h"
#include "pch.h"

EFFECT_PDEAD::EFFECT_PDEAD() {
	gh[0] = LoadGraph("playereffect1.png");
	gh[1] = LoadGraph("playereffect2.png");
	gh[2] = LoadGraph("playereffect3.png");
	for (int i = 0; i < 3; ++i) {
		GetGraphSize(gh[i], &width[i], &height[i]);
	}
	rate = 1;
	alpha = 255;
	count = 0;
	flag = false;
	srand((unsigned)time(NULL));
}

void EFFECT_PDEAD::SetFlag(double x, double y) {
	count = 0;
	flag = true;
	this->x = x;
	this->y = y;
}

bool EFFECT_PDEAD::GetFlag() {
	return flag;
}

void EFFECT_PDEAD::Move() {
	//初回だけ角度を保存
	if (count == 0) {
		for (int i = 0; i < EFFECT_PDEADNUM; ++i) {
			expand[i].rad = rand() % 628 / 100;
			expand[i].speed = rand() % 10;
			expand[i].x = x;
			expand[i].y = y;
		}
	}
	rate = 0.5 + (count*0.05);
	alpha = 255 - (255 / 40)*count;
	rota = 0.05*count;
	//座標の移動
	for (int i = 0; i < EFFECT_PDEADNUM; ++i) {
		expand[i].x += cos(expand[i].rad)*expand[i].speed;
		expand[i].y += sin(expand[i].rad)*expand[i].speed;
	}
	++count;
	if (count == 40) {
		flag = false;
		count = 0;
	}
}

void EFFECT_PDEAD::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//円形のエフェクト描画
	DrawRotaGraph(x, y, rate, 0, gh[0], TRUE);
	//円盤のエフェクト描画(2つ)
	DrawRotaGraph(x, y, 1.0, rota, gh[1], TRUE);
	DrawRotaGraph(x, y, 1.0, 6.28 - rota, gh[1], TRUE);
	//パーティクルエフェクト描画
	for (int i = 0; i < EFFECT_PDEADNUM; ++i) {
		DrawGraph(expand[i].x - width[2] / 2, expand[i].y - height[2] / 2, gh[2], TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EFFECT_PDEAD::All() {
	//フラグ立ってる時だけ実行
	if (flag) {
		Move();
		Draw();
	}
}