#include "stdafx.h"
#include "BackGround.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

BackGround::BackGround() {

}

BackGround::~BackGround() {

}

void BackGround::Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot)
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
									//L"./Shader/TextureMapping.fx",
									//L"./Shader/MultiTexture.fx",
		shaderFile.c_str(),
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

	vertice[0].position = Vector2(-WINSIZE_X / 2 + pivot.x, WINSIZE_Y / 2 + pivot.y);
	vertice[1].position = Vector2(-WINSIZE_X / 2 + pivot.x, -WINSIZE_Y / 2 + pivot.y);
	vertice[2].position = Vector2(WINSIZE_X / 2 + pivot.x, -WINSIZE_Y / 2 + pivot.y);
	vertice[3].position = Vector2(WINSIZE_X / 2 + pivot.x, WINSIZE_Y / 2 + pivot.y);

	vertice[0].color = 0xffffffff;
	vertice[1].color = 0xffffffff;
	vertice[2].color = 0xffffffff;
	vertice[3].color = 0xffffffff;

	vertice[0].uv = Vector2(0, uv.y);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(uv.x, 0);
	vertice[3].uv = uv;

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

	pTex = NULL;
}

void BackGround::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	//SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);
}

void BackGround::Update()
{
	this->DrawInterface();
}

void BackGround::Render()
{
	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			1,
			1,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			0,
			0,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void BackGround::RenderRect()
{
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
}

void BackGround::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Interface");
	{
		transform->DrawInterface();
	}
	ImGui::End();
#endif // IMGUI_USE
}