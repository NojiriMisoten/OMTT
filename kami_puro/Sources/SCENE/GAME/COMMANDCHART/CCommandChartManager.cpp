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

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartManager::CCommandChartManager(CPlayerManager* pPlayerManager)
{
	// �R�}���h�`���[�g�N���X�i�[�p�̃|�C���^��NULL��
	m_pCommandChart[PLAYER_1] = NULL;
	m_pCommandChart[PLAYER_2] = NULL;

	m_pPlayerManager = pPlayerManager;
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
CCommandChartManager* CCommandChartManager::Create(LPDIRECT3DDEVICE9* pDevice, CPlayerManager* pPlayerManager)
{
	CCommandChartManager* temp = new CCommandChartManager(pPlayerManager);
	// �R�}���h�`���[�g�}�l�[�W���̏�����
	temp->Init(pDevice);

	return temp;
}

//-----------------------------------------------------------------------------
//	����������
//-----------------------------------------------------------------------------
void CCommandChartManager::Init(LPDIRECT3DDEVICE9* pDevice)
{
	// �R�}���h�`���[�g�̐���
	m_pCommandChart[PLAYER_1] = CCommandChart::Create(pDevice, PLAYER_1, this);
	m_pCommandChart[PLAYER_2] = CCommandChart::Create(pDevice, PLAYER_2, this);
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChartManager::Update(void)
{
	// �R�}���h�`���[�g�̍X�V
	m_pCommandChart[PLAYER_1]->Update();
	m_pCommandChart[PLAYER_2]->Update();

	Draw();
}

//-----------------------------------------------------------------------------
//	�`�揈��
//-----------------------------------------------------------------------------
void CCommandChartManager::Draw(void)
{
	// �R�}���h�`���[�g�̕`��
	m_pCommandChart[PLAYER_1]->Draw();
	m_pCommandChart[PLAYER_2]->Draw();
}

//-----------------------------------------------------------------------------
//	�I������
//-----------------------------------------------------------------------------
void CCommandChartManager::Uninit(void)
{
	// �R�}���h�`���[�g�̏I��
	m_pCommandChart[PLAYER_1]->Uninit();
	m_pCommandChart[PLAYER_2]->Uninit();
	
	// ��Еt��
	SAFE_DELETE(m_pCommandChart[PLAYER_1]);
	SAFE_DELETE(m_pCommandChart[PLAYER_2]);
}

//-----------------------------------------------------------------------------
// ���[�h���Z�b�g
//-----------------------------------------------------------------------------
void CCommandChartManager::SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode)
{
	m_pCommandChart[ID]->SetCommandChartMode(mode);
}
// EOF