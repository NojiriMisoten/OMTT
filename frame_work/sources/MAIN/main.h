//=============================================================================
//
// ���C������ [main.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// ���������[�N�`�F�b�N�p
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)    // �x���΍��p
#include "dinput.h"
#include "xaudio2.h"
#ifdef _DEBUG
#include "../DEBUG_PROC/CDebugProc.h"
#endif

// ���������[�N�`�F�b�N�p
#ifdef _DEBUG
#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   calloc(c, s)          _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   realloc(p, s)         _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _recalloc(p, c, s)    _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define   _expand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 2D�p
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 3D�p
static const int SCREEN_WIDTH = 1280;												// �E�C���h�E�̕�
static const int SCREEN_HEIGHT = 720;												// �E�C���h�E�̍���

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;	// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

// ��L3�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	diffuse;
	D3DXVECTOR2	tex;
} VERTEX_3D;

bool GetDrawFlag(void);
#endif
//----EOF----