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
#include "../FADE/CFade.h"

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
CScene*		CScene::m_apTop[MAX_LIST_NUM] = { NULL };		// ���X�g�̐擪�A�h���X
CScene*		CScene::m_apCur[MAX_LIST_NUM] = { NULL };		// ���X�g�̏I�[�A�h���X
CScene::FUNC CScene::m_apDrawFunc[MAX_LIST_NUM] = 
{
	&CScene::DrawToonObjectDepthRender,
	&CScene::DrawNormalRender,
	&CScene::DrawNormalVecRender,
	&CScene::DrawUIRender,
	&CScene::DrawShadowRender,
	&CScene::DrawBaseHighLuminanceRender,
	&CScene::DrawFadeRender,
};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CScene ::CScene(OBJTYPE objType)
{
	// �I�u�W�F�^�C�v�ݒ�
	m_objType = objType;

	// �����t���O������
	m_bDelete = false;

	for (int i = 0; i < MAX_LIST_NUM; i++)
	{
		m_bInList[i] = false;
	}
	m_bDoUpdateFlag = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CScene ::~CScene(void)
{
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CScene ::Uninit(void)
{

}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CScene ::Update(void)
{

}

//*****************************************************************************
// �S�čX�V�֐�
//*****************************************************************************
void CScene ::UpdateAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while(pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext[priority];

			// �X�V
			if (!pScene->m_bDoUpdateFlag)
			{
				pScene->Update();
				pScene->m_bDoUpdateFlag = true;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �擪���w��
		pScene = m_apTop[priority];

		// �|�C���^��NULL�łȂ����
		while(pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext[priority];

			// �f�X�t���OON�Ȃ�
			if(pScene->m_bDelete)
			{
				// ���X�g�S�Ă���O��
				pScene->AllUnLinkList();
				
				// �폜
				delete pScene;
				pScene = NULL;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// �w��v���C�I���e�B�X�V�֐�
//*****************************************************************************
void CScene ::UpdateChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	// �擪���w��
	pScene = m_apTop[priority];

	// �|�C���^��NULL�łȂ����
	while(pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->m_pNext[priority];

		// �X�V
		if (!pScene->m_bDoUpdateFlag)
		{
			pScene->Update();
			pScene->m_bDoUpdateFlag = true;
		}

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}

	// �擪���w��
	pScene = m_apTop[priority];

	// �|�C���^��NULL�łȂ����
	while(pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->m_pNext[priority];

		// �f�X�t���OON�Ȃ�
		if(pScene->m_bDelete)
		{
			// ���X�g���珜�O
			pScene->AllUnLinkList();
			
			// �폜
			delete pScene;
			pScene = NULL;
		}

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}
	
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CScene ::Draw(void)
{
}

//*****************************************************************************
// �S�ĕ`��֐�
//*****************************************************************************
void CScene ::DrawAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// �����_�[�^�[�Q�b�g�ύX
		CRenderer::TYPE_RENDER_TEX renderTarget = (CRenderer::TYPE_RENDER_TEX)priority;
		if (renderTarget == CRenderer::TYPE_RENDER_UI)
		{
			renderTarget = CRenderer::TYPE_RENDER_TOON;
		}
		CRenderer::ChangeRenderTarget(renderTarget);

		pScene = m_apTop[priority];	// �|�C���^��NULL�łȂ����
		while(pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext[priority];

			// �`��
			(pScene->*m_apDrawFunc[priority])();
			pScene->m_bDoUpdateFlag = false;

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
		if (renderTarget == CRenderer::TYPE_RENDER_NORMAL_VEC)
		{
			CRenderer::DrawToon();
		}
		CRenderer::ChangeDefaultRenderTarget();
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
void CScene ::ReleaseAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// �擪���w��
		pScene = m_apTop[priority];
		
		// �|�C���^��NULL�łȂ����
		while(pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext[priority];

			// ���
			pScene->Uninit();

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}

		// �擪���w��
		pScene = m_apTop[priority];
		
		// �|�C���^��NULL�łȂ����
		while(pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->m_pNext[priority];

			// �f�X�t���OON�Ȃ�
			if(pScene->m_bDelete)
			{
				pScene->AllUnLinkList();
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
void CScene::UnLinkList(CRenderer::TYPE_RENDER_TEX priority)
{
	CScene* pScene = this;
	pScene->m_bInList[priority] = false;

	if(this->m_pPrev[priority] && this->m_pNext[priority])
	{
		CScene *pPrev = this->m_pPrev[priority];
		CScene *pNext = this->m_pNext[priority];

		// �O�̃C���X�^���X�̎��|�C���^��ύX
		pPrev->m_pNext[priority] = this->m_pNext[priority];
		
		// ���̃C���X�^���X�̑O�|�C���^��ύX
		pNext->m_pPrev[priority] = this->m_pPrev[priority];
		
	}

	// �擪�Ȃ�
	if(this->m_pPrev[priority] == NULL)
	{
		CScene *pNext = this->m_pNext[priority];

		// �擪�A�h���X�ύX
		m_apTop[priority] = pNext;

		if(pNext)
		{
			// �擪��
			pNext->m_pPrev[priority] = NULL;
		}
	}

	// �Ō�Ȃ�
	if(this->m_pNext[priority] == NULL)
	{
		CScene *pPrev = this->m_pPrev[priority];

		// �I�[�A�h���X�ύX
		m_apCur[priority] = pPrev;

		if(pPrev)
		{
			// �I�[��
			pPrev->m_pNext[priority] = NULL;
		}
	}

	// ���݂̃C���X�^���X�����X�g���珜�O
	this->m_pNext[priority] = NULL;
	this->m_pPrev[priority] = NULL;

	pScene->Uninit();
}

//*****************************************************************************
// �����N�ɒǉ��֐�
//*****************************************************************************
void CScene::AddLinkList(CRenderer::TYPE_RENDER_TEX priority)
{
	// ��d�o�^�h�~
	if (m_bInList[priority])
	{
		return;
	}
	m_bInList[priority] = true;

	// �P�ڂ̂Ƃ�
	if (CScene::m_apTop[priority] == NULL)
	{
		CScene::m_apTop[priority] = this;
		CScene::m_apCur[priority] = this;
		m_pNext[priority] = NULL;
		m_pPrev[priority] = NULL;
	}

	// ����ȊO
	else
	{
		// �P�O�̃C���X�^���X�ۑ��p
		CScene *prev = NULL;

		// �P�O�̃C���X�^���X�ۑ�
		prev = CScene::m_apCur[priority];

		// ���݂��I�[�Ȃ̂�NULL
		m_pNext[priority] = NULL;

		// �O�|�C���^�͕ۑ����Ă�����
		m_pPrev[priority] = prev;

		// ���݂��I�[
		m_apCur[priority] = this;

		// �O�|�C���^�̎��|�C���^��ύX
		prev->m_pNext[priority] = this;
	}
}

//*****************************************************************************
// �S�Ẵ��X�g����O���֐�
//*****************************************************************************
void CScene::AllUnLinkList(void)
{
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		if (m_bInList[priority])
		{
			// ���X�g���珜�O
			UnLinkList((CRenderer::TYPE_RENDER_TEX)priority);
		}
	}
}

//----EOF----