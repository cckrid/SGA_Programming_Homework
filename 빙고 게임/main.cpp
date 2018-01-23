#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

typedef struct Storage {
	int board[5][5];
	int bingo;
	int gameCount;
} Store;

// ������ ����ϴ� �Լ�
// 2���� �迭 �����ͷ� ��������
void showBoard(int board[5][5]);
// ������ �����ϴ� �Լ�
void changeBoard(int board[5][5], int number);

// ���� ����Ȯ�� �ϴ� �Լ�
int checkBoard(int board[5][5]);
// �����ǿ� ���ڰ� �ִ��� Ȯ���ϴ� �Լ�
bool checkNumber(int board[5][5], int number);

// ���� ���¸� ����� ����ü�� �����ϴ� �Լ�
void saveBoard(Store* store, int* storeSize, int bingo, int board[5][5], int gameCount);
// ���� ���¸� ����� ����ü�κ��� �о���� �Լ�
void loadBoard(Store* store, int storeSize);

// 2���� �迭 ����� �ݿ��Ǵ��� �׽�Ʈ
// 2���� �迭 ����� �ݿ���
void testBoard(int board[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = 0;
		}
	}
}

void main() {
	int board[5][5];
	int number[25];

	bool isBoardNumber; // �����ǿ� ���ڰ� �ִ��� Ȯ���ϴ� ����

	int selectNumber;

	int bingo = 0;
	int oldBingo = 0; // ���� ������ �����ϴ� ����
	int gameCount = 0;

	Store store[5];
	int storeSize = 0;

	// ���� �ʱ�ȭ
	for (int i = 0; i < 25; i++)
		number[i] = i + 1;

	// ����
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int sour = rand() % 25;
		int dest = rand() % 25;

		int temp = number[dest];
		number[dest] = number[sour];
		number[sour] = temp;
	}

	// ���õ� ���� �����ǿ� �Է�
	int cnt = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = number[cnt];
			cnt++;
		}
	}

	// ������ ����ϴ� �Լ�
	//showBoard(board);

	// 2���� �迭 �ѱ�� ���ϴ��� �׽�Ʈ
	//testBoard(board);
	//showBoard(board);
	
	// ���� ����
	while (true) {
		system("cls");

		cout << gameCount << "��° ����" << endl;

		// ���� �Է�
		cout << "���ڸ� �Է��ϼ��� : ";
		// �ڵ�ȭ�� ���� gameCount+1�� �׳� �־���
		selectNumber = gameCount + 1;
		//cin >> selectNumber;
		
		// ���� �Է� ���� ó��
		if (selectNumber > 26 || selectNumber <= 0) {
			cout << "�� �� �Է��߽��ϴ�. �ٽ� �Է��ϼ���" << endl;
			Sleep(1000);
			continue;
		}

		isBoardNumber = checkNumber(board, selectNumber);
		if (isBoardNumber == false) {
			cout << "�̹� �Էµ� �����Դϴ�. �ٽ� �Է��ϼ���" << endl;
			Sleep(1000);
			continue;
		}

		// ������ ����
		changeBoard(board, selectNumber);

		// ������ ���
		cout << endl;
		showBoard(board);
		cout << endl;

		// ���� Ȯ��
		oldBingo = bingo;
		bingo = checkBoard(board);

		if (bingo >= 5) {
			cout << "5 ���� �ϼ�, ���� �¸�" << endl;
			
			// �����صξ��� ���� ���� �о����
			loadBoard(store, storeSize);

			break;
		}
		else if (bingo != oldBingo) {
			cout << bingo << " ���� �ϼ�, " << 5 - bingo << " ���� ���ҽ��ϴ�." << endl;
			Sleep(1000);	// �ڵ����� ������ sleep ���ȿ��� �������̶� ���� ���� �Ⱥ��̴°�

			// ��� ���� ���߱����� �Է°� ��ٸ��� ���
			//getchar(); // endl �����־ �ѹ��� �Է¹�����
			//getchar();

			// ���� ��ȭ�� ������ �����ǰ� ���� Ƚ�� �����ϱ�
			saveBoard(store, &storeSize, bingo, board, gameCount);
		}

		gameCount++;
	}
}
// ������ ����ϴ� �Լ�
void showBoard(int board[5][5]) {

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (board[i][j] < 10)
				cout << " ";

			// �̹� �Էµ� ���ڴ� -1�� ����Ǿ������Ƿ� X�� ���
			if (board[i][j] == -1) 
				cout << "X" << " ";
			else
				cout << board[i][j] << " ";
		}
		cout << endl;
	}
}
// ������ �����ϴ� �Լ�
void changeBoard(int board[5][5], int number) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			// �Էµ� ���ڸ� ã�� -1�� ����
			if (number == board[i][j])
				board[i][j] = -1;
		}
	}
}
// ���� ����Ȯ�� �ϴ� �Լ�
int checkBoard(int board[5][5]) {
	int rowSum, colSum, diagSum1, diagSum2;
	rowSum = colSum = diagSum1 = diagSum2 = 0;

	int bingo = 0;

	// �� ����� �밢�� ���� Ȯ��
	for (int i = 0; i < 5; i++) {
		colSum = 0;
		for (int j = 0; j < 5; j++) {
			colSum += board[i][j];
			if (i == j)
				diagSum1 += board[i][j];
		}
		// -1�� �� �Էµ� �� 5���� ��� ���� ��� -5�̹Ƿ� ����
		if (colSum == -5)
			bingo++;
	}

	if (diagSum1 == -5)
		bingo++;

	// �� ����� ���밢�� ���� Ȯ��
	for (int j = 0; j < 5; j++) {
		rowSum = 0;
		for (int i = 0; i < 5; i++) {
			rowSum += board[i][j];
			if (i + j == 4)
				diagSum2 += board[i][j];
		}
		if (rowSum == -5)
			bingo++;
	}

	if (diagSum2 == -5)
		bingo++;

	return bingo;
}
// �����ǿ� ���ڰ� �ִ��� Ȯ���ϴ� �Լ�
bool checkNumber(int board[5][5], int number) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			// ���ڰ� ������ true ����
			if (board[i][j] == number) {
				return true;
			}
		}
	}
	// ���ڰ� ������ false ����
	return false;
}

// ���� ���¸� ����� ����ü�� �����ϴ� �Լ�
void saveBoard(Store* store, int* storeSize, int bingo, int board[5][5], int gameCount) {
	// ����ҿ� ���� ���� ����
	store[*storeSize].bingo = bingo;

	// ����ҿ� ������ ����
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			store[*storeSize].board[i][j] = board[i][j];
		}
	}

	// ����ҿ� �����϶� ������ �� ����
	store[*storeSize].gameCount = gameCount;

	// ����� �ε��� ����
	*storeSize = *storeSize + 1;
}

// ���� ���¸� ����� ����ü�κ��� �о���� �Լ�
void loadBoard(Store* store, int storeSize) {
	for (int i = 0; i < 35; i++)
		cout << "*";
	cout << endl;

	cout << "���� ���ǥ" << endl;

	for (int i = 0; i < storeSize; i++) {
		cout << endl;
		cout << store[i].gameCount << "��° ����" << endl;

		cout << endl;
		showBoard(store[i].board);
		cout << endl;

		cout << store[i].bingo << " ���� �ϼ�, " << 5 - store[i].bingo << " ���� ����" << endl;
	}

	for (int i = 0; i < 35; i++)
		cout << "*";
	cout << endl;
}