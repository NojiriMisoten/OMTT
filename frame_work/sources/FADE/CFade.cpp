//=============================================================================
//
// CFade�N���X [CFade.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../FADE/CFade.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
FADE_MODE	CFade::m_mode = MODE_FADE_NULL;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CFade ::CFade(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene2D(pDevice, nPriority)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CFade ::~CFade(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CFade :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock )
{
	CScene2D::Init(pos, width, height, texType);
	
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// �F
	m_nEndTime = 0;										// �I������
	m_bFade = false;									// �t�F�[�h�t���O
	m_mode = MODE_FADE_NULL;							// ���
	m_fMovAlpha = 0.0f;									// �����x(���x)
	
	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CFade ::Uninit(void)
{
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// �F
	m_nEndTime = 0;										// �I������
	m_bFade = false;									// �t�F�[�h�t���O
	m_mode = MODE_FADE_NULL;							// ���
	m_fMovAlpha = 0.0f;									// �����x(���x)

	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CFade ::Update(void)
{
	if(m_bFade)
	{
		switch (m_mode)
		{
			// �t�F�[�h�C���̏���
			case MODE_FADE_IN:
				
				// ���X�ɔZ��
				m_color.a -= m_fMovAlpha;

				// �����x��0�����ɂȂ�����
				if(m_color.a < 0.0f)
				{
					m_color.a = 0.0f;

					// �t�F�[�h�C���I��
					m_mode = MODE_FADE_IN_END;
				}

				// �F�Z�b�g
				SetColorPolygon(m_color);
			break;

			// �t�F�[�h�C���̏I������
			case MODE_FADE_IN_END:
				m_bFade = false;
			break;

			// �t�F�[�h�A�E�g�̏���
			case MODE_FADE_OUT:
				// ���X�ɔ���
				m_color.a += m_fMovAlpha;

				// �����x���}�b�N�X�ɂȂ�����
				if(m_color.a > MAX_ALPHA)
				{
					m_color.a = MAX_ALPHA;

					// �t�F�[�h�A�E�g�I��
					m_mode = MODE_FADE_OUT_END;
				}
				// �F�Z�b�g
				SetColorPolygon(m_color);
			break;

			// �t�F�[�h�A�E�g�̏I������
			case MODE_FADE_OUT_END:
				m_bFade = false;
			break;

			// �t�F�[�h�C���t�B�j�e�B�̏���
			case MODE_FADE_INFINITY:
				// �F�Z�b�g
				SetColorPolygon(m_color);
			break;
		}
	}
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CFade ::Draw(void)
{
	if(m_bFade)
	{
		// �|���S���̕`��
		(*m_pD3DDevice)->SetStreamSource(0,m_pD3DVtxBuff,0,sizeof(VERTEX_2D));	//(0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									//���_�t�H�[�}�b�g�̐ݒ�
		(*m_pD3DDevice)->SetTexture(0, NULL);									//�e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						//�v���~�e�B�u�̎��
									, 0											//�`����J�n���钸�_�ԍ�
									, POLYGON_NUM);								//���������|���S����
	}
}

//*****************************************************************************
// �t�F�[�h�X�^�[�g�֐�
//*****************************************************************************
void CFade ::Start(FADE_MODE mode, D3DXCOLOR color, int endToTime)
{
	// ���݂̃��[�h�ƈ���Ă���
	if(m_mode != mode)
	{
		// ��ԕύX
		m_mode = mode;

		// �t�F�[�h����F
		m_color = color;

		// �t�F�[�h�C���Ȃ�A���t�@�lMAX
		if(mode == MODE_FADE_IN)
		{
			m_color.a = MAX_ALPHA;
		}

		// �t�F�[�h�A�E�g�Ȃ�A���t�@�lMIN
		if(mode == MODE_FADE_OUT)
		{
			m_color.a = MIN_ALPHA;
		}

		// �I�����Ԑݒ�
		m_nEndTime = endToTime;

		// �t�F�[�h�t���O�I��
		m_bFade = true;

		// ���x����
		m_fMovAlpha = MAX_ALPHA / (float)m_nEndTime;
	}
}
//----EOF----