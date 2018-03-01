#include "stdafx.h"
#include "MainGame16.h"


MainGame16::MainGame16()
{
}


MainGame16::~MainGame16()
{
}

HRESULT MainGame16::Init()
{
	GameNode::Init();

	mario = new Image;
	bg = new Image;
	tile = new Image;

	mario->Init("images/mario_all.bmp", 800, 400, true, RGB(255, 0, 255));
	bg->Init("images/mario_bg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	tile->Init("images/mario_tile1.bmp", 
		WINSIZEX / 8, WINSIZEY / 4, true, RGB(255, 0, 255));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			// ������ 3, 7�� �� �̹���
			if (i == 3 && j == 7) continue;
			ani[i * 8 + j].width = 100;
			ani[i * 8 + j].height = 100;
			ani[i * 8 + j].offsetX = ani[i * 8 + j].width * j;
			ani[i * 8 + j].offsetY = ani[i * 8 + j].height * i;
		}
	}

	marioInfo.speed = 0;
	marioInfo.state = STATE_LEFTIDLE;
	marioInfo.x = WINSIZEX / 2;
	marioInfo.y = WINSIZEY - WINSIZEY / 4 - ani[0].height;
	marioInfo.offsetX = ani[STATE_LEFTIDLE].offsetX;
	marioInfo.offsetY = ani[STATE_LEFTIDLE].offsetY;
	marioInfo.width = ani[STATE_LEFTIDLE].width;
	marioInfo.height = ani[STATE_LEFTIDLE].height;
	marioInfo.stateCount = 0;

	prevTime = GetTickCount();

	gravity = 0;

	return S_OK;
}

void MainGame16::Release()
{
	GameNode::Release();
	SAFE_DELETE(mario);
	SAFE_DELETE(bg);
	SAFE_DELETE(tile);
}

