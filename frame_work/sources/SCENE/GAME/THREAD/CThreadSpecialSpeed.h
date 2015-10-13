//=============================================================================
//
// CThreadSpecialAttack�N���X [CThreadSpecialAttack.h]
// Author : �����@�Ȉ�
//
// �U�������`�Ԃ̎�
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_SPEED_H_
#define _CTHREAD_SPECIAL_SPEED_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"
#include "../PLAYER/CPlayer.h"

// �����L�яk�݂���ő厞��
static const int	GROW_THREAD_TIME = 60;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadSpecialSpeed : public CThreadBase
{
	// ���J�����o
public:
	
	CThreadSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_2D);
	~CThreadSpecialSpeed(void);

	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �v���C���ɓ����������ɌĂ΂��֐�
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CThreadSpecialSpeed *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		DIRECTION_PLAYER_FACING playerDirection);

	// �f�X�֐�
	void Death(void);

	// ���C�h�֐�
	void Ride(bool ride);
private:
	HRESULT Init(short nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerDirection);

	// ���L�т鏈��
	void GrowThread(void);

	// �t�B�[���h�A�j���[�V����
	void FieldAnim(void);

	// �㉺
	void UpDown(void);

	DIRECTION_PLAYER_FACING m_Direction;	// ����
	float					m_fAddPower;	// �L�т�傫��
	int						m_nAnimTimer;	// �A�j���[�V�����̃J�E���^�[
	float					m_fDefaultWidth;
	float					m_fDestWidth;
	float					m_fDefaultHeight;
	float					m_fDestHeight;
	bool					m_bSetNavi;
};

#endif
//----EOF----