#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"

Program::Program()
{
	srand(time(NULL));

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
	rect->Init();

	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	vEye = Vector2(0, 0);	// ī�޶��� ��ġ
}

Program::~Program()
{
	rect->Release();
	SAFE_DELETE(bg);
	SAFE_DELETE(rect);
}

void Program::Update()
{
	bg->Update();
	rect->Update();

	// ī�޶� ����°�
	Vector2 vLookAt(0, 0, 1);	// �ٶ󺸴� ��ǥ��
	vLookAt = vEye + Vector2(0, 0, 1);
	Vector2 vUp(0, 1, 0);
	matView = Matrix::View(vEye, vLookAt, vUp);
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0, 1.0f);
}

void Program::Render()
{
	// ���������� ����ϴ°� Device�� �Ѱ��ְԵ�
	D2D::GetDevice()->SetTransform(D3DTS_VIEW, &matView.ToDXMatrix());
	D2D::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProjection.ToDXMatrix());

	bg->Render();
	rect->Render();
}
