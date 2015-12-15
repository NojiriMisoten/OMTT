//=============================================================================
//
// CCrowdManager�N���X [CCrowdManager.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCrowdManager.h"
#include "CCrowd.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �G�̔䗦�́@165/80

// 2F�̃|���S���̑傫��
static const float UPPER_WIDTH = 380;
static const float UPPER_HEIGHT = 60;
// 1F�̃|���S���̑傫��
static const float LOWER_WIDTH = 165;
static const float LOWER_HEIGHT = 80;

// ���W���߂������������@���ꂽ������
static const D3DXVECTOR3 UPPWER_FRONT_POS_START = D3DXVECTOR3(-762, 50, 618);
static const D3DXVECTOR3 UPPWER_FRONT_POS_OFFSET = D3DXVECTOR3(381, 20, 52);
static const D3DXVECTOR3 UPPWER_LEFT_POS_START = D3DXVECTOR3(-650, 70, -800);
static const D3DXVECTOR3 UPPWER_LEFT_POS_OFFSET = D3DXVECTOR3(-50, 18, 400);
static const D3DXVECTOR3 UPPWER_RIGHT_POS_START = D3DXVECTOR3(792, 160, 730);
static const D3DXVECTOR3 UPPWER_RIGHT_POS_OFFSET = D3DXVECTOR3(-30, -18, -375);
static const D3DXVECTOR3 LOWER_FRONT_LEFT_POS_START = D3DXVECTOR3(-332, -30, 290);
static const D3DXVECTOR3 LOWER_FRONT_RIGHT_POS_START = D3DXVECTOR3(160, -30, 290);
static const D3DXVECTOR3 LOWER_FRONT_POS_OFFSET = D3DXVECTOR3(160, 0, 52);
static const D3DXVECTOR3 LOWER_RIGHT_POS_START = D3DXVECTOR3(322, -30, 470);
static const D3DXVECTOR3 LOWER_RIGHT_POS_OFFSET = D3DXVECTOR3(57, 0, -160);
static const D3DXVECTOR3 LOWER_LEFT_POS_START = D3DXVECTOR3(-322, -30, 470);
static const D3DXVECTOR3 LOWER_LEFT_POS_OFFSET = D3DXVECTOR3(-57, 0, -160);
static const D3DXVECTOR3 LOWER_BACK_POS_START = D3DXVECTOR3(-332, -30, 470);
static const D3DXVECTOR3 LOWER_BACK_POS_OFFSET = D3DXVECTOR3(-160, 0, -52);

// �����O����̊ϋq�̈ʒu
static const D3DXVECTOR3 CROWD_POS_BACK = D3DXVECTOR3(0, 30, -120);

