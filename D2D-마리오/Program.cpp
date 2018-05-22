#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "GameObject\Box.h"
#include "GameObject\Coin.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	//SOUND->AddSound("Test", "sounds/����3.wav", true, true);
	SOUND->AddSound("bgm", "sounds/Super Mario Bros. 2 Overworld Theme on piano.mp3", true, true);
	SOUND->AddSound("jump", "sounds/jump.mp3", false, false);
	SOUND->AddSound("coin", "sounds/coin.mp3", false, false);

	mainCamera = new Camera;

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/mario_all.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.jpg",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/coin.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));

	mario = new Rect;
	mario->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	mario->SetTexture(pTex[0]);
	mario->SetCamera(mainCamera);
	
	bg = new BackGround;
	bg->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	bg->SetTexture(pTex[1]);
	bg->SetCamera(mainCamera);

	BoxInit();
	CoinInit();

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	//SOUND->Play("Test");

	hr = D3DXCreateFont(
		D2D::GetDevice(),
		15,					// �ʺ�
		0,					// ����
		FW_BOLD,			// �β�, �β��� ���ڴٴ°�
		1,					// �� ���� ũ�� �ٲ� �� ����
		FALSE,				// ���Ÿ�ü ��� ����
		DEFAULT_CHARSET,	// �� ���� ũ�� �ٲ� �� ����
		OUT_DEFAULT_PRECIS,	// �ܰ��� ó��
		DEFAULT_QUALITY,	// ����Ƽ
							// FF_DONTCARE ���� ũ�� ������� ū �༮ �������� ����ִ°� �ʺ�� ���� �߿�
		DEFAULT_PITCH || FF_DONTCARE,		// �ɼ�, 
		L"����",
		&font
	);
	assert(SUCCEEDED(hr));

	isDebug = false;
}

Program::~Program()
{
	mario->Release();
	bg->Release();
	for (int i = 0; i < BOX_MAX; i++) {
		box[i]->Release();
		SAFE_DELETE(box[i]);
	}
	
	for (int i = 0; i < BOX2_MAX; i++) {
		box2[i]->Release();
		SAFE_DELETE(box2[i]);
	}

	for (int i = 0; i < BOX3_MAX; i++) {
		box3[i]->Release();
		SAFE_DELETE(box3[i]);
	}

	for (int i = 0; i < COIN_MAX; i++) {
		coin[i]->Release();
		SAFE_DELETE(coin[i]);
	}

	for (int i = 0; i < 3; i++)
		SAFE_RELEASE(pTex[i]);
	
	SAFE_DELETE(mario);
	SAFE_DELETE(bg);
	SAFE_DELETE(mainCamera);
}

void Program::BoxInit()
{
	for (int i = 0; i < BOX_MAX; i++)
		box[i] = new Box;

	float width = 357.0f;
	float height = 90.0f;

	box[0]->Init(L"./Shader/ColorTexture.fx",
		Vector2(-width / 2, -height / 2),
		Vector2(width / 2, height / 2));
	box[0]->SetCamera(mainCamera);
	box[0]->GetTransform()->SetWorldPosition(Vector2(
		-WINSIZE_X / 2 + width / 2,
		-WINSIZE_Y / 2 + 407.0f + height / 2));

	float temp = 737 - 409;
	box[2]->Init(L"./Shader/ColorTexture.fx",
		Vector2(-width / 2, -height / 2),
		Vector2(width / 2, height / 2));
	box[2]->SetCamera(mainCamera);
	box[2]->GetTransform()->SetWorldPosition(Vector2(
		-WINSIZE_X / 2 + width / 2,
		-WINSIZE_Y / 2 + 407.0f + height / 2 + temp));

	width = WINSIZE_X - 425.0f;
	box[1]->Init(L"./Shader/ColorTexture.fx",
		Vector2(-width / 2, -height / 2),
		Vector2(width / 2, height / 2));
	box[1]->SetCamera(mainCamera);
	box[1]->GetTransform()->SetWorldPosition(Vector2(
		WINSIZE_X / 2 - width / 2,
		-WINSIZE_Y / 2 + 407.0f + height / 2));

	width = WINSIZE_X - 425.0f;
	box[3]->Init(L"./Shader/ColorTexture.fx",
		Vector2(-width / 2, -height / 2),
		Vector2(width / 2, height / 2));
	box[3]->SetCamera(mainCamera);
	box[3]->GetTransform()->SetWorldPosition(Vector2(
		WINSIZE_X / 2 - width / 2,
		-WINSIZE_Y / 2 + 407.0f + height / 2 + temp));

	/////////////////////////////////////////////////////////

	width = 23.0f;
	height = 31.25f;
	for (int i = 0; i < BOX2_MAX; i++) {
		box2[i] = new Box;
		box2[i]->Init(L"./Shader/ColorTexture.fx",
			Vector2(-width / 2, -height / 2), Vector2(width / 2, height / 2));
		box2[i]->SetCamera(mainCamera);
	}

	for (int i = 1; i <= 3; i++) {
		box2[i]->GetTransform()->SetWorldPosition(Vector2(
			-width * i, 0));
		box2[i + 3]->GetTransform()->SetWorldPosition(Vector2(
			width * i, 0));
	}
	box2[7]->GetTransform()->SetWorldPosition(Vector2(0, -height));
	for (int i = 1; i <= 2; i++) {
		box2[i + 7]->GetTransform()->SetWorldPosition(Vector2(
			-width * i, -height));
		box2[i + 9]->GetTransform()->SetWorldPosition(Vector2(
			width * i, -height));
	}

	for (int i = 1; i <= 3; i++) {
		box2[i + 12]->GetTransform()->SetWorldPosition(Vector2(
			-width * i, 0));
		box2[i + 3 + 12]->GetTransform()->SetWorldPosition(Vector2(
			width * i, 0));
	}
	box2[7 + 12]->GetTransform()->SetWorldPosition(Vector2(0, -height));
	for (int i = 1; i <= 2; i++) {
		box2[i + 7 + 12]->GetTransform()->SetWorldPosition(Vector2(
			-width * i, -height));
		box2[i + 9 + 12]->GetTransform()->SetWorldPosition(Vector2(
			width * i, -height));
	}

	for (int i = 1; i <= 2; i++) {
		box2[i + 24]->GetTransform()->SetWorldPosition(Vector2(
			-width * i, 0));
		box2[i + 2 + 24]->GetTransform()->SetWorldPosition(Vector2(
			width * i, 0));
	}

	for (int i = 0; i < 12; i++) {
		box2[i]->GetTransform()->MovePositionSelf(Vector2(357, 323));
		box2[i + 12]->GetTransform()->MovePositionSelf(Vector2(90, 323));
	}

	for (int i = 0; i < 5; i++) {
		box2[i + 24]->GetTransform()->MovePositionSelf(Vector2(-210, 217));
	}

	////////////////////////////////////////////////////

	for (int i = 0; i < BOX3_MAX; i++) {
		box3[i] = new Box;
		//box3[i]->Init(L"./Shader/ColorTexture.fx",
		//	Vector2(-width / 2, -height / 2), Vector2(width / 2, height / 2));
		box3[i]->SetCamera(mainCamera);
	}

	for (int i = 0; i <= 2; i += 2) {
		width = 65.0f;
		height = 42.5f;

		box3[i]->Init(L"./Shader/ColorTexture.fx",
			Vector2(-width / 2, -height / 2), Vector2(width / 2, height / 2));

		width = 55.0f;
		height = 35.0f;

		box3[i + 1]->Init(L"./Shader/ColorTexture.fx",
			Vector2(-width / 2, -height / 2), Vector2(width / 2, height / 2));
		box3[i + 1]->GetTransform()->MovePositionSelf(Vector2(1.25f, 21.25f + 17.5f));
	}
	for (int i = 0; i < 2; i++) {
		box3[i]->GetTransform()->MovePositionSelf(Vector2(-107, -50));
		box3[i + 2]->GetTransform()->MovePositionSelf(Vector2(400, -50));
	}

	mario->SetBox(box, BOX_MAX, 1);
	mario->SetBox(box2, BOX2_MAX, 2);
	mario->SetBox(box3, BOX3_MAX, 3);
}

