//=============================================================================
//
// CSound�N���X [CSound.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSOUND_H_
#define _CSOUND_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	bool bLoop;			// ���[�v���邩�ǂ���
} PARAM;

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM000 = 0,					// OP
	SOUND_LABEL_SE_ENTER,					// ���艹
	SOUND_LABEL_SE_START,					// �X�^�[�g��
	SOUND_LABEL_SE_END,						// �I����
	SOUND_LABEL_SE_NORMAL_ATTACK,			// �ʏ�U����
	SOUND_LABEL_SE_NORMAL_THREAD,			// �ʏ펅�U����
	SOUND_LABEL_SE_THUNDERBOLT,				// �U���`�Ԃ̎��U���̏��e�̉�
	SOUND_LABEL_SE_THUNDERBOLT_2,			// �U���`�Ԃ̎��U���̓�e�̉�
	SOUND_LABEL_SE_AROUND_ATTACK,			// �U���`�Ԃ̎��͍U���̉�
	SOUND_LABEL_SE_CREATE_ROAD,				// �ړ��`�Ԃ̓��쐬��
	SOUND_LABEL_SE_TACKLE,					// �ړ��`�Ԃ̃^�b�N���̉�
	SOUND_LABEL_SE_TRAP_FOUNDATION,			// �W�Q�`�Ԃ�㩐ݒu��
	SOUND_LABEL_SE_TRAP_ATTACK,				// �W�Q�`�Ԃ̍U����
	SOUND_LABEL_SE_GET_TREASURE,			// �󕨂��E������
	SOUND_LABEL_SE_CURSOL_MOVE,				// �J�[�\���ړ�
	SOUND_LABEL_SE_CURSOL_SANSEN,			// �Q��
	SOUND_LABEL_SE_CURSOL_ZYUNBIKANRYOU,	// ��������
	SOUND_LABEL_ZINGLE_TITLE,				// �^�C�g���̃W���O��
	SOUND_LABEL_ZINGLE_RESULT,				// ���U���g�̃W���O��
	SOUND_LABEL_SE_GAGE_DOWN,				// �Q�[�W����
	SOUND_LABEL_SE_SPEED_DOWN,				// �X�s�[�h����
	SOUND_LABEL_SE_TRANSREATION,			// �ό`
	SOUND_LABEL_SE_DAMAGE,					// �_���[�W
	SOUND_LABEL_SE_DEST_TREASURE,			// �󕨂���������
	SOUND_LABEL_BGM_STAGE_SELECT,			// �X�e�[�W�Z���N�g�̂a�f�l
	SOUND_LABEL_SE_COUNTDOUN,				// �J�E���g�_�E��
	SOUND_LABEL_SE_GAME_START,				// �Q�[���X�^�[�g
	SOUND_LABEL_BGM_TUTORIAL,				// �`���[�g���A���̂̂a�f�l
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	// �R���X�g���N�^
	CSound();

	// �f�X�g���N�^
	~CSound();

	//==============================================================
	// ������
	// ����: �E�C���h�E�n���h��
	// �߂�l: ����
	//==============================================================
	HRESULT InitSound(HWND hWnd);

	// �I��
	void UninitSound(void);

	//==============================================================
	// ���Đ�
	// ����: �炵�������̃��x��
	// �߂�l: ����
	//==============================================================
	HRESULT PlaySound(SOUND_LABEL label);

	//==============================================================
	// �w�肵������~
	// ����: �炵�������̃��x��
	//==============================================================
	void StopSound(SOUND_LABEL label);

	// ���S�Ē�~
	void StopSound(void);

private:
	//=============================================================================
	// �`�����N�̃`�F�b�N�֐�
	// ����: �t�@�C���n���h���A�t�H�[�}�b�g�A(out)�`�����N�T�C�Y�A(out)�`�����N�f�[�^�|�W�V����
	// �߂�l: ����
	//=============================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

	//=============================================================================
	// �`�����N�f�[�^�̓ǂݍ���
	// ����: �t�@�C���n���h���A�o�b�t�@�[�|�C���^�[�A�o�b�t�@�[�T�C�Y�A�I�t�Z�b�g
	// �߂�l: ����
	//=============================================================================
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2				*m_pXAudio2;						// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice	*m_pMasteringVoice;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX];	// �I�[�f�B�I�f�[�^
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX];		// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif
//----EOF----