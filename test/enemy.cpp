#include "pch.h"
#include "enemy.h"
#include "control.h"
#include "define.h"


ENEMY::ENEMY(int type, int stype, int m_pattern, int s_pattern, int in_time, int stop_time, int shot_time, int out_time, int x, int y, int speed, int hp, int item)
{
	//サイズ
	width = 27;
	height = 25;

	//敵の種類
	this->type = type;
	this->stype = stype;

	//移動パターンとショットパターン
	this->m_pattern = m_pattern;
	this->s_pattern = s_pattern;
	this->x = x;
	this->y = y;
	this->in_time = in_time;
	this->stop_time = stop_time;
	this->shot_time = shot_time;
	this->out_time = out_time;

	//hpとアイテム代入
	this->hp = hp;
	this->item = item;

	//敵画像読み込み
	if (type == 0) {
		LoadDivGraph("enemy.png", 3, 1, 3, 27, 25, gh);
	}

	int temp;
	//弾画像とサイズ取得
	switch (stype) {
		case 0:
			temp = LoadGraph("enemyshot1.png");
			break;
		case 1:
			temp = LoadGraph("enemyshot2.png");
			break;
		case 2:
			temp = LoadGraph("enemyshot3.png");
			break;
	}

	int w, h;
	GetGraphSize(temp, &w, &h);
	
	//弾画像とサイズ取得
	for (int i = 0; i < ENEMY_SNUM; ++i) {
		shot[i].flag = true;
		shot[i].gh = temp;
		shot[i].width = w;
		shot[i].height = h;
		shot[i].pattern = s_pattern;
		shot[i].speed = speed;
		shot[i].x = x;
		shot[i].y = y;
		shot[i].gflag = false;
	}

	count = 0;
	scount = 0;
	num = 0;
	rad = 0;

	deadflag = false;
	endflag = false;
	sflag = false;
	//弾のサウンドフラグ
	s_shot = false;


}

bool ENEMY::OutCheck() {
	if (x < -50 || x>520 || y < -50 || y>530) {
		return true;
	}else {
		return false;
	}
}

bool ENEMY::ShotOutCheck(int i) {
	if (shot[i].x < -20 || shot[i].x>420 || shot[i].y < -20 || shot[i].y>500){
		return true;
	}else {
		return false;
	}
}

void ENEMY::SetGrazeFlag(int index) {
	shot[index].gflag = true;
}

bool ENEMY::GetGrazeFlag(int index) {
	return shot[index].gflag;
}

void ENEMY::Move() {
	//まだ生きているか画面内にいるときだけ処理
	if (!deadflag) {
		switch (m_pattern) {
			//途中で止まって、そのまま後ろに帰るパターン
			case 0:
				//出てきてから止まる時間までの間なら下に移動
				if (in_time < g_count && g_count < stop_time) {
					y += 2;
				//帰還時間を過ぎたら戻る。
				}else if (g_count > out_time) {
					y -= 2;
				}
				break;
			//そのまま止まらずに下に行くパターン
			case 1:
				if (in_time <= g_count) {
					y += 2;
				}
				break;
			//ちょっとずつ左に移動しながら消えていく
			case 2:
				if (in_time <= g_count) {
					y += 1;
					if (count % 10 == 0) {
						x -= 1;
					}
				}
				break;
			//ちょっとずつ右に移動しながら消えていく
			case 3:
				if (in_time <= g_count) {
					y += 1;
					if (count % 10 == 0) {
						x += 1;
					}
				}
				break;
		}
		//画面からはみ出したら,deadflag(はみだすか死ぬかのフラグ)をtrueにする。
		if (g_count >= stop_time) {
			if (OutCheck()) {
				deadflag = true;
			}
		}
		++count;
	}
}

void ENEMY::GetPosition(double *x, double *y) {
	*x = this->x+width/2;
	*y = this->y+height/2;
}

bool ENEMY::GetShotPosition(int index, double *x, double *y) {
	if (shot[index].flag) {
		*x = shot[index].x;
		*y = shot[index].y;
		return true;
	}else {
		return false;
	}
}

bool ENEMY::GetShotSound() {
	return s_shot;
}

void ENEMY::SetShotFlag(int index, bool flag) {
	shot[index].flag = flag;
}

int ENEMY::GetShotType() {
	return stype;
}

int ENEMY::GetItem() {
	return item;
}

