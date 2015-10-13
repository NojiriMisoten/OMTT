//=============================================================================
//
// CSceneScore�N���X [CSceneScore.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENESCORE_H_
#define _CSCENESCORE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int MAX_SCORE_NUM = 3;		// ����
static const int MAX_SCORE = 999;	// �ő�l

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneScore : public CScene2D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X
	//======================================================
	CSceneScore(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_UI);

	// �f�X�g���N�^
	~CSceneScore();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v
	//======================================================
	void Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);
	
	//======================================================
	// ���Z�֐�
	// ����: ������
	//======================================================
	static void Add(int addNum);

	//======================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���^�C�v�A����
	// �߂�l: �������̃A�h���X
	//======================================================
	static CSceneScore* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum);

private:
	// �g���ĂȂ��������֐�
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock){return E_FAIL;};
	
	// ���_���f�֐�
	static void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	static void SetColorPolygon(D3DXCOLOR color);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;				// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	static float		m_fWidth;					// ��
	static float		m_fHeight;					// ����
	static int			m_nScore;					// �X�R�A
	static CNumber		*m_Number[MAX_SCORE_NUM];	// ����(������)
};
#endif
//----EOF-----