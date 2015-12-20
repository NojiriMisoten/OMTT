//=============================================================================
//
// CCutIn�N���X [CCutIn.h]
// Author : �˖{�r�F
//
// Start�֐��ŃJ�b�g�C�����n�܂�B
// �ǂ����́g�v���C���h���A�ǂ́g�J�b�g�C���h�����w�肵�Ă�
//
// �J�b�g�C���̎�ނ̓w�b�_�[�ő��₵��cpp�Œ��g������
//
//=============================================================================
#ifndef _CCUTIN_H_
#define _CCUTIN_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

// ���낢��ȃJ�b�g�C�����`����
enum CutInType
{
	CUT_IN_JIJII,
	CUT_IN_MAX
};

class CCutIn2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCutIn
{
public:

	CCutIn(LPDIRECT3DDEVICE9 *pDevice);
	~CCutIn(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CCutIn *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// �J�b�g�C����������
	// �v���C���[ID�A�J�b�g�C���̃^�C�v
	void Start(int ID, CutInType type);

	void SetPos(D3DXVECTOR3& pos);
	void SetHeight(float height);
private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// ��ʓ��ɓ����Ă���X�V
	void In();
	// �\�����Ă���X�V
	void Wait();
	// ��ʊO�ɏo�Ă����X�V
	void Out();

	CCutIn2D *m_pCutInPolygon;
	CCutIn2D *m_pCutInPolygonBack;
	// �J�b�g�C��
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// �ړ�����Ƃ��ɕK�v�Ȓl
	float m_Pos;
	float m_PosDest;
	// �ړ�����ׂ̕ۊǂ��邽�߂ɕK�v�ȃ^�C��
	float m_Time;
	// �J�b�g�C����\�����鎞��(�t���[��)
	int m_IntervalMax;
	int m_IntervalCount;
	// �J�b�g�A�E�g����Ƃ��ɐF��ς��鎞�p
	D3DXCOLOR m_Color;
	// �J�b�g�C���̎�ނ�ۑ����Ă����i�t�F�[�h�A�E�g�p�j
	CutInType m_CutInType;
	// �X�N���[���̃e�N�X�`��
	UV_INDEX m_BackUV;
	// �v���C��ID��ۑ����Ă���
	int m_ID;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----