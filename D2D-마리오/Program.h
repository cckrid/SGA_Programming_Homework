#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class Rect* mario;
	class BackGround* bg;

	LPDIRECT3DTEXTURE9 pTex[3];
public:
	Program();
	~Program();

	void Update();
	void Render();
};