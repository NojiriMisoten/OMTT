//=============================================================================
//
//	CCommandChartManager�N���X�̏���(CCommandChartManager.cpp)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
// �C���N���[�h
#include "CCommandChartManager.h"
#include "../../../CONST/const.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// static
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
CCommandChart* CCommandChartManager::m_pCommandChart[MAX_PLAYER_NUM-1];

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartManager::CCommandChartManager()
{
	// �R�}���h�`���[�g�N���X�i�[�p�̃|�C���^��NULL��
	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		m_pCommandChart[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartManager::~CCommandChartManager()
{
}

//-----------------------------------------------------------------------------
//	��������
//-----------------------------------------------------------------------------
void CCommandChartManager::Create(LPDIRECT3DDEVICE9* pDevice)
{
	// �R�}���h�`���[�g�}�l�[�W���̏�����
	Init(pDevice);
}

//-----------------------------------------------------------------------------
//	����������
//-----------------------------------------------------------------------------
void CCommandChartManager::Init(LPDIRECT3DDEVICE9* pDevice)
{
	// �R�}���h�`���[�g�̐���
	m_pCommandChart[0] = CCommandChart::Create(pDevice, PLAYER_NUMBER_1);
	m_pCommandChart[1] = CCommandChart::Create(pDevice, PLAYER_NUMBER_2);
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChartManager::Update(void)
{
	// �R�}���h�`���[�g�̍X�V
	m_pCommandChart[0]->Update();
	m_pCommandChart[1]->Update();
}

//-----------------------------------------------------------------------------
//	�`�揈��
//-----------------------------------------------------------------------------
void CCommandChartManager::Draw(void)
{
	// �R�}���h�`���[�g�̕`��
	m_pCommandChart[0]->Draw();
	m_pCommandChart[1]->Draw();
}

//-----------------------------------------------------------------------------
//	�I������
//-----------------------------------------------------------------------------
void CCommandChartManager::Uninit(void)
{
	// �R�}���h�`���[�g�̏I��
	m_pCommandChart[0]->Uninit();
	m_pCommandChart[1]->Uninit();
	
	// ��Еt��
	SAFE_DELETE(m_pCommandChart[0]);
	SAFE_DELETE(m_pCommandChart[1]);
}

// EOF