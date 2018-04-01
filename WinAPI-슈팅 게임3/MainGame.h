#pragma once

#include "GameNode.h"
#include "Rocket.h"
#include "EnemyManager.h"

#include "ProgressBar.h"

#define LOADMAX 100

class MainGame : public GameNode
{
private:
	bool isDebug;
	bool isStart;			// ���� �Ǿ����� Ȯ�� �ϴ� ���� ���Ϲ� �Ŵ��� ����
	bool isLoad;			// �ε��� ���۵Ǿ����� Ȯ�� �ϴ� ����

	EnemyManager * _em;		// ���Ϲ� �Ŵ���
	Rocket * _rocket;		// �÷��̾� ����

	ProgressBar * _loading;	// �ε� �ٸ� ���� ���α׷��� ��

	int loadCount;			// �ε� �� ������ ���� ����

	float offsetY;			// ���ȭ�� ���������� ���� ����
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
	
	void ReStart();			// �ٽ� ������� ���� ������ �ʱ�ȭ�� ���� �Լ�
};
