#pragma once
#include <windows.h>

#define MARGIN 10

//プレイヤーの歩くスピード
#define PLAYER_SPEED 4
#define PLAYER_INITX 180
#define PLAYER_INITY 400
#define PLAYER_LIFE 5

//背景のスクロールスピード
#define SCROLL_SPEED 2

//メッセージボックス
#define MSG(m) {\
	MessageBox(NULL,m,"メッセージ",MB_OK);}

//extern宣言してkey配列にどこからでもアクセスできるようにする
extern char key[256];

//extern宣言してカウント数にどこからでもアクセスできるようにする
extern int g_count;

//弾の最大数
#define PSHOT_NUM  50

//弾のスピード
#define PSHOT_SPEED 14

struct SHOT {
	bool flag;//弾が発射中かどうか
	double x;//x座標
	double y;//y座標
	int gh;//グラフィックハンドル
	int width, height;//画像の幅と高さ
	double rad;//角度
	int type;//弾の種類(0なら通常、1なら追跡弾)
};

struct E_SHOT {
	bool flag;//弾が発射中かどうか
	double x;//x座標
	double y;//y座標
	double rad;//角度(ラジアン)
	int gh;//グラフィックハンドル
	int width, height;//画像の幅と高さ
	int pattern;//ショットパターン
	int speed;//弾のスピード
	int gflag;//グレイズ判定用フラグ
	int type;//弾の種類
};

#define ENEMY_SNUM 50

#define BOSS_SHOTNUM 150

#define BOSS_SHAKE 20

struct ENEMYDATA {
	int type;//敵種類
	int stype;//弾種類
	int m_pattern;//移動パターン
	int s_pattern;//発射パターン
	int in_time;//出現時間
	int stop_time;//停止時間
	int shot_time;//弾発射時間
	int out_time;//帰還時間
	int x;//x座標
	int y;//y座標
	int speed;//弾スピード
	int hp;//HP
	int item;//アイテム
};

#define ENEMY_NUM 30
#define EFFECT_EDEADNUM 20
#define EFFECT_PDEADNUM 10

#define PI 3.1415

//当たり判定用半径定義
#define PLAYER_COLLISION 4
#define ENEMY1_COLLISION 14

#define PSHOT_COLLISION 3
#define ESHOT0_COLLISION 10
#define ESHOT1_COLLISION 3
#define ESHOT2_COLLISION 2
#define ESHOT3_COLLISION 2
#define GRAZE_COLLISION 10

#define GRAZE_NUM 200

struct PEFFECT_EXPAND {
	double x, y;
	double rad;
	int speed;
};

//スコアの文字描画の基準となるX座標
#define SCORE_X 390

//アイテムの当たり判定用半径
#define ITEM_COLLISION 16
//アイテムの総数
#define ITEM_NUM 30

#define BALL_SHAKE 15

#define BALL_INITX 50
#define BALL_INITY 30

#define BOSS_COLLISION 60

#define BOSS_HP 500

