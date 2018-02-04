#include "MainGame.h"

MainGame::MainGame()
{
	bool isRead = true;

	// ���� �Ҵ� �� �ʱ�ȭ
	m_shop = new Shop2;
	isRead = m_shop->ItemSetup();

	m_player = new Player2;
	m_player->InputPlayer();

	m_gold = 10000;
	m_selectNum = 1;
	m_selectPtr = 0;

	if (isRead == true) {
		Sleep(1000);
		SetMainPage();
	}
}


MainGame::~MainGame()
{
	delete m_shop;
	delete m_player;
}

void MainGame::SetLocation(int number)
{
	switch (number) {
	case LOCATION_SHOP:
		SetShopPage();
		break;
	case LOCATION_SELL_INVENTORY:
		SetSellInventoryPage();
		break;
	case LOCATION_INVENTORY:
		SetInventoryPage();
		break;
	default:
		SetMainPage();
		break;
	}
}

void MainGame::SetMainPage()
{
	system("cls");
	cout << "================== ��ġ ==================" << endl;
	if (m_selectNum == 1) { SetColor(SET_COLOR); }
	cout << "����";
	SetDefaultColor();
	cout << " ";
	if (m_selectNum == 2) { SetColor(SET_COLOR); }
	cout << "�κ��丮[�Ǹ�]";
	SetDefaultColor();
	cout << " ";
	if (m_selectNum == 3) { SetColor(SET_COLOR); }
	cout << "�κ��丮[����]" << endl;
	SetDefaultColor();
	//cin >> m_selectNum;

	cout << "============ �����Ͻ÷��� ESC ============" << endl;

	m_selectPtr = getch();
	// test
	//printf("%d\n", m_selectPtr);
	switch (m_selectPtr) {
		// ����
	case ENTER:
	case SPACE:
		SetLocation(m_selectNum);
		break;
		// ����
	case ESC:
		break;
	case LEFT_ARROW:
		m_selectNum--;
		if (m_selectNum < 1)
			m_selectNum = 1;
		SetMainPage();
		break;
	case RIGHT_ARROW:
		m_selectNum++;
		if (m_selectNum > 3)
			m_selectNum = 3;
		SetMainPage();
		break;
	default:
		SetMainPage();
		break;
	}
}

void MainGame::SetShopPage()
{
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");
		cout << "==================== ���� =================== " << endl;
		if (m_selectNum == 1) { SetColor(SET_COLOR); }
		cout << "��";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 2) { SetColor(SET_COLOR); }
		cout << "����";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 3) { SetColor(SET_COLOR); }
		cout << "�Ǽ�����";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 4) { SetColor(SET_COLOR); }
		cout << "����" << endl;
		SetDefaultColor();
		cout << "======== ���� ȭ������ �������� ESC ========" << endl;

		//cin >> m_selectNum;
		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else {
		//	SetShopItemPage(m_selectNum);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// ����
		case ENTER:
		case SPACE:
			SetShopItemPage(m_selectNum);
			break;
			// ����
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case LEFT_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case RIGHT_ARROW:
			m_selectNum++;
			if (m_selectNum > 4)
				m_selectNum = 4;
			break;
		default:
			break;
		}
	}
}

void MainGame::SetShopItemPage(int itemKind)
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");

		// ���� ���� �������� ���
		itemCount = m_shop->ItemOutput(itemKind, m_selectNum);

		cout << "���� gold : " << m_gold << endl;
		cout << "������ �����ۿ��� ENTER or SPACE" << endl;
		cout << "itemCount : " << itemCount << endl;
		cout << "===== ���� �޴��� ������ ����ϸ� ESC =====" << endl;
		//cin >> m_selectNum;
		//if (m_selectNum == 0)
		//	break;
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "�߸� �Է�" << endl;
		//}
		//else {
		//	tagItemInfo buyItem
		//		= m_shop->BuyItem(itemKind, m_selectNum, m_gold);
		//	m_player->AddItem(buyItem);
		//	Sleep(1000);
		//}
		tagItemInfo buyItem;
		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// ����
		case ENTER:
		case SPACE:
			buyItem = m_shop->BuyItem(itemKind, m_selectNum, m_gold);
			m_player->AddItem(buyItem);
			Sleep(1000);
			break;
			// ����
		case ESC:
			m_selectNum = 1;
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "�߸� �Է�" << endl;
			break;
		}
	}
}

void MainGame::SetSellInventoryPage()
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");
		cout << "================= �κ��丮 ===============" << endl;
		cout << "���� gold : " << m_gold << endl;
		cout << "�Ǹ��� �����ۿ��� ENTER or SPACE" << endl;
		cout << "============= ���� ȭ���� ESC ============" << endl;

		itemCount = m_player->ShowInventory(m_selectNum);

		//cin >> m_selectNum;

		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "�߸� �Է�" << endl;
		//}
		//else {
		//	m_shop->AddItem(m_player->SellItem(m_selectNum, m_gold));
		//	Sleep(1000);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// ����
		case ENTER:
		case SPACE:
			m_shop->AddItem(m_player->SellItem(m_selectNum, m_gold));
			Sleep(1000);
			break;
			// ����
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "�߸� �Է�" << endl;
			break;
		}
	}
}

void MainGame::SetInventoryPage()
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;

	while (isDone) {
		system("cls");
		m_player->ShowStatus();
		cout << "=============== �κ��丮 ==============" << endl;
		cout << "������ �����ۿ��� ENTER or SPACE" << endl;
		cout << "============ ����ȭ���� ESC ===========" << endl;
		// show Inventory
		itemCount = m_player->ShowInventory(m_selectNum);

		//cin >> m_selectNum;

		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "�߸��� �Է�" << endl;
		//}
		//else {
		//	m_player->EquipItem(m_selectNum);
		//	Sleep(1000);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// ����
		case ENTER:
		case SPACE:
			m_player->EquipItem(m_selectNum);
			Sleep(1000);
			break;
			// ����
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "�߸� �Է�" << endl;
			break;
		}
	}
}
