#pragma once

#include "GameNode.h"

#include <vector>
using namespace std;

#define POKEMONMAX 151

struct tagPokemon {
	float x, y;
	float angle;
	float speed;
	float radius;
};

class MainGame14 : public GameNode
{
private:
	tagPokemon m_pokemon[POKEMONMAX];
	vector<pair<int, int>> v;	// ���ϸ� �Ӹ����� ��ġ�� ������ ����(2���� �迭 ��ġ��)

	Image * map;
	Image * pokemons;		// 12 * 13 ũ���� ���ϸ� �Ӹ� 151���� (������ �� 7����)

public:
	MainGame14();
	~MainGame14();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void PokemonMove();
};

