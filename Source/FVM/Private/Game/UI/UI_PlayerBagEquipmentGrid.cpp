// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagEquipmentGrid.h"

#include "Game/UI/UI_PlayerBag.h"
#include "Game/UI/UI_GiftBox.h"

#include "Game/UI/Tips/UI_ItemDesTip.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerDataSubsystem.h"


bool UUI_PlayerBagEquipmentGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UButton = this->GetWidgetComponent<UButton>(this, "Item_button");
	this->M_UItemGrade = this->GetWidgetComponent<UImage>(this, "ItemGrade");
	this->M_ItemCountText = this->GetWidgetComponent<UTextBlock>(this, "ItemCount");

	return true;
}

void UUI_PlayerBagEquipmentGrid::SetItemCountText(const FString& _Text, bool _Show)
{
	if (!_Show)
		this->M_ItemCountText->SetRenderOpacity(0);
	else {
		this->M_ItemCountText->SetRenderOpacity(1);
		this->M_ItemCountText->SetText(FText::FromString(_Text));
	}
}

void UUI_PlayerBagEquipmentGrid::SetItemID(int32 _ID)
{
	this->M_ItemID = _ID;
}

int32 UUI_PlayerBagEquipmentGrid::GetItemID()
{
	return this->M_ItemID;
}

int32 UUI_PlayerBagEquipmentGrid::GetItemID_EquipData()
{
	return this->M_EuipmentData->M_ItemID;
}

void UUI_PlayerBagEquipmentGrid::RelaodBagEquipment()
{
	if (this->M_BagUI)
	{
		this->M_BagUI->LoadItemsData();
	}
}

void UUI_PlayerBagEquipmentGrid::AttachToBag()
{
	if (this->M_BagUI)
	{
		FPlayerBagGirdGroup& Group = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

		switch (this->M_BagUI->GetPanel_Index())
		{
		case 0:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_EquipmentGrid, this->M_EuipmentData); break;
		case 1:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_CardGrid, this->M_EuipmentData); break;
		case 2:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_MaterialGrid, this->M_EuipmentData); break;
		}

		this->M_BagUI->CloseSelectBagPanel();

		if (this->M_EuipmentData)
		{
			//保存数据
			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("进行背包的装备操作，名称：")) + this->M_EuipmentData->ItemName.ToString());
		}
	}
}

