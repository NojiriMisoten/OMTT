//=============================================================================
//
//	CCommandChartManager�N���X�̐錾(CCommandChartManager.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHARTMANAGER_H__
#define __CCOMMNADCHARTMANAGER_H__

//-----------------------------------------------------------------------------
// �C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "CCommandChart.h"


//-----------------------------------------------------------------------------
//	�}�N��
//-----------------------------------------------------------------------------
static const int MAX_PLAYER_NUM = 2;

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------
class CCommandChart;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChartManager
{
public:
	// �R���X�g���N�^
	CCommandChartManager();

	// �f�X�g���N�^
	~CCommandChartManager();

	// ������
	// �����F�f�o�C�X
	void Init(LPDIRECT3DDEVICE9* device);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �I��
	void Uninit(void);

	// ����
	// �����F�f�o�C�X
	static CCommandChartManager* Create(LPDIRECT3DDEVICE9* device);

	// �R�}���h�`���[�g�̎擾
	// �����F�v���C���[ID
	// �߂�l�F�R�}���h�`���[�g�̍\�������Z�̃^�C�v
	COMMAND_TYPE GetCommandChartTechnic(int nID){ return m_pCommandChart[nID]->UseTechnic(); }

private:
	// �R�}���h�`���[�g�̃A�h���X�i�[�p�̃|�C���^
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];
};

#endif

// EOF