#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect;
	class BackGround* bg;

	LPDIRECT3DTEXTURE9 pTex[2];
public:
	Program();
	~Program();

	void Update();
	void Render();
};

/*
	����
	������ �پ�ٴϰ� �����ϰ� �ϴ� �� �����ϱ�
*/