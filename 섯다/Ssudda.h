#pragma once
#include "Player.h"
#include <string>

using namespace std;

class Ssudda
{
private:
	int card[20];
	int playerNumber;
	Player player[4];

	enum combo { 
		GwangTtaeng38, GwangTtaeng, 
		ZangTtaeng, Ttaeng9, Ttaeng8, Ttaeng7, Ttaeng6,
		Ttaeng5, Ttaeng4, Ttaeng3, Ttaeng2, PingTtaeng,
		Ali, Dogsa, GuPing, ZangPing, ZangSa, SeYug, 
		Gabo, Keut8, Keut7, Keut6, Keut5, Keut4, Keut3, Keut2, Keut1,
		MangTong};

#define STARTMONEY 5000

public:
	Ssudda();
	~Ssudda();

	void Shuffle();
	// �� ī�带 ���� ������ ���� ������ �ٲ��ִ� �Լ�
	int CheckCombination(int* card);	
	string ShowLevelToCard(int level);

	void Init();
	void Release();
	void Update();
	void Render();
};

