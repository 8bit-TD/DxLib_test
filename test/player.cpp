#include "pch.h"
#include "player.h"
#include "effect_pdead.h"
#include "control.h"

PLAYER::PLAYER()
{
	//画像読み込み
	
	if(-1==LoadDivGraph("charall.png",12,3,4,49,66,gh)){
		MSG("エラー発生");
	}

	width = 49;
	height = 66;

	//移動係数
	move = 1.0f;

	//横方向と縦方向のカウント数。
	xcount=0, ycount=0;

	//添字用変数
	ix=0, iy=0, result=0;

	//初期位置
	x = 180;
	y = 400;
	
	//生きているかどうか
	damageflag = false;
	endflag = false;
	dcount = 0;
	power = 10;
	life = PLAYER_LIFE;

	//弾初期化
	memset(shot, 0, sizeof(shot));

	//弾画像読み込み
	int temp = LoadGraph("shot.png");
	int w, h;
	GetGraphSize(temp, &w, &h);

	//フラグを全部falseにしとく
	//グラフィックハンドルと画像のサイズを代入しとく
	for (int i = 0; i<PSHOT_NUM; ++i) {
		shot[i].flag = false;
		shot[i].gh = temp;
		shot[i].width = w;
		shot[i].height = h;
	}

	//エフェクトクラスのインスタンス生成
	effect_pdead = new EFFECT_PDEAD();

	count = 0;
}

void PLAYER::GetPosition(double *x, double *y) {
	*x = this->x;
	*y = this->y;
}

bool PLAYER::GetShotSound() {
	return s_shot;
}

void PLAYER::SetShotFlag(int index, bool flag) {
	shot[index].flag = flag;
}

bool PLAYER::GetShotPosition(int index, double *x, double *y) {
	if (shot[index].flag) {
		*x = shot[index].x;
		*y = shot[index].y;
		return true;
	}else {
		return false;
	}
}

void PLAYER::SetDamageFlag() {
	damageflag = true;
	//ライフを減らす
	--life;
	if (life < 0) { life = 0; }
	effect_pdead->SetFlag(x, y);
}

bool PLAYER::GetDamageFlag() {
	return damageflag;
}

int PLAYER::GetLife() {
	return life;
}

void PLAYER::SetPower(int p) {
	power += p;
	if (power > 10) {
		power = 10;
	}
}

int PLAYER::GetPower() {
	return power;
}

void PLAYER::Ball() {
	if (power == 10) {
		ball.All(x, y);
	}
}

int PLAYER::NearEnemySerch() {
	CONTROL &control = CONTROL::GetInstance();
	int nearindex = -1;
	double nearresult = 0;
	double ex, ey, tx, ty;
	for (int i = 0; i < ENEMY_NUM; ++i) {
		if (!control.GetEnemyPosition(i, &ex, &ey))
			continue;
		tx = ex - x;
		ty = ey - y;
		if (nearindex == -1) {
			nearindex = i;
			nearresult = tx * tx + ty * ty;
			continue;
		}
		//比較して小さければそれを最小とする
		if (nearresult > tx*tx + ty * ty) {
			nearindex = i;
			nearresult = tx * tx + ty * ty;
		}
	}
	return nearindex;
}

void PLAYER::BallShotSet(int index) {
	double ty;
	double trad, ex, ey;
	static int toggle = 1;
	int tindex;
	CONTROL &control = CONTROL::GetInstance();
	ty = ball.GetPosition();
	tindex = NearEnemySerch();
	//戻り値が-1なら敵はもういないので、まっすぐ前に発射
	if (tindex == -1) {
		trad = -PI / 2;
	}else {
		//一番近い敵と弾との角度を取得
		control.GetEnemyPosition(tindex, &ex, &ey);
		trad = atan2(ey - ty + BALL_INITY, ex - x + (toggle*BALL_INITX));
	}
	shot[index].flag = true;
	shot[index].x = x + BALL_INITX * toggle;
	shot[index].y = ty + BALL_INITY;
	shot[index].rad = trad;
	shot[index].type = 1;

	if (toggle == 1) {
		toggle = -1;
	}else {
		toggle = 1;
	}
}

