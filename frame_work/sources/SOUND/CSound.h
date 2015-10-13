//=============================================================================
//
// CSoundクラス [CSound.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSOUND_H_
#define _CSOUND_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	bool bLoop;			// ループするかどうか
} PARAM;

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,					// OP
	SOUND_LABEL_SE_ENTER,					// 決定音
	SOUND_LABEL_SE_START,					// スタート音
	SOUND_LABEL_SE_END,						// 終了音
	SOUND_LABEL_SE_NORMAL_ATTACK,			// 通常攻撃音
	SOUND_LABEL_SE_NORMAL_THREAD,			// 通常糸攻撃音
	SOUND_LABEL_SE_THUNDERBOLT,				// 攻撃形態の糸攻撃の初弾の音
	SOUND_LABEL_SE_THUNDERBOLT_2,			// 攻撃形態の糸攻撃の二弾の音
	SOUND_LABEL_SE_AROUND_ATTACK,			// 攻撃形態の周囲攻撃の音
	SOUND_LABEL_SE_CREATE_ROAD,				// 移動形態の道作成音
	SOUND_LABEL_SE_TACKLE,					// 移動形態のタックルの音
	SOUND_LABEL_SE_TRAP_FOUNDATION,			// 妨害形態の罠設置音
	SOUND_LABEL_SE_TRAP_ATTACK,				// 妨害形態の攻撃音
	SOUND_LABEL_SE_GET_TREASURE,			// 宝物を拾った音
	SOUND_LABEL_SE_CURSOL_MOVE,				// カーソル移動
	SOUND_LABEL_SE_CURSOL_SANSEN,			// 参戦
	SOUND_LABEL_SE_CURSOL_ZYUNBIKANRYOU,	// 準備完了
	SOUND_LABEL_ZINGLE_TITLE,				// タイトルのジングル
	SOUND_LABEL_ZINGLE_RESULT,				// リザルトのジングル
	SOUND_LABEL_SE_GAGE_DOWN,				// ゲージ減少
	SOUND_LABEL_SE_SPEED_DOWN,				// スピード減少
	SOUND_LABEL_SE_TRANSREATION,			// 変形
	SOUND_LABEL_SE_DAMAGE,					// ダメージ
	SOUND_LABEL_SE_DEST_TREASURE,			// 宝物を失った音
	SOUND_LABEL_BGM_STAGE_SELECT,			// ステージセレクトのＢＧＭ
	SOUND_LABEL_SE_COUNTDOUN,				// カウントダウン
	SOUND_LABEL_SE_GAME_START,				// ゲームスタート
	SOUND_LABEL_BGM_TUTORIAL,				// チュートリアルののＢＧＭ
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	// コンストラクタ
	CSound();

	// デストラクタ
	~CSound();

	//==============================================================
	// 初期化
	// 引数: ウインドウハンドル
	// 戻り値: 成否
	//==============================================================
	HRESULT InitSound(HWND hWnd);

	// 終了
	void UninitSound(void);

	//==============================================================
	// 音再生
	// 引数: 鳴らしたい音のラベル
	// 戻り値: 成否
	//==============================================================
	HRESULT PlaySound(SOUND_LABEL label);

	//==============================================================
	// 指定した音停止
	// 引数: 鳴らしたい音のラベル
	//==============================================================
	void StopSound(SOUND_LABEL label);

	// 音全て停止
	void StopSound(void);

private:
	//=============================================================================
	// チャンクのチェック関数
	// 引数: ファイルハンドル、フォーマット、(out)チャンクサイズ、(out)チャンクデータポジション
	// 戻り値: 成否
	//=============================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

	//=============================================================================
	// チャンクデータの読み込み
	// 引数: ファイルハンドル、バッファーポインター、バッファーサイズ、オフセット
	// 戻り値: 成否
	//=============================================================================
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2				*m_pXAudio2;						// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice	*m_pMasteringVoice;					// マスターボイス
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX];	// オーディオデータ
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX];		// オーディオデータサイズ
};

#endif
//----EOF----