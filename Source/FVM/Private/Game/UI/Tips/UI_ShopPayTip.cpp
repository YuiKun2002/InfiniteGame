// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Tips/UI_ShopPayTip.h"

#include "Game/UI/UI_Shop.h"

#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/PlayerDataSubsystem.h"

void UUI_ShopPayTip::UpdateCoinText()
{
	//更新价格
	if (this->Shop_UClass)
	{
		this->Shop_UClass->UpdateCoinText();
	}
}

void UUI_ShopPayTip::SetShopUI(UUI_Shop* _UClass)
{
	this->Shop_UClass = _UClass;
}

void UUI_ShopPayTip::PayButton()
{

	this->OnPay(this->ItemPriceData);

	////更新价格
	//if (this->Shop_UClass)
	//{
	//	this->Shop_UClass->UpdateCoinText();
	//}

	//return;
	////背包类型不进行确认
	//if (this->M_FItemPrice.M_ItemType != EItemType::E_BagGrid)
	//{
	//	this->Sure();
	//}

	////读取原存档
	//UPlayerStructManager* NewLoadLocalPlayer = UGameSystemFunction::LoadCurrentPlayerData(__FUNCTION__ + FString(TEXT("准备进行支付操作")));
	//if (IsValid(NewLoadLocalPlayer))
	//{
	//	UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->SetPlayerDataInstance(NewLoadLocalPlayer);

	//	UPlayerStructManager* PlayerData = NewLoadLocalPlayer;

	//	//获取个数
	//	FString BuyCount = this->M_PlayerPayCountTextBox->GetText().ToString();
	//	int32 ItemCount = FCString::Atoi(*BuyCount);

	//	//道具名称
	//	FString ItemName;
	//	//道具价格
	//	int32 ItemPrice = 0;
	//	//货币名称
	//	FString ItemCoinName;
	//	//选择购买道具的类型
	//	EItemType ItemEItemType = EItemType::E_Card;
	//	//道具数据转换
	//	FItemPrice FItemPriceData;
	//	//购买结果
	//	bool BuyResult = false;

	//	//选择数据解析
	//	switch (this->M_EShopPayTipBuyType)
	//	{
	//	case EShopPayTipBuyType::E_DefShopBuy: {
	//		ItemName = this->M_FItemPrice.M_ItemName.ToString();
	//		ItemPrice = this->M_FItemPrice.M_ItemPrice;
	//		ItemCoinName = this->M_FItemPrice.M_ItemMoneyTypeName.ToString();
	//		ItemEItemType = this->M_FItemPrice.M_ItemType;
	//		FItemPriceData = this->M_FItemPrice;
	//	} break;
	//	case EShopPayTipBuyType::E_TicketShopBuy: {
	//		ItemName = this->M_FItemTicketPrice.M_ItemName.ToString();
	//		ItemPrice = this->M_FItemTicketPrice.M_TicketCount;
	//		ItemCoinName = this->M_FItemTicketPrice.M_TicketName.ToString();
	//		ItemEItemType = this->M_FItemTicketPrice.M_ItemType;
	//		//道具数据转换
	//		{
	//			FItemPriceData.M_ItemName = FText::FromString(ItemName);
	//			FItemPriceData.M_ItemType = ItemEItemType;
	//		}
	//	} break;
	//	}

	//	//如果不是【背包道具】
	//	if (ItemEItemType != EItemType::E_BagGrid)
	//	{
	//		if (ItemCount == 0)
	//		{
	//			UWidgetBase::CreateTipWidget(TEXT("购买失败，空间不足"));
	//			return;
	//		}
	//	}
	//	else {
	//		//如果是背包道具
	//		if (ItemEItemType == EItemType::E_BagGrid)
	//		{
	//			if (PlayerData->M_FPlayerCoin.M_Coin_1 < ItemPrice)
	//			{
	//				UWidgetBase::CreateTipWidget(TEXT("购买失败，礼券不足:") + FString::FromInt(ItemPrice));
	//				return;
	//			}
	//		}
	//	}

	//	//默认商城购买
	//	if (this->M_EShopPayTipBuyType == EShopPayTipBuyType::E_DefShopBuy)
	//	{
	//		if (ItemEItemType == EItemType::E_BagGrid)
	//		{
	//			if (FPlayerCoinAdd::Buy(PlayerData, ItemPrice, this->M_FItemPrice.M_ItemMoneyType))
	//			{
	//				BuyResult = true;
	//			}
	//		}
	//		else {
	//			if (FPlayerCoinAdd::Buy(PlayerData, ItemPrice * ItemCount, this->M_FItemPrice.M_ItemMoneyType))
	//			{
	//				BuyResult = true;
	//			}
	//		}

	//	}

	//	//券商城购买【扣除券】
	//	if (this->M_EShopPayTipBuyType == EShopPayTipBuyType::E_TicketShopBuy)
	//	{
	//		//货币数据
	//		FMaterialBase* RefData = nullptr;
	//		//货币名称
	//		for (auto& Data : PlayerData->M_PlayerItems_Material)
	//		{
	//			if (Data.ItemName.ToString().Equals(ItemCoinName))
	//			{
	//				RefData = &Data;
	//				break;
	//			}
	//		}

	//		if (RefData)
	//		{
	//			//判断是否足够
	//			if (RefData->M_Count >= this->M_FItemTicketPrice.M_TicketCount * ItemCount)
	//			{
	//				//减去资源货币
	//				RefData->M_Count -= this->M_FItemTicketPrice.M_TicketCount * ItemCount;
	//				if (RefData->M_Count <= 0)
	//				{
	//					RefData->bWaitRemove = true;
	//					UGameSystemFunction::ClearWaitingItems(PlayerData->M_PlayerItems_Material);

	//					//调用券支付成功并且券归零
	//					if (this->OnTicketPayFinishOver.IsBound())
	//					{
	//						OnTicketPayFinishOver.Execute();
	//					}
	//				}

	//				BuyResult = true;
	//			}
	//		}
	//	}

	//	if (!BuyResult)
	//	{
	//		//货币不足
	//		FString Tip = ItemCoinName + TEXT("不够:") + FString::FromInt(ItemPrice);
	//		UWidgetBase::CreateTipWidget(Tip, FVector(1.f, 0.f, 1.f));
	//	}
	//	else {
	//		//根据类型支付
	//		switch (ItemEItemType)
	//		{
	//			//将玩家背包的第ItemCount界面的背包开启
	//		case EItemType::E_BagGrid:
	//		{
	//			if (this->M_EShopPayTipBuyType == EShopPayTipBuyType::E_DefShopBuy)
	//			{
	//				UShopItemPriceStruct::AddPlayerBagGrid(PlayerData, FItemPriceData, ItemCount);
	//			}
	//		}break;
	//		//将物品添加到背包
	//		default: {
	//			//检查是否是货币资源
	//			if (PlayerData->CheckCoin(FItemPriceData.M_ItemName.ToString()))
	//			{
	//				PlayerData->SendCoin(
	//					FItemPriceData.M_ItemName.ToString(),
	//					this->M_FItemTicketPrice.M_ItemCount * ItemCount
	//				);

	//				if (UFVMGameInstance::GetDebug())
	//				{
	//					UE_LOG(LogTemp, Error,
	//						TEXT("支付成功，发生货币资源：【%s】，数量【%d】，购买量【%d】"),
	//						*FItemPriceData.M_ItemName.ToString(),
	//						this->M_FItemTicketPrice.M_ItemCount, ItemCount
	//					);
	//				}
	//			}
	//			else {

	//				UShopItemPriceStruct::AddToPlayerBag(PlayerData, FItemPriceData, ItemCount * this->M_BuyCountSendNums);
	//			}
	//		}break;
	//		}
	//		//保存数据
	//		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("支付完成")));
	//		//购买成功
	//		FString Tip = TEXT("获得道具:") + ItemName + TEXT("*") + FString::FromInt(ItemCount * this->M_BuyCountSendNums);
	//		UWidgetBase::CreateTipWidget(Tip, FVector(1.f, 0.5f, 0.f));

	//		//调用支付成功
	//		if (this->OnPayFinish.IsBound())
	//		{
	//			OnPayFinish.Execute();
	//		}

	//		//移除界面
	//		this->RemoveFromParent();
	//	}
	//}


}

