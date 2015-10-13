//=============================================================================
//
// CSceneScore�N���X [CSceneScore.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneScore.h"
#include "../UI/CNumber.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
LPDIRECT3DDEVICE9	CSceneScore:: *m_pD3DDevice = NULL;
float				CSceneScore:: m_fWidth = 0.f;
float				CSceneScore:: m_fHeight = 0.f;
int					CSceneScore:: m_nScore = 0;
CNumber*			CSceneScore:: m_Number[MAX_SCORE_NUM] = {NULL};
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneScore::CSceneScore(LPDIRECT3DDEVICE9 *pDevice, int nPriority) :CScene2D(pDevice, nPriority)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneScore ::~CSceneScore(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CSceneScore :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// ���_�̈ʒu
	SetPos(pos);

	// ���ƍ����̐ݒ�
	m_fWidth = width;
	m_fHeight = height;

	m_nScore = 0;

	pos.x += (MAX_SCORE_NUM - 1) * width;

	// �����������쐬
	for(int i = 0; i < MAX_SCORE_NUM; i++)
	{
		m_Number[i] = m_Number[i]->Create(m_pD3DDevice, pos, width, height, texType);
		pos.x -= width;
	}
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneScore ::Uninit(void)
{
	for(int i = 0; i < MAX_SCORE_NUM; i++)
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
void CSceneScore ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneScore ::Draw(void)
{
	// �����������\��
	for(int i = 0; i < MAX_SCORE_NUM; i++)
	{
		m_Number[i]->Draw();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneScore* CSceneScore::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// �쐬
	CSceneScore* p = new CSceneScore(pDevice);

	// ������
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// �����ύX�֐�
//*****************************************************************************
void CSceneScore ::Add(int addNum)
{
	m_nScore += addNum;
	if(m_nScore >= MAX_SCORE)
	{
		m_nScore = MAX_SCORE;
	}

	if(m_nScore <= 0)
	{
		m_nScore = 0;
	}

	for(int nCntPlace = 0; nCntPlace < MAX_SCORE_NUM; nCntPlace++)
	{
		int number = 0;
		//���̐������o����
		number = (m_nScore / (int)pow(10.0,(double)(nCntPlace))) % 10;	// �����Ƃ�10�i�̐������o�����߂�10���g���Ă���

		// �����Z�b�g
		m_Number[nCntPlace]->SetNumber(number);
	}
}
//----EOF----