void Program::CoinInit()
{
	for (int i = 0; i < COIN_MAX; i++)
		coin[i] = new Coin;

	float width = 25.0f;
	float height = 25.0f;

	for (int i = 0; i < COIN_MAX; i++) {
		coin[i]->Init(L"./Shader/ColorTexture.fx",
			Vector2(-width / 2, -height / 2),
			Vector2(width / 2, height / 2));
		coin[i]->SetCamera(mainCamera);
		//coin[0]->GetTransform()->SetWorldPosition(Vector2(
		//	-WINSIZE_X / 2 + width / 2,
		//	-WINSIZE_Y / 2 + 407.0f + height / 2));
		coin[i]->SetTexture(pTex[2]);
	}

	coin[0]->GetTransform()->MovePositionSelf(Vector2(-106, -125.0f));
	coin[1]->GetTransform()->MovePositionSelf(Vector2(396, -125.0f));
	coin[2]->GetTransform()->MovePositionSelf(Vector2(-213.0f, 150.0f));
	coin[3]->GetTransform()->MovePositionSelf(Vector2(355, 220.0f));
	coin[4]->GetTransform()->MovePositionSelf(Vector2(86, 220.0f));

	mario->SetCoin(coin, COIN_MAX);
}

void Program::Update()
{
	if (INPUT->GetKeyDown(VK_SHIFT)) {
		if (!SOUND->IsPlaySound("bgm"))
			SOUND->Play("bgm");
	}

	if (INPUT->GetKeyDown(VK_TAB))
		isDebug = !isDebug;

	mainCamera->UpdateCamToDevice();
	bg->Update();

	for (int i = 0; i < BOX_MAX; i++)
		box[i]->Update();
	
	for (int i = 0; i < BOX2_MAX; i++)
		box2[i]->Update();

	for (int i = 0; i < BOX3_MAX; i++)
		box3[i]->Update();

	for (int i = 0; i < COIN_MAX; i++) {
		if(!coin[i]->GetSelect())
			coin[i]->Update();
	}

	mario->Update();

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);
}

void Program::Render()
{
	bg->Render();
	mario->Render();
	
	for (int i = 0; i < COIN_MAX; i++) {
		if (!coin[i]->GetSelect())
			coin[i]->Render();
	}

	if (isDebug) {
		for (int i = 0; i < BOX_MAX; i++)
			box[i]->Render();

		for (int i = 0; i < BOX2_MAX; i++)
			box2[i]->Render();

		for (int i = 0; i < BOX3_MAX; i++)
			box3[i]->Render();
	}

	RECT rc = { 10,10, 0,0 };
	wstring str;
	str = L"x: " + to_wstring(mousePos.x);
	str += L" , ";
	str += L"y: " + to_wstring(mousePos.y);

	// ��Ƽ����Ʈ�� DrawTextA
	font->DrawTextW(
		// �̹��� 2D ��ǥ���� ���°� sprite��� ��
		NULL,
		str.c_str(),
		-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
		&rc,
		// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
		// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
		DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
							 // 0x~~ �̰� �������� �Լ��� ����
							 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		0xFFFF0000
	);
}
