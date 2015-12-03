//=============================================================================
//
// COverLay�N���X [COverLay.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _COVERLAY_H_
#define _COVERLAY_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class COverLay
{
public:
	// �I�[�o�[���C��Start����̂ɕK�v�ȃf�[�^
	struct Data{
		TEXTURE_TYPE texture;
		float fadeInSpeed;
		int interval;
		float fadeOutSpeed;

		Data() {};
		Data(
			TEXTURE_TYPE tex,
			float fadeInSpeed,
			int interval,
			float fadeOutSpeed)
		{
			this->texture = tex;
			this->fadeInSpeed = fadeInSpeed;
			this->interval = interval;
			this->fadeOutSpeed = fadeOutSpeed;
		}
	};

	COverLay(LPDIRECT3DDEVICE9 *pDevice);
	~COverLay(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �f�o�C�X
	static COverLay *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// ��P���� �e�N�X�`��
	// ��Q���� �t�F�[�h�C���̑���
	// ��R���� �t�F�[�h�C�����Ă���\�����Ă��������i�t���[���j
	// ��S���� �t�F�[�h�A�E�g�̑���
	void Start(Data *data);

private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// ��ʓ��ɓ����Ă���X�V
	void In();
	// �\�����Ă���X�V
	void Wait();
	// ��ʊO�ɏo�Ă����X�V
	void Out();
	
	// �|���S��
	CScene2D *m_p2D;
	// �J���[
	D3DXCOLOR m_Color;

	// �J�b�g�C��
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;

	// �t�F�[�h�C���̑���
	float m_FadeInSpeed;
	// �t�F�[�h�A�E�g�̑���
	float m_FadeOutSpeed;
	// �҂�����
	int m_IntervalMax;
	// �҂����ԃJ�E���g
	int m_IntervalCount;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----