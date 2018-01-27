#pragma once
class Player
{
private:
	int level;		// ���� ȭ���и� ������ �ٲ����� ��� �� ������ ���� ����
	int card[2];	// 2���� ȭ����
	int betting;	// ���ñݾ�
	int money;		// �����ݾ�
public:
	Player();
	~Player();

	int GetLevel();
	int* GetCard();
	int GetMoney();
	int GetBetting();

	void SetLevel(int level);
	void SetCard(int card1, int card2);
	void SetMoney(int money);
	bool SetBetting(int bet);
};

