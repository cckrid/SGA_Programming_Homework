#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

int Player::GetLevel()
{
	return level;
}

int * Player::GetCard()
{
	return card;
}

int Player::GetMoney()
{
	return money;
}

int Player::GetBetting()
{
	return betting;
}

void Player::SetLevel(int level)
{
	this->level = level;
}

void Player::SetCard(int card1, int card2)
{
	// ù��° ī�带 ���� ī��� �ϵ��� �ϱ� ���ؼ�
	if (card1 % 10 <= card2 % 10) {
		card[0] = card1;
		card[1] = card2;
	}
	else {
		card[0] = card2;
		card[1] = card1;
	}
}

void Player::SetMoney(int money)
{
	this->money = money;
}

bool Player::SetBetting(int bet)
{
	// ���þ��� �����ݾ׺��� ū ���
	if (money < bet) {
		return false;
	}
	else {
		this->betting = bet;
		money -= bet;
		return true;
	}
}
