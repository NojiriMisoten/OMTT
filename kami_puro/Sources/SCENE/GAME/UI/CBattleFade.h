//=============================================================================
//
// CBattleFade�N���X [CBattleFade.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CBATTLEADE_H_
#define _CBATTLEADE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "CBattleFade2D.h"

// ���낢��ȃt�F�[�h
enum BattleFade
{
	BATTLE_FADE_LIGHT,
	BATTLE_FADE_MAX
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBattleFade
{
public:

	CBattleFade(LPDIRECT3DDEVICE9 *pDevice);
	~CBattleFade(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CBattleFade *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// �^�C�v���w�肵�ăt�F�[�h��������
	void Start(BattleFade type);

private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// ��ʓ��ɓ����Ă���X�V
	void In();
	// �\�����Ă���X�V
	void Wait();
	// ��ʊO�ɏo�Ă����X�V
	void Out();

	// �w�i
	CScene2D *m_pLightBack;
	D3DXCOLOR m_ColorBack;

	// ���̃t�F�[�h�ɕK�v��2D
	struct LightData{
		CBattleFade2D *m_pLightLine;
		CBattleFade2D *m_pLightBall;
		CBattleFade2D *m_pLightBallSmall;
		D3DXCOLOR m_ColorLine;
		D3DXCOLOR m_ColorBall;
		D3DXCOLOR m_ColorBallSmall;
		D3DXVECTOR3 m_Pos;

		// �QD�ɂ��ꂼ��̐F���Z�b�g����
		void SetColorPolygon()
		{
			m_pLightBall->SetColorPolygon(m_ColorBall);
			m_pLightLine->SetColorPolygon(m_ColorLine);
			m_pLightBallSmall->SetColorPolygon(m_ColorBallSmall);
			m_pLightBall->SetColorPolygon(m_ColorBall);
			m_pLightLine->SetColorPolygon(m_ColorLine);
			m_pLightBallSmall->SetColorPolygon(m_ColorBallSmall);
		}
		// ���ׂẴJ���[�̃A���t�@���w�肵�����l�ɂ���
		void SetAlpha(float a)
		{
			m_ColorBall.a = a;
			m_ColorLine.a = a;
			m_ColorBallSmall.a = a;
			m_ColorBall.a = a;
			m_ColorLine.a = a;
			m_ColorBallSmall.a = a;
			SetColorPolygon();
		}
		// ���ׂẴJ���[�̃A���t�@���w�肵�����l�ŉ��Z
		void AddAlpha(float a)
		{
			m_ColorBall.a += a;
			m_ColorLine.a += a;
			m_ColorBallSmall.a += a;
			m_ColorBall.a += a;
			m_ColorLine.a += a;
			m_ColorBallSmall.a += a;
			SetColorPolygon();
		}
		// ���ꂼ��̍��W�ɃZ�b�g
		void SetPosPolygon()
		{
			m_Pos.y += 8;
			m_pLightBall->SetPos(m_Pos);
			m_Pos.y -= 8;
			m_pLightBallSmall->SetPos(m_Pos);
		}
	};

	LightData m_LightA;
	LightData m_LightB;

	// �X�V�𕪂��邽�߂̃t���O
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// �ړ�����ׂ̕ۊǂ��邽�߂ɕK�v�ȃ^�C��
	float m_Time;
	// �J�E���g
	int m_Count;
	// In��Out�̊Ԃ̑҂��ԁ@Start�Ŏw�肷��
	int m_WaitInterval;
	// �J�b�g�C���̎�ނ�ۑ����Ă����i�t�F�[�h�A�E�g�p�j
	BattleFade m_BattleFadeType;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----