#include "graze.h"
#include "pch.h"

GRAZE::GRAZE() {
	x, y ,gh = 0;
	//èââÒÇæÇØì«Ç›çûÇﬁ
	if (gh == 0) {
		gh = LoadGraph("graze.png");
	}
	rad = rate = 0;
	alpha = 255;
	flag = false;
	count = 0;
	srand((unsigned)time(NULL));
}

bool GRAZE::GetFlag() {
	return false;
}

void GRAZE::SetFlag(double x, double y) {
	this->x = x;
	this->y = y;
	flag = true;
}

void GRAZE::Move() {
	//èââÒÇæÇØäpìxê›íË
	if (count == 0) {
		//rad = rand() % 624 / 100;
		rad = (rand() % 360 * PI / 180);
	}
	alpha = 255 - (255 / 20)*count;
	rate = 1.0 - 0.05 * count;
	x += cos(rad) * 12;
	y += sin(rad) * 12;
	++count;
	if (count == 20) {
		count = 0;
		flag = false;
	}
}

void GRAZE::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(x, y, rate, 1, gh, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GRAZE::All() {
	if (flag) {
		Move();
		Draw();
	}
}