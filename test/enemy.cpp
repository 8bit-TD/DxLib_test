#include "pch.h"
#include "enemy.h"
#include "control.h"
#include "define.h"


ENEMY::ENEMY(int type, int stype, int m_pattern, int s_pattern, int in_time, int stop_time, int shot_time, int out_time, int x, int y, int speed, int hp, int item)
{
	//�T�C�Y
	width = 27;
	height = 25;

	//�G�̎��
	this->type = type;
	this->stype = stype;

	//�ړ��p�^�[���ƃV���b�g�p�^�[��
	this->m_pattern = m_pattern;
	this->s_pattern = s_pattern;
	this->x = x;
	this->y = y;
	this->in_time = in_time;
	this->stop_time = stop_time;
	this->shot_time = shot_time;
	this->out_time = out_time;

	//hp�ƃA�C�e�����
	this->hp = hp;
	this->item = item;

	//�G�摜�ǂݍ���
	if (type == 0) {
		LoadDivGraph("enemy.png", 3, 1, 3, 27, 25, gh);
	}

	int temp;
	//�e�摜�ƃT�C�Y�擾
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
	
	//�e�摜�ƃT�C�Y�擾
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
	//�e�̃T�E���h�t���O
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
	//�܂������Ă��邩��ʓ��ɂ���Ƃ���������
	if (!deadflag) {
		switch (m_pattern) {
			//�r���Ŏ~�܂��āA���̂܂܌��ɋA��p�^�[��
			case 0:
				//�o�Ă��Ă���~�܂鎞�Ԃ܂ł̊ԂȂ牺�Ɉړ�
				if (in_time < g_count && g_count < stop_time) {
					y += 2;
				//�A�Ҏ��Ԃ��߂�����߂�B
				}else if (g_count > out_time) {
					y -= 2;
				}
				break;
			//���̂܂܎~�܂炸�ɉ��ɍs���p�^�[��
			case 1:
				if (in_time <= g_count) {
					y += 2;
				}
				break;
			//������Ƃ����Ɉړ����Ȃ�������Ă���
			case 2:
				if (in_time <= g_count) {
					y += 1;
					if (count % 10 == 0) {
						x -= 1;
					}
				}
				break;
			//������Ƃ��E�Ɉړ����Ȃ�������Ă���
			case 3:
				if (in_time <= g_count) {
					y += 1;
					if (count % 10 == 0) {
						x += 1;
					}
				}
				break;
		}
		//��ʂ���͂ݏo������,deadflag(�݂͂��������ʂ��̃t���O)��true�ɂ���B
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
	//CONTROL�N���X�̎Q��
	CONTROL &control = CONTROL::GetInstance();
	double px, py;

	//���˃^�C�~���O�ɂȂ�����A�t���O�𗧂Ă�
	if (shot_time == g_count) {
		sflag = true;
	}

	//�t���O�����Ă�Ƃ�����
	if (sflag) {
		//�e�̃T�E���h�t���O
		s_shot = false;
		//�G�������Ă��鎞�������˂���B
		if (!deadflag) {
			//�v���C���[�̈ʒu�擾
			control.GetPlayerPosition(&px, &py);
			//�G�ƃv���C���[�̍��W������t���ڂ����߂�
			//���񂾂����s
			if (scount == 0)
				rad = atan2(py - y, px - x);
			switch (s_pattern) {
			case 0:
			//5���[�v�Ɉ�񔭎ˁB20�܂łȂ̂�5�����ˁB
			if (scount % 5 == 0 && scount <= 20) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//�t���O�������ĂȂ��e��T���āA���W�����Z�b�g
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						shot[i].rad = rad;
						break;
					}
				}
				//�V���b�g�T�E���h�t���O�𗧂Ă�
				s_shot = true;
			}
			break;
			//�v���C���[�Ɍ������Ē����V���b�g
			case 1:
			//6���[�v�Ɉ�񔭎ˁB54�܂łȂ̂�10�����ˁB
			if (scount % 6 == 0 && scount <= 54) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//�t���O�������ĂȂ��e��T���āA���W�����Z�b�g
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						shot[i].rad = rad;
						break;
					}
				}
				//�V���b�g�T�E���h�t���O�𗧂Ă�
				s_shot = true;
			}
			break;
			//3�����V���b�g
			case 2:
			//10���[�v�Ɉ�񔭎ˁB1���[�v��3���Ȃ̂�5���[�v�������15������
			if (scount % 10 == 0 && scount <= 40) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//�t���O�������ĂȂ��e��T���āA���W�����Z�b�g
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;

						//0�̎��͍����
						if (num == 0) {
							//�G�ƃv���C���[�Ƃ̋t���ڂ���10�x���������W�A������
							shot[i].rad = rad - (10 * 3.14 / 180);
							//1�̎��̓v���C���[�꒼��
						}
						else if (num == 1) {
							//�G�ƃv���C���[�Ƃ̋t���ڂ���
							shot[i].rad = rad;
							//2�̎��͉E���
						}
						else if (num == 2) {
							//�G�ƃv���C���[�Ƃ̋t���ڂ���10�x���������W�A������
							shot[i].rad = rad + (10 * PI / 180);
						}
						++num;
						//3�����˂�����Anum��0�ɂ��ă��[�v�𔲂���B
						if (num == 3) {
							num = 0;
							break;
						}
					}
				}
				//�V���b�g�T�E���h�t���O�𗧂Ă�
				s_shot = true;
			}
			break;
			case 3:
			//10���[�v�Ɉꔭ���ˁB42�܂łȂ̂�15�����ˁB
			if (scount % 3 == 0 && scount <= 42) {
				for (int i = 0; i < ENEMY_SNUM; ++i) {
					//�t���O�������ĂȂ��e��T���āA���W�����Z�b�g
					if (shot[i].flag == false) {
						shot[i].flag = true;
						shot[i].x = x;
						shot[i].y = y;
						//���񂾂�����������
						if (num = 0)
							srand((unsigned int)time(NULL));
						shot[i].rad = atan2(py - y, px - x) - (60 * PI / 180) + ((rand() % 120)*PI / 180);
						++num;
						break;
					}
				}
				//�V���b�g�T�E���h�t���O�𗧂Ă�
				s_shot = true;
			}
			break;
			}

			
		}
		//�t���O�������Ă�e�̐�
		int s = 0;
		//�t���O�����Ă�e�����A�e�̈ړ����s��
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
				//�e����ʂ���͂ݏo����t���O��߂��B
				if (ShotOutCheck(i)) {
					shot[i].flag = false;
					continue;
				}
				++s;
			}
		}
		//s���[���Ƃ������Ƃ͔��˒��̒e���Ȃ��B
		//����deadflag��TRUE�Ƃ������Ƃ͂��̓G�̃N���X�͏��ł����Ă��悢
		if (s == 0 && deadflag) {
			//�G�N���X���Ńt���O��TRUE�ɂ���
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
	//�e����ŏ��ɕ`��
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