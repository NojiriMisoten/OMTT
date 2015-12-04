//=============================================================================
//
// �V�F�[�_�[���� [CShader.h]
// Author : NOJIRI NAOKI
//
//=============================================================================
#ifndef _CSHADER_H_
#define _CSHADER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

// �V�F�[�_�[�̎��
typedef enum
{
	VS_TYPE_DRAW_2D = 0,
	VS_TYPE_NORMAL_RENDERER,
	VS_TYPE_NORMAL_VEC_RENDERER,
	VS_TYPE_TOON_OBJECT_DEPTH,
	VS_TYPE_NORMAL_RENDERER_SKINMESH,
	VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH,
	VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH,
	VS_TYPE_FIELD,
	VS_TYPE_MODEL,
	VS_TYPE_POINT,
	VS_TYPE_TEX,
	VS_TYPE_MAX
}SHADER_TYPE;

typedef enum
{
	PS_TYPE_BACK_BUFFER = 0,
	PS_TYPE_NORMAL_RENDERER,
	PS_TYPE_NORMAL_VEC_RENDERER,
	PS_TYPE_TOON,
	PS_TYPE_TOON_OBJECT_DEPTH,
	PS_TYPE_FIELD,
	PS_TYPE_GAUSS_SIDE,
	PS_TYPE_GAUSS_VERTICAL,
	PS_TYPE_HIGH_LUMINANCE,
	PS_TYPE_LIGHT_BLOOM,
	PS_TYPE_MODEL,
	PS_TYPE_POINT,
	PS_TYPE_TEX,
	PS_TYPE_FADE_WHITE,
	PS_TYPE_MAX
}PS_TYPE;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CShader
{
public:
	// �R���X�g���N�^
	CShader(void){};

	// �f�X�g���N�^
	~CShader(void){};

	// �N���G�C�g
	static void Create(LPDIRECT3DDEVICE9 *pDevice);

	// �I��
	static void Uninit(void);
	
	static LPDIRECT3DPIXELSHADER9* GetPS(PS_TYPE type){ return &m_ps[type]; };
	static LPD3DXCONSTANTTABLE* GetPSC(PS_TYPE type){ return &m_psc[type]; };

	static LPDIRECT3DVERTEXSHADER9* GetVS(SHADER_TYPE type){ return &m_vs[type]; };
	static LPD3DXCONSTANTTABLE* GetVSC(SHADER_TYPE type){ return &m_vsc[type]; };

private:
	//===================================================
	// ������
	// ����: �f�o�C�X
	// �߂�l: ����
	//===================================================
	static HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	// -----------�V�F�[�_�[�p------------------
	static LPDIRECT3DPIXELSHADER9	m_ps[PS_TYPE_MAX];
	static LPD3DXCONSTANTTABLE		m_psc[PS_TYPE_MAX];		// �萔���W�X�^�̃e�[�u��
	static LPDIRECT3DVERTEXSHADER9	m_vs[VS_TYPE_MAX];
	static LPD3DXCONSTANTTABLE		m_vsc[VS_TYPE_MAX];		// �萔���W�X�^�̃e�[�u��
	//------------------------------------------
};

#endif
//----EOF----