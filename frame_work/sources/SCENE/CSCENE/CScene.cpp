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
#include "../../FADE/CFade.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	CScene* adrr;
	D3DXVECTOR3 pos;
}SORT_INFO;

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
CScene*		CScene::m_apTop[TYPE_PRIORITY_MAX] = { NULL };		// リストの先頭アドレス
CScene*		CScene::m_apCur[TYPE_PRIORITY_MAX] = { NULL };		// リストの終端アドレス
int			CScene::m_nNumInList[TYPE_PRIORITY_MAX] = { 0 };		// リストの中身の個数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// オブジェタイプ設定
	m_objType = objType;

	// プライオリティ設定
	m_nPriority = nPriority;

	// 消去フラグ初期化
	m_bDelete = false;

	// 個数カウント
	if (m_nPriority >= 0 && m_nPriority < TYPE_PRIORITY_MAX)
	{
		m_nNumInList[m_nPriority]++;
	}

	// １個目のとき
	if (CScene::m_apTop[nPriority] == NULL)
	{
		CScene::m_apTop[nPriority] = this;
		CScene::m_apCur[nPriority] = this;
		m_pNext = NULL;
		m_pPrev = NULL;
	}

	// それ以外
	else
	{
		// １つ前のインスタンス保存用
		CScene *prev = NULL;

		// １つ前のインスタンス保存
		prev = CScene::m_apCur[nPriority];

		// 現在が終端なのでNULL
		m_pNext = NULL;

		// 前ポインタは保存してあるやつ
		m_pPrev = prev;

		// 現在が終端
		m_apCur[nPriority] = this;

		// 前ポインタの次ポインタを変更
		prev->m_pNext = this;
	}

	// 描画用リストの情報初期化
	m_pDrawNext = NULL;
	m_pDrawPrev = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScene ::~CScene(void)
{
	// 個数カウント
	if (m_nPriority >= 0 && m_nPriority < TYPE_PRIORITY_MAX)
	{
		m_nNumInList[m_nPriority]--;
	}
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CScene::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CScene::Uninit(void)
{

}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CScene::Update(void)
{

}

//*****************************************************************************
// 全て更新関数
//*****************************************************************************
void CScene::UpdateAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 更新
			pScene->Update();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// デスフラグONなら
			if (pScene->m_bDelete)
			{
				// リストから除外
				pScene->UnLinkList();

				// 削除
				delete pScene;
				pScene = NULL;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// Zソート
	if (!GetDrawFlag())
	{
		CScene::ZSort();
	}
}

//*****************************************************************************
// 指定プライオリティ更新関数
//*****************************************************************************
void CScene::UpdateChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while (pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// 更新
		pScene->Update();

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while (pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// デスフラグONなら
		if (pScene->m_bDelete)
		{
			// リストから除外
			pScene->UnLinkList();

			// 削除
			delete pScene;
			pScene = NULL;
		}

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}

	// Zソート
	if (!GetDrawFlag())
	{
		CScene::ZSort();
	}
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CScene::Draw(void)
{
}

//*****************************************************************************
// 全て描画関数
//*****************************************************************************
void CScene::DrawAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		pScene = m_apTop[priority];	// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 描画
			pScene->Draw();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// 指定プライオリティ描画関数
//*****************************************************************************
void CScene::DrawChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while (pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// 描画
		pScene->Draw();

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
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
void CScene::ReleaseAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 解放
			pScene->Uninit();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// デスフラグONなら
			if (pScene->m_bDelete)
			{
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
void CScene::UnLinkList(void)
{
	CScene* pScene = this;

	if (this->m_pPrev && this->m_pNext)
	{
		CScene *pPrev = this->m_pPrev;
		CScene *pNext = this->m_pNext;

		// 前のインスタンスの次ポインタを変更
		pPrev->m_pNext = this->m_pNext;

		// 次のインスタンスの前ポインタを変更
		pNext->m_pPrev = this->m_pPrev;

	}

	// 先頭なら
	if (this->m_pPrev == NULL)
	{
		CScene *pNext = this->m_pNext;

		// 先頭アドレス変更
		m_apTop[this->m_nPriority] = NULL;

		if (pNext)
		{
			// 先頭アドレス変更
			m_apTop[this->m_nPriority] = pNext;

			// 先頭へ
			pNext->m_pPrev = NULL;
		}
	}

	// 最後なら
	if (this->m_pNext == NULL)
	{
		CScene *pPrev = this->m_pPrev;

		// 終端アドレス変更
		m_apCur[this->m_nPriority] = NULL;

		if (pPrev)
		{
			// 終端アドレス変更
			m_apCur[this->m_nPriority] = pPrev;

			// 終端に
			pPrev->m_pNext = NULL;
		}
	}

	// 現在のインスタンスをリストから除外
	this->m_pNext = NULL;
	this->m_pPrev = NULL;

	pScene->Uninit();
}

//*****************************************************************************
// Zソート関数
//*****************************************************************************
void CScene::ZSort(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 中身がないなら次のプライオリティへ
		if (m_nNumInList[priority] <= 0)
		{
			continue;
		}

		// ソート用
		SORT_INFO *apSortCulc = new SORT_INFO[m_nNumInList[priority]];
		int culc = 0;

		pScene = m_apTop[priority];	// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 情報保存
			apSortCulc[culc].adrr = pScene;
			apSortCulc[culc].pos = pScene->GetPos();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;

			// 次へ
			culc++;
		}

		// ソート
		for (int i = 0; i < m_nNumInList[priority]; i++)
		{
			for (int j = m_nNumInList[priority] - 1; j > i; j--)
			{
				// 近ければ
				float culc = apSortCulc[j].pos.y - apSortCulc[j - 1].pos.y;
				if (culc < -5.0f)
				{
					// 入れ替え処理
					SORT_INFO keep = apSortCulc[j - 1];
					apSortCulc[j - 1] = apSortCulc[j];
					apSortCulc[j] = keep;
				}
			}
		}

		// 先頭・終端アドレス変更
		m_apTop[priority] = apSortCulc[0].adrr;
		m_apCur[priority] = apSortCulc[m_nNumInList[priority] - 1].adrr;

		// リストの変更
		pScene = m_apTop[priority];
		for (int i = 0; i < m_nNumInList[priority]; i++)
		{
			if (!pScene)
			{
				continue;
			}

			// 次のアドレス変更
			if (i < m_nNumInList[priority] - 1)
			{
				pScene->m_pNext = apSortCulc[i + 1].adrr;
			}
			else
			{
				pScene->m_pNext = NULL;
			}

			// 前のアドレス変更
			if (i > 0)
			{
				pScene->m_pPrev = apSortCulc[i - 1].adrr;
			}
			else
			{
				pScene->m_pPrev = NULL;
			}

			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// もう使わないので解放
		delete[] apSortCulc;
		apSortCulc = NULL;
	}
}


//*****************************************************************************
// オブジェタイプセット関数
//*****************************************************************************
void CScene::SetObjType(OBJTYPE type)
{
	m_objType = type;
}
//----EOF----