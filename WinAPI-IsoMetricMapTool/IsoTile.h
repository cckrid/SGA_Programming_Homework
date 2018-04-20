#pragma once

#define CELL_WIDTH 100
#define CELL_HEIGHT 50

#define RADIUS_WIDTH (CELL_WIDTH / 2)
#define RADIUS_HEIGHT (CELL_HEIGHT / 2)

// �ʱ� ��ǥ
#define INIT_X 400
#define INIT_Y 20

#define TILE_COUNT_X 20
#define TILE_COUNT_Y 20

#define TILE_SIZE_X 8
#define TILE_SIZE_Y 7

enum TILEKIND {
	TILEKIND_OBJECT,
	TILEKIND_TERRAIN,
	TILEKIND_NONE
};

struct tagTile {
	int left, top;

	vector< pair<TILEKIND, POINT> > frameIndex;
	int height;
};