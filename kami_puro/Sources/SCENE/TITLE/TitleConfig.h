//=============================================================================
//
// �^�C�g���̐ݒ��� [TitleConfig.h]
// Author : ��{ �F��
//
//=============================================================================
#ifndef _TITLE_CONFIGH_
#define _TITLE_CONFIGH_


//�������֘A

//���������̊�b�I�Ȓl
static const UV_INDEX  BASE_UV                 = { 1.0f, 0.0f, 1.0f, 0.0f };
static const D3DXCOLOR BASE_COLOR              = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

static D3DXVECTOR3     BG_FIRST_POS(             (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f );
static const float     BG_FIRST_WIDTH          = (float)SCREEN_WIDTH;
static const float     BG_FIRST_HEIGHT         = (float)SCREEN_HEIGHT;

static D3DXVECTOR3     SCROLLBG_FIRST_POS(       (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f );
static const float     SCROLLBG_FIRST_WIDTH    = (float)SCREEN_WIDTH;
static const float     SCROLLBG_FIRST_HEIGHT   = (float)SCREEN_HEIGHT;
static const int       SCROLLBG_ANIM_MAX_X = 4;
static const int       SCROLLBG_ANIM_MAX_Y = 1;
static const int       SCROLLBG_ANIM_SPD   = 2;

static D3DXVECTOR3     FIRE_FIRST_POS(           (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.75f, 0.f );
static const float     FIRE_FIRST_WIDTH        = (float)SCREEN_WIDTH;
static const float     FIRE_FIRST_HEIGHT       = (float)SCREEN_HEIGHT;

static D3DXVECTOR3     HITANYCONG_FIRST_POS(     (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.8f, 0.f );
static const float     HITANYCONG_FIRST_WIDTH =  (float)SCREEN_WIDTH*0.7f;
static const float     HITANYCONG_FIRST_HEIGHT = (float)SCREEN_HEIGHT*0.2f;

static D3DXVECTOR3     LOGO_FIRST_POS(           (float)SCREEN_WIDTH * 0.3f, (float)SCREEN_HEIGHT * 0.3f, 0.f );
static const float     LOGO_FIRST_WIDTH        = (float)SCREEN_WIDTH*0.5f;
static const float     LOGO_FIRST_HEIGHT       = (float)SCREEN_HEIGHT*0.5f;
static const D3DXCOLOR LOGO_COLOR              = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);


//�X�N���[���|���S���֘A
static const float SCROLL_MAX = 1.0f;   // �X�N���[���ő�l
static const float SCROLL_MIN = 0.0f;   // �X�N���[���ŏ��l
static const float SCROLL_SPD = 0.04f;  // �X�N���[���X�s�[�h

//�_�Ń|���S���֘A
static const float BLINKING_SPD = 0.01f; //�_�ŃX�s�[�h

#endif
//----EOF----
