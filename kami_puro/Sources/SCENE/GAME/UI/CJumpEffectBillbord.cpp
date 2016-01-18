//=============================================================================
//
// CJumpEffectBillbord�N���X [CJumpEffectBillbord.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CJumpEffectBillbord.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"
#include "../../../MATH/mersenne_twister.h"
#include "../../../CAMERA/CameraManager.h"
#include "CUiManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************

// �傫��
static const float JUMP_EFFECT_SIZE = SCREEN_WIDTH / 1280.f * 50.0f;
// ���߃X�s�[�h
static const float ALPHA_SPEED = 0.025f;
// �G�t�F�N�g�̓��ߓx
static const float ALPHA = 0.8f;
// �ړ��X�s�[�h
static const float MOVE_SPEED = 1.3f;
// ���̈ړ��ʂ̌����l
static const float VELO_RESIST = 0.9f;
// ���̈ړ��ʂ͈̔�
static const float VELO_MAX = 3;
// �r���[�|�[�g�ϊ��̍s��
static const D3DXMATRIX VIEW_PORT = {
	SCREEN_WIDTH*0.5f, 0, 0, 0,
	0, SCREEN_HEIGHT*0.5f, 0, 0,
	0, 0, 1, 0,
	SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0, 1
};
// ��Ԑ�̍��W HP�Ɗ����Q�[�W �Y�����̓v���C���ԍ��ƑΉ�
static const D3DXVECTOR3 POS_HP_BAR[2] = {
	D3DXVECTOR3(335, 121, 0),
	D3DXVECTOR3(SCREEN_WIDTH - 335, 121, 0)
};
static const D3DXVECTOR3 POS_CROWD_BAR[2] = {
	D3DXVECTOR3(425, 75, 0),
	D3DXVECTOR3(SCREEN_WIDTH - 425, 75, 0)
};
// ��Ԃ܂ł̃t���[����
static const int FLY_INTERVAL = 60;
// ��Ԏ��̈ړ��X�s�[�h ��Ԃ���time(0~1)�̉��Z�l
static const float FLY_TIME_SPEED = 0.1f;
// �k���X�s�[�h
static const float SIZE_RESIST = 0.9f;
// ��ʊO�ɍs���Ȃ��悤�ɐ���������
static const float POS2D_MIN = 100.f;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CJumpEffectBillbord::CJumpEffectBillbord(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	m_pCameraManager = NULL;
	m_pD3DDevice = pDevice;
	m_Velo = 0;
	m_Color = D3DXCOLOR(1, 1, 1, ALPHA);
	m_PlayerNum = 0;
	m_FlyCount = 0;
	m_isFly = false;
	m_FlyTime = 0;
	m_Pos3D = D3DXVECTOR3(0, 0, 0);
	m_Pos2D = D3DXVECTOR3(0, 0, 0);
	m_Velo = 0;
	m_Pos2DStart = D3DXVECTOR3(0, 0, 0);
	m_Pos2DEnd = D3DXVECTOR3(0, 0, 0);
	m_isCrowd = false;
	m_Size = JUMP_EFFECT_SIZE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CJumpEffectBillbord::~CJumpEffectBillbord(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CJumpEffectBillbord::Init(D3DXVECTOR3& pos, TEXTURE_TYPE texType)
{
	m_Pos3D = pos;
	// ���̈ړ��ʂ������_���Ō��߂�
	m_Velo = mersenne_twister_float(-VELO_MAX, VELO_MAX);

	// 3D�̍��W��2D�ɕϊ�
	m_Pos2D = D3DXVECTOR3(0, 0, 0);
	D3DXMATRIX proj = m_pCameraManager->GetMtxProj();
	D3DXMATRIX view = m_pCameraManager->GetMtxView();
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos3D, &view);
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &proj);
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &VIEW_PORT);

	// ��U3D����2D�ɕϊ�����
	CScene2D::Init(m_Pos2D, JUMP_EFFECT_SIZE, JUMP_EFFECT_SIZE, texType);

	// �ŏI�I�ɔ�Ԑ�̍��W���v�Z���Ă���
	m_isCrowd = (texType != TEXTURE_JUMP_EFFECT_HP);

}

//=============================================================================
// �I��
//=============================================================================
void CJumpEffectBillbord::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CJumpEffectBillbord::Update(void)
{
	// ���J�E���g�����炻�ꂼ��̍��W�ɔ��
	if (!m_isFly)
	{
		// 3D���W�n�ŏo�����̃A�j���[�V����
		m_Pos3D.y -= MOVE_SPEED;
		m_Pos3D.x += m_Velo;
		m_Velo *= VELO_RESIST;

		// ��ԏ���
		m_FlyCount++;
		if (m_FlyCount > FLY_INTERVAL || m_Pos2D.y < POS2D_MIN)
		{
			m_isFly = true;
			m_Pos2DStart = m_Pos2D;
			if (m_isCrowd)
			{
				m_Pos2DEnd.x = m_pUiManager->GetPosCrowdCenter(m_PlayerNum);
				m_Pos2DEnd.y = POS_CROWD_BAR[m_PlayerNum].y;
			}
			else
			{
				m_Pos2DEnd.x = m_pUiManager->GetPosHpCenter(m_PlayerNum);
				m_Pos2DEnd.y = POS_HP_BAR[m_PlayerNum].y;
			}
		}

		// 3D�̍��W��2D�ɕϊ�
		D3DXMATRIX proj = m_pCameraManager->GetMtxProj();
		D3DXMATRIX view = m_pCameraManager->GetMtxView();
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos3D, &view);
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &proj);
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &VIEW_PORT);

		// CScene2D�̏����X�V���ăX�v���C�g�̑傫����Lock�ŕύX
		m_Pos = m_Pos2D;
		SetVertexPolygon();
	}
	// ���ł�Ƃ��̏���
	else
	{
		// ��Ԃňړ�
		if (m_FlyTime <= 1.0f)
		{
			m_Pos2D.x = EasingInterpolation(m_Pos2DStart.x, m_Pos2DEnd.x, m_FlyTime);
			m_Pos2D.y = EasingInterpolation(m_Pos2DStart.y, m_Pos2DEnd.y, m_FlyTime);
			// CScene2D�̏����X�V���ăX�v���C�g�̑傫����Lock�ŕύX
			m_Pos = m_Pos2D;
			SetVertexPolygon();
			m_FlyTime += FLY_TIME_SPEED;
		}
		// �ړ����I������珬�����Ȃ��ē��߂��ď�����
		else
		{
			m_Size *= SIZE_RESIST;
			SetVertexPolygon(m_Pos, m_Size, m_Size);

			// ����
			m_Color.a -= ALPHA_SPEED;
			this->SetColorPolygon(m_Color);

			// ���߂��������
			if (m_Color.a <= 0.0f)
			{
				Release();
			}
		}
	}

}

//=============================================================================
// �`��
//=============================================================================
void CJumpEffectBillbord::DrawUIRender(void)
{
	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
		, 0											// �`����J�n���钸�_�ԍ�
		, DRAW_SQUARE_PRINITIV_NUM);				// ���������|���S����
}

//=============================================================================
// �쐬
//=============================================================================
CJumpEffectBillbord* CJumpEffectBillbord::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, TEXTURE_TYPE texType, CCameraManager* pCamera, CUiManager* pUiManager, int playerNum)
{
	CJumpEffectBillbord* p = new CJumpEffectBillbord(pDevice);
	p->m_pCameraManager = pCamera;
	p->m_PlayerNum = playerNum;
	p->m_pUiManager = pUiManager;
	p->Init(pos, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI);
	return p;
}

//----EOF----