void PLAYER::Shot()
{

	s_shot = false;
	int num = 0;

	if (!damageflag) {
		//キーが押されててかつ、6ループに一回発射
		if (key[KEY_INPUT_Z] == 1 && count % 6 == 0) {
			for (int i = 0; i<PSHOT_NUM; ++i) {
				if (shot[i].flag == false) {
					if (power < 3) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						shot[i].rad = -PI / 2;
						shot[i].type = 0;
						break;
					}else if (power >= 3 && power < 10) {
						if (num == 0) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.57;
							shot[i].type = 0;
						}else if (num == 1) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.69;
							shot[i].type = 0;
						}else if (num == 2) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.45;
							shot[i].type = 0;
						}
						++num;
						if (num == 3) {
							break;
						}
					}else if (power == 10) {
						//0の時が前方発射
						if (num == 0) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.57;
							shot[i].type = 0;
						}else if (num == 1) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.69;
							shot[i].type = 0;
						}else if (num == 2) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = -1.45;
							shot[i].type = 0;
						}else if (num > 2) {
							BallShotSet(i);
						}
						++num;
						if (num == 5) {
							break;
						}
					}
				}
			}
			//ショットサウンドフラグを立てる
			s_shot = true;
		}
	}

	//一番近い敵との角度
	double trad;
	//一番近い敵の添え字
	int index;
	double ex, ey;
	//controlクラスの参照変数
	CONTROL &control = CONTROL::GetInstance();

	//一番近い敵の添え字取得
	index = NearEnemySerch();

	
	//弾を移動させる処理
	for (int i = 0; i<PSHOT_NUM; ++i) {
		//発射してる弾だけ
		if (shot[i].flag) {
			if (shot[i].type == 0) {
				shot[i].x += cos(shot[i].rad)*PSHOT_SPEED;
				shot[i].y += sin(shot[i].rad)*PSHOT_SPEED;
			}else if (shot[i].type == 1) {
				//戻り値が-1なら敵はもういないので、まっすぐ前に発射
				if (index == -1) {
					trad = -PI / 2;
				}else {
					//一番近い敵との角度を取得
					control.GetEnemyPosition(index, &ex, &ey);
					trad = atan2(ey - shot[i].y, ex - shot[i].x);
				}

				//ボスがいる場合はボスとの角度を取得
				if (control.GetBossFlag()) {
					control.GetBossPosition(&ex, &ey);
					trad = atan2(ey - shot[i].y, ex - shot[i].x);
				}

				shot[i].rad = trad;
				shot[i].x += cos(trad)*PSHOT_SPEED;
				shot[i].y += sin(trad)*PSHOT_SPEED;
			}
			
			//画面の外にはみ出したらフラグを戻す
			if (shot[i].y<-10 || shot[i].x < -10 || shot[i].x > 410 || shot[i].y>510) {
				shot[i].flag = false;
			}
		}
	}
}

