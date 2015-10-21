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
//	�O���錾
//-----------------------------------------------------------------------------
class CCommandChart;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChartManger
{
public:
	// �R���X�g���N�^
	CCommandChartManger();

	// �f�X�g���N�^
	~CCommandChartManger();

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
	static CCommandChart* m_pCommandChart;
};

#endif

// EOF