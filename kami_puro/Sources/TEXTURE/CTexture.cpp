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
	NULL,
	"../data/TEXTURE/mono.png",
	"../data/TEXTURE/default.png",
	"../data/TEXTURE/number.png",
	"../data/TEXTURE/title.png",
	"../data/TEXTURE/result.png",
	"../data/TEXTURE/eye.bmp",
	"../data/TEXTURE/HpColorGreen.png",
	"../data/TEXTURE/HpColorRed.png",
	"../data/TEXTURE/HpFrame.png",
	"../data/TEXTURE/CrowdFrame.png",
	"../data/TEXTURE/CrowdHuman.png",
	"../data/TEXTURE/CrowdSpark.png",
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
				assert(!"�e�N�X�`����data�ɂȂ��I");

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
		assert(!"�s���ȃe�N�X�`���^�C�v�I");

		return NULL;
	}
	return m_pD3DTex[type];
}

//----EOF----