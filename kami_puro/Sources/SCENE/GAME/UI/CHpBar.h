//=============================================================================
//
// CHpBar�N���X [CHpBar.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CHPBAR_H_
#define _CHPBAR_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CHpBar : public CScene2D
{
public:
	// �o�[�̍��W���Œ肵�ē�������
	enum POSITIONBASE{
		// �E�����Œ�
		POSITIONBASE_RIGHT,
		// �������Œ�
		POSITIONBASE_LEFT,
	};

	CHpBar(LPDIRECT3DDEVICE9 *pDevice);
	~CHpBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// ���W,�o�[�̕ϓ����ɂǂ��瑤���Œ肷�邩
	static CHpBar *Create(
		D3DXVECTOR2 pos,
		float width,
		float height,
		float centerDist,
		POSITIONBASE positionBase,
		LPDIRECT3DDEVICE9 *pDevice);

	void Add(float value);

	// �l��0�ɂ��ăo�[������
	// ����̂QD�Ƃ����L�k�����Ȃ���E�E�E
	void Reset();

private:
	// ������
	void Init(D3DXVECTOR2 pos,
		float width,
		float height,
		float centerDist,
		POSITIONBASE positionBase);

	// �o�[�̒[�̍��W
	D3DXVECTOR2 m_Pos;
	// �Œ肵�����W
	D3DXVECTOR2 m_PosBase;
	// �o�[�̂ǂ�����Œ肵�ē�������
	POSITIONBASE m_PositionBase;
	// ���݂̃o�[�̒l
	float m_Value;
	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// �Ԃ��o�[�̒l�i�X�^�~�i�j
	float m_ValueRed;
	float m_WidthOneValue;
	
	// �������o�[
	CScene2D *m_HpBar;

	// �_���[�W���󂯂Ă���̃J�E���g�@�Ԃ��o�[�Ɏg��
	int m_RedResetCount;
	// �Ԃ��o�[��ύX���邩�t���O
	bool m_isRedReset;
};

#endif
//----EOF----