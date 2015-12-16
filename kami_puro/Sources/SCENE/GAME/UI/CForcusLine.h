//=============================================================================
//
// CForcusLine�N���X [CForcusLine.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _FORCUS_LINE_H_
#define _FORCUS_LINE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CForcusLine
{
public:
	CForcusLine(LPDIRECT3DDEVICE9 *pDevice);
	~CForcusLine(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CForcusLine *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// �W�������o��
	// ��1�����F�\������t���[����
	// ��2�����Ftrue�Ȃ�h��ȃe�N�X�`���ɂȂ� false�Ȃ甒��
	void Start(int interval, bool isFinish);

private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// ��ʓ��ɓ����Ă���X�V
	void In();
	// �\�����Ă���X�V
	void Wait();
	// ��ʊO�ɏo�Ă����X�V
	void Out();

	CScene2D *m_pPolygon;
	CScene2D *m_pBack;
	// �J�b�g�C��
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// �ړ�����Ƃ��ɕK�v�Ȓl
	float m_Pos;
	// �J�b�g�C����\�����鎞��(�t���[��)
	int m_IntervalMax;
	int m_IntervalCount;
	// �J�b�g�A�E�g����Ƃ��ɐF��ς��鎞�p
	D3DXCOLOR m_Color;
	D3DXCOLOR m_ColorBack;
	// �X�N���[���̃e�N�X�`��
	UV_INDEX m_BackUV;
	// �e�N�X�`����ύX����J�E���g
	int m_TextureCount;
	
	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----