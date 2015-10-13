//=============================================================================
//
// CSceneCircleクラス [CSceneCircle.cpp]
// Author : 野尻　	
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneCircle.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const short CIRCLE_VERTEX_NUM = 128;
static const short CIRCLE_PRIMITIVE_NUM = CIRCLE_VERTEX_NUM - 2;
static const float CIRCLE_VERTEX_OFFSET = D3DX_PI * -0.5f;
static const float CIRCLE_RADIAN_MAX = 2.0f * D3DX_PI;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneCircle::CSceneCircle(LPDIRECT3DDEVICE9 *pDevice) :CScene2D(pDevice, TYPE_PRIORITY_UI, OBJTYPE_2D)
{
	m_pD3DDevice = pDevice;
	m_bDraw = true;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneCircle ::~CSceneCircle(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneCircle::Init(D3DXVECTOR3 pos, float radius, D3DXCOLOR color)
{
	// 値代入
	m_vPos = pos;
	m_fRadius = radius;
	m_Color = color;
	m_fRad = CIRCLE_RADIAN_MAX;

	// ポリゴンのメモリ確保
	if (CreateVertexBuff() == E_FAIL){
		return E_FAIL;
	}

	// ポリゴンの設定
	SetVertexBuff();

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneCircle ::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneCircle ::Update(void)
{
	m_fRad -= 0.01f;
	SetVertexBuffVtx();
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneCircle ::Draw(void)
{
	// 描画フラグ
	if (!m_bDraw){
		return;
	}

	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, CIRCLE_PRIMITIVE_NUM);
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneCircle* CSceneCircle::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float radius, D3DXCOLOR color, float fMaxValue)
{
	// 作成
	CSceneCircle* p = new CSceneCircle(pDevice);

	// 初期化
	p->Init(pos, radius, color);

	// 最大値に対する数値の割合を計算
	p->CalcRatio(fMaxValue);
	return p;
}

//*****************************************************************************
// 円のおおきさ更新
//*****************************************************************************
void CSceneCircle::ChangeSize(float fValue)
{
	// セットされた値を角度にして大きくする
	m_fRad = m_fValueRaio * fValue;
	SetVertexBuffVtx();
}

//-----------------------------------------------------------------------------
// 頂点バッファの確保
//-----------------------------------------------------------------------------
HRESULT CSceneCircle::CreateVertexBuff(){

	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * CIRCLE_VERTEX_NUM
		, D3DUSAGE_WRITEONLY			// 変えない
		, FVF_VERTEX_2D					// 使用する頂点フォーマット
		, D3DPOOL_MANAGED				// 変えない
		, &m_pD3DVtxBuff				// 格納する場所
		, NULL)))						// 変えない
	{
		return E_FAIL;															// メモリを確保できなかったらメインに返す
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 頂点情報の初期化
//-----------------------------------------------------------------------------
void CSceneCircle::SetVertexBuff(){

	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

	// 一つの角度　-2は"頂点番号0"の分と"頂点番号1"に合わせるため
	float one_radius = D3DX_PI * 2 / (CIRCLE_VERTEX_NUM - 2);

	for (int i = 0; i < CIRCLE_VERTEX_NUM; i++){
		float off_set_x = cosf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		float off_set_y = sinf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		pVtx[i].vtx = D3DXVECTOR3(m_vPos.x + off_set_x, m_vPos.y + off_set_y, 0);
		pVtx[i].rhw = 1.0f;
		pVtx[i].diffuse = m_Color;
		pVtx[i].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	}
	// "頂点番号0"は中心に設定
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x, m_vPos.y, 0);
	m_pD3DVtxBuff->Unlock();																	// ロックしたら必ずアンロック！！
}

//-----------------------------------------------------------------------------
// 現在の角度で頂点情報のVtxのみ書き替え（更新で呼ぶ）
//-----------------------------------------------------------------------------
void CSceneCircle::SetVertexBuffVtx(){

	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

	// 一つの角度　-2は"頂点番号0"の分と"頂点番号1"に合わせるため
	float one_radius = m_fRad / (CIRCLE_VERTEX_NUM - 2);

	for (int i = 0; i < CIRCLE_VERTEX_NUM; i++){
		float off_set_x = cosf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		float off_set_y = sinf(CIRCLE_VERTEX_OFFSET + one_radius * (i - 1)) * m_fRadius;
		pVtx[i].vtx = D3DXVECTOR3(m_vPos.x + off_set_x, m_vPos.y + off_set_y, 0);
	}
	// "頂点番号0"は中心に設定
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x, m_vPos.y, 0);
	m_pD3DVtxBuff->Unlock();																	// ロックしたら必ずアンロック！！
}

//-----------------------------------------------------------------------------
// 最大値に対する数値の割合を最大値から計算
//-----------------------------------------------------------------------------
void CSceneCircle::CalcRatio(float max){

	m_fValueRaio = CIRCLE_RADIAN_MAX / max;
}
//----EOF----