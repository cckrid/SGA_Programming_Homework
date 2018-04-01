#include "stdafx.h"
#include "TestScene.h"

// �켱���� ť pair���� 
// ù��° ���� ����ġ�� ���� ��� 
// �ι�° ���� Ÿ���� ���ؾ� �ϹǷ�
// �� �����ڸ� �����ε� �ؾ���
// �����ϰ� ����ġ�� ���� ��� 
// �� Ÿ���� ���� ���Ͽ� ���� ū ���� ���� �������� �Ͽ���
// ���� ����� ������ �Ϸ��� ��ȣ �ݴ��
bool operator<(tagTile t1, tagTile t2) {
	return t1.row < t2.row;
}

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Init()
{
	isDebug = false;

	// tile �ʱ�ȭ
	for (int i = 0; i < TILEMAX; i++) {
		m_tile[i].width = 50;
		m_tile[i].height = 50;
		m_tile[i].row = i / TILEROW;
		m_tile[i].col = i % TILECOL;
		m_tile[i].x = 100 + m_tile[i].row * m_tile[i].width;
		m_tile[i].y = 100 + m_tile[i].col * m_tile[i].height;
		m_tile[i].block = false;
		m_tile[i].rc = RectMakeCenter(
			m_tile[i].x, m_tile[i].y,
			m_tile[i].width, m_tile[i].height);
	}

	// player �ʱ�ȭ
	ZeroMemory(&m_player, sizeof(tagPlayer));
	m_player.width = 30;
	m_player.height = 30;

	// �÷��̾� ó�� ��ġ ���� ���� �� Ÿ�Ϻ��� ����
	m_player.x = m_tile[0].x;
	m_player.y = m_tile[0].y;
	m_player.index = 0;
	
	m_player.rc = RectMakeCenter(
		m_player.x, m_player.y,
		m_player.width, m_player.height);
	m_player.speed = 2.5f;
	m_player.isMove = false;

	return S_OK;
}

void TestScene::Release()
{
}

