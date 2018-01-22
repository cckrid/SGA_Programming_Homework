#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

void makeStar() {
	for (int i = 0; i < 65; i++)
		cout << "*";
	cout << endl;
}

void ShowCard(int shape, int number) {
	// ���� ����
	switch (shape) {
	case 0:
		cout << "��";
		break;
	case 1:
		cout << "��";
		break;
	case 2:
		cout << "��";
		break;
	case 3:
		cout << "��";
		break;
	}

	// ī�� ����
	switch (number) {
	case 0:
		cout << "A";
		break;
	case 10:
		cout << "J";
		break;
	case 11:
		cout << "Q";
		break;
	case 12:
		cout << "K";
		break;
	default:
		cout << number + 1;
		break;
	}
	
	if (number < 6)
		cout << "(Low) \t";
	else if (number > 6)
		cout << "(High) \t";
	else
		cout << "(Seven) \t";
}

void Shuffle(int* card) {
	srand(time(NULL));

	int dest, sour, temp;

	for (int i = 0; i < 1000; i++) {
		sour = rand() % 52;
		dest = rand() % 52;

		temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}
}

// ������ ���� �ο� ������ �Ű������� �޾� ī�� ī����
void CardCounting(int* counts, int card) {
	// �ο� ī����
	if (card % 13 < 6) {
		counts[0]--;
	}
	// ���� ī����
	else if (card % 13 > 6) {
		counts[1]--;
	}
	// ���� ī����
	else {
		counts[2]--;
	}
}

void ShowCounting(int* counts) {
	float sum = 0;
	// �����ִ� �ο� ���� ���� ���� ���ϱ�
	for (int i = 0; i < 3; i++)
		sum += counts[i];

	cout << endl << endl;;
	cout << "\t�ο� ���� ���� Ȯ�� ���" << endl;
	for (int i = 0; i < 3; i++) {
		float prob = (float)counts[i] / sum * 100;

		switch (i) {
		case 0:
			cout << "\t�ο� : " << prob << "% (" << counts[i] << "��)"<< endl;
			break;
		case 1:
			cout << "\t���� : " << prob << "% (" << counts[i] << "��)" << endl;
			break;
		case 2:
			cout << "\t���� : " << prob << "% (" << counts[i] << "��)" << endl;
			break;
		}
	}

	cout << endl;
}

// Ȯ���� �ٰ��Ͽ� �ڵ����� �ο� ���� ������ �����ϴ� �Լ�
// �ο� ���� ���� �� ���� Ȯ���� ���� ���� ���Ͽ� ��ȯ
int AutoSelectLHS(int* counts) {
	int larger;
	// �ο� ���� �� �� Ȯ���� ���� �� ����
	if (counts[0] > counts[1])
		larger = 0; // �ο�
	else
		larger = 1; // ����

	int max;
	// ���õ� �ʰ� ���� �� �� Ȯ���� ���� �� ����
	if (counts[2] > counts[larger])
		max = 2;
	else
		max = larger;

	return max;
}

