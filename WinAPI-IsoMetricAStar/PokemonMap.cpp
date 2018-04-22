#include "stdafx.h"
#include "PokemonMap.h"


PokemonMap::PokemonMap()
{
}


PokemonMap::~PokemonMap()
{
}

HRESULT PokemonMap::Init()
{
	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = false;

	Load();

	return S_OK;
}

void PokemonMap::Release()
{
}

void PokemonMap::Update()
{
	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}
}

void PokemonMap::Render()
{
	DrawTile();
}

void PokemonMap::DrawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;

	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;

	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;

	for (int i = 1; i < 5; i++) {
		LineMake(GetMemDC(), p[i - 1], p[i]);
	}
}

void PokemonMap::DrawTile()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;

			if (left + CELL_WIDTH <0 || left > WINSIZEX
				|| top + CELL_HEIGHT < 0 || top > WINSIZEY) continue;

			//IMAGE->FrameRender("tile", GetMemDC(), left, top,
			//	0, 0);

			for (int z = 0; z <= _tileMap[i][j].index; z++) {
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE) {
					switch (_tileMap[i][j].tileNum[z])
					{
					case 0:
						IMAGE->FrameRender("tile", GetMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height * z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					case 1:
					case 2:
						IMAGE->FrameRender("tile2", GetMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height * z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					}
				}
			}

			if (_isDebug) {
				DrawRhombus(left, top);
				SetTextColor(GetMemDC(), RGB(0, 0, 0));
				sprintf_s(str, "(%d,%d)", i, j);
				TextOut(GetMemDC(),
					left + RADIUS_WIDTH / 2 + 8,
					top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
			}
		}
	}
}

void PokemonMap::Load()
{
	// api�� �������ִ� save �Լ� ���
	// ����ü�� ���� ���� �ְ�
	// ���������� �迭�ε� ���� �� ����
	HANDLE file;
	DWORD read;

	// CreateFile Binary �������� ��
	// Binary 16���� ������ �������� ��
	// Binary �ѱ� �������� ������ �־���
	// binary�� �Ϲ����� fopen ���� ���� ��� ���ٷ� �� ����
	file = CreateFile(
		"save/tileMap.map",	// ������ ���� �Ǵ� �ε��� ������ �̸�
		GENERIC_READ, /// ����	// �����̳� ��ġ�� ����ų� ������ ����
		0,				// ���� ���� ��� �Է�
		NULL,			// ���� �Ǵ� ��ġ�� �� �� ���� �� Ư��
						// �׻� ó������ �ۼ��ϰڴٴ°� �� �����
						// ���� �ٲ��ָ� ���κ��� ���� �� ����
		OPEN_EXISTING, /// ����	// �����̳� ��ġ�� �� �� ���� �ൿ
		FILE_ATTRIBUTE_NORMAL,	// ����, ��ġ�� �� ��, ���� �� ���� Ư��
		NULL			// ������� ������ ���� �� Ư�� etc ����Ǵ� �ڵ�
	);

	// �������� ���� ���� �� ���Ƿ� ������� �����ؼ� �о�;���
	ReadFile(file, _tileMap,
		sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &read, NULL);

	CloseHandle(file);
}
