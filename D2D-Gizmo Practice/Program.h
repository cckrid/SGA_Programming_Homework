#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect;

	LPDIRECT3DTEXTURE9 pTex;
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