#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Rect* rect;
	class BackGround* bg;
	class Camera* mainCamera;
	class BaseBackGround* baseBg;
	class MiniMap* miniMap;

	LPDIRECT3DTEXTURE9 pTex[3];

	// �ؽ��� ���� ������ ������ ��
	LPDIRECT3DTEXTURE9 pRenderTexture;
	// �ؽ��� ���� ǥ�� (���� ����)
	LPDIRECT3DSURFACE9 pRenderSurface;

	int currentBg;
public:
	Program();
	~Program();

	void Update();
	void Render();

	void CreateRenderTargetTexture();
};

/*
	����
	������ �پ�ٴϰ� �����ϰ� �ϴ� �� �����ϱ�
*/