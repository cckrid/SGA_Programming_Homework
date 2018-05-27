#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init(wstring shaderFile, const Vector2 min, const Vector2 max,
	const DWORD color, bool isTex)
{
	isTexture = isTex;

	InitShader(shaderFile);

	InitVertex(color, min, max);

	InitBuffer();

	transform = new Transform;
	transform->UpdateTransform();

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - (vertice[1].position);

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	pTex = NULL;

	deltaTime = 0.0f;

	for (int i = 0; i < TEMPSIZE; i++) {
		tempTrans[i] = new Transform;
	}

	tempTrans[0]->SetWorldPosition(transform->GetWorldPosition());
	isInterpolation = false;

	delta = 0;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	//SAFE_RELEASE(pTex);

	SAFE_DELETE(transform);
	SAFE_DELETE(clips);

	for (int i = 0; i < TEMPSIZE; i++)
		SAFE_DELETE(tempTrans[i]);

	SAFE_DELETE(collider);
	SAFE_DELETE(tempCollider);
}

void Rect::Update()
{
	//this->transform->DefaultControl2();
	this->DrawInterface();

	if(isInterpolation)
	tempTrans[0]->Interpolate(this->tempTrans[0],
		transform, Frame::Get()->GetFrameDeltaSec() * 3.5f);

	if(isInterpolation &&
		FLOATEQUAL(tempTrans[0]->GetWorldPosition().x, 
		tileTrans->GetWorldPosition().x) &&
		FLOATEQUAL(tempTrans[0]->GetWorldPosition().y, 
			tileTrans->GetWorldPosition().y) && !isTexture)
		delta += 0.01f;
}

void Rect::Render()
{

	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());
	this->pEffect->SetFloat("delta", delta);

	if (isTexture) {
		this->pEffect->SetVector("maxFrame",
			&D3DXVECTOR4(
				1.0f,
				1.0f,
				0.0f, 0.0f));
		this->pEffect->SetVector("currentFrame",
			&D3DXVECTOR4(
				0.0f,
				0.0f,
				0.0f, 0.0f));

		this->pEffect->SetTexture("tex", pTex);
	}

	this->pEffect->SetTechnique("MyShader");
	
	if(isInterpolation)
		this->pEffect->SetMatrix("matWorld", &tempTrans[0]->GetFinalMatrix().ToDXMatrix());
	else
		this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());

	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	//transform->RenderGizmo(true);
	//collider->RenderGizmo(transform);
}

void Rect::RenderRect()
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

void Rect::InitShader(wstring shaderFile)
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
}

void Rect::InitVertex(DWORD color, Vector2 min, Vector2 max)
{
	vertice[0].position = Vector2(min.x, max.y);
	vertice[1].position = Vector2(min.x, min.y);
	vertice[2].position = Vector2(max.x, min.y);
	vertice[3].position = Vector2(max.x, max.y);

	vertice[0].uv = Vector2(0, 1);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(1, 0);
	vertice[3].uv = Vector2(1, 1);

	vertice[0].color = color;
	vertice[1].color = color;
	vertice[2].color = color;
	vertice[3].color = color;

	stride = sizeof(Vertex);
	if(isTexture)
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	else
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
}

void Rect::InitBuffer()
{
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
}

void Rect::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Interface");
	{
		if (check) {
			ImGui::Text("Mouse Check True");
		}
		else {
			ImGui::Text("Mouse Check False");
		}

		if (colliderCheck) {
			ImGui::Text("Collider Check True");
		}
		else {
			ImGui::Text("Collider Check False");
		}

		transform->DrawInterface();
	}
	ImGui::End();
#endif // IMGUI_USE
}