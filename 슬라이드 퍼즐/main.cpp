#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>

using namespace std;

void shuffle(int* puzzle) {
	// ����
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int dest = rand() % 8;
		int sour = rand() % 8;

		int temp = puzzle[dest];
		puzzle[dest] = puzzle[sour];
		puzzle[sour] = temp;
	}
}

// ���÷� ���� ����� �Լ�
void makePuzzle(int* puzzle) {
	shuffle(puzzle);
}

void makeLine() {
	for (int i = 0; i < 35; i++) {
		cout << "=";
	}
	cout << endl;
}

void showPuzzle(int* puzzle, int gameCount) {
	makeLine();
	cout << "\t   �����̵� ����" << endl;
	makeLine();

	cout << endl;
	// ������ �����ֱ�
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)
			cout << "\t";

		if (puzzle[i] == -1)
			cout << "X\t";
		else
			cout << puzzle[i] << "\t";
		if ((i + 1) % 3 == 0)
			cout << "\n\n";
	}
	cout << endl;

	makeLine();
	cout << "\t\t������ Ƚ�� : " << gameCount << endl;
	cout << endl;
	//makeLine();
}

// X�� ��ġ�� ã���ִ� �Լ�
int findX(int* puzzle) {
	for (int i = 0; i < 9; i++) {
		if (puzzle[i] == -1)
			return i;
	}
}

// X�� �������ִ� �Լ�
void movePuzzle(char move, int* puzzle, int* moveCount) {
	int x = findX(puzzle);

	/*
		0	1	2
		3	4	5
		6	7	8
	
		4 ���� 
		���� -3
		�Ʒ��� +3
		���� -1
		������ +1

		���� ���� �ȵǰ�
	*/

	switch (move) {
	case 'w':
	case 'W':
		//cout << "Up" << endl;
		if (x > 2)
			swap(puzzle[x], puzzle[x - 3]);
		*moveCount += 1;
		break;
	case 's':
	case 'S':
		//cout << "Down" << endl;
		if (x < 6)
			swap(puzzle[x], puzzle[x + 3]);
		*moveCount += 1;
		break;
	case 'a':
	case 'A':
		//cout << "Left" << endl;
		if (x % 3 != 0)
			swap(puzzle[x], puzzle[x - 1]);
		*moveCount += 1;
		break;
	case 'd':
	case 'D':
		//cout << "Right" << endl;
		if (x % 3 != 2)
			swap(puzzle[x], puzzle[x + 1]);
		*moveCount += 1;
		break;
	default:
		break;
	}
}

// �����ϰ� �̵��ϴ� ������� ���� ����� �Լ�
void makePuzzle2(int* puzzle, char* moveArray, int* moveCount) {
	srand(time(NULL));
	int move = rand() % 11 + 5; // ��� �̵����� ������ ������ �������� ����

	for (int i = 0; i < move; ) {
		int direction = rand() % 4;
		switch (direction) {
		case 0:
			// up
			// �� ���� ��ġ�� ���� ����
			if (findX(puzzle) <= 2)
				continue;
			moveArray[*moveCount] = 's';
			movePuzzle('w', puzzle, moveCount);
			break;
		case 1:
			// down
			// �� �Ʒ��� ��ġ�� ��� ����
			if (findX(puzzle) >= 6)
				continue;
			moveArray[*moveCount] = 'w';
			movePuzzle('s', puzzle, moveCount);
			break;
		case 2:
			// left
			// �� ���ʿ� ��ġ�� ��� ����
			if (findX(puzzle) % 3 == 0)
				continue;
			moveArray[*moveCount] = 'd';
			movePuzzle('a', puzzle, moveCount);
			break;
		case 3:
			// right;
			// �� �����ʿ� ��ġ�� ��� ����
			if (findX(puzzle) % 3 == 2)
				continue;
			moveArray[*moveCount] = 'a';
			movePuzzle('d', puzzle, moveCount);
			break;
		}
		i++;
		//showPuzzle(puzzle, *moveCount);
	}
}

void showCheat(char* moveArray, int moveCount, char* selectArray) {
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	cout << " ġƮ (�� " << moveCount <<"�� �����ϰ� ��������)" << endl;
	for (int i = 0; i < moveCount; i++) {
		if (moveArray[moveCount - 1 - i] == selectArray[i]) {
			//continue;
			SetConsoleTextAttribute(hC, 10);	// ġƮ��� ģ ��� �ʷϻ����� ����
		}
		cout << moveArray[moveCount - 1 - i] << " ";
		SetConsoleTextAttribute(hC, 7);	// ������ �κ��� ���� ������
	}
	cout << endl;
	makeLine();
}

// ������ Ǯ������ üũ�ϴ� �Լ�
bool checkPuzzle(int* puzzle, int* number) {
	for (int i = 0; i < 9; i++) {
		if (puzzle[i] != number[i])
			return false;
	}
	return true;
}

void main() {
	int number[9];
	int puzzle[9];
	char moveArray[25];		// ������ ����鼭 ������ ������ ���� �迭
	int moveCount = 0;			// ������ ����鼭 �������� Ƚ��
	
	char selectMove;
	char selectArray[50] = { 0 };	// ������ �����ϸ鼭 �������� ������ ���� �迭
	int gameCount = 0;		// ������ Ƚ��

	// ���� �ʱ�ȭ
	for (int i = 0; i < 9; i++) {
		if (i == 8)
			number[i] = -1;
		else
			number[i] = i + 1;
		puzzle[i] = number[i];
	}

	//makePuzzle(puzzle, number);
	makePuzzle2(puzzle, moveArray, &moveCount);

	while (true) {
		showPuzzle(puzzle, gameCount);

		// ġƮ
		showCheat(moveArray, moveCount, selectArray);

		if (checkPuzzle(puzzle, number)) {
			cout << "\t   ���� Ŭ����!!\n" << endl;
			cout << "\t\t�� ������ Ƚ�� : " << gameCount << endl;
			makeLine();
			break;
		}

		if (gameCount == 49) {
			cout << "\t   ���� ����\n" << endl;
			cout << "\t�� �̻� ������ �� �����ϴ�." << endl;
			makeLine();
			break;
		}

		char move = getch();
		selectArray[gameCount] = move;
		movePuzzle(move, puzzle, &gameCount);

		system("cls");
	}
}