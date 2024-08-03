// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GiftBox.h"
#include "Game/UI/UI_TableTextBlock.h"
#include "Game/UI/UI_PlayerBagEquipmentGrid.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/VerticalBox.h>
#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "Data/EquipmentDataSturct.h"

#include <Kismet/KismetTextLibrary.h>

FTimeClip UUI_GiftBox::M_FTimeClip_1;

UWidgetBase* UUI_GiftBox::M_ShowTextTipsPanel = nullptr;

bool UUI_GiftBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_OpenOnce = this->GetWidgetComponent<UButton>(this, "Open_Once");
	this->M_OpenAll = this->GetWidgetComponent<UButton>(this, "Open_All");

	this->M_ScrollBox = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_46");
	this->M_UUniformGridPanel = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_35");


	this->M_OpenOnce->OnClicked.AddDynamic(this, &UUI_GiftBox::OpenGiftOnce);
	this->M_OpenAll->OnClicked.AddDynamic(this, &UUI_GiftBox::OpenGiftAll);

	return true;
}

void UUI_GiftBox::InitLoader()
{
	//加载器初始化
	this->M_ListLoaderManager = NewObject<UItemLoadManager>(this, TEXT("Gift_ListLoaderManager"));

	this->M_ListLoaderManager->InitWidget_First(this->M_UUniformGridPanel, this->M_ScrollBox, 0.01f);
	this->M_ListLoaderManager->InitRange_Second(12, 1, 500, 600.f, 60.f);
	this->M_ListLoaderManager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Init"));
	this->M_ListLoaderManager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Update"));
}

UWidget* UUI_GiftBox::WidgetCreate_Init(UItemDataTable* _Data, int32 _Index)
{
	UUI_TableTextBlock* TextBlock = CreateWidget<UUI_TableTextBlock>(this, LoadClass<UUI_TableTextBlock>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TableTextBlock.BPUI_TableTextBlock_C'")));
	TextBlock->SetText(((FItemBase*)(_Data->GetValue()))->ItemName);
	return TextBlock;
}

void UUI_GiftBox::WidgetRefresh_Update(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_TableTextBlock>(_UWidget)->SetText(((FItemBase*)(_Data->GetValue()))->ItemName);
}

void UUI_GiftBox::SetPlayerBagEquipmentGrid(UUI_PlayerBagEquipmentGrid* _GridUI)
{
	this->M_UI_PlayerBagEquipmentGrid = _GridUI;
}

UUI_PlayerBagEquipmentGrid* UUI_GiftBox::GetPlayerBagEquipmentGrid()
{
	return this->M_UI_PlayerBagEquipmentGrid;
}

void UUI_GiftBox::SetGiftBoxData(const FString& _Name)
{
	UGameSystemFunction::OpenUpdate();

	bool Result = false;

	//加载礼盒数据
	TArray<FGiftBox> Item;
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_GiftBox_Data, FGiftBox>(
		Cache->GetGiftBox(),
		Item
	);
	UEquipmentBaseStruct::GetEquipmentArraysData<FGiftBox>(_Name, Item, this->M_GiftBoxData, Result);

	if (Result)
	{
		//设置标题
		this->M_Title = this->M_GiftBoxData.ItemName;
		//设置内容
		this->M_Content = this->M_GiftBoxData.ItemDescrible;
		//加载列表
		//this->ShowGiftList();
	}
}

void UUI_GiftBox::OpenGiftOnce()
{

	this->AnalysisBegin(1);

	//礼盒数量减一
	FEquipmentBase* Item = UItemBaseStruct::GetItemFromBag<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment, this->M_Title);

	if (Item)
	{
		Item->M_Count--;
		if (Item->M_Count <= 0)
		{
			Item->bWaitRemove = true;
			UGameSystemFunction::ClearWaitingItemsForEquip(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);
		}
	}


	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("礼盒开启一次")));
	//UGameSystemFunction::LoadCurrentPlayerData();

	this->GetPlayerBagEquipmentGrid()->RelaodBagEquipment();

	this->ShowTipText();
}

void UUI_GiftBox::OpenGiftAll()
{
	//礼盒数量减一
	FEquipmentBase* Item = UItemBaseStruct::GetItemFromBag<FEquipmentBase>(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment, this->M_Title
	);

	if (Item)
	{
		//如果当前道具数量使用大于500  则最大使用500
		if (Item->M_Count > 500)
		{
			Item->M_Count -= 500;
			this->AnalysisBegin(500);
		}
		else {

			int32 UseCount = Item->M_Count;

			//否则直接全部使用
			Item->M_Count -= Item->M_Count;

			//道具标记
			Item->bWaitRemove = true;

			//使用道具
			this->AnalysisBegin(UseCount);

			UGameSystemFunction::ClearWaitingItemsForEquip(
				UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment
			);
		}
	}


	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("礼盒全部开启")));

	this->GetPlayerBagEquipmentGrid()->RelaodBagEquipment();

	this->ShowTipText();
}

