// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_BagRemoveItemBase.h"

#include "Game/UI/UI_PlayerBagRemove.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include <Components/Image.h>
#include <Components/Button.h>

void UUI_BagRemoveItemBase::SelectItems(bool _Tag)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	if (this->M_PanelNumber == 0) {

		PlayerData->M_PlayerItems_Equipment[M_ItemIndex].bWaitRemove = _Tag;

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("物品:") + PlayerData->M_PlayerItems_Equipment[M_ItemIndex].ItemName + (_Tag ? TEXT("标记") : TEXT("取消标记")));
		}

		//标记装备数据存储库
		switch (PlayerData->M_PlayerItems_Equipment[M_ItemIndex].M_EquipmentType)
		{
		case EEquipment::E_PlayerWeaponFirst:
		case EEquipment::E_PlayerWeaponSecond:
		case EEquipment::E_PlayerWeaponSuper:this->SelectItemTag(PlayerData, _Tag, PlayerData->M_FPlayerWeaponDatas); break;
		case EEquipment::E_WeaponGem:this->SelectItemTag(PlayerData, _Tag, PlayerData->M_FPlayerWeaponGemDatas); break;
		}
	}

	if (this->M_PanelNumber == 1) {
		PlayerData->M_PlayerItems_Card[M_ItemIndex].bWaitRemove = _Tag;
	}

	if (this->M_PanelNumber == 2) {
		PlayerData->M_PlayerItems_Material[M_ItemIndex].bWaitRemove = _Tag;
	}

	if (!_Tag)
	{
		this->SetCancel();
	}

}


void UUI_BagRemoveItemBase::SetUIPlayerBagRemove(UUI_PlayerBagRemove* _Class)
{
	this->M_ParentUI_Remove = _Class;
}


void UUI_BagRemoveItemBase::SetItemName(const FString& _ItemName)
{
	this->M_ItemName = _ItemName;
}

void UUI_BagRemoveItemBase::Select()
{
	if (this->M_bIsSelectItem)
	{
		//取消选择
		this->SetCancel();

		this->SelectItems(false);

		//向删除UI移出位置
		this->M_ParentUI_Remove->RemoveCurrentSelect(this->M_ItemIndex);
	}
	else {
		//选择
		this->SetSelect();

		this->SelectItems(true);

		//向删除UI申请位置
		this->M_ParentUI_Remove->AddCurrentSelect(this->M_ItemIndex, this->M_ItemName);
	}
}

void UUI_BagRemoveItemBase::Update()
{
	if (this->M_ParentUI_Remove->FindCurrentSelect(this->M_ItemIndex))
	{
		//选择
		this->SetSelect();

		this->SelectItems(true);
	}
	else {
		//取消选择
		this->SetCancel();

		this->SelectItems(false);
	}
}

bool UUI_BagRemoveItemBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_SelectButton = this->GetWidgetComponent<UButton>(this, "Select");
	this->M_ShowSelectTag = this->GetWidgetComponent<UImage>(this, "SelecState_Image");

	M_SelectButtonText = FString(TEXT("取消选择"));
	return true;
}

void UUI_BagRemoveItemBase::SetTagItems(int32 _ItemIndex, uint8 _PanelNumber)
{
	this->M_ItemIndex = _ItemIndex;
	this->M_PanelNumber = _PanelNumber;
}

void UUI_BagRemoveItemBase::SetSelect()
{
	//选择
	this->M_bIsSelectItem = true;
	UWidgetBase::SetImageBrush(this->M_ShowSelectTag, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_13.T_PB_13'");
	M_SelectButtonText = FString(TEXT("取消选择"));
}

void UUI_BagRemoveItemBase::SetCancel()
{
	this->M_bIsSelectItem = false;
	this->M_ShowSelectTag->SetVisibility(ESlateVisibility::Hidden);
	M_SelectButtonText = FString(TEXT("选择"));
}

UButton* UUI_BagRemoveItemBase::GetSelectButton()
{
	return this->M_SelectButton;
}

bool UUI_BagRemoveItemBase::GetCurrentSelect()
{
	return this->M_bIsSelectItem;
}

void UUI_BagRemoveItemBase::BindSelectFunc()
{
	FScriptDelegate Func;
	Func.BindUFunction(this, "Select");
	this->M_SelectButton->OnClicked.Add(Func);
}
