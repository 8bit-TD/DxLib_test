#include "boss.h"
#include "pch.h"
#include "control.h"

BOSS::BOSS() {
	//座標初期値
	x = 200;
	y = -100;
	prev_x = 200;
	prev_y = -100;
	
	hp = 100;

	//画像読み込み
	gh_face[0] = LoadGraph("boss.png");
	gh_face[1] = LoadGraph("boss_damage.png");
	gh_shot[0] = LoadGraph("enemyshot1.png");
	gh_shot[1] = LoadGraph("enemyshot2.png");
	gh_shot[2] = LoadGraph("enemyshot3.png");
	//弾初期化
	for (int i = 0; i < BOSS_SHOTNUM; ++i) {
		shot[i].flag = false;
		shot[i].gflag = false;
		shot[i].gh = 0;
		shot[i].pattern = 0;
		shot[i].rad = 0;
		shot[i].speed = 0;
		shot[i].x = 0;
		shot[i].y = 0;
	}

	raise = 2;
	raise2 = 2;
	angle = 0;
	move_pattern = 0;
	shot_pattern = 3;

	movex = 0;
	movey = 180;

	waitcount = 0;
	wait = false;
	p3_state = 0;

	scount = 0;
	temp_scount = 0;

	damageflag = false;
	flag = true;
}

void BOSS::Move() {
	switch (move_pattern) {
		case 0:
			Appear();
			break;
		case 1:
			MovePattern1();
			break;
		case 2:
			MovePattern2();
			break;
		case 3:
			MovePattern3();
			break;
	}
}

void BOSS::Appear() {
	double temp;
	angle += 2;
	temp = sin(angle*PI / 180);
	x = 200;
	y = prev_y + temp * movey;
	//定位置まで移動したら移動パターンを１に変更
	if (angle == 90) {
		move_pattern = 1;
		angle = 0;
		shotflag = true;
	}
}

void BOSS::MovePattern1() {
	angle += raise;
	y = 80 + sin(angle*PI / 180)*BOSS_SHAKE;
	if (angle == 90) {
		raise = -2;
	}else if (angle == -90) {
		raise = 2;
	}
	x = 200;
}

void BOSS::MovePattern2() {
	if (!wait) {
		x += raise2;
		if (x == 70) {
			raise2 = 2;
			wait = true;
		} else if (x == 330) {
			raise2 = -2;
			wait = true;
		}
	}
	if (wait) {
		++waitcount;
		if (waitcount == 20) {
			wait = false;
			waitcount = 0;
		}
	}
}

void BOSS::MovePattern3() {
	double temp;
	angle += 2;
	temp = sin(angle*PI / 180);
	x = prev_x + temp * movex;
	y = prev_y + temp * movey;
	if (angle == 90) {
		if (p3_state == 0) {
			MoveInit(70, 80, 1);
		} else if (p3_state == 1) {
			MoveInit(200, 160, 2);
		} else {
			MoveInit(330, 80, 0);
		}
	}
}

void BOSS::MoveInit(double bx, double by, int state) {
	prev_x = x;
	prev_y = y;
	movex = bx - x;
	movey = by - y;
	angle = 0;
	p3_state = state;
}

void BOSS::SetDamageFlag() {
	this->damageflag = true;
}

void BOSS::SetFlag(bool f) {
	this->flag = f;
}

bool BOSS::GetFlag() {
	return this->flag;
}

int BOSS::ShotSerch() {
	bool check = false;
	int i;
	for (i = 0; i < BOSS_SHOTNUM; ++i) {
		if (shot[i].flag == false) {
			check = true;
			break;
		}
	}
	if (check) {
		shot[i].flag = true;
		shot[i].x = x;
		shot[i].y = y;
	} else {
		i = -1;
	}
	return i;
}

bool BOSS::ShotOutCheck(int i) {
	if (shot[i].x < -20 || shot[i].x>420 || shot[i].y < -20 || shot[i].y>500) {
		return true;
	} else {
		return false;
	}
}

bool BOSS::GetShotSound() {
	return s_shot;
}

bool BOSS::GetShotPosition(int index, double *x, double *y, int *type) {
	if (shot[index].flag) {
		*x = shot[index].x;
		*y = shot[index].y;
		*type = shot[index].type;
		return true;
	} else {
		return false;
	}
}

bool BOSS::GetGrazeFlag(int index) {
	return shot[index].gflag;
}

void BOSS::SetGrazeFlag(int index) {
	shot[index].gflag = true;
}

void BOSS::SetShotFlag(int index, bool flag) {
	shot[index].flag = flag;
}

void BOSS::GetPosition(double *x, double *y) {
	*x = this->x;
	*y = this->y;
}

int BOSS::HpSet(int i) {
	this->hp = this->hp - i;
	return this -> hp;
}

