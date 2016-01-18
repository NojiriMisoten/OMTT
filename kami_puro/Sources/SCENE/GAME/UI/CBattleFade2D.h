//=============================================================================
//
// CBattleFade2D�N���X [CBattleFade2D.h]
// Author : �˖{�r�F
//
// ���Z���������
// 
//=============================================================================
#ifndef _CCUTIN2D_H_
#define _CCUTIN2D_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBattleFade2D : public CScene2D
{
public:

	CBattleFade2D(LPDIRECT3DDEVICE9 *pDevice);
	~CBattleFade2D(void);

	void Update();
	void Uninit();
	void DrawUIBattleFadeRender();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CBattleFade2D *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);


private:
	// ������
	void Init(D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);
};

#endif
//----EOF----