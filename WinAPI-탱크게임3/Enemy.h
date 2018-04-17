#pragma once

#include "GameNode.h"
#include "TankMap.h"

class Tank;

class Enemy : public GameNode
{
private:
	TANKDIRECTION _direction;
	Image * _image;
	int frameX, frameY;
	RECT _rc;
	float _x, _y;
	float _speed;

	TankMap * _tankMap;

	Ray ray;

	vector<int> vTileIndex;

	int cnt;
	TANKDIRECTION currentDir;

	Tank * tank;

	bool isLive;
public:
	Enemy();
	~Enemy();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void TankMove();
	void TankFire();
	void SetTankPosition();
	void SetTankMapMemoryLink(TankMap * tm) { _tankMap = tm; }
	void SetTankMemoryLink(Tank * tank) { this->tank = tank; }
	RECT GetRect() { return _rc; }
	void SetIsLive(bool isLive) { this->isLive = isLive; }
	bool GetIsLive() { return isLive; }
};

