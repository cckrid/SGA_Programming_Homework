#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"
#include "GameObject\BaseBackGround.h"
#include "GameObject\MiniMap.h"
#include "./Common/Camera.h"

Program::Program()
{
	srand(time(NULL));

	mainCamera = new Camera;

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
	rect->Init();
	rect->SetCamera(mainCamera);
	rect->SetBgTransform(bg->GetTransform());

	baseBg = new BaseBackGround;
	baseBg->Init();

	miniMap = new MiniMap;
	miniMap->Init();

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	CreateRenderTargetTexture();

	miniMap->SetTexture(pRenderTexture);
}

Program::~Program()
{
	rect->Release();
	bg->Release();
	baseBg->Release();
	miniMap->Release();
	SAFE_DELETE(rect);
	SAFE_DELETE(bg);
	SAFE_DELETE(baseBg);
	SAFE_DELETE(miniMap);
	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	mainCamera->UpdateCamToDevice();
	bg->Update();
	baseBg->Update();
	rect->Update();
	miniMap->Update();
}

void Program::Render()
{
	bg->Render();
	rect->Render();

	// �ĸ���ۿ� �����͸� ���� �༮�� ���� ������
	LPDIRECT3DSURFACE9 pDeviceTargetSurface;
	// stencil�̶� depth ������ �༮
	LPDIRECT3DSURFACE9 pDeviceDepthAndStencilSuface;

	D2D::GetDevice()->GetRenderTarget(0, &pDeviceTargetSurface);
	D2D::GetDevice()->GetDepthStencilSurface(
		&pDeviceDepthAndStencilSuface);

	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->
		GetSurfaceLevel(0, &texSurface))) {
		D2D::GetDevice()->SetRenderTarget(0, texSurface);
		SAFE_RELEASE(texSurface);
	}

	D2D::GetDevice()->SetDepthStencilSurface(pRenderSurface);

	D2D::GetDevice()->Clear(
		0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xFF00FF00,
		1.0f, 0);

	// beginScene endScene Present �Ƚᵵ�� �ؽ�ó�� �ٷ� �׸��°Ŵ�
	baseBg->Render();
	Vector2 pos = rect->GetTransform()->GetWorldPosition();
	rect->GetTransform()->SetWorldPosition(
		Vector2(pos.x / 2 - bg->GetTransform()->GetWorldPosition().x / 2, pos.y)
	);
	rect->Render();
	rect->GetTransform()->SetWorldPosition(pos);

	D2D::GetDevice()->SetRenderTarget(0, pDeviceTargetSurface);
	D2D::GetDevice()->SetDepthStencilSurface(pDeviceDepthAndStencilSuface);

	SAFE_RELEASE(pDeviceDepthAndStencilSuface);
	SAFE_RELEASE(pDeviceTargetSurface);

	//if (Input::Get()->GetKeyDown(VK_TAB)) {
	//	D3DXSaveTextureToFile(
	//		L"Test.png",
	//		D3DXIFF_PNG,
	//		pRenderTexture,
	//		NULL
	//	);
	//}

	miniMap->Render();
}

void Program::CreateRenderTargetTexture()
{
	int width = 1024;
	int height = 512;

	// �׳� �ؽ�ó ���� ����°�
	D2D::GetDevice()->CreateTexture(
		width, height,
		1,
		D3DUSAGE_RENDERTARGET, // ��� �뵵
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pRenderTexture,
		NULL
	);

	// depth �� stencil�� ���� �ִ� surface ����
	// �� ũ�� �����ؾߵ� texture�� surface
	D2D::GetDevice()->CreateDepthStencilSurface(
		width, height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,				// ���۸� ��ü�Ҷ� ���� ��ü ������ �������� ���� ���̳�
		&pRenderSurface,
		NULL
	);
}
