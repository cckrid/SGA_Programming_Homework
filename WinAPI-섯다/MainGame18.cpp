#include "stdafx.h"
#include "MainGame18.h"


MainGame18::MainGame18()
{
}


MainGame18::~MainGame18()
{
}

HRESULT MainGame18::Init()
{
	GameNode::Init();

	logo = new Image;
	logo->Init("images/logo.bmp", WINSIZEX / 2 , WINSIZEY / 2);

	card = new Image;
	card->Init("images/card.bmp", WINSIZEX / 4 * 3 - 70, WINSIZEY / 4 + 20, 10, 2);


	for (int i = 0; i < BG_SIZE; i++) {
		bg[i] = new Image;
		if (i == 24)
			sprintf_s(str, "images/betting.bmp");
		else
			sprintf_s(str, "images/shuffle%d.bmp", i + 1);
		bg[i]->Init(str, WINSIZEX, WINSIZEY);
	}

	for (int i = 0; i < JOKBO_SIZE; i++) {
		jokbo[i] = new Image;
		// 38 ����
		if (i == 0)
			sprintf_s(str, "images/jokbo (%d).bmp", 1);
		// 18 13 ����
		else if (i <= 2)
			sprintf_s(str, "images/jokbo (%d).bmp", 2);
		// ��
		else if (i <= 12)
			sprintf_s(str, "images/jokbo (%d).bmp", 3);
		// ���� ������
		else if (i <= 19)
			sprintf_s(str, "images/jokbo (%d).bmp", i % 10 + 1);
		// ��, ����
		else if(i <= 28)
			sprintf_s(str, "images/jokbo (%d).bmp", 11);
		// �籸, ������, ������
		else
			sprintf_s(str, "images/jokbo (%d).bmp", i-17);
		jokbo[i]->Init(str, WINSIZEX / 4 - 25, WINSIZEY / 4);
	}

	// logo�� �������� ����
	isLogo = true;
	// logo�� ���� 0�� �Ⱥ��̴°� 255 �ٺ��̴°�
	alpha = 0;

	// �÷��̾�
	// int �� ���� �Ѿ�Ƿ� long long (�� ū ������ ���� ���)
	// ���� int �� 2^32 - 1 ���� �� 10�� �ٵ� �̰� 9999�� ������
	player[0].bet = 999343891;
	player[0].name = (char*)"�����o (��)";
	// test
	//player[0].card1 = CARD_1G;
	//player[0].card2 = CARD_7B;

	// test2
	//for (int i = 0; i < 5; i++) {
	//	player[i].card1 = (CARD)RND->GetInt(20);
	//	player[i].card2 = (CARD)RND->GetInt(20);
	//}

	player[1].bet = 1717448900;
	player[1].name = (char*)"õ���ȣ";
	player[2].bet = 4007564530;
	player[2].name = (char*)"�䳢�Ͱźϴ�";
	player[3].bet = 295010000;
	player[3].name = (char*)"�ܹ�22";
	player[4].bet = 2436894732;
	player[4].name = (char*)"���콺0";

	// ���� ����
	StartGame();

	// ������ �������� Ȯ��
	isOver = false;

	nextBtn = RectMakeCenter(712, 732, 80, 50);

	return S_OK;
}

void MainGame18::Release()
{
	GameNode::Release();
	SAFE_DELETE(logo);
	SAFE_DELETE(card);
	for (int i = 0; i < BG_SIZE; i++) {
		SAFE_DELETE(bg[i]);
	}
	for (int i = 0; i < JOKBO_SIZE; i++) {
		SAFE_DELETE(jokbo[i]);
	}
}

