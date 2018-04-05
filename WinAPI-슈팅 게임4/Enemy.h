#pragma once

#include "GameNode.h"

// ���ʹ� �θ� Ŭ����

#define FIRECOUNT 300

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
protected:
	// ����ִ����� �׾��ִ��� ���δ� ��ӿ��� ���� �Ǽ� protected��
	bool _isLived;
	bool _isDied;
public:
	Enemy();
	~Enemy();

	virtual HRESULT Init();
	virtual HRESULT Init(const char* imageName, POINT position);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	// ����� �ؼ� Alien�� Boss���� �������̵� �ؾ���
	// EnemyManager���� Enemy Vector�� �����ϰ� �ֱ� ���� (�θ� Ŭ������)
	
	// ���ʹ� �̵�
	virtual void Move(Direction dir);
	// ���ʹ� �׸���
	virtual void Draw();
	// ���ʹ� �ִϸ��̼�
	virtual void Animation();
	// ���ʹ� �Ѿ� �߻� �������� Ȯ��
	virtual bool BulletCountFire();
	// ���ʹ̰� �浹�ߴ��� Ȯ��
	bool CheckCollision(Direction dir);
	
	void SetPosition(POINT pos);

	RECT GetRect() { return _rc; }
	POINT GetPos() { return _pos; }

	void SetInitPos(POINT pos) { _initPos = pos; }
	POINT GetInitPos() { return _initPos; }

	void SetLived(bool isLived) { _isLived = isLived; }
	bool GetLived() { return _isLived; }

	void SetDied(bool isDied) { _isDied = isDied; }
	bool GetDied() { return _isDied; }

	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }
};