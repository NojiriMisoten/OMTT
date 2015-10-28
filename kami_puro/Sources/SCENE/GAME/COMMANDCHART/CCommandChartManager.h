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
//	�O���錾
//-----------------------------------------------------------------------------

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
	static void Init(LPDIRECT3DDEVICE9* device);

	// �X�V
	static void Update(void);

	// �`��
	static void Draw(void);

	// �I��
	static void Uninit(void);

	// ����
	// �����F�f�o�C�X
	static void Create(LPDIRECT3DDEVICE9* device);

private:
	// �R�}���h�`���[�g�̃A�h���X�i�[�p�̃|�C���^
	static CCommandChart* m_pCommandChart[MAX_PLAYER_NUM-1];
};

#endif

// EOF