void MainGame16::Update()
{
	GameNode::Update();


	// ���� ����Ű
	if (INPUT->GetKey(VK_LEFT)) {
		// ���� ���� ��� ���� ���⸸ ���� (��� ��)
		if (marioInfo.state == STATE_RIGHTJUMPUP
			|| marioInfo.state == STATE_LEFTJUMPUP)
			marioInfo.state = STATE_LEFTJUMPUP;
		// ���� ���� ��� ���� ���⸸ ���� (�ϰ� ��)
		else if (marioInfo.state == STATE_RIGHTJUMPDOWN
			|| marioInfo.state == STATE_LEFTJUMPDOWN)
			marioInfo.state = STATE_LEFTJUMPDOWN;
		// �޸��� ���� ���
		else if (marioInfo.isRun) {
			// �ӵ��� ����
			marioInfo.speed -= 0.2f;
			// ���� �ӵ� �̻� �޸��� ���·� ����
			if (marioInfo.speed <= -10.0f)
				marioInfo.state = STATE_LEFTRUN;
			// �޸��� ���� �� ���� ��ȯ ��
			else if (marioInfo.speed > 0)
				marioInfo.state = STATE_RIGHTRUNCHANGE;
			// �� �� ���� �ӵ� ������ ���
			else
				marioInfo.state = STATE_LEFTWALK;
		}
		// �������� �޸��� �� �׳� �ȴ� ���
		else if (marioInfo.state == STATE_LEFTRUN
			|| marioInfo.state == STATE_LEFTRUNCHANGE)
		{
			// �ӵ� ����
			marioInfo.speed += 0.2f;
			// �ȱ� �ӵ� ���� �� �ȱ�� ���� �� �ȱ� ���
			if (marioInfo.speed > -5.0f) {
				marioInfo.speed = -5.0f;
				marioInfo.state = STATE_LEFTWALK;
			}
		}
		// ���������� �޸��� �� �׳� �ȴ� ���
		else if (marioInfo.state == STATE_RIGHTRUN
			|| marioInfo.state == STATE_RIGHTRUNCHANGE)
		{
			// ���� ��ȯ ������� ����
			marioInfo.state = STATE_RIGHTRUNCHANGE;
			// �ӵ� ����
			marioInfo.speed -= 0.2f;

			// �ȱ� �ӵ� ���� �� �ȱ�� ���� �� �ȱ� ���
			if (marioInfo.speed < -5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// �׳� �ȴ� ���
		else {
			marioInfo.state = STATE_LEFTWALK;
			marioInfo.speed = -5.0f;
		}
	}

	// ������ ����Ű�� ���� ���
	if (INPUT->GetKeyUp(VK_LEFT)) {
		// �Ȱ� �ִ� ���̸� ���� ���� ���·� ����
		if(marioInfo.state == STATE_LEFTWALK)
			marioInfo.state = STATE_LEFTIDLE; 
	}
	
	if (INPUT->GetKey(VK_RIGHT)) {
		// ���� ���� ��� ���� ���⸸ ���� (��� ��)
		if (marioInfo.state == STATE_LEFTJUMPUP
			|| marioInfo.state == STATE_RIGHTJUMPUP)
			marioInfo.state = STATE_RIGHTJUMPUP;
		// ���� ���� ��� ���� ���⸸ ���� (�ϰ� ��)
		else if (marioInfo.state == STATE_LEFTJUMPDOWN
			|| marioInfo.state == STATE_RIGHTJUMPDOWN)
			marioInfo.state = STATE_RIGHTJUMPDOWN;
		// �޸��� ���� ���
		else if (marioInfo.isRun) {
			marioInfo.speed += 0.2f;
			// ���� �ӵ� �̻� �޸��� ���·� ����
			if (marioInfo.speed >= 10.0f)
				marioInfo.state = STATE_RIGHTRUN;
			// �޸��� ���� �� ���� ��ȯ ��
			else if (marioInfo.speed < 0)
				marioInfo.state = STATE_LEFTRUNCHANGE;
			// �� �� ���� �ӵ� ������ ���
			else
				marioInfo.state = STATE_RIGHTWALK;
		}
		// ���������� �޸��� �� �׳� �ȴ� ���
		else if (marioInfo.state == STATE_RIGHTRUN
			|| marioInfo.state == STATE_RIGHTRUNCHANGE)
		{
			// �ӵ� ����
			marioInfo.speed -= 0.2f;
			// �ȱ� �ӵ� ���� �� �ȱ�� ���� �� �ȱ� ���
			if (marioInfo.speed < 5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// �������� �޸��� �� �׳� �ȴ� ���
		else if (marioInfo.state == STATE_LEFTRUN
			|| marioInfo.state == STATE_LEFTRUNCHANGE)
		{
			// ���� ��ȯ ������� ����
			marioInfo.state = STATE_LEFTRUNCHANGE;
			// �ӵ� ����
			marioInfo.speed += 0.2f;

			// �ȱ� �ӵ� ���� �� �ȱ�� ���� �� �ȱ� ���
			if (marioInfo.speed > 5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// �׳� �ȴ� ���
		else {
			marioInfo.state = STATE_RIGHTWALK;
			marioInfo.speed = 5.0f;
		}
	}
	// ������ ����Ű�� ���� ���
	if (INPUT->GetKeyUp(VK_RIGHT)) {
		// �Ȱ� �ִ� ���̸� ������ ���� ���·� ����
		if(marioInfo.state == STATE_RIGHTWALK)
			marioInfo.state = STATE_RIGHTIDLE; 
	}
	
	// ����Ű�� ���� �ӵ��� ������ ������ ��
	if (marioInfo.state == STATE_LEFTIDLE
		|| marioInfo.state == STATE_RIGHTIDLE
		|| marioInfo.state == STATE_LEFTRUNCHANGE
		|| marioInfo.state == STATE_RIGHTRUNCHANGE
		|| marioInfo.state == STATE_RIGHTRUN
		|| marioInfo.state == STATE_LEFTRUN) {
		// �ӵ��� 0�� ���� ���̰ų� �ø� (���� �� �� �ֵ���)
		// �ӵ� ������ �޸� �� ��� �ӵ��� ������ �� �ӵ� �ȿö�
		if (marioInfo.speed < 0)
			marioInfo.speed += 0.1f;
		else if (marioInfo.speed > 0)
			marioInfo.speed -= 0.1f;

		// 0�� ����� ���� ���޽� 0���� ���� (�Ҽ����̹Ƿ� 0�� �ȵ� �� �־)
		if (-0.001f <= marioInfo.speed &&
			marioInfo.speed <= 0.001)
			marioInfo.speed = 0;
	}

	// �ӵ��� ���� x �� ����
	marioInfo.x += marioInfo.speed;

	// ȭ�� �� ��� ��
	// ����
	if (marioInfo.x + marioInfo.width< 0)
		marioInfo.x = WINSIZEX;
	// ����
	if (marioInfo.x > WINSIZEX)
		marioInfo.x = -marioInfo.width;

	// ����
 	if (INPUT->GetKeyDown(VK_SPACE)) {
		// ���� ����� ���� ����
		if (marioInfo.state == STATE_LEFTWALK ||
			marioInfo.state == STATE_LEFTIDLE ||
			marioInfo.state == STATE_LEFTRUN) {
			marioInfo.state = STATE_LEFTJUMPUP;

			gravity = 10.0f;
		}

		// ������ ����� ������ ����
		if (marioInfo.state == STATE_RIGHTWALK ||
			marioInfo.state == STATE_RIGHTIDLE ||
			marioInfo.state == STATE_RIGHTRUN) {
			marioInfo.state = STATE_RIGHTJUMPUP;

			gravity = 10.0f;
		}
	}

	// ���丮 ����
	if (INPUT->GetKeyDown('V')) {
		// ���� ���� ���½� ���� ���丮
		if (marioInfo.state == STATE_LEFTIDLE)
			marioInfo.state = STATE_LEFTVICTORY;
		// ������ ���� ���½� ������ ���丮
		if (marioInfo.state == STATE_RIGHTIDLE)
			marioInfo.state = STATE_RIGHTVICTORY;
		// ���� ���丮�� ������ ���丮
		if (marioInfo.state == STATE_LEFTVICTORY)
			marioInfo.state = STATE_RIGHTVICTORY;
		// ������ ���丮�� ���� ���丮
		else if (marioInfo.state == STATE_RIGHTVICTORY)
			marioInfo.state = STATE_LEFTVICTORY;
	}

	// ���ӵ� ��ư Ŭ���� �޸���
	if (INPUT->GetKeyDown('A')) { marioInfo.isRun = true; }
	if (INPUT->GetKeyUp('A')) { marioInfo.isRun = false; }

	// State �� ���� �߰��� ��ȭ
	switch (marioInfo.state)
	{
		// ���� �� y�� �߷� ����
	case STATE_LEFTJUMPUP:
	case STATE_LEFTJUMPDOWN:
	case STATE_RIGHTJUMPUP:
	case STATE_RIGHTJUMPDOWN:
		marioInfo.y -= gravity;
		break;
		// �޸��� �� ���� ��ȯ�� �ӵ��� �����Ǹ� idle ���·� ����
	case STATE_LEFTRUN:
	case STATE_LEFTRUNCHANGE:
		if (marioInfo.speed == 0)
			marioInfo.state = STATE_LEFTIDLE;
		break;
	case STATE_RIGHTRUN:
	case STATE_RIGHTRUNCHANGE:
		if (marioInfo.speed == 0)
			marioInfo.state = STATE_RIGHTIDLE;
		break;
	default:
		break;
	}

	// �ٴ� �̻����� �������� �ʵ��� �ٴں��� �Ʒ��� ���
	if (marioInfo.y > WINSIZEY - WINSIZEY / 4 - ani[0].height) {
		marioInfo.y = WINSIZEY - WINSIZEY / 4 - ani[0].height;

		// ���� ���¿��� ���� ���·� ����
  		if (marioInfo.state == STATE_LEFTJUMPDOWN ||
			marioInfo.state == STATE_LEFTJUMPUP)
			marioInfo.state = STATE_LEFTIDLE;
		if (marioInfo.state == STATE_RIGHTJUMPDOWN ||
			marioInfo.state == STATE_RIGHTJUMPUP)
			marioInfo.state = STATE_RIGHTIDLE;
	}
	
	// ���� ���� �̻�� ��� ��ǿ��� �ϰ� ������� ���� (�߷��� 0�� ��)
	if (gravity == 0 && marioInfo.state == STATE_LEFTJUMPUP)
		marioInfo.state = STATE_LEFTJUMPDOWN;
	if (gravity == 0 && marioInfo.state == STATE_RIGHTJUMPUP)
		marioInfo.state = STATE_RIGHTJUMPDOWN;

	

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {
		gravity -= 2;

		// ���¿� ���� �ִϸ��̼� ����
		CheckAniForState();

		prevTime = GetTickCount();
	}
}

void MainGame16::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	bg->Render(memDC);

	for (int i = 0; i < 8; i++) {
		tile->Render(memDC, WINSIZEX / 8 * i, WINSIZEY - WINSIZEY / 4);
	}

	// test
	//mario->Render(memDC);
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		// �� �̹��� ��ġ
	//		if (i == 3 & j == 7) continue;
	//		mario->Render(memDC, 
	//			ani[i * 8 + j].offsetX,
	//			ani[i * 8 + j].offsetY,
	//			ani[i * 8 + j].offsetX,
	//			ani[i * 8 + j].offsetY,
	//			ani[i * 8 + j].width,
	//			ani[i * 8 + j].height);
	//	}
	//}
	//
	//int i = 0;
	//int j = 1;
	//
	//marioImage->Render(memDC,
	//				mario.x, mario.y,
	//				ani[i * 8 + j].offsetX,
	//				ani[i * 8 + j].offsetY,
	//				ani[i * 8 + j].width,
	//				ani[i * 8 + j].height);

	mario->Render(memDC, marioInfo.x, marioInfo.y,
		marioInfo.offsetX, marioInfo.offsetY, marioInfo.width, marioInfo.height);
	
	char str[64];
	switch (marioInfo.state)
	{
	case STATE_LEFTIDLE:
		sprintf_s(str, "State : �� ���");
		break;
	case STATE_RIGHTIDLE:
		sprintf_s(str, "State : ���� ���");
		break;
	case STATE_LEFTWALK:
		sprintf_s(str, "State : �� �ȱ�");
		break;
	case STATE_RIGHTWALK:
		sprintf_s(str, "State : ���� �ȱ�");
		break;
	case STATE_LEFTRUN:
		sprintf_s(str, "State : �� �ٱ�");
		break;
	case STATE_RIGHTRUN:
		sprintf_s(str, "State : ���� �ٱ�");
		break;
	case STATE_LEFTJUMPUP:
		sprintf_s(str, "State : �� ���� ��");
		break;
	case STATE_LEFTJUMPDOWN:
		sprintf_s(str, "State : �� ���� �ٿ�");
		break;
	case STATE_RIGHTJUMPUP:
		sprintf_s(str, "State : ���� ���� ��");
		break;
	case STATE_RIGHTJUMPDOWN:
		sprintf_s(str, "State : ���� ���� �ٿ�");
		break;
	case STATE_LEFTVICTORY:
		sprintf_s(str, "State : �� ���丮");
		break;
	case STATE_RIGHTVICTORY:
		sprintf_s(str, "State : ���� ���丮");
		break;
	case STATE_LEFTRUNCHANGE:
		sprintf_s(str, "State : �� �޸��� �� ����");
		break;	
	case STATE_RIGHTRUNCHANGE:
		sprintf_s(str, "State : ���� �޸��� �� ��");
		break;
	default:
		break;
	}
	
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "x : %f, y : %f", marioInfo.x, marioInfo.y);
	TextOut(memDC, 10, 30, str, strlen(str));

	sprintf_s(str, "speed : %f", marioInfo.speed);
	TextOut(memDC, 10, 50, str, strlen(str));

	if (marioInfo.isRun) {
		sprintf_s(str, "���� ��");
		TextOut(memDC, 10, 70, str, strlen(str));
	}

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame16::CheckAniForState()
{
	switch (marioInfo.state)
	{
	// �ϳ��� �̹����θ� �Ǿ� �ִ� ���ϸ��̼�
	case STATE_LEFTIDLE:
	case STATE_RIGHTIDLE:
	case STATE_LEFTJUMPUP:
	case STATE_LEFTJUMPDOWN:
	case STATE_RIGHTJUMPUP:
	case STATE_RIGHTJUMPDOWN:
	case STATE_LEFTVICTORY:
	case STATE_RIGHTVICTORY:
	case STATE_LEFTRUNCHANGE:
	case STATE_RIGHTRUNCHANGE:
		marioInfo.stateCount = 0;
		break;
	// �� ���� �̹����� �Ǿ� �ִ� ���ϸ��̼�
	case STATE_LEFTWALK:
	case STATE_RIGHTWALK:
		marioInfo.stateCount++;
		if (marioInfo.stateCount >= 2)
			marioInfo.stateCount = 0;
		break;
	// �� ���� �̹����� �Ǿ� �ִ� ���ϸ��̼�
	case STATE_LEFTRUN:
	case STATE_RIGHTRUN:
		marioInfo.stateCount++;
		if (marioInfo.stateCount >= 3)
			marioInfo.stateCount = 0;
		break;
	}

	marioInfo.offsetX = ani[marioInfo.state + marioInfo.stateCount].offsetX;
	marioInfo.offsetY = ani[marioInfo.state + marioInfo.stateCount].offsetY;

	
}
