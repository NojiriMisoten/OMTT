//=============================================================================
//
// CScene�N���X [CScene.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CScene.h"
#include "../../FADE/CFade.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	CScene* adrr;
	D3DXVECTOR3 pos;
}SORT_INFO;

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
CScene*		CScene::m_apTop[TYPE_PRIORITY_MAX] = { NULL };		// ���X�g�̐擪�A�h���X
CScene*		CScene::m_apCur[TYPE_PRIORITY_MAX] = { NULL };		// ���X�g�̏I�[�A�h���X
int			CScene::m_nNumInList[TYPE_PRIORITY_MAX] = { 0 };		// ���X�g�̒��g�̌�

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// �I�u�W�F�^�C�v�ݒ�
	m_objType = objType;

	// �v���C�I���e�B�ݒ�
	m_nPriority = nPriority;

	// �����t���O������
	m_bDelete = false;

	// ���J�E���g
	if (m_nPriority >= 0 && m_nPriority < TYPE_PRIORITY_MAX)
	{
		m_nNumInList[m_nPriority]++;
	}

	// �P�ڂ̂Ƃ�
	if (CScene::m_apTop[nPriority] == NULL)
	{
		CScene::m_apTop[nPriority] = this;
		CScene::m_apCur[nPriority] = this;
		m_pNext = NULL;
		m_pPrev = NULL;
	}

	// ����ȊO
	else
	{
		// �P�O�̃C���X�^���X�ۑ��p
		CScene *prev = NULL;

		// �P�O�̃C���X�^���X�ۑ�
		prev = CScene::m_apCur[nPriority];

		// ���݂��I�[�Ȃ̂�NULL
		m_pNext = NULL;

		// �O�|�C���^�͕ۑ����Ă�����
		m_pPrev = prev;

		// ���݂��I�[
		m_apCur[nPriority] = this;

		// �O�|�C���^�̎��|�C���^��ύX
		prev->m_pNext = this;
	}

	// �`��p���X�g�̏�񏉊���
	m_pDrawNext = NULL;
	m_pDrawPrev = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CScene ::~CScene(void)
{
	// ���J�E���g
	if (m_nPriority >= 0 && m_nPriority < TYPE_PRIORITY_MAX)
	{
		m_nNumInList[m_nPriority]--;
	}
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CScene::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CScene::Uninit(void)
{

}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CScene::Update(void)
{

}

//*****************************************************************************
// �S�čX�V�֐�
//*****************************************************************************
void CScene::UpdateAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// �X�V
			pScene->Update();

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// �f�X�t���OON�Ȃ�
			if (pScene->m_bDelete)
			{
				// ���X�g���珜�O
				pScene->UnLinkList();

				// �폜
				delete pScene;
				pScene = NULL;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}

	// Z�\�[�g
	if (!GetDrawFlag())
	{
		CScene::ZSort();
	}
}

//*****************************************************************************
// �w��v���C�I���e�B�X�V�֐�
//*****************************************************************************
void CScene::UpdateChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	// �擪���w��
	pScene = m_apTop[priority];

	// �|�C���^��NULL�łȂ����
	while (pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->m_pNext;

		// �X�V
		pScene->Update();

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}

	// �擪���w��
	pScene = m_apTop[priority];

	// �|�C���^��NULL�łȂ����
	while (pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->m_pNext;

		// �f�X�t���OON�Ȃ�
		if (pScene->m_bDelete)
		{
			// ���X�g���珜�O
			pScene->UnLinkList();

			// �폜
			delete pScene;
			pScene = NULL;
		}

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}

	// Z�\�[�g
	if (!GetDrawFlag())
	{
		CScene::ZSort();
	}
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CScene::Draw(void)
{
}

//*****************************************************************************
// �S�ĕ`��֐�
//*****************************************************************************
void CScene::DrawAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		pScene = m_apTop[priority];	// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// �`��
			pScene->Draw();

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// �w��v���C�I���e�B�`��֐�
//*****************************************************************************
void CScene::DrawChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	pScene = m_apTop[priority];

	// �|�C���^��NULL�łȂ����
	while (pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->m_pNext;

		// �`��
		pScene->Draw();

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}

}

//*****************************************************************************
// �����[�X�֐�
//*****************************************************************************
void CScene::Release(void)
{
	// �f�X�t���OON
	m_bDelete = true;
}

