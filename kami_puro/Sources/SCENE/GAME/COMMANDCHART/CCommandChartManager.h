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

private:
	// �R�}���h�`���[�g�̃A�h���X�i�[�p�̃|�C���^
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];
};

#endif

// EOF