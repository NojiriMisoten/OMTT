//=============================================================================
//
// CJumpEffectBillbord�N���X [CJumpEffectBillbord.h]
// Author : �˖{�r�F
//
// ���Z���������
// 
//=============================================================================
#ifndef _CJUMPEFFECTBILLBORD_H_
#define _CJUMPEFFECTBILLBORD_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

class CCameraManager;
class CUiManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CJumpEffectBillbord : public CScene2D
{
public:

	CJumpEffectBillbord(LPDIRECT3DDEVICE9 *pDevice);
	~CJumpEffectBillbord(void);

	void Update();
	void Uninit();
	void DrawUIRender();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CJumpEffectBillbord *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		TEXTURE_TYPE texType, CCameraManager* pCamera, CUiManager* pUiManager, int playerNum);


private:
	// ������
	void Init(D3DXVECTOR3& pos,
		TEXTURE_TYPE texType);

	CCameraManager* m_pCameraManager;
	CUiManager* m_pUiManager;

	// ���߂̂��߂̐F���
	D3DXCOLOR m_Color;
	// ��Ԑ�����߂邽�߂̃v���C���ԍ�
	int m_PlayerNum;
	// ��Ԃ��߂̈��J�E���g
	int m_FlyCount;
	// ���ł�X�e�[�g�t���O
	bool m_isFly;
	// ��ԂƂ��̍��W��ԗp�^�C��0~1
	float m_FlyTime;
	// 3D���W�Ńv���C���̈ʒu���擾
	D3DXVECTOR3 m_Pos3D;
	// 2D���W�ŕ`��
	D3DXVECTOR3 m_Pos2D;
	// �򕔎��̕ۊǗp���W
	D3DXVECTOR3 m_Pos2DStart;
	D3DXVECTOR3 m_Pos2DEnd;
	// ������X�̈ړ��ʁ@���񂾂񌸐�������
	float m_Velo;
	// �����Q�[�W�Ȃ�HP�Q�[�W�Ȃ̃t���O
	bool m_isCrowd;
	// ���񂾂񏬂����Ȃ邽�߂̑傫��
	float m_Size;
};

#endif
//----EOF----