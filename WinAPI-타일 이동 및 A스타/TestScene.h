#pragma once

#include "GameNode.h"

// ť, �켱���� ť
/*
	�ڷ� ����
	���� ������ ���� ������ �ڷ� ���� (FIFO - First In First Out)
	���� ����� �� �պ��� �����ϴ� �� ���� ������� �����ϸ� ��

	*���⼭ ����� priority queue(�켱 ���� ť)�� �� ������ �Ǿ� �ִ� ť��
	maxHeap���� �⺻ ���� �Ǿ� �־� 
	���� �� �� ���� ū ���� ���� ������ ��
*/
#include <queue>

// ����
/*
	�ڷᱸ��
	���� ������ ���������� ������ �ڷ� ���� (LIFO - List In First Out)
	��� �׾� �ø��� ���������� ������ �� ���� ������� �����ϸ� ��
*/
#include <stack>

using namespace std;

#define TILEROW 12	// Ÿ���� ��
#define TILECOL 12  // Ÿ���� ��
#define TILEMAX TILEROW * TILECOL // Ÿ�� �ִ� ũ�� �� * ��

// a* �˰��򿡼� ���� �� Ÿ�� ����ġ ��
struct tagCost {
	// H ����� ����ź ���(Manhattan Method)�� ����
	// ���� Ÿ�Ͽ��� ��ǥ Ÿ�Ͽ� �����ϱ� ������ �밢�� �̵��� ������ 
	// ����, ���η� �̵��� �� ���ڸ� ����ؼ� ����
	int h;	// ����� Ÿ�Ϸκ��� ���� ���� ���������� ���� �̵� ���

	// ���� �Ǵ� ���� �̵� 10, �밡�� 14�� ���
	// �밢���� ���̴� �̵�ﰢ���� ���̺� ��Ʈ2:1 �ؼ� 14 ���°�
	int g;  // ���������κ��� ���ο� Ÿ�ϱ����� �̵� ���
};

// Ÿ�� ����
struct tagTile {
	RECT rc;				// rect �̰ɷ� �׳� �簢�� �׸�
	float x, y;				// ���� x, y
	float width, height;	// �ʺ�, ����
	int row, col;			// ��, ��
	bool block;				// ���� Ÿ������ üũ�ϴ� ����

	bool dest;				// ��ǥ Ÿ������ üũ�ϴ� ����
	bool close;				// �����ִ� Ÿ������ üũ�ϴ� ���� (a* ���� ���)
	tagCost f;				// Ÿ���� ����ġ �� (a* ���� ���)
	tagTile* node;			// Ÿ���� �θ� ��� (a* ���� ���)
};

// �÷��̾� ����
struct tagPlayer {
	RECT rc;				// rect �̰ɷ� �׳� �簢�� Ÿ�Ϻ��� �۰� �׸�
	float x, y;				// ���� x, y
	float width, height;	// �ʺ�, ����
	
							// ���� Ÿ���� ������ �÷��̾� ������ ���� ����ؼ�
	float angle;			// �����̴� ���� ���ϵ��� ���� (�밢 �̵��� �� �� �ְ�)

	float speed;			// �ӵ�

	bool isMove;			// �÷��̾ �������� �ϴ��� üũ�ϴ� ����
	float destX, destY;		// ��ǥ Ÿ���� ���� x, y
	int index;				// �÷��̾ �ִ� Ÿ���� index
};

class TestScene : public GameNode
{
private:
	bool isDebug;

	tagTile m_tile[TILEMAX];
	tagPlayer m_player;

	stack<tagTile> pathStack;	// a* �� ã�� Ÿ�ϵ��� ���� �ڷᱸ�� (stack)
	stack<tagTile> showStack;	// pathStack�� ����, ���� �����ֱ� ���ؼ� �ϳ� �� ����
	stack<tagTile> lineStack;	// pathStack�� ����, ���� �������� �������� �ϳ� �� ����

	HBRUSH brush;				
	char str[128];
public:
	TestScene();
	~TestScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

								// �÷��̾� �����̴� �Լ�
	void Move();				// ��ǥ Ÿ���� �����Ǹ� ��ǥ Ÿ���� ���� �����̵��� ����
								
	void AStarPathFinding(tagTile& tile);	// a* �˰���, �ش� Ÿ�Ϸ� �� �� �ִ� �� ã��
};

