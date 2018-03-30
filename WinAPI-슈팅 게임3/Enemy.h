#pragma once

#include "GameNode.h"

// ���ʹ� �θ� Ŭ����

#define FIRECOUNT 1000

enum Direction {
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
};

class Enemy : public GameNode
{
private:
	Image * _image;
	RECT _rc;
	POINT _pos;
	float _speed;

	// ������ �̹����� ������ ���� ����
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	// ����ؼ� �ֱ������� ����
	int _fireCount;
	// ������ ���� ������ �ִٰ� fireCount�� ���̰� �Ǹ� �߻��ϴ� ��������
	int _rndFireCount;

	POINT _initPos;
	bool _isLive;
public:
	Enemy();
	~Enemy();

	virtual HRESULT Init();
	virtual HRESULT Init(const char* imageName, POINT position);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Move(Direction dir);
	virtual void Draw();
	virtual void Animation();

	bool BulletCountFire();

	bool CheckCollision(Direction dir);

	void SetPosition(POINT pos);

	RECT GetRect() { return _rc; }
};