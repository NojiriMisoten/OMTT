//=============================================================================
//
// �G�t�F�N�g�{�̂̊i�[���� [CEffectHolder.h]
// Author : ��{ �F��
//
//=============================================================================
#ifndef _CEFFECT_HOLDER_H_
#define _CEFFECT_HOLDER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CEffectManager.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �e�N�X�`���̎��
typedef enum
{
	//����(��)
	EFFECT_FOOTSTEP_SMOKE = 0,
	//����(�g)
	EFFECT_FOOTSTEP_WAVE,
	//�I�[��(�J�n)
	EFFECT_AURA_START,
	//�I�[��(���[�v)
	EFFECT_AURA_LOOP,
	//�I�[��(�I��)
	EFFECT_AURA_END,
	//�o�E���h��
	EFFECT_BOUND,
	//��U��
	EFFECT_DAGEKI_JAKU,
	//���U��
	EFFECT_DAGEKI_TYU,
	//���U��
	EFFECT_DAGEKI_KYO,
	//�h�䎞
	EFFECT_DEFENSE,
	//��(���ꎞ)
	EFFECT_ENTER_SMOKE,
	//�W�����v��
	EFFECT_JUMP,
	//�����O�o�V�R�[��
	EFFECT_RING,
	//�e�X�g�p,�D���Ɏg����!
	EFFECT_TEST1,
	//�Ռ��g
	EFFECT_SHOCK_WAVE,
	//��
	EFFECT_SMOKE,
	//�e�X�g����2
	EFFECT_TEST2,
	//����
	EFFECT_THROW,
	//���������̂��
	EFFECT_WIN,
	EFFECT_MAX					// �G�t�F�N�g����
}EFFECT_TYPE;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CEffectHolder
{
public:
	// �R���X�g���N�^
	CEffectHolder(void){};

	// �f�X�g���N�^
	~CEffectHolder(void){};

	// �I��
	static void Uninit(void);

	//============================================================
	// �N���G�C�g
	// ����:�f�o�C�X
	// ���K���G�t�F�N�g�}�l�[�W���[�𐶐�������ɌĂԂ���!
	//============================================================
	static void CreateEffect(void);

	//============================================================
	// �e�N�X�`���Q�b�^�[
	// ����:�e�N�X�`���̎��
	// �߂�l:�e�N�X�`���̃|�C���^
	//============================================================
	static ::Effekseer::Effect* GetEffect(const EFFECT_TYPE type);

private:
	//============================================================
	// ������
	// ����:�f�o�C�X
	// �߂�l:����
	//============================================================
	static	HRESULT Init(void);

	//�G�t�F�N�g�{�̂ւ̃|�C���^
	static	::Effekseer::Effect* m_pEffect[EFFECT_MAX];
	// ���łɍ�������̃t���O
	static	bool m_bDoneCreateFlag;
};

#endif
//----EOF----