#include "ball.h"
#include "pch.h"

BALL::BALL() {
	x = y = 0;
	gh = LoadGraph("awa.png");
	angle = 0;
	toggle = false;
	raise = 2;
}

double BALL::GetPosition() {
	return y;
}

void BALL::Move(double px, double py) {
	angle += raise;
	x = px;
	y = py + sin(angle*PI / 180)*BALL_SHAKE;
	if (angle == 90) {
		toggle = true;
	}else if (angle == -90) {
		toggle = false;
	}
	if (toggle) {
		raise = -2;
	}else {
		raise = 2;
	}
}

void BALL::Draw() {
	DrawRotaGraph(x + BALL_INITX, y + BALL_INITY, 1.0, 0, gh, TRUE);
	DrawRotaGraph(x - BALL_INITX, y + BALL_INITY, 1.0, 0, gh, TRUE);
}

void BALL::All(double px, double py) {
	Move(px, py);
	Draw();
}