void TestScene::Update()
{
	// ���콺 ���� ��ư Ŭ�� �� (�÷��̾ �������� ���� ��)
	if (INPUT->GetKeyDown(VK_LBUTTON) && 
		!m_player.isMove) {

		// Ÿ�� �˻�
		for (int i = 0; i < TILEMAX; i++) {
			// Ÿ�� Ŭ�� ��
			if (PtInRect(&m_tile[i].rc, g_ptMouse)) {
				// �ش� Ÿ���� ��ǥ Ÿ�Ϸ� ����
				m_tile[i].dest = true;
				// �÷��̾� �����̵��� ����
				m_player.isMove = true;
			
				// a*�� �÷��̾��� ��ġ�κ��� �ش� Ÿ�ϱ����� ��ã��
				AStarPathFinding(m_tile[i]);	
				
				// ã���� �濡�� Ÿ�� �ϳ� ���� (���ÿ� ������ �ִ°�)
				tagTile temp = pathStack.top();
				// ���� Ÿ�� ����
				pathStack.pop();	

				// ù ��ǥ Ÿ�� ����
				m_player.destX = temp.x;	// ��ǥ Ÿ���� ���� x 
				m_player.destY = temp.y;	// ��ǥ Ÿ���� ���� y
				// ��ǥ Ÿ�ϰ� �÷��̾���� ���� ���
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);

				break;
			}
		}
	}

	// ���콺 ������ ��ư���� ��ֹ� ���� �� ���� 
	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		// Ÿ�� �˻�
		for (int i = 0; i < TILEMAX; i++) {
			// �ش� Ÿ�� Ŭ�� ��
			if (PtInRect(&m_tile[i].rc, g_ptMouse))
				// Ÿ���� ��ֹ� ���� ��� (false -> true, true -> false);
				m_tile[i].block = !m_tile[i].block;
		}
	}

	// �÷��̾� �����̴� �Լ�
	Move();

	// path ���ÿ� ���� line�� show stack�� �ֱ�
	showStack = lineStack = pathStack;

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void TestScene::Render()
{
	//=================================================
	{
		// Ÿ�� �׸���
		for (tagTile tile : m_tile) {
			// ��ֹ� Ÿ���̸�
			if (tile.block) {
				// �Ķ���
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 0, 255));
			}
			// ��ǥ Ÿ��
			else if (tile.dest) {
				// ������
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 0, 0));
			}
			// ������ Ÿ��
			else {
				// ���
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));
			}
			// Ÿ�� Rect�� �׸���
			RectangleMake(GetMemDC(), tile.rc);

			DeleteObject(brush);

			// Ÿ�� ���� Ÿ���� ��, �� ����
			SetBkMode(GetMemDC(), TRANSPARENT);
			sprintf_s(str, "(%d,%d)", tile.row, tile.col);
			TextOut(GetMemDC(),
				tile.rc.left + 3, tile.rc.top, str, strlen(str));			
		}

		// �÷��̾� �׸���
		BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));
		RectangleMake(GetMemDC(), m_player.rc);
		DeleteObject(brush);

		// �� �׸��� (�� ������ �ȱ׷���)
		//while (lineStack.size() > 1) {
		//	tagTile t1 = lineStack.top();
		//	lineStack.pop();
		//	tagTile t2 = lineStack.top();
		//	lineStack.pop();
		//	LineMake(GetMemDC(), PointMake(t1.x, t1.y), PointMake(t2.x, t2.y));
		//}

		// �� �׸��� �� ������
		if (lineStack.size() > 1) {
			// ���� Ÿ�� ������ (���ÿ� ������ �ִ°�)
			tagTile start = lineStack.top();
			// ���� Ÿ���� ���ÿ��� ����
			lineStack.pop();

			// ���� Ÿ���� �������� ���� �׸��� ����
			MoveToEx(GetMemDC(), start.x, start.y, NULL);

			// ������ �� �� ����
			while (!lineStack.empty()) {
				// Ÿ�� ������ (���ÿ� ������ �ִ°�)
				tagTile temp = lineStack.top();
				// ���� Ÿ�� ����
				lineStack.pop();

				// �ش� Ÿ�ϱ��� ���� �׸���
				LineTo(GetMemDC(), temp.x, temp.y);
			}
		}

		// ������ �� ������ �̵��� Ÿ�ϵ� ����
		sprintf_s(str, "<path>");
		TextOut(GetMemDC(), WINSIZEX - 250, 75, str, strlen(str));

		int index = 0;
		// show ������ �� ������
		while (!showStack.empty()) {
			// Ÿ�� �ϳ� ������ (���ÿ� ������ �ִ°�)
			tagTile temp = showStack.top();
			// ���� Ÿ�� ����
			showStack.pop();
			sprintf_s(str, "%d's tile pos = (%d, %d)", 
				index + 1, temp.row, temp.col);
			TextOut(GetMemDC(), WINSIZEX - 300, 100 + index * 25, str, strlen(str));
			index++;
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		// �� Ÿ�ϵ��� a* �˰��򿡼��� ����ġ �׸��� 
		for (tagTile tile : m_tile) {
			sprintf_s(str, "f=%d", tile.f.h + tile.f.g);
			TextOut(GetMemDC(),
				tile.rc.left + 3, tile.rc.bottom - 15, str, strlen(str));
		}
	}
	//=================================================
}

void TestScene::Move()
{
	// �÷��̾ �����̵��� ������ ���
	if (m_player.isMove) {
		// �ش� �÷��̾��� ������ ��ǥ �������� �Ÿ� ����Ͽ�
		// ���� �Ÿ� (10) ������ ���
		if (GetDistance(
			m_player.x, m_player.y, m_player.destX, m_player.destY) < 10) {
			// path ������ ��������� �̵� ���߱�
			if (pathStack.empty()) {
				m_player.isMove = false;
				m_player.x = m_player.destX;
				m_player.y = m_player.destY;
			}
			// path ������ ���������� �̵�
			else {
				// Ÿ�� �ϳ� ������ (���� ������ �ִ°�)
				tagTile temp = pathStack.top();
				// ���� Ÿ�� ����
				pathStack.pop();

				// �ش� Ÿ�Ϸ� ��ǥ ����
				m_player.destX = temp.x;
				m_player.destY = temp.y;
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);
				m_player.index = temp.row * TILECOL + temp.col;
				// ���� Ÿ���� ��ǥ Ÿ���� ��� ��ǥ Ÿ�� ���� false�� 
				// ���������� �׸����� �׸� �Ϸ���
				if (m_tile[temp.row * TILECOL + temp.col].dest)
					m_tile[temp.row * TILECOL + temp.col].dest = false;
			}
		}
		// �ش� �÷��̾��� ������ ��ǥ �������� �Ÿ� ����Ͽ�
		// ���� �Ÿ� (10) �̻��� ��� �̵� 
		else {
			m_player.x += cosf(m_player.angle) * m_player.speed;
			m_player.y -= sinf(m_player.angle) * m_player.speed;
		}
		// �̵��� ��ġ�� rect �ٽ� �����
		m_player.rc = RectMakeCenter(
			m_player.x, m_player.y, m_player.width, m_player.height);
	}
}

