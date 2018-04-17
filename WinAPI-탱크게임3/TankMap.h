#pragma once

#include "GameNode.h"
#include "TileMap.h"

// Ÿ�� ������ ��ũ���� �Ѱ��ָ� ��ũ�� �˾Ƽ� ó���ϰ� �Ҳ�

enum TANKDIRECTION {
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_DOWN
};

class TankMap : public GameNode
{
private:
	tagTile _tiles[TILEX * TILEY];
	// DWORD Ư������ �ƴ� 4byte�� �Ȱ� ��� �༮
	DWORD _attributes[TILEX * TILEY];
	int _pos[2];
public:
	TankMap();
	~TankMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void Load();

	tagTile* GetTiles() { return _tiles; }
	int GetPosFirst() { return 0; }
	int GetPosSecond() { return TILEX * TILEY - 1; }
	DWORD* GetAttribute() { return _attributes; }


	POINT GetOBJECTFrame(OBJECT obj);
};

