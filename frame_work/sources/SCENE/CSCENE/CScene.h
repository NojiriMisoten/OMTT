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
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "../../CONST/const.h"
#include "../../MATH/math.h"

//*****************************************************************************
// 列挙体
//*****************************************************************************
// プライオリティの種類
typedef enum
{
	TYPE_PRIORITY_0 = 0,
	TYPE_PRIORITY_BG,					// BG
	TYPE_PRIORITY_FIELD,				// フィールド
	TYPE_PRIORITY_GOAL,					// ゴール
	TYPE_PRIORITY_THREAD_OF_FOOTHOLD,	// 足場になる糸
	TYPE_PRIORITY_THREAD_OF_TRAP,		// 罠の糸
	TYPE_PRIORITY_TRESURE,				// 宝
	TYPE_PRIORITY_PLAYER,				// プレイヤー
	TYPE_PRIORITY_ATTACK,				// 攻撃
	TYPE_PRIORITY_EFFECT,				// エフェクト
	TYPE_PRIORITY_UI,					// UI
	TYPE_PRIORITY_PAUSE,
	TYPE_PRIORITY_FADE,
	TYPE_PRIORITY_MAX
}TYPE_PRIORITY;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// UV値構造体
typedef struct
{
	float left;
	float right;
	float top;
	float bottom;
}UV_INDEX;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{
public:
	// オブジェタイプ
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D
		OBJTYPE_PLAYER,				// プレイヤー
		OBJTYPE_ATTACK,				// 攻撃
		OBJTYPE_THREAD,				// 糸
		OBJTYPE_FIELD,				// フィールド
		OBJTYPE_TREASURE,			// 宝箱
		OBJTYPE_SCORE,				// スコア
		OBJTYPE_GOAL,				// GOAL
		OBJTYPE_MAX					// オブジェタイプ総数
	}OBJTYPE;

	//======================================================
	// コンストラクタ
	// 引数: プライオリティ、オブジェタイプ
	//======================================================
	CScene(int nPriority = DEFAULT_PRIORITY, OBJTYPE m_objType = OBJTYPE_NONE);

	// デストラクタ
	virtual ~CScene() = 0;

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	virtual HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1) = 0;
	
	// 終了
	virtual void Uninit(void) = 0;

	// 更新
	virtual void Update(void) = 0;

	// 描画
	virtual void Draw(void) = 0;

	// ポジションアクセサ
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual void		SetPos(D3DXVECTOR3 pos) = 0;
	virtual void		SetPos(float x,float y, float z) = 0;

	// オールドポジションアクセサ
	virtual D3DXVECTOR3 GetOldPos(void) {return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual void		SetOldPos(D3DXVECTOR3 pos) {};
	virtual void		SetOldPos(float x,float y, float z) {};

	// ロットアクセサ
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual void		SetRot(D3DXVECTOR3 rot) = 0;
	virtual void		SetRot(float x,float y,float z) = 0;

	// オールドロットアクセサ
	virtual D3DXVECTOR3 GetOldRot(void){return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual void		SetOldRot(D3DXVECTOR3 rot){};
	virtual void		SetOldRot(float x,float y,float z){};

	// バウンディングボックス用アクセサ
	virtual D3DXVECTOR3 GetVertexMin(void){return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual D3DXVECTOR3 GetVertexMax(void){return D3DXVECTOR3(0.f,0.f,0.f);};

	// 自身リリース関数
	void Release(void);

	// リンクから除外関数
	void UnLinkList(void);

	//=================================================
	// オブジェタイプゲット関数
	// 戻り値: オブジェタイプ
	//=================================================
	OBJTYPE GetObjType(void){return m_objType;};
	
	//=================================================
	// オブジェタイプセット関数
	// 引数: オブジェタイプ
	//=================================================
	void SetObjType(OBJTYPE type);

	//=================================================
	// リストの先頭アドレスゲット関数
	// 引数: プライオリティ
	// 戻り値: 指定プライオリティの先頭アドレス
	//=================================================
	static CScene* GetTopAddress(int priority){return m_apTop[priority];};

	//=================================================
	// 自身の次のアドレスゲット
	// 戻り値: 次のアドレス
	//=================================================
	CScene* GetNextAddress(void){return m_pNext;};

	// 全て更新
	static void UpdateAll(void);

	//=================================================
	// 指定したプライオリティのみ更新
	// 引数: プライオリティ
	//=================================================
	static void UpdateChoice(int priority);

	// 全て更新
	static void DrawAll(void);

	//=================================================
	// 指定したプライオリティのみ描画
	// 引数: プライオリティ
	//=================================================
	static void DrawChoice(int priority);

	// 全てリリース
	static void ReleaseAll(void);

private:
	// Zソート
	static void ZSort(void);

	static CScene		*m_apTop[TYPE_PRIORITY_MAX];	// リストの先頭アドレス
	static CScene		*m_apCur[TYPE_PRIORITY_MAX];	// リストの終端アドレス
	static int			m_nNumInList[TYPE_PRIORITY_MAX];// リストの中身の個数
	CScene				*m_pPrev;						// 前のオブジェクトへのポインタ
	CScene				*m_pNext;						// 次のオブジェクトへのポインタ
	CScene				*m_pDrawPrev;					// 前のオブジェクトへのポインタ
	CScene				*m_pDrawNext;					// 次のオブジェクトへのポインタ
	int					m_nPriority;					// プライオリティ
	bool				m_bDelete;						// 消去フラグ
	OBJTYPE				m_objType;						// タイプ
};

#endif
//----EOF----