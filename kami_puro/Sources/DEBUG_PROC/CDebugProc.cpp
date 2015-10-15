//=============================================================================
//
// �f�o�b�O��񏈗� [CDebugProc.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../RENDERER/CRenderer.h"
#include "CDebugProc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const RECT DRAW_RECT = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};		// �`��͈�
static const D3DXCOLOR DRAW_TEXT_COLOR = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);	// �����̐F

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
LPD3DXFONT	CDebugProc::m_pD3DXFONT = NULL;						// �t�H���g�ւ̃|�C���^
char		CDebugProc::m_aStr[LENGTH_STRING_BUFF] = {'\0'};	// �f�o�b�O�\���p�̕�����̃o�b�t�@
bool		CDebugProc::m_bDisp = false;						// �f�o�b�O�\��ON/OFF
int			CDebugProc::m_nCounter = 0;							// �������J�E���^�[
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CDebugProc::CDebugProc(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CDebugProc::~CDebugProc(void)
{
}

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT CDebugProc::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 *pDevice = CRenderer::GetDevice();

	// �t�H���g�쐬
	hr = D3DXCreateFont(*pDevice,
						18,
						0,
						0,
						0,
						FALSE,
						SHIFTJIS_CHARSET,
						OUT_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH,
						"Terminal",
						&m_pD3DXFONT);

	// �o�b�t�@�̏�����
	ZeroMemory(&m_aStr, sizeof(char));

	return hr;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CDebugProc::Uninit(void)
{
	if(m_pD3DXFONT)
	{
		m_pD3DXFONT->Release();
		m_pD3DXFONT = NULL;
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CDebugProc::Draw(void)
{
#ifdef _DEBUG

	// �`��͈͐ݒ�
	RECT rect = DRAW_RECT;

	// �`��
	m_pD3DXFONT->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, DRAW_TEXT_COLOR);

	// �o�b�t�@������
	ZeroMemory(&m_aStr[0],sizeof(char));

	// ������������
	m_nCounter = 0;
#endif
}

//*****************************************************************************
// �o�^����
//*****************************************************************************
void CDebugProc::Print(const char *fmt, ...)
{
#ifdef _DEBUG
	char str[LENGTH_STRING_BUFF] = {"\0"};

	// �ψ����ۑ��p
	va_list args;

	// �ψ���������
	va_start(args, fmt);

	int i;
	for(i = m_nCounter; i < LENGTH_STRING_BUFF; i++)
	{
		// �������J�E���g
		m_nCounter++;

		// �o�b�t�@�I�[�o�[�t���[�h�~
		if(str[i] == '\0' || m_nCounter >= LENGTH_STRING_BUFF)
		{
			break;
		}
	}

	// �n���ꂽ������ꎞ�ۑ�
	vsprintf(str, fmt, args);

	va_end( args );

	// �����񍇐�
	strcat(m_aStr,str);
#endif
}
//----EOF----