void UUI_PlayerBagEquipmentGrid::CreateNewGiftBox()
{
	UUI_GiftBox* GiftBox = CreateWidget<UUI_GiftBox>(this, LoadClass<UUI_GiftBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_GiftBox_Panel.BPUI_GiftBox_Panel_C'")));
	GiftBox->SetPlayerBagEquipmentGrid(this);
	GiftBox->SetGiftBoxData(this->GetItemName());
	GiftBox->AddToViewport();
}

void UUI_PlayerBagEquipmentGrid::UsePlayerSuit()
{
	/*---------------------------------当前道具服装使用性别判断------------------------------------*/
	EPlayerEquipmentType LItemEquipmentType = EPlayerEquipmentType::E_Base;
	//当前服装的使用性别
	if (this->M_EuipmentData->M_PlayerUseSex == 0)
		LItemEquipmentType = EPlayerEquipmentType::E_Men;
	else if (this->M_EuipmentData->M_PlayerUseSex == 1)
		LItemEquipmentType = EPlayerEquipmentType::E_Man;

	/*---------------------------------当前道具服装使用性别判断------------------------------------*/


	/*---------------------------------当前角色性别判断------------------------------------*/

	//男女判断
	EPlayerEquipmentType LPlayerEquipmentType = EPlayerEquipmentType::E_Base;

	//当前服装的使用性别
	if (UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerSex == 0)
		LPlayerEquipmentType = EPlayerEquipmentType::E_Men;
	else if (UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerSex == 1)
		LPlayerEquipmentType = EPlayerEquipmentType::E_Man;

	/*---------------------------------当前角色性别判断------------------------------------*/


	/*---------------------------------根据当前道具使用性别与当前玩家性别进行判断(不符合则不能使用)------------------------------------*/
	//装备性别判断
	if (LItemEquipmentType != EPlayerEquipmentType::E_Base && LPlayerEquipmentType != LItemEquipmentType)
	{
		UWidgetBase::CreateTipWidget(TEXT("这个服装不适合你穿"));
		return;
	}
	/*---------------------------------根据当前道具使用性别与当前玩家性别进行判断(不符合则不能使用)------------------------------------*/


	/*---------------------------------道具名称与ID------------------------------------*/
	//服装名称
	FString SuitName = this->M_EuipmentData->ItemName.ToString();
	//服装ID
	int32 SuitID = this->GetItemID_EquipData();
	/*---------------------------------道具名称与ID------------------------------------*/

	FPlayerEquipment Data;
	//数据查询(将道具名称，当前道具使用的性别，进行查询->最终返回一个结果)
	if (UPlayerEquipmentStruct::SearchPlayerSuitByEquipment(SuitName, LItemEquipmentType, Data))
	{

		//将服装的ID绑定到背包
		Data.M_ItemID = SuitID;

		//装备当前服装
		UFVMGameInstance::GetPlayerStructManager_Static()->UseEquipByID(this->GetItemID_EquipData());

		//获取角色
		UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();

		//取消服装装备->如果没有装备的服装则不用取消->直接更新数据即可
		auto CancelSuit = [&](FPlayerSuitItem& _Data, FPlayerEquipment& _EpData, bool _Param0) {

			//从背包中取消对应名称的装备->前提是当前服装已经装备过了
			if (_Data.M_bUse)
				//这个ID是之前服装装备的ID号（并不是当前道具的ID）
				UFVMGameInstance::GetPlayerStructManager_Static()->CancelEquipByID(_Data.M_ItemID);


			//数据复制（相当于更新->将之前的数据更新成现在的服装数据）
			_Data.CopyValue(_EpData, _Param0);
			};

		//装备服装
		switch (Data.M_EPlayerEquipmentSlotPosition)
		{
		case EPlayerEquipmentSlotPosition::E_Player_FlyItem:CancelSuit(Player->M_FPlayerSuit.M_FlyItem, Data, true); break;
		case EPlayerEquipmentSlotPosition::E_Player_Hair:CancelSuit(Player->M_FPlayerSuit.M_Hair, Data, true); break;
		case EPlayerEquipmentSlotPosition::E_Player_Body:CancelSuit(Player->M_FPlayerSuit.M_Body, Data, true); break;
		case EPlayerEquipmentSlotPosition::E_Player_Head:CancelSuit(Player->M_FPlayerSuit.M_Head, Data, true);  break;
		case EPlayerEquipmentSlotPosition::E_Player_Eye:CancelSuit(Player->M_FPlayerSuit.M_Eye, Data, true); break;
		case EPlayerEquipmentSlotPosition::E_Player_Face:CancelSuit(Player->M_FPlayerSuit.M_Face, Data, true); break;
		case EPlayerEquipmentSlotPosition::E_Player_Glasses:CancelSuit(Player->M_FPlayerSuit.M_Glasses, Data, true);  break;
		case EPlayerEquipmentSlotPosition::E_Player_Cap:CancelSuit(Player->M_FPlayerSuit.M_Cap, Data, true);  break;
		case EPlayerEquipmentSlotPosition::E_Player_Suit: { CancelSuit(Player->M_FPlayerSuit.M_Suit, Data, true);  Player->M_FPlayerSuit.M_bPlayerSuit = true; }break;
		default:
			UWidgetBase::CreateTipWidget(TEXT("这个服装不适合你穿"));
			return;
		}

		//如果当前服装为->套装
		if (Data.M_EPlayerEquipmentSlotPosition == EPlayerEquipmentSlotPosition::E_Player_Suit)
		{
			Player->M_FPlayerSuit.M_HiddenFlyItem = Data.M_bHiddenFlyItemByPlayer_Suit;
		}

		//保持游戏
		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("进行角色服装装备")));


		if (IsValid(this->M_BagUI))
		{
			//刷新服装显示
			this->M_BagUI->RefreshPlayerSuit();
			//重新加载装备栏
			this->M_BagUI->LoadItemsData();
		}
	}
	else {
		UWidgetBase::CreateTipWidget(TEXT("未搜寻到该服装"));
	}
}

