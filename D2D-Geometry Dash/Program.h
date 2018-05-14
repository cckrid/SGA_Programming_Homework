#pragma once

// MainGame��� ���ư���

static DWORD scoreToColor[] = {
	0xffffffff,
	0xff0000ff,
	0xff00ff00,
	0xffa52a2a,
	0xffff6347,
	0xff6b8e23,
	0xffffd700,
};

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

	int colorIndex = 0;
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

	void ChangeBGColor();
};

/*
	����
	������ �پ�ٴϰ� �����ϰ� �ϴ� �� �����ϱ�
*/