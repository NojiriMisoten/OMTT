//=============================================================================
//
// エフェクト本体の格納処理 [CEffectHolder.cpp]
// Author : 坂本 友希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectHolder.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
// テクスチャのパス
static const wchar_t *EFFECT_PATH[EFFECT_MAX] =
{
	//足跡(煙)
	L"../data/EFECT/FootStep(smoke).efk",
	//足跡(波)
	L"../data/EFECT/FootStep(wave).efk",
	//オーラ(開始)
	L"../data/EFECT/aura_Invocation.efk",
	//オーラ(ループ)
	L"../data/EFECT/aura_loop.efk",
	//オーラ(終了)
	L"../data/EFECT/aura_end.efk",
	//バウンド時
	L"../data/EFECT/Bound001.efk",
	//弱攻撃
	L"../data/EFECT/dageki_jakku.efk",
	//中攻撃
	L"../data/EFECT/dageki_tyu-.efk",
	//強攻撃
	L"../data/EFECT/dageki_kyo.efk",
	//防御時
	L"../data/EFECT/Defense001.efk",
	//煙(入場時)
	L"../data/EFECT/EnterSmoke001.efk",
	//ジャンプ時
	L"../data/EFECT/Jump001.efk",
	//リングバシコーン
	L"../data/EFECT/RingShock001.efk",
	//テスト用,好きに使って!
	L"../data/EFECT/shock_weve001test.efk",
	//衝撃波
	L"../data/EFECT/ShockWeve001.efk",
	//煙
	L"../data/EFECT/smoke_001.efk",
	//テストその2
	L"../data/EFECT/testdayo-n.efk",
	//投げ
	L"../data/EFECT/Throw001.efk",
	//勝った時のやつ
	L"../data/EFECT/Win001.efk",
	// ホーリーランス
	L"../data/EFECT/HolyLance_Loop.efk"
};

//*****************************************************************************
// スタティックメンバ変数宣言
//*****************************************************************************
bool CEffectHolder::m_bDoneCreateFlag = false;
::Effekseer::Effect* CEffectHolder::m_pEffect[EFFECT_MAX] = { NULL };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffectHolder::Init(void)
{
	// ゲームで使う全エフェクトを作成
	for (int EffectCnt = 0; EffectCnt < EFFECT_MAX; EffectCnt++)
	{
		if (EFFECT_PATH[EffectCnt] != NULL)
		{
			m_pEffect[EffectCnt] = Effekseer::Effect::Create(CEffectManager::GetEffectManager(), (const EFK_CHAR*)EFFECT_PATH[EffectCnt]);
			// エフェクト読み込み
			if (!m_pEffect[EffectCnt])
			{
				assert(!"エフェクトがdataにない！");

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
// 終了処理
//=============================================================================
void CEffectHolder::Uninit(void)
{
	// 使った全エフェクトを開放
	for (int EffectCnt = 0; EffectCnt < EFFECT_MAX; EffectCnt++)
	{
		ES_SAFE_RELEASE(m_pEffect[EffectCnt]);
	}
}

//=============================================================================
// テクスチャ作成処理
//=============================================================================
void CEffectHolder::CreateEffect(void)
{
	CEffectHolder::Init();
}

//=============================================================================
// テクスチャポインタ取得処理
//=============================================================================
::Effekseer::Effect* CEffectHolder::GetEffect(const EFFECT_TYPE type)
{
	// 正しいテクスチャタイプか判定
	if (type < 0 || type > EFFECT_MAX)
	{
		assert(!"不正なエフェクトタイプ！");

		return NULL;
	}
	return m_pEffect[type];
}

//----EOF----