void UUI_PlayerBagEquipmentGrid::Equip_Weapon(FPlayerWepaon& _LFPlayerWepaon)
{
	//获取角色
	UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();

	//判断当前武器装备是否有已经装备的武器
	if (_LFPlayerWepaon.M_bUse)
	{
		//卸载当前装备
		int32 Index;
		if (Player->SearchPlayerWeaponDataByID_A(_LFPlayerWepaon.M_WeaponID, Index) != -1)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->CancelEquipByID(_LFPlayerWepaon.M_WeaponID);
		}
	}

	//从角色背包搜寻武器并且标记为使用
	for (auto& FLData : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
	{
		if (FLData.ItemName.EqualTo(this->M_EuipmentData->ItemName) && FLData.M_ItemID == this->M_EuipmentData->M_ItemID)
		{
			FLData.M_Used = true;

			//开始装备武器 ID
			_LFPlayerWepaon.M_WeaponID = this->M_EuipmentData->M_ItemID;
			// 武器头像
			_LFPlayerWepaon.M_WeaponHeadPath = this->M_EuipmentData->ItemTexturePath.ToString();
			// 武器名称
			_LFPlayerWepaon.M_WeaponName = this->M_EuipmentData->ItemName.ToString();
			//使用武器
			_LFPlayerWepaon.M_bUse = true;

			break;
		}
	}

	//刷新UI显示
	if (IsValid(this->M_BagUI))
	{
		//重新加载装备栏
		this->M_BagUI->LoadItemsData();
		//刷新服装显示
		this->M_BagUI->RefreshPlayerSuit();
	}

	//保存游戏
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("进行角色武器装备")));
}

void UUI_PlayerBagEquipmentGrid::Equip_MainWeapon()
{
	//获取角色
	UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	//获取基础数据(从数据库中查询具体数据)
	for (const auto& Data : Cache->GetWeaponFirst())
	{
		if (Data.M_FEquipment.ItemName.EqualTo(this->M_EuipmentData->ItemName))
		{
			//武器类
			Player->M_FPlayerSuit.M_PlayerWeapons.
				M_PlayerFirstWeapon.
				M_WeaponResource_C_Path = Data.M_FEquipment.M_WeaponClassPath.ToString();

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器数据查询成功，并且赋值"));
			}

			break;
		}
	}

	this->Equip_Weapon(Player->M_FPlayerSuit.M_PlayerWeapons.M_PlayerFirstWeapon);
}

void UUI_PlayerBagEquipmentGrid::Equip_SecondWeapon()
{
	//获取角色
	UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();

	this->Equip_Weapon(Player->M_FPlayerSuit.M_PlayerWeapons.M_PlayerSecondWeapon);
}
void UUI_PlayerBagEquipmentGrid::Equip_SuperWeapon()
{
	//获取角色
	UPlayerStructManager* const Player = UFVMGameInstance::GetPlayerStructManager_Static();

	this->Equip_Weapon(Player->M_FPlayerSuit.M_PlayerWeapons.M_PlayerSuperWeapon);
}

void UUI_PlayerBagEquipmentGrid::SetItemName(const FString& _Name)
{
	this->M_ItemName = _Name;
}

void UUI_PlayerBagEquipmentGrid::SetItemDescribe(const FString& _Index)
{
	this->M_ItemDescribe = _Index;
}

