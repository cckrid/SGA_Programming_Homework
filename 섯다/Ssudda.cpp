#include "Ssudda.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <string>
#include <stdio.h>

using namespace std;

Ssudda::Ssudda()
{
	SelectPlayer();
	Init();
}


Ssudda::~Ssudda()
{
}

void Ssudda::Shuffle()
{
	srand(time(NULL));

	for (int i = 0; i < 1000; i++) {
		int dest = rand() % 20;
		int sour = rand() % 20;
		int temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}
}
// �� ī�带 ���� ������ ���� ������ �ٲ��ִ� �Լ�
int Ssudda::CheckCombination(int * card)
{
	/*
		1, 3, 8 ��° ī�尡 ��
	*/

	// ����ϱ� ���ϰ� ������ ����
	combo level;

	int card1 = card[0] % 10 + 1;
	int card2 = card[1] % 10 + 1;

	// 38����
	if (card[0] + 1 == 3 && card[1] + 1 == 8) {
		level = GwangTtaeng38;
	}
	// ���� 18, 13 ���� �ΰ���
	else if (card[0] + 1 == 1 && card[1] + 1 == 8
		|| card[0]+1 == 1 && card[1] + 1 == 3) {
		level = GwangTtaeng;
	}
	// �嶯(10)
	else if (card1 == card2 && card1 == 10) {
		level = ZangTtaeng;
	}
	// 9��
	else if (card1 == card2 && card1 == 9) {
		level = Ttaeng9;
	}
	// 8��
	else if (card1 == card2 && card1 == 8) {
		level = Ttaeng8;
	}
	// 7��
	else if (card1 == card2 && card1 == 7) {
		level = Ttaeng7;
	}
	// 6��
	else if (card1 == card2 && card1 == 6) {
		level = Ttaeng6;
	}
	// 5��
	else if (card1 == card2 && card1 == 5) {
		level = Ttaeng5;
	}
	// 4��
	else if (card1 == card2 && card1 == 4) {
		level = Ttaeng4;
	}
	// 3��
	else if (card1 == card2 && card1 == 3) {
		level = Ttaeng3;
	}
	// 2��
	else if (card1 == card2 && card1 == 2) {
		level = Ttaeng2;
	}
	// �涯(1)
	else if (card1 == card2 && card1 == 1) {
		level = PingTtaeng;
	}
	// �˸� 1 2
	else if (card1 == 1 && card2 == 2) {
		level = Ali;
	}
	// ���� 1 4
	else if (card1 == 1 && card2 == 4) {
		level = Dogsa;
	}
	// ���� 1 9
	else if (card1 == 1 && card2 == 9) {
		level = GuPing;
	}
	// ��� 1 10
	else if (card1 == 1 && card2 == 10) {
		level = ZangPing;
	}
	// ��� 4 10
	else if (card1 == 4 && card2 == 10) {
		level = ZangSa;
	}
	// ���� 4 6
	else if (card1 == 4 && card2 == 6) {
		level = SeYug;
	}
	// ���� �� ���� �� 9
	else if ((card1 + card2) % 10 == 9) {
		level = Gabo;
	}
	// 8��
	else if ((card1 + card2) % 10 == 8) {
		level = Keut8;
	}
	// 7��
	else if ((card1 + card2) % 10 == 7) {
		level = Keut7;
	}
	// 6��
	else if ((card1 + card2) % 10 == 6) {
		level = Keut6;
	}
	// 5��
	else if ((card1 + card2) % 10 == 5) {
		level = Keut5;
	}
	// 4��
	else if ((card1 + card2) % 10 == 4) {
		level = Keut4;
	}
	// 3��
	else if ((card1 + card2) % 10 == 3) {
		level = Keut3;
	}
	// 2��
	else if ((card1 + card2) % 10 == 2) {
		level = Keut2;
	}
	// 1��
	else if ((card1 + card2) % 10 == 1) {
		level = Keut1;
	}
	// ���� �� ���� �� 0
	else
		level = MangTong;

	return level;
}

string Ssudda::ShowLevelToCard(int level)
{
	string card;

	switch (level)
	{
	case Ssudda::GwangTtaeng38:
		card = "38����";
		break;
	case Ssudda::GwangTtaeng:
		card = "����";
		break;
	case Ssudda::ZangTtaeng:
		card = "�嶯";
		break;
	case Ssudda::Ttaeng9:
		card = "9��";
		break;
	case Ssudda::Ttaeng8:
		card = "8��";
		break;
	case Ssudda::Ttaeng7:
		card = "7��";
		break;
	case Ssudda::Ttaeng6:
		card = "6��";
		break;
	case Ssudda::Ttaeng5:
		card = "5��";
		break;
	case Ssudda::Ttaeng4:
		card = "4��";
		break;
	case Ssudda::Ttaeng3:
		card = "3��";
		break;
	case Ssudda::Ttaeng2:
		card = "2��";
		break;
	case Ssudda::PingTtaeng:
		card = "�涯";
		break;
	case Ssudda::Ali:
		card = "�˸�";
		break;
	case Ssudda::Dogsa:
		card = "����";
		break;
	case Ssudda::GuPing:
		card = "����";
		break;
	case Ssudda::ZangPing:
		card = "���";
		break;
	case Ssudda::ZangSa:
		card = "���";
		break;
	case Ssudda::SeYug:
		card = "����";
		break;
	case Ssudda::Gabo:
		card = "����";
		break;
	case Ssudda::Keut8:
		card = "������";
		break;
	case Ssudda::Keut7:
		card = "�ϰ���";
		break;
	case Ssudda::Keut6:
		card = "������";
		break;
	case Ssudda::Keut5:
		card = "�ټ���";
		break;
	case Ssudda::Keut4:
		card = "�ײ�";
		break;
	case Ssudda::Keut3:
		card = "����";
		break;
	case Ssudda::Keut2:
		card = "�β�";
		break;
	case Ssudda::Keut1:
		card = "�Ѳ�";
		break;
	case Ssudda::MangTong:
		card = "����";
		break;
	}

	return card;
}

