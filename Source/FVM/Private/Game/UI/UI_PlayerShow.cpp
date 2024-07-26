// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_PlayerShow.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <Components/CanvasPanel.h>
#include <Components/Image.h>

bool UUI_PlayerShow::GetPlayerSuitConst()
{
	return this->M_bConst;
}

FPlayerSuit* UUI_PlayerShow::GetPlayerSuit()
{

	FPlayerSuit* LSuit = nullptr;

	if (this->M_bConst)
	{
		LSuit = &this->M_FPlayerSuitTemp;
	}
	else {
		LSuit = &this->M_CurrentPlayer->M_FPlayerSuit;
	}

	return LSuit;
}

bool UUI_PlayerShow::GetbConst()
{
	return this->M_bConst;
}

bool UUI_PlayerShow::Initialize()
{
	if (!Super::Initialize())
		return false;

	//角色形象----------------------------------------------------------------------------------------------------

	//角色套装
	this->M_PlayerBaseSuitImage = this->GetWidgetComponent<UImage>(this, "Player_Uni");//套装

	//
	this->InitPlayerDataBase();

	//角色形象----------------------------------------------------------------------------------------------------

	return true;
}

void UUI_PlayerShow::SetCurrentPlayer(UPlayerStructManager* _CurrentPlayer)
{
	this->M_CurrentPlayer = _CurrentPlayer;
}

void UUI_PlayerShow::SetConstCurrentPlayer()
{
	if (IsValid(this->M_CurrentPlayer))
		this->M_FPlayerSuitTemp = this->M_CurrentPlayer->M_FPlayerSuit;

	this->M_bConst = true;
}

void UUI_PlayerShow::InitPlayerDataBase()
{
	//如果两个不是0
	if (this->M_PlayerSuitPanels.Num() != 0 && this->M_PlayerBaseSuitImages.Num() != 0)
		return;

	//角色着装界面
	this->M_PlayerSuitPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Base_1_0")));
	this->M_PlayerSuitPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Base_2_0")));

	//角色基础着装图片
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_FlyItem"));//翅膀
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Head_Back"));//头发-后
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Body"));//着装
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Head"));//头
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_eye"));//眼睛
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Face"));//脸饰
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_eye_2"));//眼镜
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Head_Front"));//头发-前
	this->M_PlayerBaseSuitImages.Emplace(this->GetWidgetComponent<UImage>(this, "Player_Head_Top"));//帽子
}

