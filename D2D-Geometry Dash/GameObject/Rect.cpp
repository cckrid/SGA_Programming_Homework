#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

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

	vertice[0].color = 0xffffffff;
	vertice[1].color = 0xffffffff;
	vertice[2].color = 0xffffffff;
	vertice[3].color = 0xffffffff;

	vertice[0].uv = Vector2(0, 1);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(1, 0);
	vertice[3].uv = Vector2(1, 1);

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

	//hr = D3DXCreateTextureFromFile(
	//	D2D::GetDevice(),
	//	//L"Textures/mario_all.png",
	//	//L"Textures/Box.png",
	//	L"Textures/rect.png",
	//	&pTex
	//);
	assert(SUCCEEDED(hr));

	deltaTime = 0.0f;

	transform->SetScale(Vector2(0.5f, 0.5f));
	transform->MovePositionSelf(Vector2(-75, 75));

	isPlayer = false;
	status = STATUS_IDLE;

	//AnimationData data;
	//clips = new AnimationClip;
	//Json::Value* root = new Json::Value();
	//
	//for (int i = 0; i < 4; i++) {
	//	data.keyName = L"run_" + to_wstring(i);
	//	data.maxFrame = Vector2(8.0f, 4.0f);
	//	data.currentFrame = Vector2(float(i), 0.0f);
	//	clips->PushAnimationData(data);
	//	Json::SetValue(*root, "run_" + to_string(i), (float&)i);
	//}
	//
	//WriteJsonData(L"Test.Json", root);
	//
	//Json::Value* ReadJson = new Json::Value();
	//ReadJsonData(L"Test.Json", ReadJson);
	//
	//float temp;
	//Json::GetValue(*ReadJson, "run_1", temp);
	//
	//// SAFE_RELEASE �� �Լ� release�� �ִ� �� Ȯ���ϸ��
	//SAFE_DELETE(ReadJson);
	//SAFE_DELETE(root);
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	//SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);
	SAFE_DELETE(clips);
}

void Rect::Update()
{
	//deltaTime += Frame::Get()->GetFrameDeltaSec();
	//int currentIndexX = rand() % 8;
	//int currentIndexY = rand() % 4;
	//if (deltaTime > 0.5f) {
	//	deltaTime = 0.0f;
	//	float tempX = (float)currentIndexX / 8.0f;
	//	float tempY = (float)currentIndexY / 4.0f;
	//	vertice[0].uv = Vector2(tempX, tempY + 1.0f/4.0f);
	//	vertice[1].uv = Vector2(tempX, tempY);
	//	vertice[2].uv = Vector2(tempX + 1.0f/8.0f, tempY);
	//	vertice[3].uv = Vector2(tempX + 1.0f/8.0f, tempY + 1.0f/4.0f);
	//}
	//Vertex * pVertex = NULL;
	//HRESULT hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	//assert(SUCCEEDED(hr));
	//memcpy(pVertex, vertice, stride * 4);
	//hr = vb->Unlock();
	//assert(SUCCEEDED(hr));

	if (isPlayer) {
		switch (status)
		{
		case STATUS_IDLE:
			Jump();
			break;
		case STATUS_JUMP:
			transform->MovePositionSelf(Vector2(0, vy));
			vy += GRAVITY;
			if (transform->GetWorldPosition().y >= bottom.y) {
				transform->SetWorldPosition(
					Vector2(transform->GetWorldPosition().x,
						bottom.y));
				status = STATUS_IDLE;
			}
			break;
		}
	}
	else {
		transform->MovePositionSelf(Vector2(-2.5f, 0));
		if (transform->GetWorldPosition().x + 25 < -WINSIZE_X / 2) {
			transform->MovePositionSelf(Vector2(WINSIZE_X, 0));
		}
	}

	this->transform->DefaultControl2();
	this->DrawInterface();
	//clips->Update(AniRepeatType_Loop);
}

void Rect::Render()
{
	//D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	//D2D::GetDevice()->SetIndices(ib);
	//D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	//D2D::GetDevice()->SetFVF(FVF);
	//D2D::GetDevice()->SetTexture(0, pTex);
	//D2D::GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0, 0, 4, 0, 2);

	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	//this->pEffect->SetVector("maxFrame",
	//	&D3DXVECTOR4(
	//		clips->GetCurrentData().maxFrame.x,
	//		clips->GetCurrentData().maxFrame.y,
	//		0.0f, 0.0f));
	//this->pEffect->SetVector("currentFrame",
	//	&D3DXVECTOR4(
	//		clips->GetCurrentData().currentFrame.x,
	//		clips->GetCurrentData().currentFrame.y,
	//		0.0f, 0.0f));

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
				// ���࿡ �ؽ�ó �����ϴ� ����̸� pEffect->setTexture��
				//D2D::GetDevice()->SetTexture(0, pTex);

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

void Rect::Jump() {
	if (INPUT->GetKeyDown(VK_SPACE)) {
		vy = JUMP;
		status = STATUS_JUMP;
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
