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
static const char *TEXTURE_PATH[TEXTURE_MAX] =
{
	NULL,
	"data/TEXTURE/bg_0.png",
	"data/TEXTURE/bg_1.png",
	"data/TEXTURE/fire0.png",
	"data/TEXTURE/fire1.png",
	"data/TEXTURE/thread.png",
	"data/TEXTURE/player1.png",
	"data/TEXTURE/player2.png",
	"data/TEXTURE/player3.png",
	"data/TEXTURE/player4.png",
	"data/TEXTURE/figure_black.png",
	"data/TEXTURE/figure_white.png",
	"data/TEXTURE/figure_green.png",
	"data/TEXTURE/treasure.png",
	"data/TEXTURE/treasure_icon.png",
	"data/TEXTURE/bg_title.png",
	"data/TEXTURE/bg_tutorial.png",
	"data/TEXTURE/bg_result.png",
	"data/TEXTURE/bg_chara_select.png",
	"data/TEXTURE/bg_stage_select.png",
	"data/TEXTURE/stage01.png",
	"data/TEXTURE/stage02.png",
	"data/TEXTURE/stage03.png",
	"data/TEXTURE/stage04.png",
	"data/TEXTURE/stage05.png",
	"data/TEXTURE/stage06.png",
	"data/TEXTURE/stage07.png",
	"data/TEXTURE/stage08.png",
	"data/TEXTURE/WinLogo.png",
	"data/TEXTURE/DrawLogo.png",
	"data/TEXTURE/02.png",
	"data/TEXTURE/03.png",
	"data/TEXTURE/Spawn.png",
	"data/TEXTURE/SpawnYellow.png",
	"data/TEXTURE/SpawnRed.png",
	"data/TEXTURE/SpawnBlue.png",
	"data/TEXTURE/SpawnGreen.png",
	"data/TEXTURE/dengeki.png",				// �d��
	"data/TEXTURE/hit_effect_small.png",
	"data/TEXTURE/hit_effect_big.png",
	"data/TEXTURE/attack_around.png",
	"data/TEXTURE/attack_tackle.png",
	"data/TEXTURE/slow_effect.png",
	"data/TEXTURE/trans_effect_yellow.png",
	"data/TEXTURE/trans_effect_red.png",
	"data/TEXTURE/trans_effect_blue.png",
	"data/TEXTURE/trans_effect_green.png",
	"data/TEXTURE/kirakira.png",
	"data/TEXTURE/treasure_light.png",
	"data/TEXTURE/title_logo.png",
	"data/TEXTURE/title_pushstart.png",
	"data/TEXTURE/title_menu.png",
	"data/TEXTURE/title_cursol.png",
	"data/TEXTURE/option_menu.png",
	"data/TEXTURE/option_BG.png",
	"data/TEXTURE/cursol.png",
	"data/TEXTURE/return.png",
	"data/TEXTURE/player_num.png",
	"data/TEXTURE/result_circle_light.png",
	"data/TEXTURE/result_light.png",
	"data/TEXTURE/result_push.png",
	"data/TEXTURE/result_menu_back_0.png",
	"data/TEXTURE/result_menu_back_1.png",
	"data/TEXTURE/result_menu_back_2.png",
	"data/TEXTURE/result_menu_moji_0.png",
	"data/TEXTURE/result_menu_moji_1.png",
	"data/TEXTURE/result_menu_moji_2.png",
	"data/TEXTURE/chara_select_player_bg.png",
	"data/TEXTURE/chara_select_join.png",
	"data/TEXTURE/chara_select_start.png",
	"data/TEXTURE/chara_select_conifing.png",
	"data/TEXTURE/chara_select_cursol.png",
	"data/TEXTURE/chara_select_ready.png",
	"data/TEXTURE/chara_select_logo.png",
	"data/TEXTURE/stage_select_logo.png",
	"data/TEXTURE/result_logo.png",
	"data/TEXTURE/tutorial_0.png",
	"data/TEXTURE/tutorial_1.png",
	"data/TEXTURE/tutorial_2.png",
	"data/TEXTURE/tutorial_3.png",
	"data/TEXTURE/tutorial_arrow.png",
	"data/TEXTURE/tutorial_page_1_4.png",
	"data/TEXTURE/tutorial_page_2_4.png",
	"data/TEXTURE/tutorial_page_3_4.png",
	"data/TEXTURE/tutorial_page_4_4.png",
	"data/TEXTURE/game_start.png",
	"data/TEXTURE/thread_drop_poison.png",
	"data/TEXTURE/normal_thread.png",
	"data/TEXTURE/trap.png",
};

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ��錾
//*****************************************************************************
bool CTexture::m_bDoneCreateFlag = false;
LPDIRECT3DTEXTURE9 CTexture::m_pD3DTex[TEXTURE_MAX] = { NULL };

//=============================================================================
// ����������
//=============================================================================
HRESULT CTexture::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// �Q�[���Ŏg���S�e�N�X�`�����쐬
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		if (TEXTURE_PATH[TexCnt] != NULL)
		{
			// �e�N�X�`���ǂݍ���
			if (FAILED(D3DXCreateTextureFromFile((*pDevice), TEXTURE_PATH[TexCnt], &m_pD3DTex[TexCnt])))
			{
#ifdef _DEBUG
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
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
void CTexture::Uninit(void)
{
	// �g�����S�e�N�X�`���|�C���^���J��
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		// �e�N�X�`���|�C���^���g���Ă���ꍇ
		if (m_pD3DTex[TexCnt])
		{
			m_pD3DTex[TexCnt]->Release();
			m_pD3DTex[TexCnt] = NULL;
		}
	}

}

//=============================================================================
// �e�N�X�`���쐬����
//=============================================================================
void CTexture::CreateTexture(LPDIRECT3DDEVICE9 *pDevice)
{
	CTexture::Init(pDevice);
}

//=============================================================================
// �e�N�X�`���|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(const TEXTURE_TYPE type)
{
	// �������e�N�X�`���^�C�v������
	if (type < 0 || type > TEXTURE_MAX)
	{
#ifdef _DEBUG
		MessageBox(NULL, "�e�N�X�`���^�C�v�̔ԍ����s�K�؂ł�", "�G���[", MB_OK | MB_ICONERROR);
#endif
		return NULL;
	}
	return m_pD3DTex[type];
}

//============================================================
// �e�N�X�`���^�C�v�Q�b�^�[
//============================================================
TEXTURE_TYPE CTexture::GetTextureTye(char* texFilePath)
{
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		if (!TEXTURE_PATH[TexCnt])
		{
			continue;
		}
		if (strcmp(texFilePath, TEXTURE_PATH[TexCnt]) == 0)
		{
			return (TEXTURE_TYPE)TexCnt;
		}
	}

#ifdef _DEBUG
	MessageBox(NULL, "�e�N�X�`���p�X���s���ł�\n", "�G���[", MB_OK | MB_ICONERROR);
#endif
	return TEXTURE_NULL;

}

//----EOF----