void UUI_GiftBox::AnalysisGiftBoxData()
{
	//剩下的礼盒
	TArray<FGiftBox_Item> GiftBox_Items;

	//解析礼盒内容
	for (auto Item : this->M_GiftBoxData.M_GiftContent)
	{
		if (Item.M_SelectCurrentTypeAll)
		{
			//如果当前开启将所有道具全部选择 （对当前的类型的所有道具进行全选）
			switch (Item.M_ItemType)
			{
			case EGiftBox_Item_Type::E_Card: { UCardBaseStruct::GetAllCardName(this->M_ItemBase); }break;
			case EGiftBox_Item_Type::E_Equipment: { UEquipmentBaseStruct::GetAllEquipmentName(this->M_ItemBase); }break;
			case EGiftBox_Item_Type::E_Material: { UMaterialBaseStruct::GetAllMaterial(this->M_ItemBase); }break;
			case EGiftBox_Item_Type::E_Coin: { this->M_ItemBase.Append(FPlayerCoinAdd::GetCoinNames()); }break;
			}
		}
		else {
			//添加非全选物品
			GiftBox_Items.Emplace(Item);
		}
	}


	//如果没有任何奖品则不会显示在列表上
	if (GiftBox_Items.Num() < 1)
	{
		return;
	}

	int32 Index = 0;
	for (auto& Gift : GiftBox_Items)
	{

		FItemBase Item;

		//如果是最后一个奖品
		if (this->M_GiftBoxData.M_bEnableRandomMode && GiftBox_Items.Num() > 1 && Index == GiftBox_Items.Num() - 1)
		{
			Gift.M_GiftBox_RandomRange.M_Max = this->M_GiftBoxData.M_FGiftBox_RandomModeDetail.M_RandomMaxNumber;
			Gift.M_GiftBox_RandomRange.M_Min = GiftBox_Items[Index - 1].M_GiftBox_RandomRange.M_Max + 1;
		}

		if (this->M_GiftBoxData.M_bEnableRandomMode)
		{
			float Value = (((Gift.M_GiftBox_RandomRange.M_Max - Gift.M_GiftBox_RandomRange.M_Min + 1) * 1.f) / this->M_GiftBoxData.M_FGiftBox_RandomModeDetail.M_RandomMaxNumber) * 100.0f;
			FText _text = FText::FromString(FString::Printf(TEXT("%.3g"), Value));
			Item.ItemName = Gift.M_ItemName + " " + _text.ToString() + "%";
		}
		else {
			Item.ItemName = Gift.M_ItemName + " *" + FString::FromInt(Gift.M_ItemCount);
		}

		this->M_ItemBase.Emplace(Item);

		Index++;
	}

}

void UUI_GiftBox::ShowGiftList()
{
	this->AnalysisGiftBoxData();

	UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("解析的道具数量") + FString::FromInt(this->M_ItemBase.Num()));

	this->M_ListLoaderManager->UpdateDatatable(this->M_ItemBase);
	this->M_ListLoaderManager->SetLoadItemMaxCount(this->M_ItemBase.Num());
	this->M_ListLoaderManager->ContinueRun();
}

void UUI_GiftBox::ShowTipText()
{
	//分时加载文字

	UGameSystemFunction::ClearTimeClip(UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager(), UUI_GiftBox::M_FTimeClip_1);

	//UE_LOG(LogTemp, Error, TEXT("%d"), this->M_TipText.Num());

	//提示文字
	static TArray<FString> Tips;
	static TArray<FVector> TipsColor;
	//先清空文字
	Tips.Empty();
	TipsColor.Empty();
	//赋值文字
	Tips = this->M_TipText;
	TipsColor = this->M_TipTextColor;
	//清空文字
	this->M_TipText.Empty();
	this->M_TipTextColor.Empty();

	//创建背景
	UUI_GiftBox::M_ShowTextTipsPanel = CreateWidget<UWidgetBase>(this, LoadClass<UWidgetBase>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_OpenGiftBoxPanel.BPUI_OpenGiftBoxPanel_C'")));
	//显示背景
	UUI_GiftBox::M_ShowTextTipsPanel->AddToViewport();

	UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager().SetTimer(UUI_GiftBox::M_FTimeClip_1.M_FTimerHandle, [&]() {
		//判断索引是否有效
		if (this->UUI_GiftBox::M_FTimeClip_1.M_Count < Tips.Num())
		{

			UWidgetBase::CreateTipWidget(Tips[this->UUI_GiftBox::M_FTimeClip_1.M_Count], TipsColor[this->UUI_GiftBox::M_FTimeClip_1.M_Count]);
			this->UUI_GiftBox::M_FTimeClip_1.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager(), UUI_GiftBox::M_FTimeClip_1);
			//先清空文字
			Tips.Empty();
			TipsColor.Empty();

			//关闭背景
			if (UUI_GiftBox::M_ShowTextTipsPanel)
				UUI_GiftBox::M_ShowTextTipsPanel->RemoveFromParent();
		}
		}, 0.15f, true);
}

