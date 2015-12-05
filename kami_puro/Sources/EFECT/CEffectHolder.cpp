//=============================================================================
//
// �G�t�F�N�g�{�̂̊i�[���� [CEffectHolder.cpp]
// Author : ��{ �F��
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectHolder.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
// �e�N�X�`���̃p�X
static const wchar_t *EFFECT_PATH[EFFECT_MAX] =
{
	//����(��)
	L"../data/EFECT/FootStep(smoke).efk",
	//����(�g)
	L"../data/EFECT/FootStep(wave).efk",
	//�I�[��(�J�n)
	L"../data/EFECT/aura_Invocation.efk",
	//�I�[��(���[�v)
	L"../data/EFECT/aura_loop.efk",
	//�I�[��(�I��)
	L"../data/EFECT/aura_end.efk",
	//�o�E���h��
	L"../data/EFECT/Bound001.efk",
	//��U��
	L"../data/EFECT/dageki_jakku.efk",
	//���U��
	L"../data/EFECT/dageki_tyu-.efk",
	//���U��
	L"../data/EFECT/dageki_kyo.efk",
	//�h�䎞
	L"../data/EFECT/Defense001.efk",
	//��(���ꎞ)
	L"../data/EFECT/EnterSmoke001.efk",
	//�W�����v��
	L"../data/EFECT/Jump001.efk",
	//�����O�o�V�R�[��
	L"../data/EFECT/RingShock001.efk",
	//�e�X�g�p,�D���Ɏg����!
	L"../data/EFECT/shock_weve001test.efk",
	//�Ռ��g
	L"../data/EFECT/ShockWeve001.efk",
	//��
	L"../data/EFECT/smoke_001.efk",
	//�e�X�g����2
	L"../data/EFECT/testdayo-n.efk",
	//����
	L"../data/EFECT/Throw001.efk",
	//���������̂��
	L"../data/EFECT/Win001.efk",
	// �z�[���[�����X
	L"../data/EFECT/HolyLance_Loop.efk"
};

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ��錾
//*****************************************************************************
bool CEffectHolder::m_bDoneCreateFlag = false;
::Effekseer::Effect* CEffectHolder::m_pEffect[EFFECT_MAX] = { NULL };

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffectHolder::Init(void)
{
	// �Q�[���Ŏg���S�G�t�F�N�g���쐬
	for (int EffectCnt = 0; EffectCnt < EFFECT_MAX; EffectCnt++)
	{
		if (EFFECT_PATH[EffectCnt] != NULL)
		{
			m_pEffect[EffectCnt] = Effekseer::Effect::Create(CEffectManager::GetEffectManager(), (const EFK_CHAR*)EFFECT_PATH[EffectCnt]);
			// �G�t�F�N�g�ǂݍ���
			if (!m_pEffect[EffectCnt])
			{
				assert(!"�G�t�F�N�g��data�ɂȂ��I");

				m_pEffect[EffectCnt] = NULL;
				return E_FAIL;
			}
		}
		else
		{
			m_pEffect[EffectCnt] = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffectHolder::Uninit(void)
{
	// �g�����S�G�t�F�N�g���J��
	for (int EffectCnt = 0; EffectCnt < EFFECT_MAX; EffectCnt++)
	{
		ES_SAFE_RELEASE(m_pEffect[EffectCnt]);
	}
}

//=============================================================================
// �e�N�X�`���쐬����
//=============================================================================
void CEffectHolder::CreateEffect(void)
{
	CEffectHolder::Init();
}

//=============================================================================
// �e�N�X�`���|�C���^�擾����
//=============================================================================
::Effekseer::Effect* CEffectHolder::GetEffect(const EFFECT_TYPE type)
{
	// �������e�N�X�`���^�C�v������
	if (type < 0 || type > EFFECT_MAX)
	{
		assert(!"�s���ȃG�t�F�N�g�^�C�v�I");

		return NULL;
	}
	return m_pEffect[type];
}

//----EOF----