//*****************************************************************************
// �S�ă����[�X�֐�
//*****************************************************************************
void CScene::ReleaseAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// ���
			pScene->Uninit();

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// �f�X�t���OON�Ȃ�
			if (pScene->m_bDelete)
			{
				// ���
				delete pScene;
				pScene = NULL;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �擪,�I�[�A�h���X������
		m_apTop[priority] = NULL;
		m_apCur[priority] = NULL;
	}
}

//*****************************************************************************
// ���X�g���珜�O�֐�
//*****************************************************************************
void CScene::UnLinkList(void)
{
	CScene* pScene = this;

	if (this->m_pPrev && this->m_pNext)
	{
		CScene *pPrev = this->m_pPrev;
		CScene *pNext = this->m_pNext;

		// �O�̃C���X�^���X�̎��|�C���^��ύX
		pPrev->m_pNext = this->m_pNext;

		// ���̃C���X�^���X�̑O�|�C���^��ύX
		pNext->m_pPrev = this->m_pPrev;

	}

	// �擪�Ȃ�
	if (this->m_pPrev == NULL)
	{
		CScene *pNext = this->m_pNext;

		// �擪�A�h���X�ύX
		m_apTop[this->m_nPriority] = NULL;

		if (pNext)
		{
			// �擪�A�h���X�ύX
			m_apTop[this->m_nPriority] = pNext;

			// �擪��
			pNext->m_pPrev = NULL;
		}
	}

	// �Ō�Ȃ�
	if (this->m_pNext == NULL)
	{
		CScene *pPrev = this->m_pPrev;

		// �I�[�A�h���X�ύX
		m_apCur[this->m_nPriority] = NULL;

		if (pPrev)
		{
			// �I�[�A�h���X�ύX
			m_apCur[this->m_nPriority] = pPrev;

			// �I�[��
			pPrev->m_pNext = NULL;
		}
	}

	// ���݂̃C���X�^���X�����X�g���珜�O
	this->m_pNext = NULL;
	this->m_pPrev = NULL;

	pScene->Uninit();
}

//*****************************************************************************
// Z�\�[�g�֐�
//*****************************************************************************
void CScene::ZSort(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// ���g���Ȃ��Ȃ玟�̃v���C�I���e�B��
		if (m_nNumInList[priority] <= 0)
		{
			continue;
		}

		// �\�[�g�p
		SORT_INFO *apSortCulc = new SORT_INFO[m_nNumInList[priority]];
		int culc = 0;

		pScene = m_apTop[priority];	// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// ���ۑ�
			apSortCulc[culc].adrr = pScene;
			apSortCulc[culc].pos = pScene->GetPos();

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;

			// ����
			culc++;
		}

		// �\�[�g
		for (int i = 0; i < m_nNumInList[priority]; i++)
		{
			for (int j = m_nNumInList[priority] - 1; j > i; j--)
			{
				// �߂����
				float culc = apSortCulc[j].pos.y - apSortCulc[j - 1].pos.y;
				if (culc < -5.0f)
				{
					// ����ւ�����
					SORT_INFO keep = apSortCulc[j - 1];
					apSortCulc[j - 1] = apSortCulc[j];
					apSortCulc[j] = keep;
				}
			}
		}

		// �擪�E�I�[�A�h���X�ύX
		m_apTop[priority] = apSortCulc[0].adrr;
		m_apCur[priority] = apSortCulc[m_nNumInList[priority] - 1].adrr;

		// ���X�g�̕ύX
		pScene = m_apTop[priority];
		for (int i = 0; i < m_nNumInList[priority]; i++)
		{
			if (!pScene)
			{
				continue;
			}

			// ���̃A�h���X�ύX
			if (i < m_nNumInList[priority] - 1)
			{
				pScene->m_pNext = apSortCulc[i + 1].adrr;
			}
			else
			{
				pScene->m_pNext = NULL;
			}

			// �O�̃A�h���X�ύX
			if (i > 0)
			{
				pScene->m_pPrev = apSortCulc[i - 1].adrr;
			}
			else
			{
				pScene->m_pPrev = NULL;
			}

			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext;

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �����g��Ȃ��̂ŉ��
		delete[] apSortCulc;
		apSortCulc = NULL;
	}
}


//*****************************************************************************
// �I�u�W�F�^�C�v�Z�b�g�֐�
//*****************************************************************************
void CScene::SetObjType(OBJTYPE type)
{
	m_objType = type;
}
//----EOF----