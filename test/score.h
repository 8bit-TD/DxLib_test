#pragma once
enum SCOREDATA {
	HIGH_SCORE,
	CURRENT_SCORE,
	GRAZE_SCORE,
	POWER_SCORE,
	LIFE_SCORE
};

class SCORE {
private:
	//グラフィックハンドル
	int g_board[4], g_number[10];
	int high_score, score, graze, life;

private:
	void Draw();
public:
	void SetScore(SCOREDATA data, int val);
	int GetScore(SCOREDATA data);
	void All();
	SCORE();
};
