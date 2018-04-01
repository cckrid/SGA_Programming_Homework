#include "stdafx.h"
#include "Alien.h"

Alien::Alien()
{

}

Alien::~Alien()
{

}

HRESULT Alien::Init(const char * imageName, POINT position)
{
	// ���ʹ̿��� ���� init
	Enemy::Init(imageName, position);
	
	// �߰������� Init
	this->_died = IMAGE->FindImage("alien_died");

	_frameCount = 0;
	_currentFrameX2 = 0;

	return S_OK;
}

void Alien::Release()
{
	Enemy::Release();
}

void Alien::Update()
{
	Animation();
}

void Alien::Render()
{
	Draw();
}

void Alien::Draw()
{
	// ��� �ִ� ���¸� ���ʹ̲��� �׸��� (idle �̹���)
	if (_isLived)
		Enemy::Draw();
	// ��� �ִ� ���°� �ƴϸ�
	// �״� �̹��� �׸���
	else
		_died->FrameRender(GetMemDC(),
			GetRect().left - 25, GetRect().top - 25, _currentFrameX2, 0);
}

void Alien::Animation()
{
	// ��� �ִ� ���¸� ���ʹ� ���� �ִϸ��̼� ���� (idle)
	if(_isLived)
		Enemy::Animation();
	// �ƴϸ� �״� �ִϸ��̼� ����
	else {
		_frameCount++;
		if (_frameCount % 5 == 0) {
			_currentFrameX2++;
			if (_currentFrameX2 == _died->GetMaxFrameX())
				_isDied = true;
		}
	}
}
