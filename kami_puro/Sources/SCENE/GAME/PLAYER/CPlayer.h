//=============================================================================
//
// CPlayerクラス [CPlayer.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CPLAYER_H_
#define _CPLAYER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CSceneX.h"
#include "../../../SKINMESH/CSkinMesh.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CPlayer;

//*****************************************************************************
// アニメーションコールバック用ハンドラークラス定義
// キャラクターごとに持たないといけない
//*****************************************************************************
class CCallBackHandlerPlayer : public ID3DXAnimationCallbackHandler
{
public:
	// コールバック関数
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	CPlayer* m_pMyAddress;	// 自身のアドレス
};

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const int DEFFAULT_JAMP_POWER = 3;							// ジャンプの力
static const int DEFAULT_HP_PARAMETER = 1000;						// HPの量
static const int MOVE_HEAL_AMOUNT = 1;								// 後ろ移動時回復量

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CSceneX
{
public:
	typedef enum
	{
		PLAYER_FINISH = 0,//
		PLAYER_FINISH_DAMAGE,//
		PLAYER_ELBOW_DAMAGE,//
		PLAYER_STUNNER_RIGHT,//
		PLAYER_STUNNER_DAMAGE_RIGHT,//
		PLAYER_DROP_KICK_LEFT,//
		PLAYER_DROP_KICK_DAMAGE_LEFT,//
		PLAYER_FLYING_SHOULDER_ATTACK_LEFT,//
		PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT,//
		PLAYER_ROLLING_ELBOW_LEFT,//
		PLAYER_ROLLING_ELBOW_DAMAGE_LEFT,//
		PLAYER_LARIAT_LEFT,			//
		PLAYER_LARIAT_DAMAGE,			//
		PLAYER_BACKDROP,				//
		PLAYER_BACKDROP_DAMAGE,			//
		PLAYER_SLAPPING_RIGHT,			//
		PLAYER_SLAPPING_DAMAGE_RIGHT,//
		PLAYER_WAIT,					//
		PLAYER_LARIAT_RIGHT,			//
		PLAYER_ELBOW_LEFT,				//
		PLAYER_ELBOW_RIGHT,				//
		PLAYER_DAMAGE_SMALL,			//
		PLAYER_CHOP_LEFT,				//
		PLAYER_CHOP_RIGHT,				//
		PLAYER_ANIM_MAX
	}PLAYER_ANIM_TYPE;

	typedef enum
	{
		RENDERER_TYPE_NORMAL = 0,
		RENDERER_TYPE_NORMAL_VEC,
		RENDERER_TYPE_DEPTH,
		RENDERER_TYPE_MAX
	}PLAYER_RENDERER_TYPE;

	// コンストラクタ
	CPlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// デストラクタ
	~CPlayer(void);

	// 作成
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);
	void DrawNormalVecRender(void);
	void DrawToonObjectDepthRender(void);

	// スキンメッシュで計算したワールドマトリクスをもとにセットする
	void SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type);
	void SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx);
	void SetWorldMtxForNormalVecRender(D3DXMATRIX* worldMtx);
	void SetWorldMtxForToonObjectDepthRender(D3DXMATRIX* worldMtx);

	int GetHP(void);
	PLAYER_ANIM_TYPE GetAnimState(void);
	
	// アニメーション変更
	void SetAnimType(int type, double moveRate = DEFFAULT_CHANGE_ANIM_SPD);

	// アニメーションを時間の状態に指定
	void SetAnimMortionOfTime(int percent);

	// アニメーション速度セット
	void SetAnimSpd(double spd);

	// ダメージ処理
	void TakeDamage( int damage );

	// 回復処理
	void TakeHeal( int heal );

	// FINISH技使用可能フラグアクセサ
	bool GetUseFinishFlag(void){ return m_isUseFinish; };
	void SetUseFinishFlag(bool flag){ m_isUseFinish = flag; };

	// ロープフラグアクセサ
	bool GetRopeFlag(void){ return m_isRope; };
	void SetRopeFlag(bool flag){ m_isRope = flag; };

	void SetPos(D3DXVECTOR3& pos) { 
		m_Pos = pos;
		m_DestPos = pos; 
	};

private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// 移動
	D3DXVECTOR3 Move(void);

	// 移動フェーズ
	void MovePhase(void);

	// 攻撃フェーズ
	void AttackPhase(void);

	// プレイヤージャンプ
	void PlayerJamp(void);

	CManager			*m_pManager;			// マネージャー
	D3DXVECTOR3			m_vecFront;				// 前ベクトル
	D3DXVECTOR3			m_vecRight;				// 右ベクトル
	D3DXVECTOR3			m_vecUp;				// 上ベクトル
	D3DXVECTOR3			m_DestRot;				// 目標角度
	D3DXVECTOR3			m_DestPos;				// 目標座標
	D3DXVECTOR3			m_MovPos;				// 移動速度
	D3DXMATRIX			m_OldWorldMtx;			// 以前のワールドマトリックス

	CSkinMesh				*m_pCSkinMesh;			// スキンメッシュアニメーション用
	CCallBackHandlerPlayer	*m_pFunc;				// アニメーションコールバック用
	CALLBACK_TIMING			*m_pCallBackTimiming;	// コールバックのタイミング

	LPDIRECT3DVERTEXSHADER9	*m_pVS;
	LPD3DXCONSTANTTABLE		*m_pVSC;
	LPDIRECT3DPIXELSHADER9	*m_pPS;
	LPD3DXCONSTANTTABLE		*m_pPSC;

	int					m_ID;					// プレイヤーのID
	int					m_HP;					// プレイヤーのHP、
	int					m_JampPower;			// ジャンプの瞬間的なパワー
	bool				m_JampFlag;				// ジャンプするためのフラグ
	PLAYER_ANIM_TYPE	m_AnimState;			// アニメの状態
	bool				m_isUseFinish;			// FINISH技使用可能かフラグ
	bool				m_isRope;				// ロープ中フラグ
};

#endif
//----EOF----