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
	SOUND_LABEL_BGM_TITLE_FIRST = 0,	// OP
	SOUND_LABEL_BGM_TITLE_LOOP,		// OP
	SOUND_LABEL_BGM_GAME,			// GAME
	SOUND_LABEL_BGM_FINISH,			// FINISH
	SOUND_LABEL_BGM_RESULT,			// RESULT
	SOUND_LABEL_SE_CHAIN,			// くさり
	SOUND_LABEL_SE_START_GONG,		// ゴング
	SOUND_LABEL_SE_FINISH_GONG,		// ゴング
	SOUND_LABEL_SE_THROW1,			// 投げ１
	SOUND_LABEL_SE_THROW2,			// 投げ２
	SOUND_LABEL_SE_THROWFINISH,		// 投げフィニッシュ
	SOUND_LABEL_SE_SLAP01,
	SOUND_LABEL_SE_SLAP02,
	SOUND_LABEL_SE_SLAP03,
	SOUND_LABEL_SE_BATTLE_FADE,		// バトルフェード
	SOUND_LABEL_SE_CROWD01,			// 歓声
	SOUND_LABEL_SE_CROWD02,			// 歓声
	SOUND_LABEL_SE_CROWD03,			// 歓声
	SOUND_LABEL_SE_DOWN01,			// ダウン
	SOUND_LABEL_SE_DOWN02,			// ダウン
	SOUND_LABEL_SE_DOWN03,			// ダウン
	SOUND_LABEL_SE_ELBOW01,			// エルボー
	SOUND_LABEL_SE_ELBOW02,			// エルボー
	SOUND_LABEL_SE_ELBOW03,			// エルボー
	SOUND_LABEL_SE_FINISH_START,	// FINISHの最初
	SOUND_LABEL_SE_FINISH_END,		// FINISHの最後
	SOUND_LABEL_SE_SLOW,			// スロー
	SOUND_LABEL_SE_FOOT_STEP01,		// 足音
	SOUND_LABEL_SE_FOOT_STEP02,		// 足音
	SOUND_LABEL_SE_FOOT_STEP03,		// 足音
	SOUND_LABEL_SE_LARGE_DAMAGE01,	// 大ダメージ
	SOUND_LABEL_SE_LARGE_DAMAGE02,	// 大ダメージ
	SOUND_LABEL_SE_LARGE_DAMAGE03,	// 大ダメージ
	SOUND_LABEL_SE_FALL01,			// 落下
	SOUND_LABEL_SE_DECIDE,			// 決定音
	SOUND_LABEL_SE_INPUT_COMMAND,	// コマンド入力音
	SOUND_LABEL_SE_COMPLETE_COMMAND,// コマンド完成音
	SOUND_LABEL_SE_CHARGE,			// チャージ音
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

	//==============================================================
	// 指定した音の大きさ変更
	// 引数: 鳴らしたい音のラベル, 大きさ(0～１：無音～通常)
	//==============================================================
	void SetVolume(SOUND_LABEL label, float volume);
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