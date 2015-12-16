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
	SOUND_LABEL_BGM000 = 0,		// OP
	SOUND_LABEL_BGM001,			// GAME
	SOUND_LABEL_SE_CHAIN,		// ������
	SOUND_LABEL_SE_START_GONG,	// �S���O
	SOUND_LABEL_SE_FINISH_GONG,	// �S���O
	SOUND_LABEL_SE_THROW1,		// �����P
	SOUND_LABEL_SE_THROW2,		// �����Q
	SOUND_LABEL_SE_THROWFINISH,	// �����t�B�j�b�V��
	SOUND_LABEL_SE_SLAP01,
	SOUND_LABEL_SE_SLAP02,
	SOUND_LABEL_SE_SLAP03,
	SOUND_LABEL_SE_BATTLE_FADE,	// �o�g���t�F�[�h
	SOUND_LABEL_SE_CROWD01,		// ����
	SOUND_LABEL_SE_CROWD02,		// ����
	SOUND_LABEL_SE_CROWD03,		// ����
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

	//==============================================================
	// �w�肵�����̑傫���ύX
	// ����: �炵�������̃��x��, �傫��(0�`�P�F�����`�ʏ�)
	//==============================================================
	void SetVolume(SOUND_LABEL label, float volume);
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