UEditableTextBox* UUI_ShopPayTip::GetEditableTextBox()
{
	return this->M_PlayerPayCountTextBox;
}

UButton* UUI_ShopPayTip::GetInputTextButton()
{
	return this->M_PlayerInputButton;
}

void UUI_ShopPayTip::InputNum(FString _value)
{
	if (this->_Arrays_CoinNums.Num() >= 4)
		return;

	this->_Arrays_CoinNums.Emplace(_value);

	//更新文字
	this->_InputNumText.Empty();
	for (auto Text : this->_Arrays_CoinNums)
	{
		this->_InputNumText.Append(Text);
	}
}

void UUI_ShopPayTip::PopText()
{
	if (this->_Arrays_CoinNums.Num() == 0)
		return;


	this->_Arrays_CoinNums.Pop();

	//更新文字
	this->_InputNumText.Empty();
	for (auto Text : this->_Arrays_CoinNums)
	{
		this->_InputNumText.Append(Text);
	}
}

void UUI_ShopPayTip::Sure()
{
	//获取购买个数或者背包索引
	int32 ItemCount = FCString::Atoi(*this->_InputNumText);

	//当前数据修改->如果不是背包结构  才会进行修改
	if (this->M_FItemPrice.M_ItemType != EItemType::E_BagGrid && ItemCount == 0)
	{
		ItemCount = 1;
	}

	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//道具名称
	FString ItemName;
	//道具价格
	int32 ItemPrice = 0;
	//选择购买道具的类型
	EItemType ItemEItemType = EItemType::E_Card;

	//选择数据解析
	switch (this->M_EShopPayTipBuyType)
	{
	case EShopPayTipBuyType::E_DefShopBuy: {
		ItemName = this->M_FItemPrice.M_ItemName.ToString();
		ItemPrice = this->M_FItemPrice.M_ItemPrice;
		ItemEItemType = this->M_FItemPrice.M_ItemType;
	} break;
	case EShopPayTipBuyType::E_TicketShopBuy: {
		ItemName = this->M_FItemTicketPrice.M_ItemName.ToString();
		ItemPrice = this->M_FItemTicketPrice.M_TicketCount;
		ItemEItemType = this->M_FItemTicketPrice.M_ItemType;
	} break;
	}

	//验证购买个数是否符合要求
	switch (ItemEItemType)
	{
	case EItemType::E_Equipment:
	{
		//从背包寻找数据
		FEquipmentBase* SourceData = PlayerData->GetEquipmentByName(ItemName);
		if (SourceData)
		{
			//找到
			if (SourceData->M_IsOverlap)
			{
				//判断购买个数+原来的个数是否 超过9999
				int32 _ResultNum = SourceData->M_Count + ItemCount - 9999;
				ItemCount = _ResultNum <= 0 ? ItemCount : 9999 - SourceData->M_Count;
			}
			else {
				//判断背包个数
				int32 _ResultNum = PlayerData->GetBagNum(0) + ItemCount - PlayerData->GetBagMaxCount(0);
				ItemCount = _ResultNum <= 0 ? ItemCount : PlayerData->GetBagMaxCount(0) - PlayerData->GetBagNum(0);
			}
		}
		else {
			FEquipmentBase Data;
			UEquipmentBaseStruct::SearchEquipmentFromDataTable(ItemName, Data);

			if (Data.M_IsOverlap)
			{
				if (ItemCount - PlayerData->GetBagMaxCount(0) - PlayerData->GetBagNum(0) > 0)
				{
					//判断购买个数+原来的个数是否 超过9999
					int32 _ResultNum = ItemCount - 9999;
					ItemCount = _ResultNum <= 0 ? ItemCount : 9999 - ItemCount;
				}
			}
			else {
				//调整个数
				int32 _ResultNum = PlayerData->GetBagNum(0) + ItemCount - PlayerData->GetBagMaxCount(0);
				ItemCount = _ResultNum <= 0 ? ItemCount : PlayerData->GetBagMaxCount(0) - PlayerData->GetBagNum(0);
			}
		}
	}break;
	case EItemType::E_Card:
	{
		int32 _ResultNum = PlayerData->GetBagNum(1) + ItemCount - PlayerData->GetBagMaxCount(1);
		ItemCount = _ResultNum <= 0 ? ItemCount : PlayerData->GetBagMaxCount(1) - PlayerData->GetBagNum(1);
	}break;
	case EItemType::E_MaterialItem:
	{
		//如果不是货币
		if (!PlayerData->CheckCoin(ItemName))
		{
			FMaterialBase* SourceData = PlayerData->GetMaterialByName(ItemName);
			//查询数据
			if (SourceData)
			{
				//判断购买个数+原来的个数是否 超过9999
				int32 _ResultNum = SourceData->M_Count + ItemCount - 9999;
				ItemCount = _ResultNum <= 0 ? ItemCount : 9999 - SourceData->M_Count;
			}
			else {
				if (PlayerData->GetBagNum(2) == PlayerData->GetBagMaxCount(2))
				{
					ItemCount = 0;
				}
				else {
					if (ItemCount <= 0)
						ItemCount = 1;
					if (ItemCount > 9999)
						ItemCount = 9999;
				}
			}
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("券支付界面【货币获取】：%s，获得数量%d，购买量:%d"), *ItemName, this->M_FItemTicketPrice.M_ItemCount, ItemCount);
			}
		}


	}break;
	}

	if (ItemCount <= 0)
		ItemCount = 0;

	//更新个数
	this->GetEditableTextBox()->SetText(FText::FromString(FString::FromInt(ItemCount)));
	//更新价格
	this->M_ItemPirceText = FString::FromInt(ItemCount * ItemPrice);
}

bool UUI_ShopPayTip::Initialize()
{
	if (!Super::Initialize())
		return false;


	this->M_PlayerPayButton = this->GetWidgetComponent<UButton>(this, "Pay");
	this->M_PlayerInputButton = this->GetWidgetComponent<UButton>(this, "Button_434");
	this->M_PlayerPayCountTextBox = this->GetWidgetComponent<UEditableTextBox>(this, "Input_PlayerBuyCount");

	if (this->M_PlayerPayButton)
	{
		FScriptDelegate CallFunc;
		CallFunc.BindUFunction(this, "PayButton");
		this->M_PlayerPayButton->OnClicked.Add(CallFunc);
	}

	return true;
}


