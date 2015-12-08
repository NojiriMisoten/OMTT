//=============================================================================
//
// CSceneクラス [CScene.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENE_H_
#define _CSCENE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "../TEXTURE/CTexture.h"
#include "../CONST/const.h"
#include "../MATH/math.h"
#include "../RENDERER/CRenderer.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{
public:
	typedef void (CScene::*FUNC)(void);

	// オブジェタイプ
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D
		OBJTYPE_3D,					// 3D
		OBJTYPE_X,					// Xモデル
		OBJTYPE_ENEMY,				// 敵
		OBJTYPE_PLAYER,				// プレイヤー
		OBJTYPE_FIELD,				// フィールド
		OBJTYPE_LIGHT,				// ライト
		OBJTYPE_MAX					// オブジェタイプ総数
	}OBJTYPE;

	//======================================================
	// コンストラクタ
	// 引数: プライオリティ、オブジェタイプ
	//======================================================
	CScene(OBJTYPE m_objType = OBJTYPE_NONE);

	// デストラクタ
	virtual ~CScene() = 0;

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	void Init(void){};
	
	// 終了
	virtual void Uninit(void) = 0;

	// 更新
	virtual void Update(void) = 0;

	// 描画
	virtual void Draw(void);

	// ポジションアクセサ
	virtual D3DXVECTOR3& GetPos(void){ return m_Pos; };
	virtual void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	virtual void		SetPos(float& x, float& y, float& z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ロットアクセサ
	virtual D3DXVECTOR3& GetRot(void){ return m_Rot; };
	virtual void		SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	virtual void		SetRot(float& x, float& y, float& z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };
	
	// 自身リリース関数
	void Release(void);

	// リンクに追加関数
	void AddLinkList(CRenderer::TYPE_RENDER_TEX priority);

	// リンクから除外関数
	void UnLinkList(CRenderer::TYPE_RENDER_TEX priority);

	//=================================================
	// オブジェタイプゲット関数
	// 戻り値: オブジェタイプ
	//=================================================
	OBJTYPE GetObjType(void){return m_objType;};
	
	//=================================================
	// オブジェタイプセット関数
	// 引数: オブジェタイプ
	//=================================================
	void SetObjType(OBJTYPE type){m_objType = type;};

	//=================================================
	// リストの先頭アドレスゲット関数
	// 引数: プライオリティ
	// 戻り値: 指定プライオリティの先頭アドレス
	//=================================================
	static CScene* GetTopAddress(int priority){return m_apTop[priority];};

	//=================================================
	// 自身の次のアドレスゲット
	// 引数: プライオリティ
	// 戻り値: 次のアドレス
	//=================================================
	CScene* GetNextAddress(int priority){ return m_pNext[priority]; };

	// 全て更新
	static void UpdateAll(void);

	//=================================================
	// 指定したプライオリティのみ更新
	// 引数: プライオリティ
	//=================================================
	static void UpdateChoice(int priority);

	// 全て描画
	static void DrawAll(void);

	// 全て解放
	static void ReleaseAll(void);

	// CRenderer.hのenumと同じ順番に関数ポインタに入れる
	virtual void DrawToonObjectDepthRender(void){};
	virtual void DrawNormalRender(void){};
	virtual void DrawNormalVecRender(void){};
	virtual void DrawUIRender(void){};
	virtual void DrawShadowRender(void){};
	virtual void DrawBaseHighLuminanceRender(void){};
	virtual void DrawFadeRender(void){};

	//=======================================================================
	// 自動描画フラグセット関数
	// 引数:真偽値
	//=======================================================================
	void SetDrawFlag(bool flag){ m_AutoDraw = flag; };

	//=======================================================================
	// 自動描画フラグゲットット関数
	// 戻り値:真偽値
	//=======================================================================
	bool GetDrawFlag(void){ return m_AutoDraw; };
private:
	// 全てのリストから外れる
	void AllUnLinkList(void);

	static const int MAX_LIST_NUM = CRenderer::TYPE_RENDER_MAX - 7;	// リストの数

	static CScene				*m_apTop[MAX_LIST_NUM];	// リストの先頭アドレス
	static CScene				*m_apCur[MAX_LIST_NUM];	// リストの終端アドレス
	CScene						*m_pPrev[MAX_LIST_NUM];	// 前のオブジェクトへのポインタ
	CScene						*m_pNext[MAX_LIST_NUM];	// 次のオブジェクトへのポインタ
	bool						m_bDelete;				// 消去フラグ
	OBJTYPE						m_objType;				// タイプ
	bool						m_bInList[MAX_LIST_NUM];// リスト登録してあるか
	bool						m_bDoUpdateFlag;		// 既に更新したか

	// 関数ポインタテーブル
	static FUNC m_apDrawFunc[MAX_LIST_NUM];

protected:
	D3DXVECTOR3					m_Pos;		// 座標
	D3DXVECTOR3					m_Rot;		// 角度
	bool		m_AutoDraw;				// 自動で描画するか
};

#endif
//----EOF----