void main() {
	int card[52];
	int shape[4];
	int number[4];

	int selectBetting; // ���� ���� ������ ���� ����
	int selectLHS; // �ο� ���� ���� ������ ���� ����
	int betting;
	int money = 10000;
	int hightestMoney = money; // �ִ� ������ �ݾ�
	int gameCount = 0;
	int hightestGameCount = gameCount; // �ִ� �ݾ׿� �����ߴ� ������
	int turn = 0;

	int counts[3];; // �ο� ���� ���� ī������ ���� ���� 
	counts[0] = 24;	// low ���� a ~ 6 * 4 = 24
	counts[1] = 24;	// high ���� 8 ~ k * 4 = 24
	counts[2] = 4;	// sevn ���� 7 * 4 = 24

	// �ʱ�ȭ
	for (int i = 0; i < 52; i++) {
		card[i] = i;
	}

	// ����
	// ī�� �迭�� �����ͷ� �迭�� �Լ��� ���ڷ� �Ѱ��ش�.
	Shuffle(card);

	// ���� �÷���
	while (true) {
		makeStar();

		cout << gameCount << "��° ����" << endl;

		cout << endl;
		for (int i = 0; i < 4; i++) {
			shape[i] = card[i + turn] / 13; // ���� 0 ~ 3
			number[i] = card[i + turn] % 13; // ���� 0 ~ 12

			// ī�� ī���� �Լ�
			if(i != 3)
				CardCounting(counts, number[i]);

			// ġƮ
			if (i == 3)
				cout << "ġƮ : ";

			// ����� ���� �����ִ� ī�� �Լ�
			ShowCard(shape[i], number[i]);
		}
		cout << endl << endl;

		// ������ �����ְ� ���� �����Ͽ� �Է¹ޱ�
		cout << "������ : " << money;
		
		cout << "\t�����Ͻðڽ��ϱ�? ( ����(1), ����(0) ) : ";
		
		// �ڵ�ȭ�� ���� ������ �����ϵ��� ����
		selectBetting = 1;
		cout << 1 << endl;
		//cin >> selectBetting;
		
		if (selectBetting == 1) {
			// Ȯ�� �����ִ� �Լ�
			ShowCounting(counts);

			cout << "���ð� �Է� : ";
			
			// �ڵ�ȭ�� ���� ������ �� �����ϵ��� ����
			betting = (rand() % money / 1000 + 1) * 1000;
			cout << betting << endl;
			//cin >> betting;

			if (betting < 1000 || betting > money) {
				cout << "���ð� �Է��� �ٽ� ���ּ���" << endl;
				continue;
			}

			cout << "����� �����Ͻðڽ��ϱ�? ( �ο�(0), ����(1), ����(2) ) : ";
			
			// �ڵ�ȭ�� ���� ġƮ�� �ƴ� Ȯ���� �ٰ��Ͽ� �����ϵ��� ����
			selectLHS = AutoSelectLHS(counts);
			cout << endl;
			//cin >> selectLHS;

			if (selectLHS > 2) {
				cout << "�ٽ� �Է����ּ���" << endl;
				continue;
			}

			cout << endl;
			switch (selectLHS) {
			case 0:
				cout << "�ο��� " << betting << " ������ �ϼ̽��ϴ�.";
				break;
			case 1:
				cout << "������ " << betting << " ������ �ϼ̽��ϴ�.";
				break;
			case 2:
				cout << "������ " << betting << " ������ �ϼ̽��ϴ�.";
				break;
			}
			cout << endl;

			// ���� �ڵ�ȭ�� ���� ���� ����
			//Sleep(1000);

			// ���� �ο� ���� üũ

			CardCounting(counts, number[3]);
			cout << "ī�� ���� ";
			ShowCard(shape[3], number[3]);

			// �ο�(0) ����
			if (number[3] < 6 && selectLHS == 0) {
				cout << "�ο� ���� ���� +" << betting << endl;
				money += betting;
			}
			// ����(1) ����
			else if (number[3] > 6 && selectLHS == 1) {
				cout << "���� ���� ���� +" << betting << endl;
				money += betting;
			}
			// ����(2) ����
			else if(number[3] == 6 && selectLHS == 2) {
				cout << "���� ���� ���� +" << betting << " * 13" << endl;
				money += betting * 13; // 13�� ȹ��
			}
			// ����(2) ����
			else if (selectLHS == 2) {
				cout << "���� ���� ���� -" << betting << " * 13" << endl;
				money -= betting * 13;
			}
			else {
				cout << "���� ���� -" << betting << endl;
				money -= betting;
			}

			if (money > hightestMoney) {
				hightestMoney = money;
				hightestGameCount = gameCount;
			}
			
		} 
		else if (selectBetting == 0) {
			cout << "\n������ ���� �մϴ�." << endl;
			cout << "�� ȹ�� �ݾ� : " << money << endl;
			cout << "�ִ� ���� �ݾ� : " << hightestMoney << "(" << hightestGameCount << "��° ����)" << endl;
			break;
		}

		if (money < 1000) {
			cout << "\n" << gameCount << "��° ���ӿ��� �Ļ��Ͽ����ϴ�." << endl;
			cout << "�ִ� ���� �ݾ� : " << hightestMoney << " (" << hightestGameCount << "��° ����)" << endl;
			break;
		}

		if (gameCount == 100) {
			cout << "\n100��° ���ӱ��� ��� ���ҽ��ϴ�." << endl;
			cout << "�ִ� ���� �ݾ� : " << hightestMoney << " (" << hightestGameCount << "��° ����)" << endl;
			break;
		}

		// �ڵ�Ȯ Ȯ���� ���� �ּ�ó��
		//system("cls");

		turn += 4;
		// test
		//cout << turn << "��" << endl;
		// ī�� ��� ���� �� �ٽ� �����Ͽ� ����
		if (turn == 52) {
			turn = 0;
			Shuffle(card);
			// ī��Ʈ �ʱ�ȭ
			counts[0] = 24;
			counts[1] = 24;
			counts[2] = 4;
		}
		gameCount++;
	}
}