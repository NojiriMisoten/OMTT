//=============================================================================
//
// �X�L�����b�V������ [CInheritanceHierarchy.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CINHERITANCEHEIRARCHY_H_
#define _CINHERITANCEHEIRARCHY_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �X�L�����b�V���p�\����
//*****************************************************************************
// �h���t���[���\���́i���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME: public D3DXFRAME
{	
	D3DXMATRIX CombinedTransformationMatrix;	//�s����������邽�߂̍s��
};

//*****************************************************************************
// �h�����b�V���R���e�i�[�\����
//*****************************************************************************
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*	ppTextures;				// �R���e�i�[���e�N�X�`���𕡐����Ă�悤�ɂ���|�C���^�[�̃|�C���^�[��ǉ�����
	DWORD				dwWeight;				// �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���j
	DWORD				dwBoneNum;				// �{�[���̐�
	LPD3DXBUFFER		pBoneBuffer;			// �{�[���e�[�u��
	D3DXMATRIX**		ppBoneMatrix;			// �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX*			pBoneOffsetMatrices;	// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
};

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CInheritanceHierarchy: public ID3DXAllocateHierarchy
{
public:
	CInheritanceHierarchy(){}
	STDMETHOD(CreateFrame)			(THIS_ LPCSTR  , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)	(THIS_ LPCTSTR , CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)			(THIS_ LPD3DXFRAME );			//�폜�p ���Ȃ��ƃ��������[�N
	STDMETHOD(DestroyMeshContainer)	(THIS_ LPD3DXMESHCONTAINER );	//�폜�p ���Ȃ��ƃ��������[�N

private:
	//=============================================================================
	// ���b�V���̃R���o�[�g�֐�
	//============================================================================
	void ConvertMesh(LPD3DXMESH* pMesh);
};

#endif