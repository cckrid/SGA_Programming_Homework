#pragma once

#include "GameNode.h"
#include "Bullet.h"

#include "Alien.h"
#include "Boss.h"
#include "Rocket.h"

// ����� ���� ����
class ResultScene;

class EnemyManager : public GameNode
{
private:
	typedef vector<Enemy*> vEnemy;
	typedef vector<Enemy*>::iterator viEnemy;
private:
	// ���ʹ̸� �����ϴ� vector
	vEnemy _vAlien;
	viEnemy _viAlien;
	// ���ʹ��� �Ѿ�
	Bullet* _bullet;

	// ������ �����ϴ� vector
	vEnemy _vBoss;
	viEnemy _viBoss;
	// ������ �Ѿ�
	Bullet* _bossBullet;

	Rocket* _rocket;

	// ���ʹ��� �����ӿ� �ʿ��� ������
	bool _isLeft;	
	bool _isDown;
	bool _isInit;
	// ���ʹ��� �������� ó������ (ó���̸� ���� ��ġ���� �����;��ؼ�)
	bool _isFirst;

	// ������ �����ӿ� �ʿ��� ������
	// ������ �������� ó������
	bool _isBossFirst;
	bool _isBossLeft;

	// ������ �� �󸶵��� �������� ���ϱ� ���� �ʿ��� ����
	int _delay;

	// �׽�Ʈ ���ϰ� �ϱ� ���� ġƮ
	bool _cheat;

	ResultScene * _resultScene;
public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	// ���ʹ� ����
	void SetAlien();
	// ���ʹ� �߰� (���� ���� �� ���ʹ� �߰��ϴ� ������ �־)
	void AddAlien();
	// ���ʹ̵��� �Ѿ� �߻�
	void AlienBulletFire();
	// ���ʹ̵��� �̵�
	void AlienMove();

	// ���� ����
	void SetBoss();
	// ������ �Ѿ� �߻�
	void BossBulletFire();
	// ������ �̵�
	void BossMove();

	void SetRocket(Rocket* rocket) { _rocket = rocket; }
	void SetResultScene(ResultScene * resultScene) { _resultScene = resultScene; }
};