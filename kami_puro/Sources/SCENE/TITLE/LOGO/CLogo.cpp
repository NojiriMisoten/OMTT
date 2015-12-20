//=============================================================================
//
// CLogo�N���X [CLogo.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CLogo.h"
#include "../../../SHADER/CShader.h"
#include "../../../MATH/math.h"
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CLogo::CLogo(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene2D(pDevice, objType)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CLogo ::~CLogo(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CLogo::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);
	m_Alpha = 1.f;
	m_FadeTime = 0.f;
	m_isFade = false;
	m_FadeSpd = 0.f;
	m_GrayScal = D3DXVECTOR3(0.298912f, 0.586611f, 0.114478f) * 0.5f;
	m_isFadeIN = false;
	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CLogo::Update(void)
{
	UpdateFade();
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CLogo::DrawUIRender(void)
{
	// �����`��ON�Ȃ�
	if (!m_AutoDraw)
	{
		return;
	}

	PS_TYPE type = PS_TYPE_2D_GRAY;

	LPDIRECT3DPIXELSHADER9	*ps;
	LPD3DXCONSTANTTABLE		*psc;
	ps = CShader::GetPS(type);
	psc = CShader::GetPSC(type);
	(*m_pD3DDevice)->SetPixelShader(*ps);
	HRESULT hr = 0;
	UINT texSampler = (*psc)->GetSamplerIndex("texSampler");
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	(*psc)->SetFloatArray((*m_pD3DDevice), "gMonochromeScale", (float*)&m_GrayScal, 3);
	(*psc)->SetFloat((*m_pD3DDevice), "gAlpha", m_Alpha);

	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(texSampler, m_pD3DTexBuff);							// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
									, 0											// �`����J�n���钸�_�ԍ�
									, DRAW_SQUARE_PRINITIV_NUM);				// ���������|���S����

	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CLogo* CLogo::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	// �쐬
	CLogo* p = new CLogo(pDevice);

	// ������
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// �t�F�[�h�C���X�^�[�g�֐�
//*****************************************************************************
void CLogo::StartFadeIN(int maxFadeInFrame)
{
	m_isFade = true;
	m_FadeSpd = 1.f / (float)maxFadeInFrame;
	m_FadeTime = 0.f;
	m_Alpha = 0.f;
	m_isFadeIN = true;
}

//*****************************************************************************
// �t�F�[�h�A�E�g�X�^�[�g�֐�
//*****************************************************************************
void CLogo::StartFadeOUT(int maxFadeInFrame)
{
	m_isFade = true;
	m_FadeSpd = 1.f / (float)maxFadeInFrame;
	m_FadeTime = 0.f;
	m_Alpha = 1.f;
	m_isFadeIN = false;
}

//*****************************************************************************
// �t�F�[�h�X�V�֐�
//*****************************************************************************
void CLogo::UpdateFade(void)
{
	if (!m_isFade)
	{
		return;
	}

	if (m_isFadeIN)
	{
		m_Alpha = EasingInterpolation(0.f, 1.f, m_FadeTime);
	}
	else
	{
		m_Alpha = EasingInterpolation(1.f, 0.f, m_FadeTime);
	}
	m_FadeTime += m_FadeSpd;
	if (m_FadeTime > 1.f)
	{
		m_isFade = false;
	}
}
//----EOF----