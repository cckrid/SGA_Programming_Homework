#include "stdafx.h"
#include "MainGame10.h"


MainGame10::MainGame10()
{
}


MainGame10::~MainGame10()
{
}

HRESULT MainGame10::Init()
{
	GameNode::Init();

	angle = 90 * M_PI / 180;
	angleSpeed = 3.0f;
	speed = 3.0f;
	tailCount = 0;

	head.x = WINSIZEX / 2;
	head.y = WINSIZEY / 2;
	head.radius = 50;
	head.rc = RectMakeCenter(
		head.x, head.y, head.radius, head.radius);
	head.eye = RectMakeCenter(
		head.x + cos(angle) * 20, head.y + -sin(angle) * 20,
		head.radius - 25, head.radius - 25);
	
	prevTime = GetTickCount();

	return S_OK;
}

void MainGame10::Release()
{
	GameNode::Release();
}

void MainGame10::Update()
{
	GameNode::Update();

	if (INPUT->GetKey(VK_LEFT)) { angle += angleSpeed * M_PI / 180; }
	if (INPUT->GetKey(VK_RIGHT)) { angle -= angleSpeed * M_PI / 180; }
	if (INPUT->GetKeyDown(VK_UP)) { angleSpeed += 0.01f; }
	if (INPUT->GetKeyDown(VK_DOWN)) { angleSpeed -= 0.01f; }

	WormMove();

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {
		WormTail();
		prevTime = GetTickCount();
	}

	//for (int i = tailCount; i >= 1; i--) {
	//	if (GetDistance(tail[i].x, tail[i].y, tail[i - 1].x, tail[i - 1].y) > 30) {
	//		float angle = GetAngle(tail[i].x, tail[i].y, tail[i - 1].x, tail[i - 1].y);
	//		tail[i].x += cosf(angle) * 3.0f;
	//		tail[i].y += -sinf(angle) * 3.0f;
	//		tail[i].rc =
	//			RectMakeCenter(tail[i].x, tail[i].y, tail[i].radius, tail[i].radius);
	//	}
	//}
}

void MainGame10::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==================================================

	for (int i = tailCount; i >= 0; i--) {
		EllipseMake(memDC, tail[i].rc);
	}

	EllipseMake(memDC, head.rc);
	EllipseMake(memDC, head.eye);

	//for (int i = 0; i < tailCount; i++)
	//	EllipseMake(memDC, tail[i].rc);


	char str[64];
	sprintf_s(str, "꼬리 : %d", tailCount);
	TextOut(memDC, 10, 10, str, strlen(str));
	sprintf_s(str, "각도 : %f", angle);
	TextOut(memDC, 10, 30, str, strlen(str));
	sprintf_s(str, "회전각도 : %f", angleSpeed);
	TextOut(memDC, 10, 50, str, strlen(str));


	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame10::WormMove()
{
	head.x += cosf(angle) * speed;
	head.y += -sinf(angle) * speed;

	// 벽 충돌
	// left
	if (head.x - head.radius < 0) {
		head.x = head.radius;
		angle = M_PI - angle;
	}
	// right
	if (head.x + head.radius > WINSIZEX) {
		head.x = WINSIZEX - head.radius;
		angle = M_PI - angle;
	}
	// top
	if (head.y - head.radius < 0) {
		head.y = head.radius;
		angle = 2 * M_PI - angle;
	}
	// bottom
	if (head.y + head.radius > WINSIZEY) {
		head.y = WINSIZEY - head.radius;
		angle = 2 * M_PI - angle;
	}

	head.rc = RectMakeCenter(
		head.x, head.y, head.radius, head.radius);
	head.eye = RectMakeCenter(
		head.x + cos(angle) * 20, head.y + -sin(angle) * 20,
		head.radius - 25, head.radius - 25);
}

void MainGame10::WormTail()
{
	if(tailCount != MAX_WORM - 1)
		tailCount++;

	for (int i = tailCount; i >= 1; i--) {
		tail[i] = tail[i - 1];
	}

	tail[0].rc = head.rc;
	tail[0].x = head.x;
	tail[0].y = head.y;
	tail[0].radius = head.radius;


}
