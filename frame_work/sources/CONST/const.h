//=============================================================================
//
// �萔�t�@�C�� [const.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CONST_H_
#define _CONST_H_
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"

#define SAFE_RELEASE(p)			if((p)!=NULL){(p)->Release();(p)=NULL;}
#define SAFE_DELETE(p)			if((p)!=NULL){delete (p);(p)=NULL;}
#define SAFE_DELETE_ARRAY(p)	if((p)!=NULL){delete[] (p);(p)=NULL;}

// MODEL�t�H���_�ւ̑��΃p�X
static const char MODEL_FOLDER_PATH[] = "data/MODEL/";

// TEXTURE�t�H���_�ւ̑��΃p�X
static const char TEX_FOLDER_PATH[] = "data/TEXTURE/";

static const int MAX_LENGTH_FILE_PATH = 255;

// �f�t�H���g�̃|���S���J���[
static const D3DXCOLOR DEFAULT_POLYGON_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

// �A���t�@�ő�l
static const float MAX_ALPHA = 1.0f;

// �A���t�@�ŏ��l
static const float MIN_ALPHA = 0.f;

// �v���C�I���e�B�̏����l
static const int DEFAULT_PRIORITY = 0;

// �O�p�`�쐬�|���S����
static const int NUM_POLYGON_CREATE_TRIANGLE = 3;

// �f�t�H���g�̃A�b�v�x�N�g��
static const D3DXVECTOR3 DEFAULT_UP_VECTOR(0.f, 1.f, 0.f);

// �f�t�H���g�̃t�����g�x�N�g��
static const D3DXVECTOR3 DEFAULT_FRONT_VECTOR(0.f, 0.f, 1.f);

// �f�t�H���g�̃��C�g�x�N�g��
static const D3DXVECTOR3 DEFAULT_RIGHT_VECTOR(1.f, 0.f, 0.f);

// �e�N�X�`���̍ő�UV�l
static const float MAX_UV_VALUE = 1.0f;

// �e�N�X�`���̍ŏ�UV�l
static const float MIN_UV_VALUE = 0.0f;

// �l�p�`�`�掞�̃|���S����
static const int DRAW_SQUARE_PRINITIV_NUM = 2;

#endif
//----EOF----