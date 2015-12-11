//=============================================================================
//
// CCrowdManager�N���X [CCrowdManager.h]
// Author : �˖{�r�F
//
// �����O�̑O��ɂ���ϋq����
// �R�c�̔|��
//
//=============================================================================
#ifndef _CCROWDMANAGER_H_
#define _CCROWDMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CCrowd;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCrowdManager
{
public:
	// ��̋q�Ȃ̉��̗�
	static const short UPPER_CHAIR_COL_MAX = 5;
	// ��̋q�Ȃ̍s��
	static const short UPPER_CHAIR_ROW_MAX = 6;
	// ��̋q�Ȃ̉��̗�
	static const short UPPER_SIDE_CHAIR_COL_MAX = 6;
	// ��̋q�Ȃ̍s��
	static const short UPPER_SIDE_CHAIR_ROW_MAX = 3;
	// ���̋q�Ȃ̑O�̗�
	static const short LOWER_FRONT_CHAIR_COL_MAX = 2;
	// ���̋q�Ȃ̑O�ȊO�̗�
	static const short LOWER_RIGHT_CHAIR_COL_MAX = 5;
	// ���̋q�Ȃ̍s��
	static const short LOWER_CHAIR_ROW_MAX = 5;

	CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CCrowdManager();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �N���G�C�g
	static CCrowdManager* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

	// �`�悵�Ȃ�
	void SetImvisible(void);
	// �`�悷��
	void SetVisible(void);

private:
	void Init();

	// �ۑ����Ƃ������|�C���^
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	CManager *m_pManager;

	// 2F�̋q�� �ォ��O�A���A�E
	CCrowd *m_pUpperFront[UPPER_CHAIR_COL_MAX][UPPER_CHAIR_ROW_MAX];
	CCrowd *m_pUpperLeft[UPPER_SIDE_CHAIR_COL_MAX][UPPER_SIDE_CHAIR_ROW_MAX];
	CCrowd *m_pUpperRight[UPPER_SIDE_CHAIR_COL_MAX][UPPER_SIDE_CHAIR_ROW_MAX];
	// 1F�̑O�̋q�ȁ@���T�C�h
	CCrowd *m_pLowerFrontLeft[LOWER_FRONT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1F�̑O�̋q�ȁ@�E�T�C�h
	CCrowd *m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1F�̉E�̋q��
	CCrowd *m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1F�̍��̋q��
	CCrowd *m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1F�̌��̋q��
	CCrowd *m_pLowerBack[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];

};

#endif
//----EOF----