void Ssudda::SelectPlayer()
{
	while (true) {
		cout << "�÷����� ����ڸ� �Է��ϼ��� (2 ~ 4) : ";
		cin >> playerNumber;

		if (playerNumber >= 2 && playerNumber <= 4) {
			// �÷��̾�鿡 �ʱ� �ݾ� �����ϱ�
			for (int i = 0; i < playerNumber; i++) {
				player[i].SetMoney(STARTMONEY);
			}

			break;
		}
		else {
			cout << "�ٽ� �Է��ϼ���" << endl;
		}
	}
}

void Ssudda::Init()
{
	for (int i = 0; i < 20; i++)
		card[i] = i;

	Shuffle();

	tableMoney = 0;		// ���̺� �ݾ� �ʱ�ȭ
}

void Ssudda::Release()
{
}

void Ssudda::Update()
{
	system("cls");

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

	cout << "\t\t      ����" << endl;
	cout << "\t\t\t\t�÷��� �ο� : " << playerNumber << "��" << endl;

	int draw = 0;

	// �÷��̾�鿡�� ī�� �����ֱ�
	for (int i = 0; i < playerNumber; i++) {
		player[i].SetCard(card[draw], card[draw + 1]);
		draw += 2;

		// test
		//player[0].SetCard(2, 7);

		// �÷��̾�� ��� �����ϵ��� ����
		player[i].SetBetting(player[i].GetMoney());
		// �÷��̾�� ���� �ݾ�, ���̺� �ݾ׿� ���ϱ�
		tableMoney += player[i].GetBetting();

		// ���� ������ ���� �÷��̾� ���� �����ϱ�
		player[i].SetLevel(CheckCombination(player[i].GetCard()));
	}

	// ȭ�鿡 ī�� �����ֱ�
	Render();

	/*
		���� ���κа� �ݾ� ���κ� (���þ׿� ���� �޶�����)
		�����ϸ� ������ 
		���ºο� ���� ó����
	*/

	bool isDraw = false;
	int winLevel = INT_MAX;
	int winner;
	// ���� ��� , ������ ���� ���� �÷��̾ ����
	for (int i = 0; i < playerNumber; i++) {
		// ����(����)�� ���� ��� ���º�
		if (winLevel == player[i].GetLevel())
			isDraw = true;
		// ������ ���� ���� ���� (���� ������ 38����)
		if (player[i].GetLevel() < winLevel) {
			winLevel = player[i].GetLevel();
			winner = i;
		}
	}

	if (isDraw == false) {
		// �ݾ� ���
		// ���ڴ� ���̺��� ��� �ݾ׸�ŭ ȹ��
		int money = player[winner].GetMoney();
		player[winner].SetMoney(money + tableMoney);

		// ���� ǥ�� �� �÷��̾�� �ݾ� �����ֱ�
		for (int i = 0; i < playerNumber; i++) {
			if (i == winner) {
				cout << "�÷��̾�" << winner + 1 << " �¸� ";
				cout << "������ : " << player[winner].GetMoney();
				cout << " (�� +" << tableMoney << ")" << endl;
			} 
			else {
				cout << "�÷��̾�" << i + 1 << " �й� ";
				cout << "������ : " << player[i].GetMoney();
				cout << " (�� -" << player[i].GetBetting() << ")" << endl;
			}
		}
	}
	else {
		cout << "���º� �Դϴ�." << endl;
		// ���õ� �ݾ� �����ޱ�
		for (int i = 0; i < playerNumber; i++) {
			// �÷��̾��� �ݾ�
			int money = player[i].GetMoney();
			// �÷��̾��� ���þ�
			int bet = player[i].GetBetting();
			player[i].SetMoney(money + bet);
		}

		// �÷��̾�� �ݾ� �����ֱ�
		for (int i = 0; i < playerNumber; i++) {
			cout << "�÷��̾�" << i + 1 << " ���º� ";
			cout << "������ : " << player[winner].GetMoney();
			cout << " (�� +" << player[i].GetBetting() << ")" << endl;
		}
	}

}

void Ssudda::Render()
{
	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

	for (int i = 0; i < playerNumber; i++) {
		/*cout << "�÷��̾� " << i + 1 << " �� ī�� : ";
		cout << player[i].GetCard()[0] % 10 + 1 << " " << player[i].GetCard()[1] % 10 + 1 << " ";
		cout << "(" << ShowLevelToCard(player[i].GetLevel()) << ") ";
		cout << "���þ� : " << player[i].GetBetting() << endl;*/

		// printf�Լ��� ���� ������ ���Ͽ� �ֿܼ� ���
		printf("�÷��̾�%d ", i + 1);
		
		if (player[i].GetMoney() == 0)
			printf("(All-in):");
		else
			printf("(%6d):", player[i].GetMoney());

		printf(" %2d %2d (%6s) ���þ� : %d\n",
			player[i].GetCard()[0] % 10 + 1,
			player[i].GetCard()[1] % 10 + 1,
			ShowLevelToCard(player[i].GetLevel()).c_str(),
			player[i].GetBetting());
	}
	cout << "\n���̺� �ݾ� : " << tableMoney << endl;

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

}