void BOSS::Shot() {
	//何発発射したか
	int num = 0;
	//空いてる弾の添え字
	int index;
	//scountをもどすかどうかのフラグ
	bool scflag = false;

	CONTROL &control = CONTROL::GetInstance();

	double px, py;
	static double trad;
	if (!damageflag) {
		control.GetPlayerPosition(&px, &py);
		if (scount == 0)
			trad = atan2(py - y, px - x);
		//サウンドフラグを戻す
		s_shot = false;
		//弾のセット
		switch (shot_pattern) {
			case 0:
				if (scount % 5 == 0 && scount <= 15) {
					while ((index = ShotSerch()) != -1) {
						shot[index].gh = gh_shot[1];
						shot[index].pattern = 0;
						shot[index].speed = 6;
						if (num == 0) {
							shot[index].rad = trad - (10 * PI / 180);
						} else if (num == 1) {
							shot[index].rad = trad - (5 * PI / 180);
						} else if (num == 2) {
							shot[index].rad = trad;
						} else if (num == 3) {
							shot[index].rad = trad + (5 * PI / 180);
						} else if (num == 4) {
							shot[index].rad = trad + (10 * PI / 180);
						}
						++num;
						s_shot = true;
						if (num == 5) {
							break;
						}
					}
				}
				break;
			case 1:
				if (scount % 5 == 0) {
					if ((index = ShotSerch()) != -1) {
						shot[index].gh = gh_shot[2];
						shot[index].speed = 4;
						shot[index].rad = atan2(py - y, px - x) + (rand() % 41 - 20)*PI / 180;
						shot[index].pattern = 1;
						s_shot = true;
					}
				}
				break;
			case 2:
				if (scount % 10 == 0) {
					trad = atan2(py - y, px - x);
					while ((index = ShotSerch()) != -1) {
						shot[index].gh = gh_shot[0];
						shot[index].speed = 5;
						shot[index].rad = trad + num * ((360 / 20)*PI / 180);
						shot[index].pattern = 2;
						++num;
						if (num == 20) {
							break;
						}
						s_shot = true;
					}
				}
				break;
			case 3:
				if (scount % 15 == 0) {
					while ((index = ShotSerch()) != -1) {
						shot[index].gh = gh_shot[0];
						shot[index].speed = 3;
						shot[index].pattern = 3;
						shot[index].rad = ((360 / 20)*PI / 180)*num + ((scount / 15)*0.08);
						++num;
						if (num == 20) {
							break;
						}
						s_shot = true;
					}
				}
				num = 0;
				if (scount % 5 == 0 && temp_scount <= scount) {
					while((index = ShotSerch()) != -1) {
						shot[index].gh = gh_shot[1];
						shot[index].speed = 6;
						shot[index].pattern = 3;
						if (num == 0) {
							shot[index].x = x - 50;
							shot[index].rad = atan2(py - y, px - (x - 50));
						} else if (num == 1) {
							shot[index].x = x + 50;
							shot[index].rad = atan2(py - y, px - (x + 50));
						}
						++num;
						if (num == 2) {
							//5発分打ち終わったら、60ループ(1秒)停止
							if (temp_scount + 20 == scount) {
								temp_scount += 60;
							}
							break;
						}
						s_shot = true;
					}
				}

				break;

		}
		for (int i = 0; i < BOSS_SHOTNUM; ++i) {
			if (shot[i].flag) {
				switch (shot[i].pattern) {
					case 0:
						shot[i].x += shot[i].speed * cos(shot[i].rad);
						shot[i].y += shot[i].speed * sin(shot[i].rad);
						if (scflag == false && scount == 40) {
							scflag = true;
						}
						break;
					case 1:
						shot[i].x += shot[i].speed * cos(shot[i].rad);
						shot[i].y += shot[i].speed * sin(shot[i].rad);
						break;
					case 2:
						shot[i].x += shot[i].speed * cos(shot[i].rad);
						shot[i].y += shot[i].speed * sin(shot[i].rad);
						break;
					case 3:
						shot[i].x += shot[i].speed * cos(shot[i].rad);
						shot[i].y += shot[i].speed * sin(shot[i].rad);
						break;
				}
				//弾がはみ出てたらフラグを戻す
				if (ShotOutCheck(i)) {
					shot[i].flag = false;
				}
			}
		}
		++scount;
		if (scflag) {
			scount = 0;
		}
	}
}

void BOSS::Draw() {
	//弾から最初に描画
	for (int i = 0; i < BOSS_SHOTNUM; ++i) {
		if (shot[i].flag) {
			DrawRotaGraph(shot[i].x, shot[i].y, 1.0, shot[i].rad + 90 * PI / 180, shot[i].gh, TRUE);
		}
	}
	//弾があたった時はダメージ用の画像を描画
	if (damageflag) {
		DrawRotaGraph(x, y, 1.0, 0, gh_face[1], TRUE);
	}else {
	//何も無いときは通常描画
		DrawRotaGraph(x, y, 1.0, 0, gh_face[0], TRUE);
	}
	damageflag = false;
}

void BOSS::All() {
	Move();
	if (shotflag) {
		Shot();
	}
	Draw();
	++count;
}