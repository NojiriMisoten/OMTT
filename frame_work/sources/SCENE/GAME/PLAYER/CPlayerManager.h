//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.h]
// Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CPLAYERMANAGER_H__
#define __CPLAYERMANAGER_H__

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CPlayer;
class CAttackManager;
class CThreadManager;
class CEffectManager;

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
// �v���C���[�̍ő吔
static const short MAXIMUM_NUMBER_OF_PLAYER = 4;

//-----------------------------------------------------------------------------
// �v���C���[�}�l�[�W���N���X��`
//-----------------------------------------------------------------------------
class CPlayerManager
{
public:
	// �R���X�g���N�^
	CPlayerManager(
		CAttackManager *pAttackManager, 
		CThreadManager *pThreadManager,
		CEffectManager *pEffectManager);

	// �f�X�g���N�^
	~CPlayerManager();

	// ������
	// CPU�l���A�v���C���[�l��
	void Init(int nNumPlayer, int nManualPlayer, bool *bPlayerControl);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �v���C���[�|�C���^�̃Q�b�^�[
	//	�����@�@�Ăяo���v���C���[���Z�b�g���Ă���z��ԍ�
	//	�߂�l�@�v���C���[�̃|�C���^
	static CPlayer* GetPlayer(int nNumber){ return m_apPlayer[nNumber]; }

	// �v���C���[�|�C���^�̃Z�b�^�[
	//	�����@�@�v���C���[�|�C���^�A�Z�b�g�����̔z��ԍ�
	void SetPlayer(CPlayer* pPlayer, int nNumber){ m_apPlayer[nNumber] = pPlayer; }

private:
	// �v���C���[�̃|�C���^�i�[�p�z��
	static CPlayer* m_apPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// �e�X�g�p�̃^�C�}�[
	int	m_nTimer;

	// �U�����ɕK�v�ȃ}�l�[�W���[
	CAttackManager *m_pAttackManager;
	// ���o�����ɕK�v�ȃ}�l�[�W���[
	CThreadManager *m_pThreadManager;
	// �G�t�F�N�g�ɕK�v�ȃ}�l�[�W���[
	CEffectManager *m_pEffectManager;

};

#endif //__CPLAYERMANAGER_H__

// EOF