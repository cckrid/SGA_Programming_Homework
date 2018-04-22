#include "stdafx.h"
#include "IsoMap.h"


IsoMap::IsoMap()
{
}


IsoMap::~IsoMap()
{
}

HRESULT IsoMap::Init()
{
	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = true;

	int test = RADIUS_HEIGHT;
	int test2 = CELL_HEIGHT;

	x = y = 0;

	_currentCTRL = CTRL_DRAW;

	MapToolSetup();

	return S_OK;
}

void IsoMap::Release()
{
}

void IsoMap::Update()
{
	if (INPUT->GetKey('W')) { _startY += 5; }
	if (INPUT->GetKey('S')) { _startY -= 5; }
	if (INPUT->GetKey('A')) { _startX += 5; }
	if (INPUT->GetKey('D')) { _startX -= 5; }

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (!SUBWIN->GetIsActive() &&
		INPUT->GetKey(VK_LBUTTON)) {
		float cellX = (float)(g_ptMouse.x - _startX);

		if (cellX < 0) {
			cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
		}
		else {
			cellX = cellX / (float)CELL_WIDTH;
		}

		int cellY = abs(g_ptMouse.y - _startY) / CELL_HEIGHT;

		cellY = (g_ptMouse.y < _startY) ? cellY * -1 : cellY;

		int isoX = (int)cellX + (int)cellY;
		int isoY = (int)cellY - (int)cellX;

		if (isoX >= 0 && isoX < TILE_COUNT_X &&
			isoY >= 0 && isoY < TILE_COUNT_Y) {
			int corner = GetCornerIndex(isoX, isoY);

			if (IsInRhombus(corner, isoX, isoY))
				corner = 0;

			switch (corner)
			{
			case 1:
				isoX = isoX - 1;
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			SetMap(isoX, isoY, false);

			_center = corner;
			_isoX = isoX;
			_isoY = isoY;
		}
	}

	if (!SUBWIN->GetIsActive() &&
		INPUT->GetKeyDown(VK_RBUTTON)) {
		float cellX = (float)(g_ptMouse.x - _startX);

		if (cellX < 0) {
			cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
		}
		else {
			cellX = cellX / (float)CELL_WIDTH;
		}

		int cellY = abs(g_ptMouse.y - _startY) / CELL_HEIGHT;

		cellY = (g_ptMouse.y < _startY) ? cellY * -1 : cellY;

		int isoX = (int)cellX + (int)cellY;
		int isoY = (int)cellY - (int)cellX;

		if (isoX >= 0 && isoX < TILE_COUNT_X &&
			isoY >= 0 && isoY < TILE_COUNT_Y) {
			int corner = GetCornerIndex(isoX, isoY);

			if (IsInRhombus(corner, isoX, isoY))
				corner = 0;

			switch (corner)
			{
			case 1:
				isoX = isoX - 1;
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			SetMap(isoX, isoY, true);

			_center = corner;
			_isoX = isoX;
			_isoY = isoY;
		}
	}
}

void IsoMap::Render()
{
	sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
		_isoX, _isoY, _center);
	TextOut(GetMemDC(), 700, 20, str, strlen(str));

	DrawTileMap();
}

void IsoMap::DrawTileMap()
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

void IsoMap::DrawRhombus(int left, int top)
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

int IsoMap::GetCornerIndex(int isoX, int isoY)
{
	int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

	float dx = (g_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (g_ptMouse.y - top) / (float)CELL_HEIGHT;

	if (dx < 0.5f && dy < 0.5f) return 1;
	if (dx >= 0.5f && dy < 0.5f) return 2;
	if (dx < 0.5f && dy >= 0.5f) return 3;
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool IsoMap::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) <= dx) return true;
		else return false;
	}
	// righttop
	else if (corner == 2) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy >= dx) return true;
		else return false;
	}
	// leftdown
	else if (corner == 3) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy < dx) return true;
		else return false;
	}
	// rightdown
	else if (corner == 4) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) > dx) return true;
		else return false;
	}

	return false;
}

void IsoMap::MapToolSetup()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].index = -1;
			_tileMap[i][j].height = 50;
		}
	}
}