void TestScene::AStarPathFinding(tagTile& tile)
{
	// a* ���� ����� �켱���� ť
	// ����ġ�� �ش� Ÿ���� pair�� ��� ���� ����
	// ����� �ִ°� ���۸� �������� ���� ����� �ִ°Ŷ�� ���� ��
	// �켱 ���� ť�� max������ �⺻ ���� �Ǿ� �־
	// �ְ� �� �� ���� ū�� ������ ���� ���� �� (���� ���) �� ������ ���ϹǷ�
	// �پ��� ����� ������ ����ġ�� - �� ���̴� ����� ����� ����
	// 50 30 10 -> -50 -30 -10 �̷��� ������ -10 �� �����Ƿ� (���� ũ��)

	priority_queue< pair<int, tagTile> > pq;

	// a* �����ϱ� ���� Ÿ�ϵ� �ʱ�ȭ
	for (int i = 0; i < TILEROW * TILECOL; i++) {
		// ��ֹ��� ��� ���� ������� ����
		m_tile[i].close = m_tile[i].block ? true : false;
		// �θ� ��� �� �ʱ�ȭ
		m_tile[i].node = NULL;
	}
	// path ���� �� ����
	while (!pathStack.empty())
		pathStack.pop();

	// �÷��̾ �ִ� Ÿ�� ���� ������� ����
	m_tile[m_player.index].close = true;
	// �÷��̾��� Ÿ�� �켱���� ť�� �ֱ� (����ġ ���� ũ�� �ؼ� ���� ������ (0))
	pq.push(make_pair(0, m_tile[m_player.index]));

	// �̵��� ��ġ ���Ʒ��¿� �Ÿ��� ����ġ ����ؼ� pq�� �ֱ�
	// �밢���� ���� Ÿ�� �̵��̹Ƿ� �ʿ�� �밢���� �߰��ϸ� ��
	while (!pq.empty()) {
		// �켱 ���� ť���� ���� ū �� ������ 
		// ����ġ - �̹Ƿ� ����ġ�� ���� ���� Ÿ�� ������
		tagTile temp = pq.top().second;
		// ���� Ÿ�� ����
		pq.pop();
		
		// ���� Ÿ���� ��ǥ Ÿ���� ��� ���� ������ ����
		if (temp.row == tile.row && temp.col == tile.col)
			break;

		// �ε��� ��� Ÿ���� �� * �� ���� + Ÿ���� �� �ϸ� ��
		int index = temp.row * TILECOL + temp.col;

		// �÷��̾� �ֺ� Ÿ�� �Ÿ��� ����ġ ���
		// up
		// Ÿ���� �� �Ʒ� �ຸ�� ũ�� �ش� Ÿ���� ���� ����� �ƴϸ�
		if ((index >= TILECOL)
			&& !m_tile[index - TILECOL].close) {
			// �ش� Ÿ�� ���� ������� ����
			m_tile[index - TILECOL].close = true;
			// �̵���� ���� �̹Ƿ� 10
			m_tile[index - TILECOL].f.g = 10;
			// ����ź ������� �����̵���� ���
			m_tile[index - TILECOL].f.h =
				abs(tile.row - m_tile[index - TILECOL].row)
				+ abs(tile.col - m_tile[index - TILECOL].col);
			// �ش� Ÿ���� �θ� ���� ���� ���õ� Ÿ�Ϸ� ����
			m_tile[index - TILECOL].node = &m_tile[index];
			// �켱���� ť�� ����ġ �� ����Ͽ� Ÿ�ϰ� ��� �ֱ�
			pq.push(
				make_pair(
					-m_tile[index - TILECOL].f.h +
					-m_tile[index - TILECOL].f.g,
					m_tile[index - TILECOL]));
		}
		// down
		// Ÿ���� �� �� �ຸ�� �۰� �ش� Ÿ���� ���� ����� �ƴϸ�
		if ((index < TILEROW * TILECOL - TILECOL)
			&& !m_tile[index + TILECOL].close) {
			// �ش� Ÿ�� ���� ������� ����
			m_tile[index + TILECOL].close = true;
			// �̵���� ���� �̹Ƿ� 10
			m_tile[index + TILECOL].f.g = 10;
			// ����ź ������� �����̵���� ���
			m_tile[index + TILECOL].f.h =
				abs(tile.row - m_tile[index + TILECOL].row)
				+ abs(tile.col - m_tile[index + TILECOL].col);
			// �ش� Ÿ���� �θ� ���� ���� ���õ� Ÿ�Ϸ� ����
			m_tile[index + TILECOL].node = &m_tile[index];
			// �켱���� ť�� ����ġ �� ����Ͽ� Ÿ�ϰ� ��� �ֱ�
			pq.push(
				make_pair(
					-m_tile[index + TILECOL].f.h +
					-m_tile[index + TILECOL].f.g,
					m_tile[index + TILECOL]));
		}
		// left
		// Ÿ���� �� ���� ������ ũ�� �ش� Ÿ���� ���� ����� �ƴϸ�
		if ((index % TILECOL > 0)
			&& !m_tile[index - 1].close) {
			// �ش� Ÿ�� ���� ������� ����
			m_tile[index - 1].close = true;
			// �̵���� ���� �̹Ƿ� 10
			m_tile[index - 1].f.g = 10;
			// ����ź ������� �����̵���� ���
			m_tile[index - 1].f.h =
				abs(tile.row - m_tile[index - 1].row)
				+ abs(tile.col - m_tile[index - 1].col);
			// �ش� Ÿ���� �θ� ���� ���� ���õ� Ÿ�Ϸ� ����
			m_tile[index - 1].node = &m_tile[index];
			// �켱���� ť�� ����ġ �� ����Ͽ� Ÿ�ϰ� ��� �ֱ�
			pq.push(
				make_pair(
					-m_tile[index - 1].f.h +
					-m_tile[index - 1].f.g,
					m_tile[index - 1]));
		}
		// right
		// Ÿ���� �� ������ ������ �۰� �ش� Ÿ���� ���� ����� �ƴϸ�
		if ((index % TILECOL < TILECOL - 1)
			&& !m_tile[index + 1].close) {
			// �ش� Ÿ�� ���� ������� ����
			m_tile[index + 1].close = true;
			// �̵���� ���� �̹Ƿ� 10
			m_tile[index + 1].f.g = 10;
			// ����ź ������� �����̵���� ���
			m_tile[index + 1].f.h =
				abs(tile.row - m_tile[index + 1].row)
				+ abs(tile.col - m_tile[index + 1].col);
			// �ش� Ÿ���� �θ� ���� ���� ���õ� Ÿ�Ϸ� ����
			m_tile[index + 1].node = &m_tile[index];
			// �켱���� ť�� ����ġ �� ����Ͽ� Ÿ�ϰ� ��� �ֱ�
			pq.push(
				make_pair(
					-m_tile[index + 1].f.h +
					-m_tile[index + 1].f.g,
					m_tile[index + 1]));
		}
	}

	// ��ǥ Ÿ�� �ӽ� Ÿ���� �ֱ�
	tagTile temp = tile;

	// ��ǥ Ÿ���� path ���ÿ� �ֱ� (��ǥ Ÿ�Ͽ��� �����ؾ� �ϹǷ�)
	pathStack.push(temp);

	// ��ǥŸ���� ��, ���� �ӽ�Ÿ���� ��, ���� ���� �� ���� �ݺ�
	while ((temp.row != m_tile[m_player.index].row) ||
		(temp.col != m_tile[m_player.index].col)) {
		// �ӽ� Ÿ���� �θ� ��带 path ���ÿ� �ֱ�
		pathStack.push(*temp.node);
		// �ӽ� Ÿ���� �θ� Ÿ�Ϸ� ����
		temp = *temp.node;
	}
}