void MainGame18::Update()
{
	GameNode::Update();

	// ó�� ���۽� logo �κ� ���̱� ���� ���
	if (isLogo && alpha != 255)
		alpha += 1;
	else {
		isLogo = false;
	}

	if (isLogo) return;

	// ��� ȭ�� �ִϸ��̼� ����
	if (moveFrame < BG_SIZE - 1)
		moveFrame += 0.1f;
	// �ִϸ��̼��� ������ ī�� ���� ��
	else {
		isShuffle = true;
		isJokbo = true;
		if (!isOver)
			EndGame();
	}

	if (INPUT->GetKeyDown(VK_LBUTTON) && isOver)
	{
		if (PtInRect(&nextBtn, g_ptMouse)) {
			StartGame();
		}
	}

	// �ִϸ��̼� test �κ�
	{
		// �������� �׽�Ʈ
		//if (INPUT->GetKeyDown(VK_CONTROL)) { isCombi = !isCombi; }

		//if (INPUT->GetKeyDown(VK_SPACE)) { moveFrame += 1.0f; }

		// BG test
		//if(moveFrame < BG_SIZE-1)
		//	moveFrame += 0.1f;

		// COMBI test
		//if (moveFrame < COMBI_SIZE - 1)
		//	moveFrame += 0.1f;

		//// card test
		//if (moveFrame < 20)
		//	moveFrame += 0.1f;
	}
}

