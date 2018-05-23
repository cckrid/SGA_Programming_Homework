#pragma once

// MainGame��� ���ư���

enum BodyPart {
	BODYPART_HEAD,
	BODYPART_BODY,
	BODYPART_LEFTARM,
	BODYPART_RIGHTARM,
	BODYPART_LEFTLEG,
	BODYPART_RIGHTLEG,
	BODYPART_END
};

class Program
{
private:
	class Camera* mainCamera;

	class Rect* bodyPart[BODYPART_END];

	int currentPart;

	LPDIRECT3DTEXTURE9 pTex[BODYPART_END];

	Json::Value* root;
	Json::Value* readJson;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);

	void InitTransform(class Transform* pTrans);
	void SaveTransform(class Transform* pTrans, wstring name);
	void LoadTransform(class Transform* outTrans, wstring name);
};