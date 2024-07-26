// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_PlayerInformationShow.h"
//角色形象
#include "Game/UI/UI_PlayerShow.h"
//游戏实例
#include "GameSystem/FVMGameInstance.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/CanvasPanel.h>

bool UUI_PlayerInformationShow::Initialize()
{
	if (!Super::Initialize())
		return false;

	//获取角色形象界面
	this->M_PlayerPoint = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerPoint");

	this->M_PlayerSuitBG_Img = this->GetWidgetComponent<UImage>(this, "Player_Bg");
	this->M_PlayerSuitBG_Panel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerSuitBG");
	this->M_PlayerFristWeaponBG_Panel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerFristWeaponBG");
	this->M_PlayerSecondWeaponBG_Panel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerSecondWeaponBG");
	this->M_PlayerSuperWeaponBG_Panel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerSuperWeaponBG");

	return true;
}

void UUI_PlayerInformationShow::InitPlayerBaseData()
{
	if (this->M_UUI_PlayerShow)
		return;

	//角色形象----------------------------------------------------------------------------------------------------
	this->M_UUI_PlayerShow = CreateWidget<UUI_PlayerShow>(this, LoadClass<UUI_PlayerShow>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerShow.BP_PlayerShow_C'")));
	this->M_PlayerPoint->AddChildToCanvas(this->M_UUI_PlayerShow);

	//角色基础着装图片
	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_FlyItem"));

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Hair"));//头发

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Body"));//着装

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Eye"));//眼睛

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Face"));//脸饰

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Glasses"));//眼镜

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_Cap"));//帽子

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_WepaonFirst"));//主武器

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_WepaonSecond"));//副武器

	this->M_PlayerBaseSuitButtons.Emplace(this->GetWidgetComponent<UButton>(this, "But_WepaonSuper"));//超级武器

	this->M_PlayerBaseSuitButton = this->GetWidgetComponent<UButton>(this, "But_Suit");

	//角色形象----------------------------------------------------------------------------------------------------
}

void UUI_PlayerInformationShow::SetPlayerSuitConst()
{
	if (IsValid(this->M_UUI_PlayerShow))
	{
		this->M_UUI_PlayerShow->SetConstCurrentPlayer();
		this->RefreshPlayerSuit();
	}
}

void UUI_PlayerInformationShow::InitPlayerSuit()
{
	//套装与服装界面显示与关闭
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_bPlayerSuit)
	{
		//显示套装
		if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Suit.M_bUse)
		{
			UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButton, this->M_UUI_PlayerShow->GetPlayerSuit()->M_Suit.M_SuitHeadPath.ToString());

			this->M_PlayerBaseSuitButton->SetVisibility(ESlateVisibility::Visible);
			this->M_PlayerSuitBG_Img->SetVisibility(ESlateVisibility::HitTestInvisible);
			this->M_PlayerSuitBG_Panel->SetVisibility(ESlateVisibility::HitTestInvisible);

		}
	}
	else {
		this->M_PlayerBaseSuitButton->SetVisibility(ESlateVisibility::Collapsed);
		this->M_PlayerSuitBG_Panel->SetVisibility(ESlateVisibility::Collapsed);
		this->M_PlayerSuitBG_Img->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->翅膀
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_FlyItem.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[0], this->M_UUI_PlayerShow->GetPlayerSuit()->M_FlyItem.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[0]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[0]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->头发
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Hair.M_bUse)
	{
		//显示头发前
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[1], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Hair.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[1]->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		this->M_PlayerBaseSuitButtons[1]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->着装
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Body.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[2], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Body.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[2]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[2]->SetVisibility(ESlateVisibility::Collapsed);
	}


	//显示->眼睛
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Eye.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[3], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Eye.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[3]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[3]->SetVisibility(ESlateVisibility::Collapsed);
	}


	//显示->脸饰
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Face.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[4], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Face.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[4]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[4]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->眼镜
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Glasses.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[5], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Glasses.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[5]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[5]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示->帽子
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_Cap.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[6], this->M_UUI_PlayerShow->GetPlayerSuit()->M_Cap.M_SuitHeadPath.ToString());

		this->M_PlayerBaseSuitButtons[6]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_PlayerBaseSuitButtons[6]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示主武器
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerFirstWeapon.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[7], this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerFirstWeapon.M_WeaponHeadPath);
		this->M_PlayerBaseSuitButtons[7]->SetVisibility(ESlateVisibility::Visible);
		this->M_PlayerFristWeaponBG_Panel->SetVisibility(ESlateVisibility::HitTestInvisible);


	}
	else {
		this->M_PlayerBaseSuitButtons[7]->SetVisibility(ESlateVisibility::Collapsed);
		this->M_PlayerFristWeaponBG_Panel->SetVisibility(ESlateVisibility::Collapsed);
	}

	//显示副武器
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSecondWeapon.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[8], this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSecondWeapon.M_WeaponHeadPath);
		this->M_PlayerBaseSuitButtons[8]->SetVisibility(ESlateVisibility::Visible);
		this->M_PlayerSecondWeaponBG_Panel->SetVisibility(ESlateVisibility::HitTestInvisible);

	}
	else {
		this->M_PlayerBaseSuitButtons[8]->SetVisibility(ESlateVisibility::Collapsed);
		this->M_PlayerSecondWeaponBG_Panel->SetVisibility(ESlateVisibility::Collapsed);

	}

	//显示超级武器
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSuperWeapon.M_bUse)
	{
		UWidgetBase::SetButtonStyle(this->M_PlayerBaseSuitButtons[9], this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSuperWeapon.M_WeaponHeadPath);
		this->M_PlayerBaseSuitButtons[9]->SetVisibility(ESlateVisibility::Visible);
		this->M_PlayerSuperWeaponBG_Panel->SetVisibility(ESlateVisibility::HitTestInvisible);

	}
	else {
		this->M_PlayerBaseSuitButtons[9]->SetVisibility(ESlateVisibility::Collapsed);
		this->M_PlayerSuperWeaponBG_Panel->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void UUI_PlayerInformationShow::RefreshPlayerSuit()
{
	//初始化显示
	this->M_UUI_PlayerShow->InitPlayerSuit();

	this->InitPlayerSuit();
}

void UUI_PlayerInformationShow::UnInstallPlayerSuit(EPlayerEquipmentSlotPosition _LEPlayerEquipmentSlotPosition)
{
	//卸载
	this->M_UUI_PlayerShow->UnInstallPlayerSuit(_LEPlayerEquipmentSlotPosition);

	this->RefreshPlayerSuit();

	if (this->OnLoaddingEnd.IsBound())
		this->OnLoaddingEnd.Execute();
}

void UUI_PlayerInformationShow::PlayerSuitHidden(UButton* _Button_)
{
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_ShowPlayerSuit)
	{
		this->M_UUI_PlayerShow->PlayerSuitHidden(false);

		//修改按钮样式
		if (_Button_)
			UWidgetBase::SetButtonStyle(_Button_, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/569.569'"));
	}
	else {
		this->M_UUI_PlayerShow->PlayerSuitHidden(true);

		//修改按钮样式
		if (_Button_)
			UWidgetBase::SetButtonStyle(_Button_, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/568.568'"));
	}

	this->RefreshPlayerSuit();
}

void UUI_PlayerInformationShow::InitPlayerSuitButtonStyle(UButton* _Button_)
{
	if (this->M_UUI_PlayerShow->GetPlayerSuit()->M_ShowPlayerSuit)
	{
		//修改按钮样式
		if (_Button_)
			UWidgetBase::SetButtonStyle(_Button_, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/568.568'"));
	}
	else {
		//修改按钮样式
		if (_Button_)
			UWidgetBase::SetButtonStyle(_Button_, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/569.569'"));
	}
}

void UUI_PlayerInformationShow::UnEquip_Weapon(EEquipment _LEEquipment)
{
	//卸载武器
	auto UnEquip = [&](FPlayerWepaon& _LWepaon) {
		if (_LWepaon.M_bUse)
		{
			if (this->M_UUI_PlayerShow->GetbConst())
			{
				_LWepaon.M_bUse = false;
			}
			else {
				//获取角色
				UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();

				int32 LIndex;
				if (Player->SearchPlayerWeaponDataByID_A(_LWepaon.M_WeaponID, LIndex) != -1)
				{

					UFVMGameInstance::GetPlayerStructManager_Static()->CancelEquipByID(_LWepaon.M_WeaponID);

					_LWepaon.M_bUse = false;
				}
			}
		}
	};

	//武器卸载
	switch (_LEEquipment)
	{
	case EEquipment::E_PlayerWeaponFirst:UnEquip(this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerFirstWeapon); break;
	case EEquipment::E_PlayerWeaponSecond:UnEquip(this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSecondWeapon); break;
	case EEquipment::E_PlayerWeaponSuper:UnEquip(this->M_UUI_PlayerShow->GetPlayerSuit()->M_PlayerWeapons.M_PlayerSuperWeapon); break;
	default:
		break;
	}

	//刷新UI
	//刷新服装显示
	this->RefreshPlayerSuit();

	//代理函数
	if (this->OnLoaddingEnd.IsBound())
		this->OnLoaddingEnd.Execute();
}

void UUI_PlayerInformationShow::SetCurrentPlayer(UPlayerStructManager* _CurrentPlayer)
{
	if (this->M_UUI_PlayerShow)
		this->M_UUI_PlayerShow->SetCurrentPlayer(_CurrentPlayer);
}

bool UUI_PlayerInformationShow::GetPlayerSuitConst()
{
	return this->M_UUI_PlayerShow->GetPlayerSuitConst();
}
