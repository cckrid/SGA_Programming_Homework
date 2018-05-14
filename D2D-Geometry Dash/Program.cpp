#include "stdafx.h"
#include "Program.h"

#include "GameObject\BackGround.h"
#include "GameObject\Rect.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/����3.wav", true, true);
	SOUND->AddSound("bg", "sounds/Forever Bound - Stereo Madness.mp3", true, true);

	mainCamera = new Camera;

	for (int i = 0; i < 2; i++) {
		rect[i] = new Rect;
		rect[i]->Init();
		rect[i]->SetCamera(mainCamera);

		bg[i] = new BackGround;
		bg[i]->Init();
		bottom[i] = new BackGround;
		bottom[i]->Init();
		bottom[i]->GetTransform()->MovePositionSelf(Vector2(0, WINSIZE_Y / 2 + 100));
	}
	bg[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X, 0));
	bottom[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X, 0));

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	//SOUND->Play("Test");
	SOUND->Play("bg", 0.5f);

	HRESULT hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));
	
	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/bg2.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/rect.png",
		&pTex[2]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/enemy.png",
		&pTex[3]
	);
	assert(SUCCEEDED(hr));

	for (int i = 0; i < 2; i++) {
		bg[i]->SetTexture(pTex[0]);
		bottom[i]->SetTexture(pTex[1]);
		bg[i]->SetSpeed(1.0f);
	}
	rect[0]->SetTexture(pTex[2]);
	rect[0]->SetPlayer(true);

	rect[1]->SetTexture(pTex[3]);
	rect[1]->GetTransform()->MovePositionSelf(Vector2(WINSIZE_X / 2, 0));

	isPause = true;

	hr = D3DXCreateFont(
		D2D::GetDevice(),
		25,					// �ʺ�
		0,					// ����
		FW_BOLD,			// �β�, �β��� ���ڴٴ°�
		1,					// �� ���� ũ�� �ٲ� �� ����
		FALSE,				// ���Ÿ�ü ��� ����
		DEFAULT_CHARSET,	// �� ���� ũ�� �ٲ� �� ����
		OUT_DEFAULT_PRECIS,	// �ܰ��� ó��
		DEFAULT_QUALITY,	// ����Ƽ
							// FF_DONTCARE ���� ũ�� ������� ū �༮ �������� ����ִ°� �ʺ�� ���� �߿�
		DEFAULT_PITCH || FF_DONTCARE,		// �ɼ�, 
		L"�ü�",
		&font
	);
	assert(SUCCEEDED(hr));

	root = new Json::Value();
}

Program::~Program()
{
	for (int i = 0; i < 4; i++) {
		SAFE_RELEASE(pTex[i]);
	}

	for (int i = 0; i < 2; i++) {
		bg[i]->Release();
		SAFE_DELETE(bg[i]);
		bottom[i]->Release();
		SAFE_DELETE(bottom[i]);
		rect[i]->Release();
		SAFE_DELETE(rect[i]);
	}
	
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(root);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();

	if (INPUT->GetKeyDown('Z')) Init();
	if (INPUT->GetKeyDown('X')) Save();
	if (INPUT->GetKeyDown('C')) Load();

	if (INPUT->GetKeyDown('P')) isPause = !isPause;
	if (isPause) return;

	score++;

	for (int i = 0; i < 2; i++) {
		rect[i]->Update();
		bg[i]->Update();
		bottom[i]->Update();
	}
}

void Program::Render()
{
	// �̷����ϸ� �������� �������� �𸣰���
	//for (int i = 0; i < 2; i++) {
	//	bg[i]->Render();
	//	bottom[i]->Render();
	//}

	for (int i = 0; i < 2; i++) {
		bg[i]->Render();
	}
	for (int i = 0; i < 2; i++) {
		bottom[i]->Render();
	}

	rect[0]->Render();

	for(int i=1; i<2; i++)
		rect[i]->Render();

	// RenderText
	RECT rc = { 10, 10, 0, 0 };
	wstring str;

	str = L"Score : ";
	str += to_wstring(score);

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
		0xFFFFFFFF
	);
}

void Program::Init()
{
	score = 0;
	rect[0]->GetTransform()->SetWorldPosition(Vector2(-75, 75));
	rect[0]->SetStatus(STATUS_IDLE);

	rect[1]->GetTransform()->SetWorldPosition(Vector2(WINSIZE_X / 2, 75));
}

void Program::Save()
{
	int status = rect[0]->GetStatus();
	float vy = rect[0]->GetVy();
	float bottomY = rect[0]->GetBottomY();
	Vector2 pos = rect[0]->GetTransform()->GetWorldPosition();

	Vector2 pos2 = rect[1]->GetTransform()->GetWorldPosition();

	Json::SetValue(*root, "score", score);
	Json::SetValue(*root, "player_status", status);
	
	Json::SetValue(*root, "player_vy", vy);
	Json::SetValue(*root, "player_bottomY", bottomY);
	Json::SetValue(*root, "player_x", pos.x);
	Json::SetValue(*root, "player_y", pos.y);

	Json::SetValue(*root, "enemy_x", pos2.x);
	Json::SetValue(*root, "enemy_y", pos2.y);

	WriteJsonData(L"./save/data.Json", root);
}

void Program::Load()
{
	int status;
	float vy;
	float bottomY;
	Vector2 pos;
	Vector2 pos2;

	ReadJsonData(L"./save/data.Json", root);
	Json::GetValue(*root, "score", score);
	Json::GetValue(*root, "player_status", status);

	Json::GetValue(*root, "player_vy", vy);
	Json::GetValue(*root, "player_bottomY", bottomY);
	Json::GetValue(*root, "player_x", pos.x);
	Json::GetValue(*root, "player_y", pos.y);

	Json::GetValue(*root, "enemy_x", pos2.x);
	Json::GetValue(*root, "enemy_y", pos2.y);

	rect[0]->SetStatus((STATUS)status);
	rect[0]->SetVy(vy);
	rect[0]->SetBottomY(bottomY);
	rect[0]->GetTransform()->SetWorldPosition(pos);

	rect[1]->GetTransform()->SetWorldPosition(pos2);
}

void Program::WriteJsonData(wstring fileName, Json::Value * root)
{
	ofstream stream;
	string temp = String::WStringToString(fileName);
	stream.open(temp);
	{
		Json::StyledWriter writer;
		stream << writer.write(*root);
	}
	stream.close();
}

void Program::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}
