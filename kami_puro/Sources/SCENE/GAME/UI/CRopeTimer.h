//=============================================================================
//
// CRopeTimer�N���X [CRopeTimer.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CROPETIMER_H_
#define _CROPETIMER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRopeTimer
{
public:

	CRopeTimer(LPDIRECT3DDEVICE9 *pDevice);
	~CRopeTimer(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CRopeTimer *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// ���[�v�^�C�}�[���N������֐�
	// ��P���� �o�[���g�k�A�j���[�V�����ŕ\������܂ł̃t���[����
	// ��Q���� �o�[�����炵�n�߂Ă���0�ɂȂ�܂ł̃t���[����
	void Start(int appearFrame, int reduceFrame);

private:
	// ������
	void Init();
	// �o�[�𓮂����X�V
	void UpdateBarMove();
	// �o�[���o������A�j���[�V�����X�V
	void UpdateAppearAnime();

	// �o���A�j���[�V�����p
	struct AnimeData
	{
		float width;
		float height;
		AnimeData(){};
		AnimeData(float w, float h)
		{
			width = w;
			height = h;
		}
	};

	// �o�[�̏o���A�j���[�V����
	bool m_isAppearAnime;
	// �o�[�𓮂����X�V
	bool m_isMoveBar;
	// �o�[
	CScene2D *m_pBar;
	// �g
	CScene2D *m_pFrame;
	// �[�����̍��W
	float m_PosLeft;
	float m_PosRight;
	// �A�j���[�V�����p���W �o�[
	AnimeData m_AnimeBarSize;
	AnimeData m_AnimeBarSizeDest;
	// �A�j���[�V�����p���W
	AnimeData m_AnimeFrameSize;
	AnimeData m_AnimeFrameSizeDest;
	// ��ԗp�̃^�C��
	float m_Time;
	// �J�E���g
	int m_Count;
	int m_CountMax;
	// 1�t���[��������Ɍ��炷��
	float m_WidthOneFrame;
	// 1�t���[��������ɑ��₷�^�C��
	float m_TimeOneFrame;

//	//-------------------------------------
//	// �A�j���[�V�����X�V
//	void UpdateAnime();
//	// �J�n�A�j�������邽�߂̃J�E���g
//	int m_AnimeCount;
//	// �J�n�A�j���̏I���@�t���[����
//	int m_AnimeCountMax;
//	// �J�n�A�j�����Ă���t���O
//	bool m_isAnime;
//	// �J�n�A�j���[�V�����p�̕ۊǃ^�C��
//	float m_AnimeEasingOneFrame;
//	float m_AnimeEasingTimer;
//	// �J�n�A�j��1�t���[���ŕύX����A���t�@�l
//	float m_AnimeOneFrameAlpha;
//	D3DXCOLOR m_Anime2DColor;
	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----