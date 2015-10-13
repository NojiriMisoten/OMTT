//=============================================================================
//
// CSceneTime�N���X [CSceneTime.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneTime.h"
#include "../UI/CNumber.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int	MINUS_TIME_COUNT = 180;			// �^�C�����P����܂ł̎���
static const int	DEFAULT_MINUS_TIME_VALUE = -1;	// �^�C��������f�t�H���g�̗�

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneTime ::CSceneTime(LPDIRECT3DDEVICE9 *pDevice):CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneTime ::~CSceneTime(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CSceneTime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// ���_�̈ʒu
	SetPos(pos);

	// ���ƍ����̐ݒ�
	m_fWidth = width;
	m_fHeight = height;

	// ����
	m_nNumberNum = numberNum;

	// �^�C�}�[������
	m_nTime = 0;

	// �J�E���^�[������
	m_nCounter = 0;

	// �P�̈ʂ̏ꏊ
	pos.x += (MAX_TIME_NUM - 1) * width;

	// �����������쐬
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		m_Number[i] = m_Number[i]->Create(m_pD3DDevice, pos, width, height, texType);
		pos.x -= width;
	}
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneTime ::Uninit(void)
{
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		if(m_Number[i])
		{
			m_Number[i]->Uninit();
			delete m_Number[i];
			m_Number[i] = NULL;
		}
	}
	Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneTime ::Update(void)
{
	// �J�E���g�A�b�v
	m_nCounter++;

	// ��莞�ԉ߂�����
	if(m_nCounter >= MINUS_TIME_COUNT)
	{
		// �^�C�}�[���炷
		Add(DEFAULT_MINUS_TIME_VALUE);

		// �J�E���g������
		m_nCounter = 0;
	}
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneTime ::Draw(void)
{
	// �����������\��
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		m_Number[i]->Draw();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneTime* CSceneTime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// �쐬
	CSceneTime* p = new CSceneTime(pDevice);

	// ������
	p->Init(pos, width, height, texType, numberNum);

	return p;
}

//*****************************************************************************
// �����ύX�֐�
//*****************************************************************************
void CSceneTime ::Add(int addNum)
{
	m_nTime += addNum;
	if(m_nTime >= MAX_TIME)
	{
		m_nTime = MAX_TIME;
	}

	if(m_nTime <= 0)
	{
		m_nTime = 0;
	}

	for(int nCntPlace = 0; nCntPlace < MAX_TIME_NUM; nCntPlace++)
	{
		int number = 0;
		//���̐������o����
		number = (m_nTime / (int)pow(10.0,(double)(nCntPlace))) % 10;	// 10�i�̐������o���̂�10���g���Ă�

		// �����Z�b�g
		m_Number[nCntPlace]->SetNumber(number);
	}
}
//----EOF----