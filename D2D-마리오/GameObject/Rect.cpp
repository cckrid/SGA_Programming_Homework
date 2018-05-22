#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot)
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

	vertice[0].position = Vector2(-50 + pivot.x, 50 + pivot.y);
	vertice[1].position = Vector2(-50 + pivot.x, -50 + pivot.y);
	vertice[2].position = Vector2(50 + pivot.x, -50 + pivot.y);
	vertice[3].position = Vector2(50 + pivot.x, 50 + pivot.y);

	vertice[0].color = 0xffcccccc;
	vertice[1].color = 0xffcccccc;
	vertice[2].color = 0xffcccccc;
	vertice[3].color = 0xffcccccc;

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

	deltaTime = 0.0f;

	for (int i = 0; i < MAX_SHADOW; i++) {
		shadows[i] = new Transform;
	}

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - vertice[0].position;

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	shadowCnt = 0;

	InitAnimation();

	isLeft = 0;
	status = STATUS_NORMAL;

	clips[status + isLeft]->Play();

	transform->SetWorldPosition(Vector2(-WINSIZE_X / 2 + 50, -17));
	transform->SetScale(Vector2(0.5f, 0.5f));
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);

	SAFE_DELETE(transform);
	for (int i = 0; i < STATUS_END * 2; i++)
		SAFE_DELETE(clips[i]);

	for (int i = 0; i < MAX_SHADOW; i++)
		SAFE_DELETE(shadows[i]);

	SAFE_DELETE(collider);
}

void Rect::Update()
{
	this->transform->DefaultControl2();
	this->DrawInterface();

	switch (status)
	{
	case STATUS_NORMAL:
		Jump();

		if (INPUT->GetKey(VK_LEFT)) {
			status = STATUS_ONLAND;
			isLeft = STATUS_END;
			clips[status + isLeft]->Play();
		}
		if (INPUT->GetKey(VK_RIGHT)) {
			status = STATUS_ONLAND;
			isLeft = 0;
			clips[status + isLeft]->Play();
		}

		break;
	case STATUS_ONLAND:
		Move();
		Jump();

		if (INPUT->GetKeyUp(VK_LEFT)) {
			status = STATUS_NORMAL;
			isLeft = STATUS_END;
			clips[status + isLeft]->Play();
		}
		if (INPUT->GetKeyUp(VK_RIGHT)) {
			status = STATUS_NORMAL;
			isLeft = 0;
			clips[status + isLeft]->Play();
		}

		break;
	case STATUS_TOUP:

		Move();
		transform->MovePositionSelf(Vector2(0, vy));
		vy += GRAVITY;
		if (vy >= 0) {
			status = STATUS_TOLOW;
			clips[status + isLeft]->Play();
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
			clips[status + isLeft]->Play();
		}
		break;
	}

	if (shadowCnt > 0) {
		shadows[0]->Interpolate(this->shadows[0],
			transform, Frame::Get()->GetFrameDeltaSec() * 10.0f);
	}
	for (int i = 1; i < shadowCnt; i++) {
		shadows[i]->Interpolate(
			this->shadows[i],
			shadows[i - 1],
			// �� ���� 1�� ������ �پ ������
			Frame::Get()->GetFrameDeltaSec() * 10.0f);
	}

	clips[status + isLeft]->Update(AniRepeatType_Loop);
}

void Rect::Render()
{
	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			clips[status + isLeft]->GetCurrentData().maxFrame.x,
			clips[status + isLeft]->GetCurrentData().maxFrame.y,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			clips[status + isLeft]->GetCurrentData().currentFrame.x,
			clips[status + isLeft]->GetCurrentData().currentFrame.y,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");
	
	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	for (int i = 0; i < shadowCnt; i++) {
		this->pEffect->SetMatrix("matWorld", &shadows[i]->GetFinalMatrix().ToDXMatrix());
		this->RenderRect();
	}

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
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

void Rect::InitAnimation()
{
	AnimationData data;
	for (int i = 0; i < STATUS_END * 2; i++) {
		clips[i] = new AnimationClip;
	}

	data.maxFrame = Vector2(8.0f, 4.0f);
	//data.time = 5.0f;

	for (int i = 0; i < STATUS_END; i++) {
		switch (i)
		{
		case STATUS_NORMAL:
			data.keyName = L"Normal_Left_" + to_wstring(0);
			data.currentFrame = Vector2(4, 3);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"Normal_Right_" + to_wstring(0);
			data.currentFrame = Vector2(0, 3);
			clips[i]->PushAnimationData(data);
			break;
		case STATUS_ONLAND:
			data.keyName = L"ONLAND_Left_" + to_wstring(0);
			data.currentFrame = Vector2(4, 0);
			clips[i + STATUS_END]->PushAnimationData(data);
			data.keyName = L"ONLAND_Left_" + to_wstring(1);
			data.currentFrame = Vector2(5, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"ONLAND_Right_" + to_wstring(0);
			data.currentFrame = Vector2(0, 0);
			clips[i]->PushAnimationData(data);
			data.keyName = L"ONLAND_Right_" + to_wstring(1);
			data.currentFrame = Vector2(1, 0);
			clips[i]->PushAnimationData(data);

			break;
		case STATUS_TOUP:
			data.keyName = L"TOUP_Left_" + to_wstring(0);
			data.currentFrame = Vector2(6, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"TOUP_Right_" + to_wstring(0);
			data.currentFrame = Vector2(2, 0);
			clips[i]->PushAnimationData(data);

			break;
		case STATUS_TOLOW:
			data.keyName = L"TOUP_Left_" + to_wstring(0);
			data.currentFrame = Vector2(7, 0);
			clips[i + STATUS_END]->PushAnimationData(data);

			data.keyName = L"TOUP_Right_" + to_wstring(0);
			data.currentFrame = Vector2(3, 0);
			clips[i]->PushAnimationData(data);

			break;
		}

	}
	 
}

void Rect::Move() {
	float posX = transform->GetWorldPosition().x;

	if (INPUT->GetKey(VK_LEFT)) {
		if (posX - 25 > -WINSIZE_X / 2) {
			transform->MovePositionSelf(Vector2(-SPEED, 0));
		}

		isLeft = STATUS_END;
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		if (posX + 25 < WINSIZE_X / 2) {
			transform->MovePositionSelf(Vector2(SPEED, 0));
		}

		isLeft = 0;
	}
}

void Rect::Jump() {
	if (INPUT->GetKeyDown(VK_SPACE)) {
		vy = JUMP;
		// �ѹ� ������ �ϱ� ���ؼ� ���� �ٲٰ� �ű⼭�� Jump() �Լ� ȣ�� ���ϰ�
		// ���� �����ϰ� ������ �ű⼭�� Jump() ȣ���ϸ��
		status = STATUS_TOUP;
		bottom = transform->GetWorldPosition();

		clips[status + isLeft]->Play();
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

void Rect::WriteJsonData(wstring fileName, Json::Value * root)
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

void Rect::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}