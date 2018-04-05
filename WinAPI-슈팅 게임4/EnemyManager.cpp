#include "stdafx.h"
#include "EnemyManager.h"

#include "ResultScene.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::Init()
{
	_bullet = new Bullet;
	_bullet->Init((char*)"bullet", 100, 1000);

	_bossBullet = new Bullet;
	_bossBullet->Init((char*)"bullet", 300, 1000);

	this->SetAlien();
	//this->SetBoss();

	_isLeft = false;
	_isDown = false;
	_isInit = false;
	_isFirst = true;

	_isBossFirst = true;
	_isBossLeft = false;

	_cheat = true;

	return S_OK;
}

void EnemyManager::Release()
{
	for (int i = 0; i < _vAlien.size(); i++) {
		SAFE_DELETE(_vAlien[i]);
	}
	_vAlien.clear();

	for (int i = 0; i < _vBoss.size(); i++) {
		SAFE_DELETE(_vBoss[i]);
	}
	_vBoss.clear();
	//SAFE_DELETE(_bullet);
}

void EnemyManager::Update()
{
	//for (int i = 0; i < _vAlien.size(); i++) {
	//	_vAlien[i]->Update();
	//}

	// ���ʹ̵� ������Ʈ
	for (_viAlien = _vAlien.begin(); _viAlien != _vAlien.end();) {
		(*_viAlien)->Update();

		// ���ʹ̰� ��� �ְ� ������ �̻��Ͽ� �浹 ��
		if ((*_viAlien)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viAlien)->GetRect())) {
			// ���ʹ� ��� �ִ� ���� false�� ����
			(*_viAlien)->SetLived(false);

			SOUND->Play("alien_die", 0.5f);
			SAVEBOOK->AddKill();
		}

		// ���ʹ̰� �״� ��� 
		// ��� �ִ� ���°� false�� �״� �ִϸ��̼��� ������
		if ((*_viAlien)->GetDied()) {
			// ���Ϳ��� ���ʹ� ����
			_viAlien = _vAlien.erase(_viAlien);
			// ���ʹ̰� ��� �������� ������ �������� ó���̸�
			if (_vAlien.size() == 0 && _isBossFirst)
				// ���� ����
				SetBoss();
		}
		else
			++_viAlien;
	}

	// ���� ������Ʈ
	for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end();) {
		(*_viBoss)->Update();

		// ���� ��� �ְ� ������ �̻��Ͽ� �浹 ��
		if ((*_viBoss)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viBoss)->GetRect())) {
			// ������ ��� �ִ� ���� false�� ����
			(*_viBoss)->SetLived(false);
			//(*_viBoss)->SetDied(true);
			SOUND->Play("boss_damaged", 0.5f);
		}

		// ���� �״� ���
		// ��� �ִ� ���� false�� ���� hp 0�̸�
		if ((*_viBoss)->GetDied()) {
			// ���Ϳ��� ���� �����
			_viBoss = _vBoss.erase(_viBoss);
			// ���� ������ ũ�Ⱑ 0�̸�
			if (_vBoss.size() == 0) {
				// ������� �̹��� ending���� ����
				_resultScene->SetImage(IMAGE->FindImage("ending"));
				// ��������� �̵�
				SCENE->ChangeScene("Result");
				if (SOUND->IsPlaySound("bgm"))
					SOUND->Stop("bgm");
				if (SOUND->IsPlaySound("boss_bgm"))
					SOUND->Stop("boss_bgm");
				SOUND->Play("ending", 0.5f);
				SAVEBOOK->AddEnding();
			}
		}
		else
			++_viBoss;
	}

	// ġƮŰ ���
	if (INPUT->GetKeyDown(VK_TAB)) {
		_cheat = !_cheat;
	}

	// �Ѿ� ������Ʈ
	_bullet->Update();
	// ���� �Ѿ� ������Ʈ
	_bossBullet->Update();
	// �÷��̾ �Ѿ��� ���� �� (ġƮ �������־�� ��)
	if (_bullet->CheckCollision(_rocket->GetRect())) {
		if (!_cheat) {
			// ���� hp ����
			_rocket->SetHp(_rocket->GetHp() - 1);
			if (_rocket->GetHp() <= 0)
				_rocket->SetHp(0);
		}
		SOUND->Play("rocket_damaged", 0.5f);
		SAVEBOOK->AddHit();
	}
	// �÷��̾ ���� �Ѿ��� ���� �� (ġƮ �������־�� ��)
	if (_bossBullet->CheckCollision(_rocket->GetRect())) {
		if (!_cheat) {
			// ���� hp ����
			_rocket->SetHp(_rocket->GetHp() - 1);
			if (_rocket->GetHp() <= 0)
				_rocket->SetHp(0);
		}
		SOUND->Play("rocket_damaged", 0.5f);
		SAVEBOOK->AddHit();
	}

	// ���ʹ� �̵��� ó���� �ƴϸ�
	// ���� ��ġ�� �̵� �� ������ �Ѿ� �߻� ���ϱ� ���ؼ�
	if (!_isFirst) {
		// ���ʹ� �Ѿ� �߻�
		this->AlienBulletFire();
	}
	// ���� �̵��� ó���� �ƴϸ�
	// ���� ��ġ�� �̵� �� �� ���� �Ѿ� �߻� ���ϱ� ���ؼ�
	if (!_isBossFirst) {
		// ���� �Ѿ� �߻�
		BossBulletFire();
	}
	// ���ʹ� �̵�
	this->AlienMove();
	// ���� �̵�
	this->BossMove();

	// ���� ������ 0�̰� ���ʹ� ������ 0�̸�
	// ������� ���ؼ� ���ʹ� �Ŵ��� release()
	// ���ʹ� �Ŵ����� ���� �ƴ϶� �̷��� ���� �ʿ�
	if (_vBoss.size() == 0 && _vAlien.size() == 0)
		this->Release();
}

