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
		L"./Shader/TextureMapping.fx",
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

	vertice[0].position = Vector2(-50, 50); // ���ϴ�
	vertice[1].position = Vector2(-50, -50); // �»��
	vertice[2].position = Vector2(50, -50); // ����
	vertice[3].position = Vector2(50, 50); // ���ϴ�

	//vertice[0].color = 0xffff0000;
	//vertice[1].color = 0xffffff00;
	//vertice[2].color = 0xff00ff00;
	//vertice[3].color = 0xff0000ff;
	vertice[0].uv = Vector2(0, 1);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(1, 0);
	vertice[3].uv = Vector2(1, 1);

	stride = sizeof(Vertex);
	//FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	FVF = D3DFVF_XYZ | D3DFVF_TEX1;

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
		//L"Textures/mario_all.png",
		L"Textures/BodyParts_6x1.png",
		&pTex[0]
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTextureFromFile(
		D2D::GetDevice(),
		//L"Textures/mario_all.png",
		L"Textures/BodyParts2_6x1.png",
		&pTex[1]
	);
	assert(SUCCEEDED(hr));

	deltaTime = 0.0f;

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

	SettingBodyPart();

	isRight = true;
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pTex[0]);
	SAFE_RELEASE(pTex[1]);

	SAFE_DELETE(transform);
	SAFE_DELETE(clips);

	for (int i = 0; i < BodyKind_End; i++) {
		SAFE_DELETE(child[i]);
	}
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

	this->transform->DefaultControl2();
	this->DrawInterface();
	//clips->Update(AniRepeatType_Loop);

	if (INPUT->GetKey(VK_UP)) transform->MovePositionWorld(Vector2(0, -10));
	if (INPUT->GetKey(VK_DOWN)) transform->MovePositionWorld(Vector2(0, 10));
	if (INPUT->GetKey(VK_LEFT)) {
		transform->MovePositionWorld(Vector2(-10, 0));
		//isRight = false;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		transform->MovePositionWorld(Vector2(10, 0));
		//isRight = true;
	}
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

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetTexture("tex", pTex[0]);

	this->pEffect->SetTechnique("MyShader");

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());

	//RenderRect();

	for (int i = 0; i < BodyKind_End; i++) {
		this->pEffect->SetVector("maxFrame",
			&D3DXVECTOR4(6, 1, 0.0f, 0.0f));
		this->pEffect->SetVector("currentFrame",
			&D3DXVECTOR4(i, 0,0.0f, 0.0f));
		
		pEffect->SetMatrix("matWorld", &child[i]->GetFinalMatrix().ToDXMatrix());

		RenderRect();
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

void Rect::SettingBodyPart()
{
	for (int i = 0; i < BodyKind_End; i++) {
		child[i] = new Transform;
	}
	
	Transform* temp;

	// root
	transform->MovePositionSelf(Vector2(0, 0));
	
	// ����
	temp = child[BodyKind_Body];
	temp->MovePositionSelf(Vector2(0, -110));
	
	// �Ӹ�
	temp = child[BodyKind_Head];
	temp->MovePositionSelf(Vector2(-10, -195));

	// �ٸ�
	temp = child[BodyKind_LeftLeg];
	temp->MovePositionSelf(Vector2(2.5f,  -50));
	//temp->RotateSelf(45.0f * D3DX_PI / 180.0f);
	
	temp = child[BodyKind_RightLeg];
	temp->MovePositionSelf(Vector2(27.5f, -50));
	//temp->RotateSelf(-45.0f * D3DX_PI / 180.0f);

	// ��
	temp = child[BodyKind_LeftArm];
	temp->MovePositionSelf(Vector2(-35, -110));

	temp = child[BodyKind_RightArm];
	temp->MovePositionSelf(Vector2(35, -110));

	// �θ� �ڽ� ���� ����
	transform->AddChild(child[BodyKind_Body]);
	child[BodyKind_Body]->AddChild(child[BodyKind_Head]);
	child[BodyKind_Body]->AddChild(child[BodyKind_LeftLeg]);
	child[BodyKind_Body]->AddChild(child[BodyKind_RightLeg]);
	child[BodyKind_Body]->AddChild(child[BodyKind_LeftArm]);
	child[BodyKind_Body]->AddChild(child[BodyKind_RightArm]);
	transform->SetScale(Vector2(1.5f, 1.5f));
	transform->MovePositionSelf(Vector2(-100, 110 + 200));
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