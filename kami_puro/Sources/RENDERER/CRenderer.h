//=============================================================================
//
// CRenderer�N���X [CRenderer.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CRENDERER_H_
#define _CRENDERER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

class CScene2D;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:
	// �����_�[�^�[�Q�b�g�̎��
	typedef enum
	{
		TYPE_RENDER_TOON_OBJECT_DEPTH = 0,	// �g�D�[��������I�u�W�F�N�g�̐[�x�l����
		TYPE_RENDER_NORMAL,				// �����Ƃ���
		TYPE_RENDER_NORMAL_VEC,			// �@������
		TYPE_RENDER_SHADOW,				// �e����
		TYPE_RENDER_BASE_HIGH_LUMINANCE,// �P�x���o�����������
		TYPE_RENDER_UI,					// UI����
		TYPE_RENDER_FADE,

		// �ȉ��̓��X�g�o�^�����I�u�W�F�N�g�͏����Ȃ�����
		TYPE_RENDER_TOON,			// �g�D�[������
		TYPE_RENDER_HALF_SIDE,		// 1/2�T�C�Y
		TYPE_RENDER_HALF_GAUS,		// 1/2�T�C�Y�K�E�X
		TYPE_RENDER_QUARTER_SIDE,	// 1/4�T�C�Y
		TYPE_RENDER_QUARTER_GAUS,	// 1/4�T�C�Y�K�E�X
		TYPE_RENDER_HIGH_LUMINANCE,	// ���P�x���o
		TYPE_RENDER_LIGHT_BLOOM,	// ���C�g�u���[��
		TYPE_RENDER_MAX
	}TYPE_RENDER_TEX;

	// �R���X�g���N�^
	CRenderer();

	// �f�X�g���N�^
	~CRenderer();

	//================================================
	// ������
	// ����: �E�C���h�E�n���h���A�E�C���h�E���[�h�ŕ\�����邩
	// �߂�l: ����
	//================================================
	HRESULT Init(HWND hWnd, BOOL bWindow);

	// �I��
	void Uninit(void);

	// �X�V
	void Update();

	//================================================
	// �w��v���C�I���e�B�̂ݍX�V
	// ����: �v���C�I���e�B
	//================================================
	void Update(int priority);
	
	// �`��
	void Draw();

	//================================================
	// �f�o�C�X�Q�b�^�[
	// �߂�l: �f�o�C�X�̃A�h���X
	//================================================
	static LPDIRECT3DDEVICE9 *GetDevice(void){return &m_pD3DDevice;};

	//================================================
	// �����_�[�^�[�Q�b�g�ύX
	// ����: �����_�[�^�[�Q�b�g�̎��
	//================================================
	static void ChangeRenderTarget(TYPE_RENDER_TEX idx);

	//================================================
	// ���̃����_�[�^�[�Q�b�g�ɕύX
	//================================================
	static void ChangeDefaultRenderTarget(void);

	//================================================
	// �����_�[�e�N�X�`���[�Q�b�g
	// ����: �����_�[�^�[�Q�b�g�̎��
	// �߂�l: �e�N�X�`���̃|�C���^
	//================================================
	static LPDIRECT3DTEXTURE9* GetRenderTexture(TYPE_RENDER_TEX idx);

	//================================================
	// �����_�[�^�[�Q�b�g�̃N���A
	// ����: �����_�[�^�[�Q�b�g�̎��
	//================================================
	static void ClearRenderTarget(TYPE_RENDER_TEX idx);

	static void TeachCameraRot(float rot){ m_fCameraRot = rot; };
	static void TeachProjMtx(D3DXMATRIX& proj){ m_ProjMtx = proj; };
	static void TeachViewMtx(D3DXMATRIX& view){ m_ViewMtx = view; };
	#ifdef _DEBUG
	//================================================
	// FPS�Z�b�g
	// ����: FPS
	//================================================
	void		SetFPS(int FPS){m_nCountFPS = FPS;};
	#endif
private:
	// �o�b�N�o�b�t�@�[�ɏ���
	void DrawBackBuffer(void);

	// �g�D�[���ɂ���
	void DrawToon(void);

	// ���C�g�e�N�X�`����������
	void DrawMixLight(void);

	// �K�E�X����
	void DrawGauss(TYPE_RENDER_TEX orijinTex, TYPE_RENDER_TEX sideRender, TYPE_RENDER_TEX gausRender);

	// ���P�x���o
	void DrawBrightness(void);

	// ���C�g�u���[��
	void DrawLightBloom(void);

	LPDIRECT3D9					m_pD3D;									// Direct3D�I�u�W�F�N�g
	static LPDIRECT3DDEVICE9	m_pD3DDevice;							// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	static LPDIRECT3DTEXTURE9	m_pRenderTexture[TYPE_RENDER_MAX];		// �����_�[�^�[�Q�b�g�p�̃e�N�X�`��
	static LPDIRECT3DSURFACE9	m_pRenderSurface[TYPE_RENDER_MAX];		// �����_�[�^�[�Q�b�g�p�̃T�[�t�F�X
	static LPDIRECT3DSURFACE9	m_pRenderSurfaceDepth[TYPE_RENDER_MAX];	// �����_�[�^�[�Q�b�g�p�̃T�[�t�F�X
	static LPDIRECT3DSURFACE9	m_pDefaultSurface;						// ���̃T�[�t�F�X
	static LPDIRECT3DSURFACE9	m_pDefaultSurfaceDepth;					// ���̃T�[�t�F�X
	static LPDIRECT3DSURFACE9	m_pKeepSurface;							// ����ւ��p�̃T�[�t�F�X
	static LPDIRECT3DSURFACE9	m_pKeepSurfaceDepth;					// ����ւ��p�̃T�[�t�F�X
	static LPDIRECT3DTEXTURE9	m_pKeepTexture;							// �e�N�X�`���̓��ꂦ�悤
	static D3DVIEWPORT9			m_DefaultViewPort;
	static D3DVIEWPORT9			m_ChangeViewPort[TYPE_RENDER_MAX];
	static LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	static VF					m_Vertex[];
	static IDirect3DVertexBuffer9 *m_pVertexBuff;
	static bool					m_bBeginScene;
	static float				m_fCameraRot;
	static D3DXMATRIX			m_ProjMtx;
	static D3DXMATRIX			m_ViewMtx;

	#ifdef _DEBUG
	int							m_nCountFPS;			// FPS�J�E���^
	#endif
};

#endif
//----EOF----