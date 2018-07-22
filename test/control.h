#include "player.h"
#include "back.h"
#include "enemy.h"
#include "EFFECT_EDEAD.h"
#include "graze.h"
#include "score.h"
#include "item.h"
#include "boss.h"

class CONTROL {

	//プレイヤークラス
	PLAYER *player;

	//背景クラス
	BACK *back;

	//敵クラス
	ENEMY *enemy[ENEMY_NUM];

	BOSS *boss;

	//敵消滅エフェクトクラス
	EFFECT_EDEAD *effect_edead[EFFECT_EDEADNUM];

	//グレイズクラス
	GRAZE *graze[GRAZE_NUM];

	//スコアクラス
	SCORE *score;

	//アイテムクラス
	ITEM *item[ITEM_NUM];

	//サウンドハンドル
	int s_eshot;
	int s_pshot;
	int s_edead;
	int s_graze;
	int s_item;

	//サウンドを鳴らすかどうかのフラグ
	bool eshot_flag;
	bool pshot_flag;
	bool graze_flag;
	bool item_flag;

	//敵消滅音フラグセット
	bool edead_flag;
	//プレイヤー消滅音フラグをセット
	bool pdead_flag;

private:
	CONTROL();
	~CONTROL();
	void SoundAll();
	void CollisionAll();
	void BossCollisionAll();
	bool CircleCollision(double c1, double c2, double cx1, double cx2, double cy1, double cy2);
	void EnemyDeadEffect(double x, double y);
public:
	void All();
	void GetPlayerPosition(double *x, double *y);
	bool GetEnemyPosition(int index, double *x, double *y);
	
	static CONTROL& GetInstance() {
		static CONTROL control;
		return control;
	}
};
