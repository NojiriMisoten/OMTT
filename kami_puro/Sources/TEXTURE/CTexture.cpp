//=============================================================================
//
// �e�N�X�`������ [CTexture.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTexture.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
// �e�N�X�`���̃p�X
static const char *TEXTURE_PATH[ TEXTURE_MAX ] = 
{
	"../data/TEXTURE/mono.png",
	"../data/TEXTURE/default.png",
	"../data/TEXTURE/number.png",
	"../data/TEXTURE/title.png",
	"../data/TEXTURE/result.png",
	"../data/TEXTURE/HpColorGreen.png",
	"../data/TEXTURE/HpColorRed.png",
	"../data/TEXTURE/HpFrame.png",
	"../data/TEXTURE/HpFrameTop.png",
	"../data/TEXTURE/CrowdFrame.png",
	"../data/TEXTURE/CrowdHuman.png",
	"../data/TEXTURE/CrowdSpark.png",
	"../data/TEXTURE/jijii.png",
	"../data/TEXTURE/jijii_back.png",
	"../data/TEXTURE/blue.png",
	"../data/TEXTURE/red.png",
	"../data/TEXTURE/metalicgray.png",
	"../data/TEXTURE/windows.jpg",
	"../data/TEXTURE/CommandButton.png",				// �{�^���̃e�N�X�`��
	"../data/TEXTURE/rope.png",
	"../data/TEXTURE/FadeEffectLine.png",		// �o�g���t�F�[�h
	"../data/TEXTURE/FadeEffectBall.png",		// �o�g���t�F�[�h
	"../data/TEXTURE/FadeEffectBall2.png",		// �o�g���t�F�[�h
	"../data/TEXTURE/CutIn0.png",				// �J�b�g�C��0
	"../data/TEXTURE/CutIn1.png",				// �J�b�g�C��1
	"../data/TEXTURE/CutInBack.png",			// �J�b�g�C���w�iblue
	"../data/TEXTURE/CutInBack2.png",			// �J�b�g�C���w�ired
	"../data/TEXTURE/SKILL_NAME/Chop.png",	
	"../data/TEXTURE/SKILL_NAME/Elbow.png",	
	"../data/TEXTURE/SKILL_NAME/Lariat.png",	
	"../data/TEXTURE/SKILL_NAME/Slap.png",	
	"../data/TEXTURE/SKILL_NAME/BackDrop.png",	
	"../data/TEXTURE/SKILL_NAME/Stunner.png",	
	"../data/TEXTURE/SKILL_NAME/RollingElbow.png",	
	"../data/TEXTURE/SKILL_NAME/FlyingShoulder.png",	
	"../data/TEXTURE/SKILL_NAME/DropKick.png",
	"../data/TEXTURE/SKILL_NAME/SmallAttack.png",
	"../data/TEXTURE/SKILL_NAME/BigAttack.png",
	"../data/TEXTURE/SKILL_NAME/Throw.png",
	"../data/TEXTURE/SKILL_NAME/rope.png",
	"../data/TEXTURE/SKILL_NAME/Finish.png",
	"../data/TEXTURE/ElectricalScoreboard.png",
	"../data/TEXTURE/commandchart_back.png",
	"../data/TEXTURE/tempCommandBackPoly.png",
	"../data/TEXTURE/TITLE/black.png",
	"../data/TEXTURE/TITLE/logo.png",
	"../data/TEXTURE/TITLE/logoany.png",
	"../data/TEXTURE/TITLE/star.png",
	"../data/TEXTURE/TITLE/fire.png",
	"../data/TEXTURE/chain.png",
	"../data/TEXTURE/READY.png",
	"../data/TEXTURE/FIGHT.png",
	"../data/TEXTURE/K.O.png",
	"../data/TEXTURE/human.png",		// �ϋq�̃e�N�X�`��
};

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ��錾
//*****************************************************************************
bool CTexture::m_bDoneCreateFlag = false;
LPDIRECT3DTEXTURE9 CTexture::m_pD3DTex[TEXTURE_MAX] = {NULL};

//=============================================================================
// ����������
//=============================================================================
HRESULT CTexture::Init( LPDIRECT3DDEVICE9 *pDevice )
{
	// �Q�[���Ŏg���S�e�N�X�`�����쐬
	for( int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++ )
	{
		if(TEXTURE_PATH[TexCnt] != NULL)
		{
			// �e�N�X�`���ǂݍ���
			if( FAILED( D3DXCreateTextureFromFile((*pDevice), TEXTURE_PATH[TexCnt], &m_pD3DTex[TexCnt])))
			{
#ifdef _DEBUG
				assert(!"�e�N�X�`����data�ɂȂ��I");
#endif
				m_pD3DTex[TexCnt] = NULL;
				return E_FAIL;
			}
		}
		else
		{
			m_pD3DTex[TexCnt] = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTexture::Uninit( void )
{
	// �g�����S�e�N�X�`���|�C���^���J��
	for( int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++ )
	{
		// �e�N�X�`���|�C���^���g���Ă���ꍇ
		if(m_pD3DTex[TexCnt])
		{
			m_pD3DTex[TexCnt]->Release();
			m_pD3DTex[TexCnt] = NULL;
		}
	}

}

//=============================================================================
// �e�N�X�`���쐬����
//=============================================================================
void CTexture::CreateTexture( LPDIRECT3DDEVICE9 *pDevice)
{
	CTexture::Init(pDevice);
}

//=============================================================================
// �e�N�X�`���|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture( const TEXTURE_TYPE type )
{
	// �������e�N�X�`���^�C�v������
	if( type < 0 || type > TEXTURE_MAX )
	{
#ifdef _DEBUG
		assert(!"�s���ȃe�N�X�`���^�C�v�I");
#endif
		return NULL;
	}
	return m_pD3DTex[type];
}

//----EOF----