void UUI_GiftBox::CloseTextTips()
{
	UGameSystemFunction::ClearTimeClip(UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager(), UUI_GiftBox::M_FTimeClip_1);
}

void UUI_GiftBox::TClosePanel()
{
	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
}

void UUI_GiftBox::AnalysisBegin(int32 _OpenCount)
{
	//等待抽奖的物品
	TArray<FGiftBox_Item> Items;

	this->AnalysisGift(Items);

	//当数量大于0 则该抽奖
	int32 ItemCount = Items.Num();

	if (ItemCount > 0)
	{
		//存储通过抽奖或者其他方式获得的物品(被抽出来的物品(你得到的物品))
		TArray<FGiftBox_Item> _Items_;
		//解析抽奖模式(抽奖或者不抽奖)
		this->AnalysisGiftBoxMode(_OpenCount, Items, _Items_);
		//处理最后一次礼盒
		this->AnalysisItem(_Items_);
	}
}

void UUI_GiftBox::AnalysisGift(TArray<FGiftBox_Item>& Items)
{
	//将所有的礼盒遍历
	for (FGiftBox_Item& Item : this->M_GiftBoxData.M_GiftContent)
	{
		//全部都选择，并且没有开启随机模式->那么将获得所有道具
		if (Item.M_SelectCurrentTypeAll && !this->M_GiftBoxData.M_bEnableRandomMode)
		{
			//如果当前开启将所有道具全部选择
			switch (Item.M_ItemType)
			{
			case EGiftBox_Item_Type::E_Card: {
				UCardBaseStruct::CreateAllCardToBag(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager(), Item.M_CardGrade);
			}break;
			case EGiftBox_Item_Type::E_Equipment: {

				TArray<FItemBase> LItems;
				UEquipmentBaseStruct::GetAllEquipmentName(LItems);

				for (auto TItem = LItems.CreateConstIterator(); TItem; ++TItem)
				{
					if ((*TItem).ItemName.Equals(TEXT("全装备礼盒")))
						continue;

					UGameSystemFunction::SendEquipmentToPlayerBag((*TItem).ItemName, 1, false);
				}

			}; break;
			case EGiftBox_Item_Type::E_Material: {
				TArray<FItemBase> LItems;
				UMaterialBaseStruct::GetAllMaterial(LItems);

				for (auto TItem = LItems.CreateConstIterator(); TItem; ++TItem)
				{
					UGameSystemFunction::SendMaterialToPlayerBag((*TItem).ItemName, 9999, false);
				}
			}break;
			case EGiftBox_Item_Type::E_Coin: {
				for (int32 i = 0; i < 9999; i++)
				{
					//每一个货币类型全部添加一次
					if (!UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->AddCoin(99999, i))
						break;
				}
			}break;
			}

			//将文字加入到列表准备一次显示
			FString Text = TEXT("恭喜你获得全部道具");
			this->M_TipText.Emplace(Text);
			this->M_TipTextColor.Emplace(FVector(1.f, 0.2f, 0.5f));
		}
		else {
			//将其他剩余物品全部加入到列表
			Items.Emplace(Item);
		}
	}
}