FString UUI_PlayerBagEquipmentGrid::GetItemName()
{
	return this->M_ItemName;
}

void UUI_PlayerBagEquipmentGrid::PlayAnimation_1()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

void UUI_PlayerBagEquipmentGrid::ShowText()
{
	//创建物品提示按钮
	UUI_ItemDesTip* TipComp = CreateWidget<UUI_ItemDesTip>(this, LoadClass<UUI_ItemDesTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'")));
	TipComp->SetContentTitleText(this->M_EuipmentData->ItemName.ToString());
	TipComp->SetContentText(this->M_ItemDescribe);
	TipComp->SetOkButtonTitleText(TEXT("了解"));
	TipComp->SetCancelButtonTitleText(TEXT("知道了"));
	TipComp->SetItemHeadPath(this->M_EuipmentData->ItemTexturePath.ToString());
	TipComp->AddToViewport(1);
}

void UUI_PlayerBagEquipmentGrid::ShowPlayerSuit()
{
	//创建物品提示按钮
	UUI_ItemDesTip* TipComp = CreateWidget<UUI_ItemDesTip>(this, LoadClass<UUI_ItemDesTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'")));
	TipComp->SetContentTitleText(this->M_EuipmentData->ItemName.ToString());
	TipComp->SetContentText(this->M_ItemDescribe);
	TipComp->SetOkButtonTitleText(TEXT("装备"));
	TipComp->SetCancelButtonTitleText(TEXT("返回"));
	TipComp->SetItemHeadPath(this->M_EuipmentData->ItemTexturePath.ToString());

	FScriptDelegate LAdd;
	LAdd.BindUFunction(this, TEXT("UsePlayerSuit"));
	TipComp->GetOkButtonComponent()->OnClicked.Add(LAdd);

	TipComp->AddToViewport(1);
}

void UUI_PlayerBagEquipmentGrid::ShowPlayerWeapon()
{
	//创建物品提示按钮
	UUI_ItemDesTip* TipComp = CreateWidget<UUI_ItemDesTip>(this, LoadClass<UUI_ItemDesTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'")));

	if (UFVMGameInstance::GetDebug())
	{
		TipComp->SetContentTitleText(this->M_EuipmentData->ItemName.ToString() + TEXT(" ") + FString::FromInt(this->M_EuipmentData->M_ItemID));
	}
	else {
		TipComp->SetContentTitleText(this->M_EuipmentData->ItemName.ToString());
	}


	TipComp->SetContentText(this->M_ItemDescribe);
	TipComp->SetOkButtonTitleText(TEXT("装备武器"));
	TipComp->SetCancelButtonTitleText(TEXT("返回"));
	TipComp->SetItemHeadPath(this->M_EuipmentData->ItemTexturePath.ToString());

	FScriptDelegate LAdd;

	switch (this->M_EuipmentData->M_EquipmentType)
	{
	case EEquipment::E_PlayerWeaponFirst:LAdd.BindUFunction(this, TEXT("Equip_MainWeapon")); break;
	case EEquipment::E_PlayerWeaponSecond:LAdd.BindUFunction(this, TEXT("Equip_SecondWeapon")); break;
	case EEquipment::E_PlayerWeaponSuper:LAdd.BindUFunction(this, TEXT("Equip_SuperWeapon")); break;
	default:
		break;
	}

	TipComp->GetOkButtonComponent()->OnClicked.Add(LAdd);

	TipComp->AddToViewport(1);
}

void UUI_PlayerBagEquipmentGrid::SetItemGrade(const FString& _GradePath)
{
	if (_GradePath.Equals(""))
		this->M_UItemGrade->SetVisibility(ESlateVisibility::Collapsed);
	else
	{
		this->M_UItemGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UWidgetBase::SetImageBrush(this->M_UItemGrade, _GradePath);
	}
}

UButton* UUI_PlayerBagEquipmentGrid::GetButton()
{
	return this->M_UButton;
}