void ENEMY::Shot() {
	//CONTROLクラスの参照
	CONTROL &control = CONTROL::GetInstance();
	double px, py;

	//発射タイミングになったら、フラグを立てる
	if (shot_time == g_count) {
		sflag = true;
	}

	//フラグ立ってるときだけ
	if (sflag) {
		//弾のサウンドフラグ
		s_shot = false;
		//敵が生きている時だけ発射する。
		if (!deadflag) {
			//プレイヤーの位置取得
			control.GetPlayerPosition(&px, &py);
			//敵とプレイヤーの座標差から逆正接を求める
			//初回だけ実行
			if (scount == 0)
				rad = atan2(py - y, px - x);
			switch (s_pattern) {
			case 0:
			//5ループに一回発射。20までなので5発発射。
			if (scount % 5 == 0 && scount <= 20) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//フラグがたってない弾を探して、座標等をセット
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						shot[i].rad = rad;
						break;
					}
				}
				//ショットサウンドフラグを立てる
				s_shot = true;
			}
			break;
			//プレイヤーに向かって直線ショット
			case 1:
			//6ループに一回発射。54までなので10発発射。
			if (scount % 6 == 0 && scount <= 54) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//フラグが立ってない弾を探して、座標等をセット
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						shot[i].rad = rad;
						break;
					}
				}
				//ショットサウンドフラグを立てる
				s_shot = true;
			}
			break;
			//3直線ショット
			case 2:
			//10ループに一回発射。1ループに3発なので5ループさせると15発発射
			if (scount % 10 == 0 && scount <= 40) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//フラグが立ってない弾を探して、座標等をセット
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;

						//0の時は左寄り
						if (num == 0) {
							//敵とプレイヤーとの逆正接から10度引いたラジアンを代入
							shot[i].rad = rad - (10 * 3.14 / 180);
							//1の時はプレイヤー一直線
						}
						else if (num == 1) {
							//敵とプレイヤーとの逆正接を代入
							shot[i].rad = rad;
							//2の時は右寄り
						}
						else if (num == 2) {
							//敵とプレイヤーとの逆正接から10度足したラジアンを代入
							shot[i].rad = rad + (10 * PI / 180);
						}
						++num;
						//3発発射したら、numを0にしてループを抜ける。
						if (num == 3) {
							num = 0;
							break;
						}
					}
				}
				//ショットサウンドフラグを立てる
				s_shot = true;
			}
			break;
			case 3:
			//10ループに一発発射。42までなので15発発射。
			if (scount % 3 == 0 && scount <= 42) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//フラグが立ってない弾を探して、座標等をセット
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						//初回だけ乱数初期化
						if (num = 0)
							srand((unsigned int)time(NULL));
						shot[i].rad = atan2(py - y, px - x) - (60 * PI / 180) + ((rand() % 120)*PI / 180);
						++num;
						break;
					}
				}
				//ショットサウンドフラグを立てる
				s_shot = true;
			}
			break;
			}

			
		}
		//フラグが立ってる弾の数
		int s = 0;
		//フラグ立ってる弾だけ、弾の移動を行う
		for (int i = 0; i < ENEMY_SNUM; ++i) {
			if (shot[i].flag) {
				switch (shot[i].pattern) {
				case 0:
				shot[i].y += shot[i].speed;
				break;
				case 1:
				shot[i].x += shot[i].speed*cos(shot[i].rad);
				shot[i].y += shot[i].speed*sin(shot[i].rad);
				break;
				case 2:
				shot[i].x += shot[i].speed*cos(shot[i].rad);
				shot[i].y += shot[i].speed*sin(shot[i].rad);
				break;
				case 3:
				shot[i].x += shot[i].speed*cos(shot[i].rad);
				shot[i].y += shot[i].speed*sin(shot[i].rad);
				break;
				}
				//弾が画面からはみ出たらフラグを戻す。
				if (ShotOutCheck(i)) {
					shot[i].flag = false;
					continue;
				}
				++s;
			}
		}
		//sがゼロということは発射中の弾がない。
		//かつdeadflagがTRUEということはこの敵のクラスは消滅させてもよい
		if (s == 0 && deadflag) {
			//敵クラス消滅フラグをTRUEにする
			endflag = true;
		}
		++scount;
	}

}

void ENEMY::SetDeadFlag() {
	deadflag = true;
}

bool ENEMY::GetDeadFlag() {
	return deadflag;
}

void ENEMY::Draw()
{
	int temp;
	//弾から最初に描画
	for (int i = 0; i < ENEMY_SNUM; ++i) {
		if (shot[i].flag) {
			if (stype == 0 || stype == 2) {
				DrawGraph(shot[i].x , shot[i].y , shot[i].gh, true);
			}else {
				DrawRotaGraph(shot[i].x + shot[i].width , shot[i].y + shot[i].height, 1.0, shot[i].rad + (90 * PI / 180), shot[i].gh, true);
			}
		}
	}

	if (!deadflag) {
		temp = count % 40 / 10;
		if (temp == 3)
			temp = 1;
		DrawGraph(x, y, gh[temp], TRUE);
	}
}

bool ENEMY::All() {
	Move();
	Shot();
	Draw();
	++count;
	return endflag;
}