//=============================================================================
//
// CSceneクラス [CScene.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CScene.h"
#include "../FADE/CFade.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
CScene*		CScene::m_apTop[MAX_LIST_NUM] = { NULL };		// リストの先頭アドレス
CScene*		CScene::m_apCur[MAX_LIST_NUM] = { NULL };		// リストの終端アドレス
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
// コンストラクタ
//*****************************************************************************
CScene ::CScene(OBJTYPE objType)
{
	// オブジェタイプ設定
	m_objType = objType;

	// 消去フラグ初期化
	m_bDelete = false;

	for (int i = 0; i < MAX_LIST_NUM; i++)
	{
		m_bInList[i] = false;
	}
	m_bDoUpdateFlag = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScene ::~CScene(void)
{
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CScene ::Uninit(void)
{

}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CScene ::Update(void)
{

}

//*****************************************************************************
// 全て更新関数
//*****************************************************************************
void CScene ::UpdateAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext[priority];

			// 更新
			if (!pScene->m_bDoUpdateFlag)
			{
				pScene->Update();
				pScene->m_bDoUpdateFlag = true;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext[priority];

			// デスフラグONなら
			if(pScene->m_bDelete)
			{
				// リスト全てから外す
				pScene->AllUnLinkList();
				
				// 削除
				delete pScene;
				pScene = NULL;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// 指定プライオリティ更新関数
//*****************************************************************************
void CScene ::UpdateChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while(pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext[priority];

		// 更新
		if (!pScene->m_bDoUpdateFlag)
		{
			pScene->Update();
			pScene->m_bDoUpdateFlag = true;
		}

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while(pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext[priority];

		// デスフラグONなら
		if(pScene->m_bDelete)
		{
			// リストから除外
			pScene->AllUnLinkList();
			
			// 削除
			delete pScene;
			pScene = NULL;
		}

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}
	
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CScene ::Draw(void)
{
}

//*****************************************************************************
// 全て描画関数
//*****************************************************************************
void CScene ::DrawAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// レンダーターゲット変更
		CRenderer::TYPE_RENDER_TEX renderTarget = (CRenderer::TYPE_RENDER_TEX)priority;
		if (renderTarget == CRenderer::TYPE_RENDER_UI)
		{
			renderTarget = CRenderer::TYPE_RENDER_TOON;
		}
		CRenderer::ChangeRenderTarget(renderTarget);

		pScene = m_apTop[priority];	// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext[priority];

			// 描画
			(pScene->*m_apDrawFunc[priority])();
			pScene->m_bDoUpdateFlag = false;

			// 次のインスタンスを対象のインスタンスにする
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
// リリース関数
//*****************************************************************************
void CScene::Release(void)
{
	// デスフラグON
	m_bDelete = true;
}

//*****************************************************************************
// 全てリリース関数
//*****************************************************************************
void CScene ::ReleaseAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];
		
		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext[priority];

			// 解放
			pScene->Uninit();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];
		
		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext[priority];

			// デスフラグONなら
			if(pScene->m_bDelete)
			{
				pScene->AllUnLinkList();
				// 解放
				delete pScene;
				pScene = NULL;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭,終端アドレス初期化
		m_apTop[priority] = NULL;
		m_apCur[priority] = NULL;
	}
}

//*****************************************************************************
// リストから除外関数
//*****************************************************************************
void CScene::UnLinkList(CRenderer::TYPE_RENDER_TEX priority)
{
	CScene* pScene = this;
	pScene->m_bInList[priority] = false;

	if(this->m_pPrev[priority] && this->m_pNext[priority])
	{
		CScene *pPrev = this->m_pPrev[priority];
		CScene *pNext = this->m_pNext[priority];

		// 前のインスタンスの次ポインタを変更
		pPrev->m_pNext[priority] = this->m_pNext[priority];
		
		// 次のインスタンスの前ポインタを変更
		pNext->m_pPrev[priority] = this->m_pPrev[priority];
		
	}

	// 先頭なら
	if(this->m_pPrev[priority] == NULL)
	{
		CScene *pNext = this->m_pNext[priority];

		// 先頭アドレス変更
		m_apTop[priority] = pNext;

		if(pNext)
		{
			// 先頭へ
			pNext->m_pPrev[priority] = NULL;
		}
	}

	// 最後なら
	if(this->m_pNext[priority] == NULL)
	{
		CScene *pPrev = this->m_pPrev[priority];

		// 終端アドレス変更
		m_apCur[priority] = pPrev;

		if(pPrev)
		{
			// 終端に
			pPrev->m_pNext[priority] = NULL;
		}
	}

	// 現在のインスタンスをリストから除外
	this->m_pNext[priority] = NULL;
	this->m_pPrev[priority] = NULL;

	pScene->Uninit();
}

//*****************************************************************************
// リンクに追加関数
//*****************************************************************************
void CScene::AddLinkList(CRenderer::TYPE_RENDER_TEX priority)
{
	// 二重登録防止
	if (m_bInList[priority])
	{
		return;
	}
	m_bInList[priority] = true;

	// １個目のとき
	if (CScene::m_apTop[priority] == NULL)
	{
		CScene::m_apTop[priority] = this;
		CScene::m_apCur[priority] = this;
		m_pNext[priority] = NULL;
		m_pPrev[priority] = NULL;
	}

	// それ以外
	else
	{
		// １つ前のインスタンス保存用
		CScene *prev = NULL;

		// １つ前のインスタンス保存
		prev = CScene::m_apCur[priority];

		// 現在が終端なのでNULL
		m_pNext[priority] = NULL;

		// 前ポインタは保存してあるやつ
		m_pPrev[priority] = prev;

		// 現在が終端
		m_apCur[priority] = this;

		// 前ポインタの次ポインタを変更
		prev->m_pNext[priority] = this;
	}
}

//*****************************************************************************
// 全てのリストから外れる関数
//*****************************************************************************
void CScene::AllUnLinkList(void)
{
	for (int priority = 0; priority < MAX_LIST_NUM; priority++)
	{
		if (m_bInList[priority])
		{
			// リストから除外
			UnLinkList((CRenderer::TYPE_RENDER_TEX)priority);
		}
	}
}

//----EOF----