#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class BackGround* bg[2];
	class BackGround* bottom[2];
	class Rect* rect[2];

	LPDIRECT3DTEXTURE9 pTex[4];
	
	LPD3DXFONT font;

	bool isPause;
	int score = 0;

	Json::Value* root;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void Init();
	void Save();
	void Load();

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);
};

/*
	����
	������ �پ�ٴϰ� �����ϰ� �ϴ� �� �����ϱ�
*/