void PLAYER::Move(){
	
	if(key[KEY_INPUT_LEFT]==1 || key[KEY_INPUT_RIGHT]==1){
		if(key[KEY_INPUT_UP]==1 || key[KEY_INPUT_DOWN]==1){
			//移動係数を０．７１に設定
			move=0.71f;
		}else{
			//斜めじゃなければ１．０に設定
			move=1.0f;
		}
	}else if(key[KEY_INPUT_UP]==1 || key[KEY_INPUT_DOWN]==1){
		move=1.0f;
	}

	if (key[KEY_INPUT_LEFT] == 1) {
		x -= (int)PLAYER_SPEED * move;
	}
	if (key[KEY_INPUT_RIGHT] == 1) {
		x += (int)PLAYER_SPEED * move;
	}
	if (key[KEY_INPUT_UP] == 1) {
		y -= (int)PLAYER_SPEED * move;
	}
	if (key[KEY_INPUT_DOWN] == 1) {
		y += (int)PLAYER_SPEED * move;
	}

	//キャラの移動制御
	if (x > 400 - width/2) {
		x = 400 - width/2;
	}
	else if (x < 0 + width / 2) {
		x = 0 + width / 2;
	}

	if (y > 480 - height/2) {
		y = 480 - height/2;
	}
	else if (y < 0 + height / 2) {
		y = 0 + height / 2;
	}

	//左キーが押されてて、かつxcountが０以上なら０にしてから１引く。
	//それ以外は１引く
	if (key[KEY_INPUT_LEFT] == 1) {
		if (xcount > 0)
			xcount = 0;
		--xcount;
	}
	//右キーが押されてて、かつxcountが０以下なら０にしてから１足す。
	//それ以外は１引く
	if (key[KEY_INPUT_RIGHT] == 1) {
		if (xcount < 0)
			xcount = 0;
		++xcount;
	}
	//上キーが押されてて、かつycountが０以上なら０にしてから１引く。
	//それ以外は１引く
	if (key[KEY_INPUT_UP] == 1) {
		if (ycount > 0)
			ycount = 0;
		--ycount;
	}
	//下キーが押されてて、かつycountが０以下なら０にしてから１足す。
	//それ以外は１足す
	if (key[KEY_INPUT_DOWN] == 1) {
		if (ycount < 0)
			ycount = 0;
		++ycount;
	}

	//カウント数から添字を求める。
	ix = abs(xcount) % 30 / 10;
	iy = abs(ycount) % 30 / 10;

	//xカウントがプラスなら右向きなので2行目の先頭添字番号を足す。
	if (xcount > 0) {
		ix += 3;
		result = ix;
	}
	else if (xcount < 0) {
		//マイナスなら左向きなので、4行目の先頭添字番号を足す。
		ix += 9;
		result = ix;
	}

	//yカウントがプラスなら下向きなので、3行目の先頭添字番号を足す。
	if (ycount > 0) {
		iy += 6;
		result = iy;
	}
	else if (ycount < 0) {
		//１行目の先頭添字番号は０なので何もする必要なし。(分かりやすくするために書いときました)
		iy += 0;
		result = iy;
	}

	//斜め移動の場合は横顔を優先
	if (move == 0.71f)
	result = ix;

	//押されてなければカウントをゼロにする。
	if (key[KEY_INPUT_LEFT] != 1 && key[KEY_INPUT_RIGHT] != 1) {
		xcount = 0;
	}
	if (key[KEY_INPUT_UP] != 1 && key[KEY_INPUT_DOWN] != 1) {
		ycount = 0;
	}

}

void PLAYER::Draw()
{
	//弾描画
	for (int i = 0; i<PSHOT_NUM; ++i) {
		if (shot[i].flag) {
			DrawRotaGraph(shot[i].x , shot[i].y , 1.0, shot[i].rad+(90*PI/180), shot[i].gh, TRUE);
		}
	}
	//生きてれば描画
	if (damageflag) {
		if (dcount > 20) {
			if (dcount % 2 == 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
				DrawGraph(PLAYER_INITX - width / 2, PLAYER_INITY - height / 2 + 60 - (dcount - 20), gh[1], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}else {
				DrawGraph(PLAYER_INITX - width / 2, PLAYER_INITY - height / 2 + 60 - (dcount - 20), gh[1], TRUE);
			}
		}
		++dcount;
		if (dcount == 80) {
			damageflag = false;
			dcount = 0;
			//座標を初期値に戻す
			x = PLAYER_INITX;
			y = PLAYER_INITY;
			//上向きの画像にする
			result = 1;
		}
	}else {
		//通常描画
		DrawGraph(x - width / 2, y - height / 2, gh[result], TRUE);
		//DrawCircle(x, y,20,255,1);
	}
}

void PLAYER::All()
{
	if (!damageflag) {
		Move();
	}
	Shot();
	effect_pdead->All();
	Ball();
	Draw();
	
	++count;
}