// �e�N�X�`���̈�̃R�}�̑傫��X�݂̂�
static const float HUMAN_TEXTURE_ONE_WIDTH = 1.0f / 4.0f;
// �e�N�X�`���A�j���[�V�����ň�R�}�i�߂�܂ł̃t���[����
static const short HUMAN_TEXTURE_INTERVAL[CCrowdManager::TEXTURE_SPEED_KIND] = { 20, 30};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCrowdManager::CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;
	for (int i = 0; i < TEXTURE_SPEED_KIND; i++)
	{
		m_TextureCount[i] = 0;
		m_TextureCurX[i] = 0;
	}
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCrowdManager ::~CCrowdManager(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CCrowdManager::Init()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posStart;
	D3DXVECTOR3 posOffset;

	// 2F�̐���
	posStart = UPPWER_FRONT_POS_START;
	posOffset = UPPWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pUpperFront[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pUpperFront[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

			m_pUpperFront[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}

	// 2F�̍���
	posStart = UPPWER_LEFT_POS_START;
	posOffset = UPPWER_LEFT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++){
			pos.z += posOffset.z;
			m_pUpperLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pUpperLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
			m_pUpperLeft[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.y += posOffset.y;
		pos.x += posOffset.x;
		pos.z = posStart.z;
	}

	// 2F�̉E��
	posStart = UPPWER_RIGHT_POS_START;
	posOffset = UPPWER_RIGHT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++){
			pos.z += posOffset.z;
			m_pUpperRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pUpperRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
			m_pUpperRight[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.y += posOffset.y;
		pos.x += posOffset.x;
		pos.z = posStart.z;
	}

	// 1F�̑O���̍��̊ϋq
	posStart = LOWER_FRONT_LEFT_POS_START;
	posOffset = LOWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerFrontLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pLowerFrontLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
			m_pLowerFrontLeft[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// ���[�̊p
	m_pLowerFrontLeft[0][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerFrontLeft[0][1]->AddPos(D3DXVECTOR3(15, 0, 0));
	m_pLowerFrontLeft[0][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerFrontLeft[0][0]->AddPos(D3DXVECTOR3(20, 0, 0));

	// 1F�̑O���̉E�̊ϋq
	posStart = LOWER_FRONT_RIGHT_POS_START;
	posOffset = LOWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerFrontRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pLowerFrontRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
			m_pLowerFrontRight[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// �E�[�̊p
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1F�̉E���̊ϋq
	posStart = LOWER_RIGHT_POS_START;
	posOffset = LOWER_RIGHT_POS_OFFSET;

	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pLowerRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
			m_pLowerRight[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// �p
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1F�̍����̊ϋq
	posStart = LOWER_LEFT_POS_START;
	posOffset = LOWER_LEFT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pLowerLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
			m_pLowerLeft[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// �p
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1F�̌��̊ϋq
	posStart = LOWER_BACK_POS_START;
	posOffset = LOWER_BACK_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerBack[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_HUMAN, m_pManager);
			m_pLowerBack[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI, 0));
			m_pLowerBack[i][n]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH);
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}

	// �͂ݏo���Ȃ��悤�ɃT�C�Y�ς�����p�̃e�N�X�`�����W
	m_pLowerFrontLeft[0][1]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.5f);
	m_pLowerFrontLeft[0][0]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.25f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][1]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.5f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][0]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.25f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.5f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.25f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.5f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetTextureV(0, HUMAN_TEXTURE_ONE_WIDTH * 0.25f);
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CCrowdManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CCrowdManager::Update(void)
{
	// �e�N�X�`���A�j���[�V���������
	for (int i = 0; i < TEXTURE_SPEED_KIND; i++)
	{
		m_TextureCount[i]++;
		if (m_TextureCount[i] > HUMAN_TEXTURE_INTERVAL[i])
		{
			m_TextureCount[i] = 0;
			m_TextureCurX[i] += HUMAN_TEXTURE_ONE_WIDTH;
			// �e�N�X�`���̃��[�v
			if (m_TextureCurX[i] >= 1.0f)
			{
				m_TextureCurX[i] = 0;
			}
		}
	}

	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			m_pUpperFront[i][n]->SetTextureOffsetV(
				n % 2 == 0 ? m_TextureCurX[0] : m_TextureCurX[1] + HUMAN_TEXTURE_ONE_WIDTH);
		}
	}
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++){
			m_pUpperLeft[i][n]->SetTextureOffsetV(m_TextureCurX[0]);
			m_pUpperRight[i][n]->SetTextureOffsetV(m_TextureCurX[0]);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontLeft[i][n]->SetTextureOffsetV(
				n % 2 == 0 ? m_TextureCurX[0] : m_TextureCurX[1] + HUMAN_TEXTURE_ONE_WIDTH);
			m_pLowerFrontRight[i][n]->SetTextureOffsetV(
				n % 2 == 0 ? m_TextureCurX[0] : m_TextureCurX[1] + HUMAN_TEXTURE_ONE_WIDTH);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerRight[i][n]->SetTextureOffsetV(m_TextureCurX[0]);
			m_pLowerLeft[i][n]->SetTextureOffsetV(m_TextureCurX[0]);
			m_pLowerBack[i][n]->SetTextureOffsetV(m_TextureCurX[0]);
		}
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CCrowdManager* CCrowdManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	// �쐬
	CCrowdManager* p = new CCrowdManager(pDevice, pManager);

	// ������
	p->Init();

	return p;

}

//*****************************************************************************
// �`�悵�Ȃ�
//*****************************************************************************
void CCrowdManager::SetImvisible(void)
{
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			m_pUpperFront[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++)
	{
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++)
		{
			m_pUpperLeft[i][n]->SetDrawFlag(false);
			m_pUpperRight[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontLeft[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontRight[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerRight[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerLeft[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerBack[i][n]->SetDrawFlag(false);
		}
	}
}

//*****************************************************************************
// �`�悷��
//*****************************************************************************
void CCrowdManager::SetVisible(void)
{
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			m_pUpperFront[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++)
	{
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++)
		{
			m_pUpperLeft[i][n]->SetDrawFlag(true);
			m_pUpperRight[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontLeft[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontRight[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerRight[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerLeft[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerBack[i][n]->SetDrawFlag(true);
		}
	}
}
//----EOF----