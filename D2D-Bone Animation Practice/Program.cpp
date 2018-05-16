#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	SOUND->AddSound("Test", "sounds/����3.wav", true, true);

	mainCamera = new Camera;

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
	rect->Init();
	rect->SetCamera(mainCamera);

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	//SOUND->Play("Test");
}

Program::~Program()
{
	bg->Release();
	rect->Release();

	SAFE_DELETE(bg);
	SAFE_DELETE(rect);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();
	bg->Update();
	rect->Update();

}

void Program::Render()
{
	bg->Render();
	rect->Render();
}
