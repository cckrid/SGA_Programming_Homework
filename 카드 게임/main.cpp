#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>

using namespace std;

void makeStar() {
	for (int i = 0; i < 40; i++)
		cout << "*";
	cout << endl;
}
// ���ڷ� �̷���� ī�带 ����� ���ڷ� ����� �Լ�
string showCard(int num) {
	string card;
	string cardType[4] = { "��","��","��","��" };
	int type, cardNum;
	// �����̵�
	if (num <= 12) {
		type = 0;
		cardNum = num;
	}
	// ��Ʈ
	else if (num <= 25) {
		type = 1;
		cardNum = num - 13;
	}
	// ���̾Ƹ��
	else if (num <= 38) {
		type = 2;
		cardNum = num - 26;
	}
	// Ŭ��
	else {
		type = 3;
		cardNum = num - 39;
	}
	card = cardType[type];

	if (cardNum == 0)
		card += "A";
	else if (cardNum == 10)
		card += "J";
	else if (cardNum == 11)
		card += "Q";
	else if (cardNum == 12)
		card += "K";
	else
		card += to_string(cardNum+1);

	// test
	//cout << card << endl;

	return card;
}

int cheat(int card1, int card2, int card3) {
	// �׽�Ʈ�� ���� ġƮ
	int min, max;
	if (card1 % 13 <= card3 % 13) {
		min = card1 % 13;
		max = card3 % 13;
	}
	else {
		min = card3 % 13;
		max = card1 % 13;
	}

	// ī�尡 ���̿� ������ ����
	if (min <= card2 % 13 && card2 % 13 <= max) 
		return 1;
	// �ƴ� ��� �н�
	else
		return 0;
}

void main()
{
	int money = 5000;
	int card[52];

	// ī�� �����
	/*
		�����̵�	��Ʈ	���̾Ƹ��	Ŭ�� ��
		0~12		13~25	26~38		39~51
	*/
	for (int i = 0; i < 52; i++) {
		card[i] = i;
	}

	// test
	cout << "ī�� �����ֱ� �׽�Ʈ" << endl;
	for (int i = 0; i < 52; i++) {
		cout << showCard(card[i]) << " ";
		if (i % 13 == 12)
			cout << endl;
	}
	cout << endl;

	// ����
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int sour, dest, temp;
		sour = rand() % 52;
		dest = rand() % 52;
		
		temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}

	//test
	cout << "ī�� ���� �� �׽�Ʈ" << endl;
	for (int i = 0; i < 52; i++) {
		cout << showCard(card[i]) << " ";
		if (i % 3 == 2 || i == 51)
			cout << endl;
	}
	cout << endl;

	int GameCount = 0;

	while (true) {
		cout << GameCount << "��° ����" << endl;
		cout << "\t\t���� �ݾ� : " << money << endl;

		int table[3];
		// ���� ī��κ��� �� ���� �̴´�.
		for (int i = 0; i < 3; i++) {
			table[i] = card[i + GameCount * 3];
		}

		cout << endl;
		cout << "\t" << showCard(table[0]) << "\t??\t"
			<< showCard(table[2]) << endl;
		cout << endl;

		cout << "�����Ͻǲ��� 1�� �н��� 2�� : " << endl;
		int select;
		//cin >> select;
		select = cheat(table[0], table[1], table[2]);

		if (select == 1) {
			cout << "������ �����մϴ�." << endl;
			int bet;
			cout << "���þ� �Է� : ";
			//cin >> bet;
			// ġƮ
			bet = (rand() % (money / 1000) + 1) * 1000;
			cout << "���þ� : " << bet << endl;

			if (bet < 1000) {
				cout << "���þ��� �ּ� 1000 �̻� �����մϴ�." << endl;
				continue;
			}
			else if (bet > money) {
				cout << "���� �ݾ��� �Ѿ ���� �� �� �����ϴ�" << endl;
				continue;
			}

			cout << endl;
			cout << "���� :\t";

			for (int i = 0; i < 3; i++) {
				cout << showCard(table[i]) << "\t";
			}
			cout << endl;
			cout << endl;

			int min, max;
			if (table[0] % 13 <= table[2] % 13) {
				min = table[0] % 13;
				max = table[2] % 13;
			}
			else {
				min = table[2] % 13;
				max = table[0] % 13;
			}

			if (min <= table[1] % 13 && table[1] % 13 <= max) {
				cout << "���ڰ� ���̿� �ֽ��ϴ�. ���þ� +" << bet << endl;
				money += bet;
			}
			else {
				cout << "���ڰ� ���̿� ���� �ʽ��ϴ�. ���þ� -" << bet << endl;
				money -= bet;
			}

			if (money <= 0) {
				cout << "�ݾ��� ��� ���������ϴ�." << endl;
				break;
			}
		}
		else {
			cout << "�н��մϴ�. ���� �������� �Ѿ�ϴ�." << endl;
		}

		if (GameCount == 16) {
			cout << "ī�尡 ��� �����Ǿ� ������ ���� �Ǿ����ϴ�." << endl;
			cout << "���� ȹ�� �ݾ� : " << money << endl;
			break;
		}
		makeStar();
		GameCount++;
	}
}

/*
	������ ����

	ī�� �� 52��
	ī�� 3���� �̰�
	ù��� ������ ���� ������ ��
	������ �� ���� 
	��� ī�尡 �ȿ� �ִ����� ���ߴ� ����
	a ~ 10 j q k
	0~ 13
*/

/*
	������ �� �� �߰�
	1~10 ���̷� �ؾߵǴµ�
	10 5 1 �̷����϶��� üũ �ȵ�

	���þ� 1000�̻� ������
*/