void MainGame18::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	// �ִϸ��̼� ���� test
	{
		//logo->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		// logo test
		//if(isLogo)
		//	logo->AlphaRender(memDC, WINSIZEX / 4, WINSIZEY / 4, alpha);
		//else
		//	bg[24]->Render(memDC);

		//card->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		//bg[24]->Render(memDC);
		//// card 1 test
		//card->FrameRender(memDC, 
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6, WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int) moveFrame / 10);
		//// card 2 test
		//card->FrameRender(memDC,
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6 + card->GetFrameWidth(), WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int)moveFrame / 10);
		//bg[(int)moveFrame]->Render(memDC);
		//if(isCombi)
		//	combi[(int)moveFrame]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);

		// mouse x y
		//sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		//TextOut(memDC, 10, 10, str, strlen(str));

		// other card test
		/*card->FrameRender(memDC,
			38, 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			38 + card->GetFrameWidth(), 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			8, 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			8 + card->GetFrameWidth(), 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			864, 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			864 + card->GetFrameWidth(), 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			1017 - card->GetFrameWidth() * 2, 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			1017 - card->GetFrameWidth(), 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);*/

		// money text test
		// player
		//sprintf_s(str, "%4d�� %4d�� %4d��", 1234, 5678, 9999);
		//TextOut(memDC, 324, 161, str, strlen(str));

		//sprintf_s(str, "9999�� 9999�� 9999��");
		//TextOut(memDC, 58, 237, str, strlen(str));

		//sprintf_s(str, "9999�� 9999�� 9999��");
		//TextOut(memDC, 29, 440, str, strlen(str));

		//sprintf_s(str, "9999�� 9999�� 9999��");
		//TextOut(memDC, 832, 237, str, strlen(str));

		//sprintf_s(str, "9999�� 9999�� 9999��");
		//TextOut(memDC, 862, 439, str, strlen(str));

		//// betting text test
		//sprintf_s(str, "%d����", 1234);
		//TextOut(memDC, 485, 501, str, strlen(str));

		//sprintf_s(str, "�⺻�Ӵ� %d����", 1000);
		//TextOut(memDC, 448, 550, str, strlen(str));

		// text test
		/*sprintf_s(str, "���ȱ���");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 5,
			str, strlen(str));

		sprintf_s(str, " �� �� �� ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 65,
			str, strlen(str));
		
		sprintf_s(str, "   ��  ��   ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 115,
			str, strlen(str));*/
	}

	{
		// �ΰ� �κ� �׷��ֱ�
		if(isLogo)
			logo->AlphaRender(memDC, WINSIZEX / 4, WINSIZEY / 4, alpha);
		// �ΰ� �κ� ������ ���� ���� �κ� �׸���
		else {
			// ��� ȭ�� �׸���
			bg[(int)moveFrame]->Render(memDC);
			
			// ���þ� �׸���
			{
				// player�� ���þ�
				// ����
				sprintf_s(str, "%4d�� %4d�� %4d��",
					// �� �ڸ�
					player[0].bet / 100000000,
					// �� �ڸ�
					(player[0].bet / 10000) % 10000,
					// �� �ڸ�
					player[0].bet % 10000);
				TextOut(memDC, 324, 161, str, strlen(str));

				sprintf_s(str, "%4d�� %4d�� %4d��",
					// �� �ڸ�
					player[1].bet / 100000000,
					// �� �ڸ�
					(player[1].bet / 10000) % 10000,
					// �� �ڸ�
					player[1].bet % 10000);
				TextOut(memDC, 58, 237, str, strlen(str));

				sprintf_s(str, "%4d�� %4d�� %4d��",
					// �� �ڸ�
					player[2].bet / 100000000,
					// �� �ڸ�
					(player[2].bet / 10000) % 10000,
					// �� �ڸ�
					player[2].bet % 10000);
				TextOut(memDC, 29, 440, str, strlen(str));

				sprintf_s(str, "%4d�� %4d�� %4d��",
					// �� �ڸ�
					player[3].bet / 100000000,
					// �� �ڸ�
					(player[3].bet / 10000) % 10000,
					// �� �ڸ�
					player[3].bet % 10000);
				TextOut(memDC, 832, 237, str, strlen(str));

				sprintf_s(str, "%4d�� %4d�� %4d��",
					// �� �ڸ�
					player[4].bet / 100000000,
					// �� �ڸ�
					(player[4].bet / 10000) % 10000,
					// �� �ڸ�
					player[4].bet % 10000);
				TextOut(memDC, 862, 439, str, strlen(str));

				// ���þ� �κ�
				sprintf_s(str, "%d����", tableBet);
				TextOut(memDC, 485, 501, str, strlen(str));

				sprintf_s(str, "�⺻�Ӵ� %d����", baseBet);
				TextOut(memDC, 448, 550, str, strlen(str));
			}

			// ī�� �׸���
			if (isShuffle) {
				// ���� ī��
				// card 1
				card->FrameRender(memDC, 
					WINSIZEX / 2 - WINSIZEX / 8 - 6, WINSIZEY - WINSIZEY / 6 - 13,
					player[0].card1 % 10, player[0].card1 / 20);
				// card 2
				card->FrameRender(memDC,
					WINSIZEX / 2 - WINSIZEX / 8 - 6 + card->GetFrameWidth(), WINSIZEY - WINSIZEY / 6 - 13,
					player[0].card2 % 10, player[0].card2 / 20);

				// ������ �÷��̾�
				card->FrameRender(memDC,
				38, 216 - card->GetFrameHeight(),
					player[1].card1 % 10, player[1].card1 / 20);
				card->FrameRender(memDC,
				38 + card->GetFrameWidth(), 216 - card->GetFrameHeight(),
					player[1].card2 % 10, player[1].card2 / 20);

				card->FrameRender(memDC,
				8, 417 - card->GetFrameHeight(),
					player[2].card1 % 10, player[2].card1 / 20);
				card->FrameRender(memDC,
				8 + card->GetFrameWidth(), 417 - card->GetFrameHeight(),
					player[2].card2 % 10, player[2].card2 / 20);

				card->FrameRender(memDC,
				864, 217 - card->GetFrameHeight(),
					player[3].card1 % 10, player[3].card1 / 20);
				card->FrameRender(memDC,
				864 + card->GetFrameWidth(), 217 - card->GetFrameHeight(),
					player[3].card2 % 10, player[3].card2 / 20);

				card->FrameRender(memDC,
				1017 - card->GetFrameWidth() * 2, 415 - card->GetFrameHeight(),
					player[4].card1 % 10, player[4].card1 / 20);
				card->FrameRender(memDC,
				1017 - card->GetFrameWidth(), 415 - card->GetFrameHeight(),
					player[4].card2 % 10, player[4].card2 / 20);
			}

			// ���� �׸���
			if (isJokbo) {
				// ���� ����
				switch (player[0].jokbo)
				{
				case JB_38G:
					sprintf_s(str, "���ȱ���");
					jokbo[0]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_18G:
					sprintf_s(str, "���ȱ���");
					jokbo[1]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_13G:
					sprintf_s(str, "�ϻﱤ��");
					jokbo[1]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_1010:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_99:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_88:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_77:
					sprintf_s(str, "   ĥ  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_66:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_55:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_44:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_33:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_22:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_11:
					sprintf_s(str, "   ��  ��   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_12:
					sprintf_s(str, "   ��  ��   ");
					jokbo[3]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_14:
					sprintf_s(str, "   ��  ��   ");
					jokbo[4]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_91:
					sprintf_s(str, "   ��  ��   ");
					jokbo[5]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_101:
					sprintf_s(str, "   ��  ��   ");
					jokbo[6]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_104:
					sprintf_s(str, "   ��  ��   ");
					jokbo[7]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_46:
					sprintf_s(str, "   ��  ��   ");
					jokbo[8]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_9:
					sprintf_s(str, "   ��  ��   ");
					jokbo[9]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_8:
					sprintf_s(str, " �� �� �� ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_7:
					sprintf_s(str, " �� �� �� ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_6:
					sprintf_s(str, " �� �� �� ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_5:
					sprintf_s(str, " �� �� �� ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_4:
					sprintf_s(str, "   ��  ��   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_3:
					sprintf_s(str, "   ��  ��   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_2:
					sprintf_s(str, "   ��  ��   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_1:
					sprintf_s(str, "   ��  ��   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_0:
					sprintf_s(str, "   ��  ��   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_49:
					sprintf_s(str, "   ��  ��   ");
					jokbo[11]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_NNC:
					sprintf_s(str, " �� �� �� ");
					jokbo[12]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_GC:
					sprintf_s(str, "������");
					jokbo[13]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				}

				TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 115,
					str, strlen(str));
			}

			// ���� �׸���
			if (isOver) {
				HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
					VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
				HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

				int pad = 25;

				if (winnerCount > 1) {
					sprintf_s(str, "���º�");
					TextOut(memDC, WINSIZEX / 2 - 35, WINSIZEY / 2, str, strlen(str));

					sprintf_s(str, "+%d����", baseBet);
					TextOut(memDC, 324, 161 + pad, str, strlen(str));
					TextOut(memDC, 58, 237 + pad, str, strlen(str));
					TextOut(memDC, 29, 440 + pad, str, strlen(str));
					TextOut(memDC, 832, 237 + pad, str, strlen(str));
					TextOut(memDC, 862, 439 + pad, str, strlen(str));
				}
				else {
					for (int i = 0; i < 5; i++) {
						if (player[i].isWin) {
							sprintf_s(str, "%s �¸�", player[i].name);
							TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 12, WINSIZEY / 2, 
								str, strlen(str));
							switch (i) {
							case 0:
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d����", tableBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 1:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d����", tableBet);
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 2:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d����", tableBet);
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 3:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d����", tableBet);
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 4:
								sprintf_s(str, "-%d����", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d����", tableBet);
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							}
						}
					}
				}
				
				// �ٽ� �������
				SetTextColor(memDC, RGB(0, 0, 0));

				// ������ ������ Next ��ư �׸���
				RectangleMake(memDC, nextBtn);

				sprintf_s(str, "Next");
				TextOut(memDC, 685, 720, str, strlen(str));

				SelectObject(memDC, OldFont);
				DeleteObject(hFont);
			}
		}
	
		// test
		sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(memDC, 10, 10, str, strlen(str));
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame18::StartGame()
{

	// ������ ���� ����
	moveFrame = 0;
	// ī�尡 ���� �Ǿ����� Ȯ���ϴ� ���� (ī�尡 �� ����������)
	isShuffle = false;
	// �ش� ������ ������ ����
	isJokbo = false;
	// ���� ������ �ʱ�ȭ
	isOver = false;

	// ��� �й�� ����
	for (int i = 0; i < 5; i++) {
		player[i].isWin = false;
	}

	// ���þ�
	baseBet = 1000;
	tableBet = 0;
	for (int i = 0; i < 5; i++) {
		player[i].bet -= baseBet;
		tableBet += baseBet;
	}

	// ī�� ����
	Shuffle();
	// ���� ī�� �����ֱ�
	// ī�� �׽�Ʈ
	//player[0].card1 = CARD_4A;
	//player[0].card2 = CARD_9A;
	//for (int i = 1; i < 5; i++) {
	//	player[i].card1 = cardNum[i];
	//	player[i].card2 = cardNum[i + 5];
	//}

	for (int i = 0; i < 5; i++) {
		player[i].card1 = cardNum[i];
		player[i].card2 = cardNum[i + 5];
	}

	// ���� üũ
	for (int i = 0; i < 5; i++) {
		player[i].jokbo = CheckJokbo(player[i].card1, player[i].card2);
	}
}

void MainGame18::EndGame()
{
	// ���� ���
	// Ư�� ���� ���
	for (int i = 0; i < 5; i++) {
		// �籸
		if (player[i].jokbo == JB_49) {
			// ���ºη� ó�� ���ºδ� ��� win���� ǥ��
			for (int j = 0; j < 5; j++) {
				player[j].isWin = true;
			}
			break;
		}
		// ������
		if (player[i].jokbo == JB_NNC) {
			for (int j = 0; j < 5; j++) {
				// �ڽ� ����
				if (i == j) continue;
				// ���� �ִ� ���
				if (player[j].jokbo >= JB_1010 && player[j].jokbo <= JB_11) {
					// �̱�
					player[i].isWin = true;
					break;
				}
			}
			// ���� ��ã�� ��� �������� ����
			if (player[i].isWin == false) {
				player[i].jokbo = JB_0;
			}
			else
				break;
		}
		// ������
		if (player[i].jokbo == JB_GC) {
			for (int j = 0; j < 5; j++) {
				// �ڽ� ����
				if (i == j) continue;
				// ������ �ִ� ��� 38���� ����
				if (player[j].jokbo > JB_38G && player[j].jokbo <= JB_13G) {
					// �̱�
					player[i].isWin = true;
					break;
				}
			}
			// ������ ��ã�� ��� �Ѳ����� ����
			if (player[i].isWin == false) {
				player[i].jokbo = JB_1;
			}
			else
				break;
		}
	}

	// Ư�� ������ ���� �̰���� Ȯ��
	isOver = false;
	for (int i = 0; i < 5; i++) {
		if (player[i].isWin == true) {
			isOver = true;
			break;
		}
	}
	// ������ ���� ��� ���� ���
	if (!isOver) {
		// ���� ���ǹǷ� ����
		isOver = true;
		box = JB_0;
		for (int i = 0; i < 5; i++) {
			if (box > player[i].jokbo) {
				box = player[i].jokbo;
			}
		}
		for (int i = 0; i < 5; i++) {
			if (box == player[i].jokbo) {
				player[i].isWin = true;
			}
		}
	}
	
	// ���� ��� ���þ� ���
	// ���º� üũ
	winnerCount = 0;;
	// ����� ����� �� �̻��̸� ���º� ó�� 
	for (int i = 0; i < 5; i++) {
		if (player[i].isWin == true) {
			winnerCount++;
		}
	}
	// ���º��� ��� ���þ� �����ֱ�
	if (winnerCount > 1) {
		for (int i = 0; i < 5; i++) {
			player[i].bet += baseBet;
			tableBet -= baseBet;
		}
	}
	// �ºΰ� �� ��� ���ڿ��� ���þ� �����ֱ�
	else {
		for (int i = 0; i < 5; i++) {
			if (player[i].isWin == true) {
				player[i].bet += tableBet;
				//tableBet = 0;
			}
		}
	}
}

void MainGame18::Shuffle()
{
	int dest, sour;
	CARD temp;

	// �ʱ�ȭ
	for (int i = 0; i < 20; i++)
		cardNum[i] = CARD(i);

	for (int i = 0; i < 1000; i++) {
		dest = RND->GetInt(20);
		sour = RND->GetInt(20);

		temp = cardNum[dest];
		cardNum[dest] = cardNum[sour];
		cardNum[sour] = temp;
	}
}

JOKBO MainGame18::CheckJokbo(CARD card1, CARD card2)
{
	CARD temp;
	// ���� ī����ڰ� card1�� ���� �� swap -> ��� : card1 < card2
	if (card1 > card2) {
		temp = card1;
		card1 = card2;
		card2 = temp;
	}
	if (card1 == CARD_3G && card2 == CARD_8G) {
		return JB_38G;
	}
	else if (card1 == CARD_1G && card2 == CARD_8G) {
		return JB_18G;
	}
	else if (card1 == CARD_1G && card2 == CARD_3G) {
		return JB_13G;
	}
	else if (card1 == CARD_10A && card2 == CARD_10B) {
		return JB_1010;
	}
	else if (card1 == CARD_9A && card2 == CARD_9B) {
		return JB_99;
	}
	else if (card1 == CARD_8G && card2 == CARD_8B) {
		return JB_88;
	}
	else if (card1 == CARD_7A && card2 == CARD_7B) {
		return JB_77;
	}
	else if (card1 == CARD_6A && card2 == CARD_6B) {
		return JB_66;
	}
	else if (card1 == CARD_5A && card2 == CARD_5B) {
		return JB_55;
	}
	else if (card1 == CARD_4A && card2 == CARD_4B) {
		return JB_44;
	}
	else if (card1 == CARD_3G && card2 == CARD_3B) {
		return JB_33;
	}
	else if (card1 == CARD_2A && card2 == CARD_2B) {
		return JB_22;
	}
	else if (card1 == CARD_1G && card2 == CARD_1B) {
		return JB_11;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_2A ||
		card1 == CARD_1G && card2 == CARD_2B ||
		card1 == CARD_2A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_2B) {
		return JB_12;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_4A ||
		card1 == CARD_1G && card2 == CARD_4B ||
		card1 == CARD_4A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_4B) {
		return JB_14;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_9A ||
		card1 == CARD_1G && card2 == CARD_9B ||
		card1 == CARD_9A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_9B) {
		return JB_91;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_10A ||
		card1 == CARD_1G && card2 == CARD_10B ||
		card1 == CARD_10A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_10B) {
		return JB_101;
	}
	else if (
		card1 == CARD_4A && card2 == CARD_10A ||
		card1 == CARD_4A && card2 == CARD_10B ||
		card1 == CARD_10A && card2 == CARD_4B ||
		card1 == CARD_4B && card2 == CARD_10B) {
		return JB_104;
	}
	else if (
		card1 == CARD_4A && card2 == CARD_6A ||
		card1 == CARD_4A && card2 == CARD_6B ||
		card1 == CARD_6A && card2 == CARD_4B ||
		card1 == CARD_4B && card2 == CARD_6B) {
		return JB_46;
	}
	// �籸
	else if (card1 == CARD_4A && card2 == CARD_9A) {
		return JB_49;
	}
	// ������
	else if (
		card1 == CARD_3G && card2 == CARD_7A ||
		card1 == CARD_3G && card2 == CARD_7B ||
		card1 == CARD_3B && card2 == CARD_7A ||
		card1 == CARD_3B && card2 == CARD_7B) {
		return JB_NNC;
	}
	// ������
	else if (card1 == CARD_4A && card2 == CARD_7A) {
		return JB_GC;
	}
	else {
		int a, b, c;
		JOKBO jok;
		a = card1 + 1;
		b = card2 + 1;
		jok = JOKBO(JB_0 - (a + b) % 10);
		return jok;
	}
}
