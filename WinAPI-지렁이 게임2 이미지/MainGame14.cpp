#include "stdafx.h"
#include "MainGame14.h"


MainGame14::MainGame14()
{
}


MainGame14::~MainGame14()
{
}

HRESULT MainGame14::Init()
{
	GameNode::Init();

	// �ʱ�ȭ ���ִ� �Լ�
	memset(m_pokemon, 0, sizeof(m_pokemon));
	//memset(m_pokemon, 0, sizeof(tagBead) * BEADMAX);

	for (int i = 0; i < POKEMONMAX; i++) {
		m_pokemon[i].radius = 30.0f;
		m_pokemon[i].speed = 5.0f;
		// ���� ������ (����) ? �� : ����;
		m_pokemon[i].angle = (i == 0) ? RND->GetInt(7) : m_pokemon[i - 1].angle;
		m_pokemon[i].x = (i == 0) ? RND->GetFromInto(100, WINSIZEX - 100)
			: m_pokemon[i].x - cosf(m_pokemon[i].angle) *
			(m_pokemon[i - 1].radius * 2 + m_pokemon[i].radius * 2);
		m_pokemon[i].y = (i == 0) ? RND->GetFromInto(100, WINSIZEY - 100)
			: m_pokemon[i].y - (-sinf(m_pokemon[i].angle)) *
			(m_pokemon[i - 1].radius * 2 + m_pokemon[i].radius * 2);
	}

	map = new Image;
	pokemons = new Image;

	map->Init("images/map.bmp", WINSIZEX, WINSIZEY);
	pokemons->Init(
		"images/pokemon.bmp", WINSIZEX, WINSIZEY, true, RGB(255,0,255));

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 13 && j == 7) break;	// 151���������� �� ���Ĵ� break
			v.push_back(make_pair(i, j));	// 151������ ��ġ�� �� �־��
		}
	}

	

	return S_OK;
}

void MainGame14::Release()
{
	GameNode::Release();
}

void MainGame14::Update()
{
	GameNode::Update();

	if (INPUT->GetKey(VK_LEFT)) {
		m_pokemon[0].angle += 0.05f;
	}
	if (INPUT->GetKey(VK_RIGHT)) {
		m_pokemon[0].angle -= 0.05f;
	}

	PokemonMove();
}

void MainGame14::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	map->Render(memDC);
	// test
	//pokemons->Render(memDC);

	// ���ϸ� �Ӹ� �ϳ� 85, 59

	//// �׽�Ʈ2
	//for (int i = 0; i < 12; i++) {
	//	for(int j=0; j<12;j++)
	//		pokemons->Render(memDC, 85*i, 85*j, 85*i, 59*j, 85, 59);
	//}

	for (int i = 0; i < POKEMONMAX; i++) {
		//EllipseMakeCenter(memDC, m_pokemon[i].x, m_pokemon[i].y, m_pokemon[i].radius);

		pokemons->Render(memDC, m_pokemon[i].x, m_pokemon[i].y, 
			85*v[i].second, 59*v[i].first, 85, 59);
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame14::PokemonMove()
{
	for (int i = 0; i < POKEMONMAX; i++) {
		if (i == 0) {
			m_pokemon[i].x += cosf(m_pokemon[i].angle) * m_pokemon[i].speed;
			m_pokemon[i].y += -sinf(m_pokemon[i].angle) * m_pokemon[i].speed;

			// �� �浹
			// ��ġ ���� �ʿ伺��
			// �ӵ��� �ʹ� ������ �� ���ĳ����� �ʰ� �ϴ°���
			// �ӵ� ���� �ϸ� ���ص� �Ǵ� ���
			// ���� 
			if (m_pokemon[i].x - m_pokemon[i].radius < 0)
				m_pokemon[i].angle = PI - m_pokemon[i].angle;
			// ������
			if (m_pokemon[i].x + m_pokemon[i].radius > WINSIZEX)
				m_pokemon[i].angle = PI - m_pokemon[i].angle;
			// ��
			if (m_pokemon[i].y - m_pokemon[i].radius < 0)
				m_pokemon[i].angle = -m_pokemon[i].angle;
			// �Ʒ�
			if (m_pokemon[i].y + m_pokemon[i].radius > WINSIZEY)
				m_pokemon[i].angle = -m_pokemon[i].angle;
		}
		else {
			// �̷������� �ص� �Ǵµ� ��ǻ�� ���ɿ� �ٶ� 
			// �Ѳ����� ȸ���ϴ� ��쵵 ���� �� ����� ���� ����
			//m_pokemon[i-1].angle = m_pokemon[i-1].angle;

			float distance = GetDistance(m_pokemon[i].x, m_pokemon[i].y,
				m_pokemon[i - 1].x, m_pokemon[i - 1].y);

			// m_pokemon[i].radius + m_pokemon[i-1].radius
			if (m_pokemon[i].radius * 2 < distance) {

				// GetAngle ������ �μ� �ݴ�� ������ ���� �� �� �� ���� 180 ����
				// �׷��� ��ź �߻� �� ������ �浹�� �� �ݴ�� �ִ°�
				m_pokemon[i].angle = GetAngle(
					m_pokemon[i].x, m_pokemon[i].y, m_pokemon[i - 1].x, m_pokemon[i - 1].y);

				m_pokemon[i].x = m_pokemon[i - 1].x - cosf(m_pokemon[i].angle) *
					m_pokemon[i].radius * 2;
				m_pokemon[i].y = m_pokemon[i - 1].y - (-sinf(m_pokemon[i].angle)) *
					m_pokemon[i].radius * 2;
			}

			m_pokemon[i].x += cosf(m_pokemon[i].angle) * m_pokemon[i].speed;
			m_pokemon[i].y += -sinf(m_pokemon[i].angle) * m_pokemon[i].speed;
		}
	}
}
