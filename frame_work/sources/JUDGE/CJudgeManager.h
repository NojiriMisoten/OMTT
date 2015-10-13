//=============================================================================
//
// CJudgeManagerクラス [CJudgeManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CJUDGEMANAGER_H_
#define _CJUDGEMANAGER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CJudge;
class CPlayerManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CJudgeManager
{
public:
	// コンストラクタ
	CJudgeManager(void);

	// デストラクタ
	~CJudgeManager(void);

	// クリエイト
	static CJudgeManager* Create(CPlayerManager* playerManager);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	CPlayerManager* GetPlayerManager(void){ return m_pPlayerManager; };

private:
	// 初期化
	void Init(CPlayerManager* playerManager);

	CJudge* m_pJudge;	//	judgeクラス
	CPlayerManager* m_pPlayerManager;
};

#endif
//----EOF----