//=============================================================================
//
// CAnimationReader�N���X [CAnimationReader.cpp]
// Author : �r���@���
//
//=============================================================================
#ifndef _CANIMATIONREADER_H_
#define _CANIMATIONREADER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include <stdio.h>

//*****************************************************************************
// �O���錾
//*****************************************************************************


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAnimationReader
{
public:
	typedef struct
	{
		int				id;					// �A�j���[�V����ID
		int				dur;				// ���t���[����

	}ANIMATION_DATA;

	typedef struct
	{
		int				duration;			// ���t���[����
		D3DXVECTOR3		endP;				// �I�����_
		D3DXVECTOR3		endR;				// �I�������_
		bool			isShake;			// �V�F�C�N������
		float			amplitude;			// �V�F�C�N�U��
		int				delay;				// �V�F�C�N�J�n�x���t���[����
		int				time;				// �V�F�C�N�p���t���[��
	}CAMERA_CP_DATA;

	typedef struct
	{
		char			*file;				// �t�@�C����
		D3DXVECTOR3		pos;				// ���W
		D3DXVECTOR3		ang;				// �p�x
		D3DXVECTOR3		scl;				// �k��
		int				startFrame;			// �J�n�t���[��
		int				endFrame;			// �I���t���[��
	}EFFECT_DATA;
	
	// �R���X�g���N�^
	CAnimationReader( void );

	// �f�X�g���N�^
	~CAnimationReader( void );

	// ������
	LRESULT Init( char *pFile );

	// �I��
	void Uninit( void );

	// �ǂݎ��
	void Read( void );

private:
	FILE				*m_pFile;				// �ǂݍ��ރt�@�C��
	ANIMATION_DATA		m_Animation;			// �A�j���[�V����
	CAMERA_CP_DATA		m_ControlPoint[10];		// �J�����R���g���[���|�C���g
	EFFECT_DATA			m_Effect[10];			// �G�t�F�N�g

	void ReadAnimation( void );
	void ReadCamera( void );
	void ReadControl( int id );
	void ReadEffect( int id );

};

#endif

//----EOF----