void EnemyManager::Render()
{
	for (int i = 0; i < _vBoss.size(); i++) {
		_vBoss[i]->Render();
	}

	for (int i = 0; i < _vAlien.size(); i++) {
		_vAlien[i]->Render();
	}

	_bullet->Render();
	_bossBullet->Render();

	if (_cheat) {
		SetBkMode(GetMemDC(), TRANSPARENT);
		SetTextColor(GetMemDC(), RGB(255, 0, 0));
		TextOut(GetMemDC(), WINSIZEX - 100, WINSIZEY - 50,
			"On Cheat", strlen("On Cheat"));
	}
}

void EnemyManager::SetAlien()
{
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		Enemy* alien;
	//		alien = new Alien;
	//		alien->Init("alien", PointMake(
	//			100 + j * 60, 100 + i * 60));
	//		_vAlien.push_back(alien);
	//	}
	//}
	for (int i = 0; i < 32; i++) {
		Enemy* alien;
		alien = new Alien;
		//alien->Init("alien", PointMake(
		//	100 + (i % 8) * 60, 100 + (i / 8) * 60));
		// ó�� ��ġ ���� �� ������ ����
		alien->Init("alien_idle", PointMake(
			100 + (i % 8) * 60, 100 + (i / 8) * 60 - 400 - 50 * (31 - i)));
		// ó�� ��ġ ���� ����
		alien->SetInitPos(PointMake(
			100 + (i % 8) * 60, 100 + (i / 8) * 60));
		_vAlien.push_back(alien);
	}
}

void EnemyManager::AddAlien()
{
	//_viBoss = _vBoss.begin();
	//for (; _viBoss != _vBoss.end(); ++_viBoss) {
	//	Enemy* alien;
	//	alien = new Alien;
	//	alien->Init("alien_idle", 
	//		PointMake((*_viBoss)->GetPos().x, (*_viBoss)->GetPos().y + 50));
	//	alien->SetInitPos(
	//		PointMake((*_viBoss)->GetPos().x, (*_viBoss)->GetPos().y + 50));
	//	_vAlien.push_back(alien);
	//}

	Enemy* alien;
	alien = new Alien;
	alien->Init("alien_idle",
		PointMake(
		(*_viBoss)->GetPos().x + RND->GetFromInto(-150, 150), 
			(*_viBoss)->GetPos().y + 150));
	alien->SetSpeed(alien->GetSpeed() + RND->GetFromInto(-1, 2));
	alien->SetInitPos(
		PointMake(
		(*_viBoss)->GetPos().x + RND->GetFromInto(-150, 150), 
			(*_viBoss)->GetPos().y + 150));
	_vAlien.push_back(alien);
}

