//=============================================================================
//
// CAnimation�N���X [CAnimation.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CANIMATION_H_
#define _CANIMATION_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �A�j���[�V����
typedef enum
{
	ATTACK_SMALL_CHOP = 0,
	ATTACK_SMALL_ELBOW,
	ATTACK_SMALL_LARIAT,
	ATTACK_BIG_ROLLING,
	ATTACK_BIG_SHOLDER,
	ATTACK_BIG_DROPKICK,
	ATTACK_THROW_SLAP,
	ATTACK_THROW_BACKDROP,
	ATTACK_THROW_STUNNER,
	ATTACK_FINISH_BODYPRESS,
	ATTACK_FINSH_ATOMICBUSTER
}ATTACK_LIST;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAnimation
{
public:
	// �R���X�g���N�^
	CAnimation( void );

	// �f�X�g���N�^
	~CAnimation( void );

	// ������
	void Init( void );

	// �I��
	void Uninit( void );

	// �X�V
	void Update( void );

private:
	//void AnimCafera

};

#endif
//----EOF----