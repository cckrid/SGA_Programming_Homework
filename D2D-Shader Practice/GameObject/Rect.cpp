#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init()
{
	// ���̴� �ʱ�ȭ
	// ������ Ȯ���� �����Ƿ� 
	// ���̴� �ε��� ������ ���� ���� ������ �޾ƿ� ��
	LPD3DXBUFFER pError = NULL;

	// ���̴� �ɼ� ����
	DWORD shaderFlag = 0;
	// �����Ǿ�߸� ������ ���̴� Ȯ���� ������
#ifdef _DEBUG
	shaderFlag |= D3DXSHADER_DEBUG;
#endif // _DEBUG

	// ���̴� ���ο��� �� �� �ִ� �� #define, #include 
	D3DXCreateEffectFromFile(
		D2D::GetDevice(),			// ����̽�
									//L"./Shader/BaseColor.fx",	// ���̴� ����
		L"./Shader/ColorTexture.fx",
		NULL,						// ���̴� �����Ͻ� �߰� #define
		NULL,						// ���̴� �����Ͻ� �߰� #include
									// include�� �� �� �ִ°�
									// �ܺο��� ���η� �߰� �� ���� ����
									// �߾��� ����
		shaderFlag,					// ���̴� �ɼ�
		NULL, // ���̴� �Ű������� ������ �޸�Ǯ
		&pEffect,
		&pError
	);

	if (pError != NULL) {
		string lastError = (char*)pError->GetBufferPointer();

		MessageBox(NULL, String::StringToWString(lastError).c_str(),
			L"Shader Error", MB_OK);
		SAFE_RELEASE(pError);
	}

	vertice[0].position = Vector2(-50, 50);
	vertice[1].position = Vector2(-50, -50);
	vertice[2].position = Vector2(50, -50);
	vertice[3].position = Vector2(50, 50);

	//vertice[0].color = 0xff808080;
	//vertice[1].color = 0xff808080;
	//vertice[2].color = 0xff808080;
	//vertice[3].color = 0xff808080;

	vertice[0].color = 0xff000000;
	vertice[1].color = 0xff000000;
	vertice[2].color = 0xff000000;
	vertice[3].color = 0xff000000;

	vertice[0].uv = Vector2(0, 1.0f / 4.0f);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(1.0f / 8.0f, 0);
	vertice[3].uv = Vector2(1.0f / 8.0f, 1.0f / 4.0f);

	//vertice[0].uv = Vector2(0, 1);
	//vertice[1].uv = Vector2(0, 0);
	//vertice[2].uv = Vector2(1, 0);
	//vertice[3].uv = Vector2(1, 1);

	stride = sizeof(Vertex);
	//FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	HRESULT hr = D2D::GetDevice()->CreateVertexBuffer(
		stride * 4,
		D3DUSAGE_WRITEONLY,		// dynamic ���� �Ǹ�
		FVF,
		D3DPOOL_MANAGED,		// �̰ɷ� ������� default ���ָ� data �������� ����
		&vb,
		NULL
	);
	assert(SUCCEEDED(hr));

	Vertex * pVertex = NULL;
	hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	DWORD indices[] = {
		0,1,2,
		0,2,3,
	};

	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&ib,
		NULL
	);
	assert(SUCCEEDED(hr));

	void* pIndex;
	hr = ib->Lock(0, 0, &pIndex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pIndex, indices, sizeof(DWORD) * 6);
	hr = ib->Unlock();
	assert(SUCCEEDED(hr));

	transform = new Transform;
	
	transform->UpdateTransform();

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		L"Textures/mario_all.png",
		//L"Textures/Box.png",
		&pTex
	);
	assert(SUCCEEDED(hr));

	isLeft = false;
	status = STATUS_NORMAL;

	transform->SetWorldPosition(Vector2(0, WINSIZE_Y / 2 - 25 - 50));
	transform->SetScale(Vector2(0.5f, 0.5f));
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);
}