void UUI_GiftBox::AnalysisGiftBoxMode(int32 _ItemCount, TArray<FGiftBox_Item>& Get_Items, TArray<FGiftBox_Item>& Items)
{
	//开启随机模式
	if (this->M_GiftBoxData.M_bEnableRandomMode)
	{
		//当前获取的个数
		int32 GetCount = 0;

		//(当奖品大于大于2个的时候)自动设置最后一个奖励的概率
		if (Get_Items.Num() > 1)
		{
			Get_Items[Get_Items.Num() - 1].M_GiftBox_RandomRange.M_Max = this->M_GiftBoxData.M_FGiftBox_RandomModeDetail.M_RandomMaxNumber;
			Get_Items[Get_Items.Num() - 1].M_GiftBox_RandomRange.M_Min = Get_Items[Get_Items.Num() - 2].M_GiftBox_RandomRange.M_Max + 1;
		}

		//一共抽取的次数->表示你开启了多少个盒子
		for (int32 _CurrentItem = 0; _CurrentItem < _ItemCount; _CurrentItem++)
		{
			//抽取道具次数
			for (; GetCount < this->M_GiftBoxData.M_FGiftBox_RandomModeDetail.M_GetGiftMaxCount;)
			{
				//随机抽奖
				int32 RandomNumber = UGameSystemFunction::GetRandomRange(0, this->M_GiftBoxData.M_FGiftBox_RandomModeDetail.M_RandomMaxNumber);

				if (UFVMGameInstance::GetDebug())
					UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("随机数字：") + FString::FromInt(RandomNumber));

				//查询重复
				bool bSerachResult = false;

				//查看当前随机数在那个奖品范围内
				for (auto& Gift : Get_Items)
				{
					//得到当前抽到的奖品
					if (RandomNumber >= Gift.M_GiftBox_RandomRange.M_Min && RandomNumber <= Gift.M_GiftBox_RandomRange.M_Max)
					{

						//查询是否有当前道具
						for (auto& ItemData : Items)
						{
							if (ItemData.M_ItemName.Equals(Gift.M_ItemName)) {
								ItemData.M_ItemCount += Gift.M_ItemCount;
								bSerachResult = true;
							}
						}

						//没有添加
						if (!bSerachResult)
							Items.Emplace(Gift);

						bSerachResult = false;
						GetCount++;

						break;
					}
				}
			}

			//重置抽奖奖品个数
			GetCount = 0;
		}
	}
	else {
		//没有开启随机模式->不会受到礼盒个数的限制
		for (int32 i = 0; i < Get_Items.Num(); i++)
		{
			//开启了多少个礼盒就*多少倍
			Get_Items[i].M_ItemCount *= _ItemCount;

			Items.Emplace(Get_Items[i]);
		}
	}
}

void UUI_GiftBox::AnalysisItem(TArray<FGiftBox_Item>& Items)
{
	//解析道具（分发道具）
	for (FGiftBox_Item& ItemData : Items)
	{
		switch (ItemData.M_ItemType)
		{
		case EGiftBox_Item_Type::E_Card: this->CardToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Equipment:this->EquipmentToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Material:this->MaterialToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Coin:this->CoinToBag(ItemData); break;
		}
	}
}

void UUI_GiftBox::CardToBag(FGiftBox_Item& Item)
{
	FItemCard Card;

	if (UCardBaseStruct::SearchCardFromDataTable(Item.M_ItemName, Card))
	{
		Card.M_CardGrade = Item.M_CardGrade;

		//根据数量发送背包
		for (int32 i = 0; i < Item.M_ItemCount; i++)
		{
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(Card);
		}

		// 1.f 0.2f 0.f
		//将文字加入到列表准备一次显示
		FString Text = TEXT("恭喜你获得:") + Card.ItemName + TEXT("*") + FString::FromInt(Item.M_ItemCount);
		this->M_TipText.Emplace(Text);
		this->M_TipTextColor.Emplace(FVector(1.f, 0.2f, 0.f));
	}

}

void UUI_GiftBox::EquipmentToBag(FGiftBox_Item& Item)
{
	//发送道具
	UGameSystemFunction::SendEquipmentToPlayerBag(Item.M_ItemName, Item.M_ItemCount, false);

	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Item.M_ItemName + TEXT("*") + FString::FromInt(Item.M_ItemCount);
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(1.f, 0.f, 1.f));
}

void UUI_GiftBox::MaterialToBag(FGiftBox_Item& Item)
{

	UGameSystemFunction::SendMaterialToPlayerBag(Item.M_ItemName, Item.M_ItemCount, false);

	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Item.M_ItemName + TEXT("*") + FString::FromInt(Item.M_ItemCount);
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(1.f));
}

void UUI_GiftBox::CoinToBag(FGiftBox_Item& Item)
{
	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->AddCoin(Item.M_ItemCount, Item.M_CoinType);

	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Item.M_ItemName + TEXT("[") + FString::FromInt(Item.M_ItemCount) + TEXT("]");
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(0.5f, 0.f, 1.f));
}