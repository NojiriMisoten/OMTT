//=============================================================================
//
// �A�j���[�V�����N���X [CAnimation.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAnimation.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAnimation::CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority)
{
	m_bDraw = true;
	m_bFall = false;
	m_bFadeOut = false;
	m_bFadeIn = false;
	m_fGravity = 0;
	m_fFloor = 0;
	m_fAlpha = 1.0f;
	m_fFadeSpeed = 0.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAnimation ::~CAnimation(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CAnimation :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY)
{
	CScene2D ::Init(pos, width, height, texType);

	// �l�̃`�F�b�N
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;

	// �e�N�X�`�����p�Z�b�g
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CAnimation ::Uninit(void)
{
	CScene2D :: Uninit();

}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CAnimation ::Update(void)
{
	// ������ݒ�Ȃ�
	if (m_bFall){
		m_fGravity += GRAVITY_SCALE;
		m_vPos.y += m_fGravity;
		if (m_vPos.y > m_fFloor){
			m_fGravity *= -REFLECT_SCALE;
			m_vPos.y = m_fFloor;
		}
	}

	// �t�F�[�h�A�E�g����Ȃ�
	if (m_bFadeOut){
		m_fAlpha -= m_fFadeSpeed;
		if (m_fAlpha < 0.0f){
			m_fAlpha = 0.0f;
		}
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}
	if (m_bFadeIn){
		m_fAlpha += m_fFadeSpeed;
		if (m_fAlpha > 1.0f){
			m_fAlpha = 1.0f;
		}
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	}
}


//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CAnimation ::Draw(void)
{
	if(m_bDraw)
	{
		CScene2D ::Draw();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY)
{
	// �쐬
	CAnimation* p = new CAnimation(pDevice, nPriority);

	p->m_pD3DDevice = pDevice;

	// ������
	p->Init(pos, width, height, texType, sepalateX, sepalateY);

	return p;
}

//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CAnimation::SetIndex(int nIdx)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
	m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
	m_fTop = fV * (m_nIdx / m_nSepalateNumX);
	m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CAnimation::SetIndex(int nIdx, bool reverse)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	if (reverse){
		m_fRight = fU * (m_nIdx % m_nSepalateNumX);
		m_fLeft = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}
	else{
		m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
		m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}

//=============================================================================
// ������Z�b�g
//=============================================================================
void CAnimation::SetFall(D3DXVECTOR3 pos, float fFloorHeight){
	m_vPos = pos;
	m_bFall = true;
	m_fFloor = fFloorHeight;
	m_fGravity = 0;
}

//=============================================================================
// �����邵�Ȃ��Z�b�g
//=============================================================================
void CAnimation::SetNonFall(D3DXVECTOR3 pos){
	m_vPos = pos;
	m_bFall = false;
	m_fFloor = 0;
	m_fGravity = 0;
}

//=============================================================================
// �t�F�[�h�A�E�g�Z�b�g
//=============================================================================
void CAnimation::SetFadeOut(float alpha, float fFadeSpeed){

	m_bFadeOut = true;
	m_fAlpha = alpha;
	m_fFadeSpeed = fFadeSpeed;
	SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=============================================================================
// �t�F�[�h�A�E�g���Ȃ��Z�b�g
//=============================================================================
void CAnimation::SetNonFadeOut(){

	m_bFadeOut = false;
	m_fAlpha = 1.0f;
	m_fFadeSpeed = 0.0f;
	SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}
//=============================================================================
// �t�F�[�h�C���Z�b�g
//=============================================================================
void CAnimation::SetFadeIn(float alpha, float fFadeSpeed){

	m_bFadeIn = true;
	m_fAlpha = alpha;
	m_fFadeSpeed = fFadeSpeed;
	SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=============================================================================
// �t�F�[�h�C�����Ȃ��Z�b�g
//=============================================================================
void CAnimation::SetNonFadeIn(){

	m_bFadeIn = false;
	m_fAlpha = 0.0f;
	m_fFadeSpeed = 0.0f;
	SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}
//----EOF----