void IsoMap::SetMap(int isoX, int isoY, bool isAdd)
{
	if (isoX < 0 || isoY < 0) return;

	int index = SUBWIN->GetFrameIndex();

	switch (SUBWIN->GetFrameIndex())
	{
	case 0:
		imageFrame = SUBWIN->GetFramePoint();
		break;
	case 1:
		imageFrame.x = SUBWIN->GetFramePoint().y;
		imageFrame.y = SUBWIN->GetFramePoint().x;
		break;
	case 2:
		if (SUBWIN->GetFramePoint().y < 5) {
			imageFrame.x = SUBWIN->GetFramePoint().x + 10;
			imageFrame.y = SUBWIN->GetFramePoint().y;
		}
		else {
			imageFrame.x = SUBWIN->GetFramePoint().x + 16;
			imageFrame.y = SUBWIN->GetFramePoint().y - 5;
		}
		break;
	}

	_currentCTRL = SUBWIN->GetCTRL();

	switch (_currentCTRL)
	{
	case CTRL_DRAW:
		if (isAdd) {
			if (KindSelect(imageFrame.x, imageFrame.y) == TILEKIND_OBJECT
				&& _tileMap[isoX][isoY].index == -1)
				break;

			_tileMap[isoX][isoY].index++;
			if (_tileMap[isoX][isoY].index >= TILE_MAX)
				_tileMap[isoX][isoY].index = TILE_MAX - 1;
			_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
			_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] =
				KindSelect(imageFrame.x, imageFrame.y);
			_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index]
				= imageFrame;
		}
		else {
			if (KindSelect(imageFrame.x, imageFrame.y) == TILEKIND_OBJECT)
				break;

			if (_tileMap[isoX][isoY].index == -1) {
				_tileMap[isoX][isoY].index++;
				if (_tileMap[isoX][isoY].index >= TILE_MAX)
					_tileMap[isoX][isoY].index = TILE_MAX - 1;
				_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
				_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] =
					KindSelect(imageFrame.x, imageFrame.y);
				_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index]
					= imageFrame;
			}
			else {
				_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
				_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] =
					KindSelect(imageFrame.x, imageFrame.y);
				_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index]
					= imageFrame;
			}
		}
		break;
	case CTRL_ERASER:
		if (_tileMap[isoX][isoY].index > -1) {
			_tileMap[isoX][isoY].index--;
		}
		break;
	}
}

TILEKIND IsoMap::KindSelect(int frameX, int frameY)
{
	if (frameX == -1 && frameY == -1) return TILEKIND_NONE;

	if (SUBWIN->GetFrameIndex() == 0) {
		if (frameY >= 7) return TILEKIND_NONE;
		if (frameY <= 4) return TILEKIND_TERRAIN;
		else return TILEKIND_OBJECT;
	}
	if (SUBWIN->GetFrameIndex() == 2) {
		if (frameX >= 21) return TILEKIND_NONE;
	}
	return TILEKIND_TERRAIN;
}

void IsoMap::TileInit()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].index = -1;
		}
	}
}

void IsoMap::Load()
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

void IsoMap::Save()
{
	// api�� �������ִ� save �Լ� ���
	// ����ü�� ���� ���� �ְ�
	// ���������� �迭�ε� ���� �� ����
	// ���̳ʸ� �����̶� �̹��� ���� �����Ѱ� �ٵ�
	HANDLE file;
	DWORD write;

	// CreateFile Binary �������� ��
	// Binary 16���� ������ �������� ��
	// Binary �ѱ� �������� ������ �־���
	// binary�� �Ϲ����� fopen ���� ���� ��� ���ٷ� �� ����
	file = CreateFile(
		"save/tileMap.map",	// ������ ���� �Ǵ� �ε��� ������ �̸�
		GENERIC_WRITE,	// �����̳� ��ġ�� ����ų� ������ ����
		0,				// ���� ���� ��� �Է�
		NULL,			// ���� �Ǵ� ��ġ�� �� �� ���� �� Ư��
						// �׻� ó������ �ۼ��ϰڴٴ°� �� �����
						// ���� �ٲ��ָ� ���κ��� ���� �� ����
		CREATE_ALWAYS,	// �����̳� ��ġ�� �� �� ���� �ൿ
		FILE_ATTRIBUTE_NORMAL,	// ����, ��ġ�� �� ��, ���� �� ���� Ư��
		NULL			// ������� ������ ���� �� Ư�� etc ����Ǵ� �ڵ�
	);

	// �������� ���� ���� �� ���Ƿ� ������� �����ؼ� �о�;���
	WriteFile(file, _tileMap,
		sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &write, NULL);

	CloseHandle(file);
}
