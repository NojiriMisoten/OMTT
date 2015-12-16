//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
//
// �y�T�v�z
// ��o�[�𐶐����āA�|���S���̒��_�����E�Ɉړ������Ă���
// 
// �y�g�����z
// Add�Ɂ{�|�F�X�l������Ɗϋq�o�[�����E�Ɉړ�����
// 
// �y�ύX�ł��鐔�l�z
// �E�e�N�X�`��
// �E�o�[�̑��x�i�������x�A���̉����x�j
// �E�o�[�̐F�i�E��1P���A����2P���j
// 
//=============================================================================
#ifndef _CCROWNDBAR_H_
#define _CCROWNDBAR_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

// TODO ���̗�
static const float CROWD_MAX = 500;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCrowdBar
{
public:

	CCrowdBar(LPDIRECT3DDEVICE9 *pDevice);
	~CCrowdBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CCrowdBar *Create(
		float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	// �o�[�𑝌�������
	void Add( int value );

	void SetAmount( float value ) { m_Value = value; };

	float GetAmount( void ) { return m_Value; };

	// �J�n�A�j���[�V����������@������
	// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
	void StartAnimation(int endCount);

	void SetImvisible(void);
	void SetVisible(void);
private:
	// ������
	void Init(float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	void Init();

	// �o�[�𓮂����X�V
	void UpdateBarMove();

	//-------------------------------------
	// ��̃o�[
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;
	// ���[�ƉE�[�̍��W�i�����Ȃ��j
	float m_PosLeftX;
	float m_PosRightX;
	// �^�񒆂̍��W�i�����Ȃ��j
	float m_PosCenterX;
	// �Z�b�g������W��Dest
	float m_PosDestX;
	// ���݂̋��ڂ̍��W
	float m_PosCurrentX;
	// �Z�b�g������W�̈ړ���
	float m_PosVel;
	// �Z�b�g������W������Ƃ��t���O
	bool m_isPosMove;
	// ���݂̃o�[�̒l
	float m_Value;
	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// �o�[�̒l�im_Value�j�P������̃s�N�Z����
	float m_WidthOneValue;

	//-------------------------------------
	// �g
	CScene2D *m_pFrame;
	// �ϋq�����̊G
	CScene2D *m_pCrowd;
	UV_INDEX m_CrowdUV;
	// �ϋq�����̃A�j���[�V����
	void UpdateCrowdAnimation();
	// �e�N�X�`����ύX����J�E���g
	int m_CrowdAnimeCount;
	// �t�u��ύX����i�E�ɂ���ă��[�v����j
	void CrowdChangeUV();

	//-------------------------------------
	// �o�`�o�`
	CScene2D *m_pSpark;
	CScene2D *m_pSpark2;
	// ���݂̊p�x
	float m_SparkRot;
	// �p�x��ύX���邽�߂̃J�E���g
	int m_SparkCountRot;
	// �A�j���[�V�����p�t���O
	bool m_isSparkAdd;
	// ���W
	D3DXVECTOR3 m_SparkPos;
	// �A�j���[�V�����X�V
	void UpdateSparkAnimation();
	// �e�N�X�`�����W
	UV_INDEX m_SparkUV;
	// �e�N�X�`�����W��ύX���邽�߂̃J�E���g
	int m_SparkCountUV;

	//-------------------------------------
	// ����I�ɐk����n
	// ��ԗp�̖ړI���W
	float m_ShakePosStart;
	float m_ShakePosEnd;
	// �k������W�����肷��J�E���g
	int m_ShakeCount;
	// �k������W�̕�ԗp�̃^�C�}
	float m_ShakeTime;

	//-------------------------------------
	// �A�j���[�V�����X�V
	void UpdateAnime();
	// �J�n�A�j�������邽�߂̃J�E���g
	int m_AnimeCount;
	// �J�n�A�j���̏I���@�t���[����
	int m_AnimeCountMax;
	// �J�n�A�j�����Ă���t���O
	bool m_isAnime;
	// �J�n�A�j���[�V�����p�̕ۊǃ^�C��
	float m_AnimeEasingOneFrame;
	float m_AnimeEasingTimer;
	// �J�n�A�j��1�t���[���ŕύX����A���t�@�l
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;
	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----