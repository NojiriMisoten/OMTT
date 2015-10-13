//=============================================================================
//
// CSceneCircle�N���X [CSceneCircle.cpp]
// Author : ��K�@	
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneCircle.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const short CIRCLE_VERTEX_NUM = 128;
static const short CIRCLE_PRIMITIVE_NUM = CIRCLE_VERTEX_NUM - 2;
static const float CIRCLE_VERTEX_OFFSET = D3DX_PI * -0.5f;
static const float CIRCLE_RADIAN_MAX = 2.0f * D3DX_PI;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneCircle::CSceneCircle(LPDIRECT3DDEVICE9 *pDevice) :CScene2D(pDevice, TYPE_PRIORITY_UI, OBJTYPE_2D)
{
	m_pD3DDevice = pDevice;
	m_bDraw = true;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneCircle ::~CSceneCircle(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneCircle::Init(D3DXVECTOR3 pos, float radius, D3DXCOLOR color)
{
	// �l���
	m_vPos = pos;
	m_fRadius = radius;
	m_Color = color;
	m_fRad = CIRCLE_RADIAN_MAX;

	// �|���S���̃������m��
	if (CreateVertexBuff() == E_FAIL){
		return E_FAIL;
	}

	// �|���S���̐ݒ�
	SetVertexBuff();

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneCircle ::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneCircle ::Update(void)
{
	m_fRad -= 0.01f;
	SetVertexBuffVtx();
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneCircle ::Draw(void)
{
	// �`��t���O
	if (!m_bDraw){
		return;
	}

	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, CIRCLE_PRIMITIVE_NUM);
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneCircle* CSceneCircle::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float radius, D3DXCOLOR color, float fMaxValue)
{
	// �쐬
	CSceneCircle* p = new CSceneCircle(pDevice);

	// ������
	p->Init(pos, radius, color);

	// �ő�l�ɑ΂��鐔�l�̊������v�Z
	p->CalcRatio(fMaxValue);
	return p;
}

//*****************************************************************************
// �~�̂��������X�V
//*****************************************************************************
void CSceneCircle::ChangeSize(float fValue)
{
	// �Z�b�g���ꂽ�l���p�x�ɂ��đ傫������
	m_fRad = m_fValueRaio * fValue;
	SetVertexBuffVtx();
}

//-----------------------------------------------------------------------------
// ���_�o�b�t�@�̊m��
//-----------------------------------------------------------------------------
HRESULT CSceneCircle::CreateVertexBuff(){

	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * CIRCLE_VERTEX_NUM
		, D3DUSAGE_WRITEONLY			// �ς��Ȃ�
		, FVF_VERTEX_2D					// �g�p���钸�_�t�H�[�}�b�g
		, D3DPOOL_MANAGED				// �ς��Ȃ�
		, &m_pD3DVtxBuff				// �i�[����ꏊ
		, NULL)))						// �ς��Ȃ�
	{
		return E_FAIL;															// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// ���_���̏�����
//-----------------------------------------------------------------------------
void CSceneCircle::SetVertexBuff(){

	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

	// ��̊p�x�@-2��"���_�ԍ�0"�̕���"���_�ԍ�1"�ɍ��킹�邽��
	float one_radius = D3DX_PI * 2 / (CIRCLE_VERTEX_NUM - 2);

	for (int i = 0; i < CIRCLE_VERTEX_NUM; i++){
		float off_set_x = cosf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		float off_set_y = sinf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		pVtx[i].vtx = D3DXVECTOR3(m_vPos.x + off_set_x, m_vPos.y + off_set_y, 0);
		pVtx[i].rhw = 1.0f;
		pVtx[i].diffuse = m_Color;
		pVtx[i].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	}
	// "���_�ԍ�0"�͒��S�ɐݒ�
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x, m_vPos.y, 0);
	m_pD3DVtxBuff->Unlock();																	// ���b�N������K���A�����b�N�I�I
}

//-----------------------------------------------------------------------------
// ���݂̊p�x�Œ��_����Vtx�̂ݏ����ւ��i�X�V�ŌĂԁj
//-----------------------------------------------------------------------------
void CSceneCircle::SetVertexBuffVtx(){

	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

	// ��̊p�x�@-2��"���_�ԍ�0"�̕���"���_�ԍ�1"�ɍ��킹�邽��
	float one_radius = m_fRad / (CIRCLE_VERTEX_NUM - 2);

	for (int i = 0; i < CIRCLE_VERTEX_NUM; i++){
		float off_set_x = cosf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		float off_set_y = sinf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		pVtx[i].vtx = D3DXVECTOR3(m_vPos.x + off_set_x, m_vPos.y + off_set_y, 0);
	}
	// "���_�ԍ�0"�͒��S�ɐݒ�
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x, m_vPos.y, 0);
	m_pD3DVtxBuff->Unlock();																	// ���b�N������K���A�����b�N�I�I
}

//-----------------------------------------------------------------------------
// �ő�l�ɑ΂��鐔�l�̊������ő�l����v�Z
//-----------------------------------------------------------------------------
void CSceneCircle::CalcRatio(float max){

	m_fValueRaio = CIRCLE_RADIAN_MAX / max;
}
//----EOF----