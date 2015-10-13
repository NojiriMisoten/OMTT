//=============================================================================
//
// CSceneAnime�N���X [CSceneAnime.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneAnime.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneAnime::CSceneAnime(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_nMaxLoopNum = 0;
	m_nCurLoopNum = 0;
	m_bAutoUpdate = true;
	m_bDraw = true;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneAnime ::~CSceneAnime(void)
{
}

//*****************************************************************************
// �������֐� ���[�v�Ȃ��o�[
//*****************************************************************************
HRESULT CSceneAnime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime)
{
	CScene2D ::Init(pos, width, height, texType);

	// �l�̃`�F�b�N
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;
	m_nEndTime = endTime;
	m_nChangeTime = m_nEndTime / ((m_nSepalateNumX) * (m_nSepalateNumY));
	m_nDoAnimCounter = 0;

	// �e�N�X�`�����p�Z�b�g
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// �������֐� ���[�v����o�[ loopNum���}�C�i�X�l�Ȃ疳�����[�v
//*****************************************************************************
HRESULT CSceneAnime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum)
{
	CScene2D ::Init(pos, width, height, texType);

	// �l�̃`�F�b�N
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;
	m_nChangeTime = animSpd;
	m_nDoAnimCounter = 0;
	m_nMaxLoopNum = loopNum;

	// �e�N�X�`�����p�Z�b�g
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneAnime ::Uninit(void)
{
	CScene2D :: Uninit();
	//Release();

}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneAnime ::Update(void)
{
	if(!m_bAutoUpdate)
	{
		return;
	}
	// ���[�v�Ȃ�
	if(m_nMaxLoopNum == 0)
	{
		Update_Normal();
	}

	// ���[�v����
	else
	{
		Update_Loop();
	}
}

// ���[�v�Ȃ�
void CSceneAnime ::Update_Normal(void)
{
	m_nDoAnimCounter++;

	// �A�j���[�V�������Ԓ�������
	if(m_nDoAnimCounter >= m_nEndTime)
	{
		this->Release();
	}

	else if(m_nDoAnimCounter % m_nChangeTime == 0)
	{
		m_nIdx++;
		if(m_nIdx >= m_nMaxIdx)
		{
			m_nIdx = 0;
		}
		SetIndex(m_nIdx);
	}
}

// ���[�vver
void CSceneAnime ::Update_Loop(void)
{
	m_nDoAnimCounter++;

	// �A�j���[�V�������Ԓ�������
	if(m_nCurLoopNum >= m_nMaxLoopNum && m_nMaxLoopNum >= 0)
	{
		this->Release();
	}

	else if(m_nDoAnimCounter % m_nChangeTime == 0)
	{
		m_nIdx++;
		if(m_nIdx >= m_nMaxIdx)
		{
			m_nIdx = 0;
		}
		SetIndex(m_nIdx);
	}
}


//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneAnime ::Draw(void)
{
	if(m_bDraw)
	{
		CScene2D ::Draw();
	}
}

//*****************************************************************************
// �N���G�C�g�֐��@���[�v�Ȃ�Ver
//*****************************************************************************
CSceneAnime* CSceneAnime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime)
{
	// �쐬
	CSceneAnime* p = new CSceneAnime(pDevice);

	p->m_pD3DDevice = pDevice;

	// ������
	p->Init(pos, width, height, texType, sepalateX, sepalateY, endTime);

	return p;
}

//*****************************************************************************
// �N���G�C�g�֐��@���[�vVer
//*****************************************************************************
CSceneAnime* CSceneAnime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum, int priority)
{
	// �쐬
	CSceneAnime* p = new CSceneAnime(pDevice, priority);

	p->m_pD3DDevice = pDevice;

	// ������
	p->Init(pos, width, height, texType, sepalateX, sepalateY, animSpd, loopNum);

	return p;
}

//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CSceneAnime::SetIndex(int nIdx)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
	m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
	m_fTop = fV * (m_nIdx / m_nSepalateNumX);
	m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CSceneAnime::SetIndex(int nIdx, bool reverse)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	if (reverse){
		m_fRight = fU * (m_nIdx % m_nSepalateNumX);
		m_fLeft = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}
	else{
		m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
		m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//----EOF----