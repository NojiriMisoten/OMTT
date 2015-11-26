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
CCommandChartManager::CCommandChartManager()
{
	// �R�}���h�`���[�g�N���X�i�[�p�̃|�C���^��NULL��
	m_pCommandChart[MY_ID_1] = NULL;
	m_pCommandChart[MY_ID_2] = NULL;
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
CCommandChartManager* CCommandChartManager::Create(LPDIRECT3DDEVICE9* pDevice)
{
	CCommandChartManager* temp = new CCommandChartManager;
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
	m_pCommandChart[MY_ID_1] = CCommandChart::Create(pDevice, MY_ID_1, this);
	m_pCommandChart[MY_ID_2] = CCommandChart::Create(pDevice, MY_ID_2, this);
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChartManager::Update(void)
{
	// �R�}���h�`���[�g�̍X�V
	m_pCommandChart[MY_ID_1]->Update();
	m_pCommandChart[MY_ID_2]->Update();

	Draw();
}

//-----------------------------------------------------------------------------
//	�`�揈��
//-----------------------------------------------------------------------------
void CCommandChartManager::Draw(void)
{
	// �R�}���h�`���[�g�̕`��
	m_pCommandChart[MY_ID_1]->Draw();
	m_pCommandChart[MY_ID_2]->Draw();
}

//-----------------------------------------------------------------------------
//	�I������
//-----------------------------------------------------------------------------
void CCommandChartManager::Uninit(void)
{
	// �R�}���h�`���[�g�̏I��
	m_pCommandChart[MY_ID_1]->Uninit();
	m_pCommandChart[MY_ID_2]->Uninit();
	
	// ��Еt��
	SAFE_DELETE(m_pCommandChart[MY_ID_1]);
	SAFE_DELETE(m_pCommandChart[MY_ID_2]);
}

//-----------------------------------------------------------------------------
// ���[�h���Z�b�g
//-----------------------------------------------------------------------------
void CCommandChartManager::SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode)
{
	m_pCommandChart[ID]->SetCommandChartMode(mode);
}
// EOF