void EnemyManager::AlienBulletFire()
{
	_viAlien = _vAlien.begin();
	for (; _viAlien != _vAlien.end(); ++_viAlien) {
		// ���ʹ̰� �Ѿ��� �߻��ؾ��ϴ� �����̸�
		if ((*_viAlien)->BulletCountFire()) {
			RECT rc = (*_viAlien)->GetRect();
			// ���ϰ� ���ʹ̰��� ���� ���
			float angle =
				GetAngle(
					rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 30,
					_rocket->GetImage()->GetX()
					+ _rocket->GetImage()->GetWidth() / 2,
					_rocket->GetImage()->GetY()
					+ _rocket->GetImage()->GetHeight() / 2
				);
			// ������ �߻�
			_bullet->Fire(
				rc.left + (rc.right - rc.left) / 2,
				rc.bottom + 30, angle, 2.5f);
			SOUND->Play("alien_fire", 0.5f);
		}
	}
}

void EnemyManager::AlienMove()
{
	if (_vAlien.size() == 0) return;

	// ó�� ��ġ������ ��� �Ʒ��� �̵�
	// ó�� �̵��̸�
	if (_isFirst) {

		for (int i = 0; i < _vAlien.size(); i++) {
			//if ((_vAlien[i]->GetPos().x != _vInitPos[i].x) ||
			//	(_vAlien[i]->GetPos().y != _vInitPos[i].y))
			
			// ���ʹ��� �ʱ� ��ġ�� ���� ��ġ���� �Ÿ��� 10���� ũ�� �̵�
			if(GetDistance(_vAlien[i]->GetPos().x, _vAlien[i]->GetPos().y,
				_vAlien[i]->GetInitPos().x, _vAlien[i]->GetInitPos().y) > 10)
				_vAlien[i]->Move(DIRECTION_DOWN);
		}

		//if ((_vAlien[0]->GetPos().x != _vInitPos[0].x) &&
		//	(_vAlien[0]->GetPos().y != _vInitPos[0].y)) {
		// ���� �������� �������� ���ʹ̰� ó�� ��ġ�� �����ϸ� ó�� ��ġ�� �̵� ��

		// ù��°�� ���ʹ��� �ʱ� ��ġ�� ���� ��ġ���� �Ÿ��� 
		// 10���� �۰ų� ������ ó�� �̵� ����
		// ù��° ���ʹ̰� ���� ���������� �����ͼ� �̷��� ����
		if (GetDistance(_vAlien[0]->GetPos().x, _vAlien[0]->GetPos().y,
			_vAlien[0]->GetInitPos().x, _vAlien[0]->GetInitPos().y) <= 10) {
			_isFirst = false;
			_isLeft = false;
		}
	}

	// ó�� �̵��� �ƴϸ�
	if (!_isFirst) {
		// ���� �̵��̰� �Ʒ� �̵��� �ƴϸ�
		if (_isLeft && !_isDown) {
			// ���ʹ� ��� �������� �̵�
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_LEFT);

				// ���ʹ� �Ѹ��̶�
				// �������� �̵��� �� �浹�ϸ� �Ʒ��� �̵� ���� 
				// ������ �ݴ�� ����
				if (_vAlien[i]->CheckCollision(DIRECTION_LEFT)) {
					_isLeft = false;
					_isDown = true;
					_delay = 0;
				}
			}
		}
		// ���� �̵��� �ƴϰ� �Ʒ� �̵��� �ƴϸ�
		else if (!_isLeft && !_isDown) {
			// ���ʹ� ��� ���������� �̵�
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_RIGHT);

				// ���ʹ� �Ѹ��̶�
				// ���������� �̵��� �� �浹�ϸ� �Ʒ��� �̵� ����
				// ���� �ݴ�� ����
				if (_vAlien[i]->CheckCollision(DIRECTION_RIGHT)) {
					_isLeft = true;
					_isDown = true;
					_delay = 0;
				}
			}
		}

		// �Ʒ��� �̵���
		if (_isDown) {
			// �Ʒ��� �̵��ð� ����
			_delay++;

			// ���ʹ� ��� �Ʒ��� �̵�
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_DOWN);

				// ���� ������ �� �Ʒ� �̵� ����
				if (_delay % 10 == 0)
					_isDown = false;

				// ���ʹ� �ϳ��� �Ʒ� ���� �浹 ��
				// �ʱ� ��ġ�� �̵�
				if (_vAlien[i]->CheckCollision(DIRECTION_DOWN)) {
					_isInit = true;
				}
			}
			//_isDown = false;
		}
	}

	// �ʱ� ��ġ�� �̵��ؾ��ϴ� ���
	if (_isInit) {
		_isInit = false;
		// �Ʒ��� �̵��ϴ� ��Ȳ ����
		_isDown = false;

		//for (int i = 0; i < 4; i++) {
		//	for (int j = 0; j < 8; j++) {
		//		_vAlien[i * 8 + j]->SetPosition(
		//			PointMake(100 + j * 60, 100 + i * 60));
		//	}
		//}
		
		// ���ʹ̵� ��ġ ��� �ʱ� ��ġ�� ���� 
		for (int i = 0; i < _vAlien.size(); i++) {
			_vAlien[i]->SetPosition(_vAlien[i]->GetInitPos());
		}
	}
}

