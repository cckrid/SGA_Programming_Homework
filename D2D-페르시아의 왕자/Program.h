#pragma once

// MainGame��� ���ư���

class Program
{
private:
	Matrix matView;
	Matrix matProjection;
	// viewPort �ʿ���� 
	// ������ ���ڴٰ� ����� �������� window �������� �Ǿ�����
	Vector2 vEye;

	class Rect* rect;
	class BackGround* bg;
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