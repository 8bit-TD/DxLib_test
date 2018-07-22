#include "score.h"
#include "pch.h"


SCORE::SCORE() {

	if (-1 == LoadDivGraph("board2.png", 4, 1, 4, 160, 33, g_board)) {
		MSG("エラー発生");
	}
	if (-1 == LoadDivGraph("number.png",10,10,1,19,27,g_number))
	{
		MSG("エラー発生");
	}
	high_score = score = graze = life = 0;
}

void SCORE::SetScore(SCOREDATA data, int val) {
	switch (data) {
		case HIGH_SCORE:
			high_score += val;
			break;
		case CURRENT_SCORE:
			score += val;
			break;
		case GRAZE_SCORE:
			graze += val;
			break;
		case LIFE_SCORE:
			life = val;
			break;
	}
}

int SCORE::GetScore(SCOREDATA data) {
	switch (data) {
		case HIGH_SCORE:
			return high_score;
			break;
		case CURRENT_SCORE:
			return score;
			break;
		case GRAZE_SCORE:
			return graze;
			break;
		case LIFE_SCORE:
			return life;
			break;
	}
	//該当しないときは-1を返す。
	return -1;
}

void SCORE::Draw() {
	char buf[100];
	int num;
	//スコアボードの文字の描画
	DrawGraph(SCORE_X, 10, g_board[0], TRUE);
	DrawGraph(SCORE_X, 70, g_board[1], TRUE);
	DrawGraph(SCORE_X, 140, g_board[2], TRUE);
	DrawGraph(SCORE_X, 170, g_board[3], TRUE);

	//ハイスコア描画
	num = sprintf(buf, "%d", high_score);
	for (int i = 0; i < num; ++i) {
		DrawGraph(SCORE_X + 20 + i * 19, 35, g_number[(buf[i] - '0')], TRUE);
	}

	//スコア描画
	num = sprintf(buf, "%d", score);
	for (int i = 0; i < num; ++i) {
		DrawGraph(SCORE_X + 20 + i * 19, 95, g_number[(buf[i] - '0')], TRUE);
	}

	//グレイズ数描画
	num = sprintf(buf, "%d", graze);
	for (int i = 0; i < num; ++i) {
		DrawGraph(SCORE_X + 100 + i * 19, 145, g_number[(buf[i] - '0')], TRUE);
	}

	//ライフ数描画
	num = sprintf(buf, "%d", life);
	for (int i = 0; i < num; ++i) {
		DrawGraph(SCORE_X + 100 + i * 19, 175, g_number[(buf[i] - '0')], TRUE);
	}
	
}

void SCORE::All() {
	Draw();
}