void EnemyManager::SetBoss()
{
	Boss* boss;
	boss = new Boss;
	boss->Init("boss_idle", PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 150 - 500));
	boss->SetInitPos(PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 150));
	boss->SetEnemyManager(this);
	_vBoss.push_back(boss);
	_isFirst = false;

	if (SOUND->IsPlaySound("bgm"))
		SOUND->Stop("bgm");
	SOUND->Play("boss_bgm", 0.7f);
}

void EnemyManager::BossBulletFire()
{
	_viBoss = _vBoss.begin();
	for (; _viBoss != _vBoss.end(); ++_viBoss) {
		// ������ �Ѿ��� �߻��ؾ� �ϴ� ��Ȳ�̸�
		if ((*_viBoss)->BulletCountFire()) {

			RECT rc = (*_viBoss)->GetRect();
			// ���� �߽����� �Ʒ� �������� 
			// 15������ 165������ 15������ �� 16���� �Ѿ� �߻� 
			// ��ä�÷� �Ѿ� �߻�
			for (int i = 0; i < 16; i++) {
				_bossBullet->Fire(
					rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 30, (-15 - (10 * i)) * PI/180, 2.0f);
				// boss_fire ���ο��� �̰ɷ�
				SOUND->Play("alien_fire", 0.5f);
			}
		}
	}
}

void EnemyManager::BossMove()
{
	// ������ ������ �׳� ����
	if (_vBoss.size() == 0) return;

	// ó�� ��ġ������ ��� �Ʒ��� �̵�
	if (_isBossFirst) {

		for (int i = 0; i < _vBoss.size(); i++) {
			//if ((_vBoss[i]->GetPos().x != _vInitPos[i].x) ||
			//	(_vBoss[i]->GetPos().y != _vInitPos[i].y))
			if (GetDistance(_vBoss[i]->GetPos().x, _vBoss[i]->GetPos().y,
				_vBoss[i]->GetInitPos().x, _vBoss[i]->GetInitPos().y) > 10)
				_vBoss[i]->Move(DIRECTION_DOWN);
		}

		//if ((_vBoss[0]->GetPos().x != _vInitPos[0].x) &&
		//	(_vBoss[0]->GetPos().y != _vInitPos[0].y)) {
		
		// ���� �������� �������� ���ʹ̰� ó�� ��ġ�� �����ϸ� ó�� ��ġ�� �̵� ��
		if (GetDistance(_vBoss[0]->GetPos().x, _vBoss[0]->GetPos().y,
			_vBoss[0]->GetInitPos().x, _vBoss[0]->GetInitPos().y) <= 10) {
			_isBossFirst = false;
			_isBossLeft = false;
		}
	}

	// ���ʹ̿� ���� ������� ���� �̵�
	if (!_isBossFirst) {
		if (_isBossLeft) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_LEFT);

				if (_vBoss[i]->CheckCollision(DIRECTION_LEFT)) {
					_isBossLeft = false;
				}
			}
		}
		else if (!_isBossLeft) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_RIGHT);

				if (_vBoss[i]->CheckCollision(DIRECTION_RIGHT)) {
					_isBossLeft = true;
				}
			}
		}
	}
}