void Rect::Update()
{
	switch (status)
	{
	case STATUS_NORMAL:
		Jump();

		if (isLeft) {
			currentIndexX = 4;
			currentIndexY = 3;
		}
		else {
			currentIndexX = 0;
			currentIndexY = 3;
		}

		if (INPUT->GetKey(VK_LEFT)) {
			status = STATUS_ONLAND;
			isLeft = true;
			currentIndexX = 4;
			currentIndexY = 0;
		}
		if (INPUT->GetKey(VK_RIGHT)) {
			status = STATUS_ONLAND;
			isLeft = false;
			currentIndexX = 0;
			currentIndexY = 0;
		}

		break;
	case STATUS_ONLAND:
		Move();
		Jump();

		if (INPUT->GetKeyUp(VK_LEFT)) {
			status = STATUS_NORMAL;
			isLeft = true;
			currentIndexX = 4;
			currentIndexY = 3;
		}
		if (INPUT->GetKeyUp(VK_RIGHT)) {
			status = STATUS_NORMAL;
			isLeft = false;
			currentIndexX = 0;
			currentIndexY = 3;
		}
		
		break;
	case STATUS_TOUP:
		if (isLeft) {
			currentIndexX = 6;
			currentIndexY = 0;
		}
		else {
			currentIndexX = 2;
			currentIndexY = 0;
		}

		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		if (vy >= 0) {
			status = STATUS_TOLOW;
			if (isLeft) {
				currentIndexX = 7;
				currentIndexY = 0;
			}
			else {				
				currentIndexX = 3;
				currentIndexY = 0;
			}
		}
		break;
	case STATUS_TOLOW:
		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		if (transform->GetWorldPosition().y >= bottom.y) {
			transform->SetWorldPosition(
				Vector2(transform->GetWorldPosition().x,
				bottom.y));
			status = STATUS_NORMAL;
		}
		break;
	}
	

	float tempX = (int)currentIndexX / 8.0f;
	float tempY = (int)currentIndexY / 4.0f;
	vertice[0].uv = Vector2(tempX, tempY + 1.0f/4.0f);
	vertice[1].uv = Vector2(tempX, tempY);
	vertice[2].uv = Vector2(tempX + 1.0f/8.0f, tempY);
	vertice[3].uv = Vector2(tempX + 1.0f/8.0f, tempY + 1.0f/4.0f);

	Vertex * pVertex = NULL;
	HRESULT hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	this->transform->DefaultControl2();
	this->DrawInterface();
}

void Rect::Render()
{
	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	//D2D::GetDevice()->SetIndices(ib);
	//D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	//D2D::GetDevice()->SetFVF(FVF);
	//D2D::GetDevice()->SetTexture(0, pTex);
	//D2D::GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0, 0, 4, 0, 2);

	
	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");

	// ���̴��� ����
	UINT iPassNum = 0;

	this->pEffect->Begin(
		&iPassNum,			// pEffect�� �ִ� �н� ���� �޾ƿ´�.
		0					// �÷��� 
	);
	{
		for (UINT i = 0; i < iPassNum; i++) {
			this->pEffect->BeginPass(i);
			{
				D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
				D2D::GetDevice()->SetIndices(ib);
				D2D::GetDevice()->SetFVF(FVF);

				D2D::GetDevice()->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, 4, 0, 2);
			}
			this->pEffect->EndPass();
		}
	}

	this->pEffect->End();
	
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void Rect::Move() {
	float posX = transform->GetWorldPosition().x;
	
	if (INPUT->GetKey(VK_LEFT)) {
		//transform->MovePositionSelf(Vector2(-SPEED, 0));

		if (posX - 25> -WINSIZE_X / 2) {
			if (posX >= WINSIZE_X / 4)
				transform->MovePositionSelf(Vector2(-SPEED, 0));
			else if (bgTransform->GetWorldPosition().x <
				WINSIZE_X / 2)
				bgTransform->MovePositionSelf(Vector2(SPEED, 0));
			else
				transform->MovePositionSelf(Vector2(-SPEED, 0));
		}
		
		isLeft = true;

		// �ٰ��ִ� ������ ���
		if (status == STATUS_ONLAND) {
			currentIndexX += 0.125f;
			if (currentIndexX >= 6) {
				currentIndexX = 4;
			}
		}
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		//transform->MovePositionSelf(Vector2(SPEED, 0));

		if (posX + 25 < WINSIZE_X / 2) {
			if (posX <= -WINSIZE_X / 4)
				transform->MovePositionSelf(Vector2(SPEED, 0));
			else if (bgTransform->GetWorldPosition().x >
				-WINSIZE_X / 2)
				bgTransform->MovePositionSelf(Vector2(-SPEED, 0));
			else
				transform->MovePositionSelf(Vector2(SPEED, 0));
		}

		isLeft = false;

		// �ٰ��ִ� ������ ���
		if (status == STATUS_ONLAND) {
			currentIndexX += 0.125f;
			if (currentIndexX >= 2) {
				currentIndexX = 0;
			}
		}
	}
}

void Rect::Jump() {
	if (INPUT->GetKeyDown(VK_SPACE)) {
		if (isLeft) {
			currentIndexX = 6;
			currentIndexY = 0;
		}
		else {
			currentIndexX = 2;
			currentIndexY = 0;
		}
		vy = JUMP;
		// �ѹ� ������ �ϱ� ���ؼ� ���� �ٲٰ� �ű⼭�� Jump() �Լ� ȣ�� ���ϰ�
		// ���� �����ϰ� ������ �ű⼭�� Jump() ȣ���ϸ��
		status = STATUS_TOUP;
		bottom = transform->GetWorldPosition();
	}
}

void Rect::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Interface");
	{
		transform->DrawInterface();
	}
	ImGui::End();
#endif // IMGUI_USE
}
