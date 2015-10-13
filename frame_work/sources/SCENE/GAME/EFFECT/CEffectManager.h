//=============================================================================
//
// CEffectManager�N���X [CEffectManager.h]
// Author : �˖{�@�r�F
//
// �󕨃}�l�[�W���[�@��������ĕ󕨂𐶐�����
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// �萔
//=============================================================================
enum EFFECT_TYPE{
	EFFECT_ATTACK_HIT = 0,
	EFFECT_EXPLOSION,				// ����
	EFFECT_SPECIAL_ATTACK_ATTACK,	// �U�������U����
	EFFECT_SPECIAL_ATTACK_SPEED,	// �ړ������U����
	EFFECT_SPECIAL_THREAD_ATTACK,	// �U���������U����
	EFFECT_CAUGHT_TRAP,				// 㩂ɂ��������G�t�F�N�g��
	EFFECT_CREATE_ROAD,				// ���쐬���̃G�t�F�N�g��
	EFFECT_FLAG_GET,				// �t���O�擾�G�t�F�N�g��
	EFFECT_FLAG_HOLD,				// �t���O�������G�t�F�N�g��
	EFFECT_PLAYER_DEAD,				// �v���C���[�̗͂O�G�t�F�N�g
	EFFECT_MP_ATTACK,				// MP�_���[�W�G�t�F�N�g��
	EFFECT_NORMAL_ATTACK_CAP,		// �X�q�U���G�t�F�N�g��
	EFFECT_NORMAL_ATTACK_THREAD,	// �ʏ펅���ˍU����
	EFFECT_PLAYER_REVIAVE,			// �v���C���[�����G�t�F�N�g
	EFFECT_TRAP_FOUNDATION,			// 㩐ݒu�G�t�F�N�g��
	EFFECT_METAMORPHORSE,			// �ό`�G�t�F�N�g

	EFFECT_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectManager
{
	// ���J�����o
public:
	CEffectManager(LPDIRECT3DDEVICE9 *pDevice);
	~CEffectManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �G�t�F�N�g�����֐�
	// ���W�ƃ^�C�v��ݒ�
	static void CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity);

	// �G�t�F�N�g�����֐�
	// ���W�ƃv���C���[�i���o�[��ݒ�
	static void CreateEffectMeta(D3DXVECTOR3 pos, short playerNum);

	// �G�t�F�N�g�����֐�(�)
	// ���W�ƃv���C���[�i���o�[���Z�b�g
	static void CreateEffectTrap(D3DXVECTOR3 pos, short playerNum);

	// ����J�����o
private:

	// �f�o�C�X
	static LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----