void UUI_PlayerShow::InitPlayerSuit()
{

	FPlayerSuit* LSuit = nullptr;

	if (this->M_bConst)
	{
		LSuit = &this->M_FPlayerSuitTemp;
	}
	else {
		if (this->M_CurrentPlayer)
			LSuit = &this->M_CurrentPlayer->M_FPlayerSuit;
	}

	if (!LSuit)
		return;

	//套装与服装界面显示与关闭
	if (LSuit->M_bPlayerSuit && LSuit->M_ShowPlayerSuit)
	{
		if (this->M_PlayerSuitPanels[0])
			this->M_PlayerSuitPanels[0]->SetVisibility(ESlateVisibility::Hidden);
		if (this->M_PlayerSuitPanels[1])
			this->M_PlayerSuitPanels[1]->SetVisibility(ESlateVisibility::HitTestInvisible);

		//显示套装
		if (LSuit->M_Suit.M_bUse)
		{
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImage, LSuit->M_Suit.M_SuitFirstShowPath.ToString());
		}
	}
	else {
		if (this->M_PlayerSuitPanels[1])
			this->M_PlayerSuitPanels[1]->SetVisibility(ESlateVisibility::Hidden);
		if (this->M_PlayerSuitPanels[0])
			this->M_PlayerSuitPanels[0]->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	//显示->翅膀
	if (LSuit->M_FlyItem.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[0], LSuit->M_FlyItem.M_SuitFirstShowPath.ToString());
	}
	else
	{
		if (this->M_PlayerBaseSuitImages[0])
			this->M_PlayerBaseSuitImages[0]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->头发
	if (LSuit->M_Hair.M_bUse)
	{
		//显示头发前
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[7], LSuit->M_Hair.M_SuitFirstShowPath.ToString());

		//显示头发后
		if (LSuit->M_Hair.M_SuitSecondShowPath.ToString().Equals(""))
		{
			if (this->M_PlayerBaseSuitImages[1])
				this->M_PlayerBaseSuitImages[1]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[1], LSuit->M_Hair.M_SuitSecondShowPath.ToString());
	}
	else {
		//根据性别显示基础着装
		if (this->M_CurrentPlayer->M_PlayerSex == 0)
		{
			//女
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[1], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础头发_女_后.角色基础头发_女_后'"));
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[7], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础头发_女_前.角色基础头发_女_前'"));
		}
		else
		{
			//男
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[7], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础头发-男_前.角色基础头发-男_前'"));
			if (this->M_PlayerBaseSuitImages[1])
				this->M_PlayerBaseSuitImages[1]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//显示->着装
	if (LSuit->M_Body.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[2], LSuit->M_Body.M_SuitFirstShowPath.ToString());
	}
	else
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[2], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础服装.角色基础服装'"));
	}


	//显示->头
	if (LSuit->M_Head.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[3], LSuit->M_Head.M_SuitFirstShowPath.ToString());
	}
	else
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[3], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础脸.角色基础脸'"));

	//显示->眼睛
	if (LSuit->M_Eye.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[4], LSuit->M_Eye.M_SuitFirstShowPath.ToString());
	}
	else
	{
		//根据性别显示基础着装
		if (this->M_CurrentPlayer->M_PlayerSex == 0)
		{
			//女
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[4], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础眼睛_女.角色基础眼睛_女'"));
		}
		else
		{
			//男
			UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[4], TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/角色基础眼睛_男.角色基础眼睛_男'"));
		}
	}


	//显示->脸饰
	if (LSuit->M_Face.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[5], LSuit->M_Face.M_SuitFirstShowPath.ToString());
	}
	else
	{
		if (this->M_PlayerBaseSuitImages[5])
			this->M_PlayerBaseSuitImages[5]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->眼镜
	if (LSuit->M_Glasses.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[6], LSuit->M_Glasses.M_SuitFirstShowPath.ToString());
	}
	else
	{
		if (this->M_PlayerBaseSuitImages[6])
			this->M_PlayerBaseSuitImages[6]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->帽子
	if (LSuit->M_Cap.M_bUse)
	{
		UWidgetBase::SetImageBrush(this->M_PlayerBaseSuitImages[8], LSuit->M_Cap.M_SuitFirstShowPath.ToString());
	}
	else
	{
		if (this->M_PlayerBaseSuitImages[8])
			this->M_PlayerBaseSuitImages[8]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_PlayerShow::UnInstallPlayerSuit(EPlayerEquipmentSlotPosition _LEPlayerEquipmentSlotPosition)
{
	FPlayerSuit* LSuit = nullptr;

	if (this->M_bConst)
	{
		LSuit = &this->M_FPlayerSuitTemp;
	}
	else {
		if (this->M_CurrentPlayer)
			LSuit = &this->M_CurrentPlayer->M_FPlayerSuit;
	}

	if (!LSuit)
		return;

	//使用音效
	this->PlayOperateAudio();

	//卸载
	auto UnIns = [&](FPlayerSuitItem& LData) {

		LData.M_bUse = false;

		if (!this->M_bConst)
			this->M_CurrentPlayer->CancelEquipByID(LData.M_ItemID);
	};

	switch (_LEPlayerEquipmentSlotPosition)
	{
	case EPlayerEquipmentSlotPosition::E_Player_FlyItem:UnIns(LSuit->M_FlyItem); break;
	case EPlayerEquipmentSlotPosition::E_Player_Hair:UnIns(LSuit->M_Hair); break;
	case EPlayerEquipmentSlotPosition::E_Player_Body:UnIns(LSuit->M_Body); break;
	case EPlayerEquipmentSlotPosition::E_Player_Head:UnIns(LSuit->M_Head); break;
	case EPlayerEquipmentSlotPosition::E_Player_Eye:UnIns(LSuit->M_Eye); break;
	case EPlayerEquipmentSlotPosition::E_Player_Face:UnIns(LSuit->M_Face); break;
	case EPlayerEquipmentSlotPosition::E_Player_Glasses:UnIns(LSuit->M_Glasses); break;
	case EPlayerEquipmentSlotPosition::E_Player_Cap:UnIns(LSuit->M_Cap); break;
	case EPlayerEquipmentSlotPosition::E_Player_Suit: {
		UnIns(LSuit->M_Suit);
		LSuit->M_bPlayerSuit = false;
		LSuit->M_HiddenFlyItem = false;
	}break;
	default:
		UWidgetBase::CreateTipWidget(TEXT("服装卸载失败"));
		return;
	}

	if (!this->M_bConst)
		UGameSystemFunction::SaveCurrentPlayerData();
}

void UUI_PlayerShow::PlayerSuitHidden(bool _bShowSuit)
{
	FPlayerSuit* LSuit = nullptr;
	if (this->M_bConst)
	{
		LSuit = &this->M_FPlayerSuitTemp;
		LSuit->M_ShowPlayerSuit = _bShowSuit;
	}
	else {
		LSuit = &this->M_CurrentPlayer->M_FPlayerSuit;
		this->M_CurrentPlayer->M_FPlayerSuit.M_ShowPlayerSuit = _bShowSuit;
	}
}