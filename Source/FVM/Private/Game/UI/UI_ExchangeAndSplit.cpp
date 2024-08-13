// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_ExchangeAndSplit.h"

#include "GameSystem/Tools/ItemLoadManager.h"
#include "Game/UI/UI_TicketShopGrid.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/UniformGridPanel.h>
#include <Components/ScrollBox.h>

//兑换区
UWidget* UUI_ExchangeAndSplit::WidgetCreate_ExchangeInit(UItemDataTable* _Data, int32 _Index)
{
	UUI_TicketShopGrid* _Grid = CreateWidget<UUI_TicketShopGrid>(this, LoadClass<UUI_TicketShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TicketShopGrid.BPUI_TicketShopGrid_C'")));
	//绑定函数
	_Grid->OnPayFinish.BindUFunction(this, TEXT("PayFinish"));
	_Grid->OnPayFinishOver.BindUFunction(this, TEXT("TicketPayFinishOver"));
	//设置券的图标
	_Grid->SetTicketHead(((FItemTicketPrice*)(_Data->GetValue()))->M_TicketHeadTexture2DPath.ToString());
	//设置物品的头像
	_Grid->SetItemHead(((FItemTicketPrice*)(_Data->GetValue()))->M_ItemHeadTexture2DPath.ToString());
	//设置数据
	_Grid->SetItemData(*((FItemTicketPrice*)(_Data->GetValue())));
	//设置初始化
	_Grid->InitBase();
	//播放动画
	_Grid->Anim();

	return _Grid;
}

void UUI_ExchangeAndSplit::WidgetRefresh_ExchangeUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	//设置券的图标
	Cast<UUI_TicketShopGrid>(_UWidget)->SetTicketHead(((FItemTicketPrice*)(_Data->GetValue()))->M_TicketHeadTexture2DPath.ToString());
	//设置物品的头像
	Cast<UUI_TicketShopGrid>(_UWidget)->SetItemHead(((FItemTicketPrice*)(_Data->GetValue()))->M_ItemHeadTexture2DPath.ToString());
	//设置数据
	Cast<UUI_TicketShopGrid>(_UWidget)->SetItemData(*((FItemTicketPrice*)(_Data->GetValue())));
	//设置初始化
	Cast<UUI_TicketShopGrid>(_UWidget)->InitBase();
}

//分解区-材料
UWidget* UUI_ExchangeAndSplit::WidgetCreate_SplitMaterialInit(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LoadClass<UUI_PlayerBagMaterialGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")));

	Grid->SetUI_Other(this);

	Grid->GetButton()->OnClicked.AddDynamic(Grid, &UUI_PlayerBagMaterialGrid::AddExchangeSplit_SplitSlot);

	this->SetSplitMaterials(_Data, _Index, Cast<UWidget>(Grid));

	return Grid;
}

void UUI_ExchangeAndSplit::WidgetRefresh_SplitMaterialUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetSplitMaterials(_Data, _Index, _UWidget);
}

void UUI_ExchangeAndSplit::SetSplitMaterials(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_PlayerBagMaterialGrid* const Grid = Cast<UUI_PlayerBagMaterialGrid>(_UWidget);
	FMaterialBase* const Data = (FMaterialBase*)_Data->GetValue();

	Grid->SetMaterialData(Data);
	Grid->SetIndex(_Index);
	Grid->UpdateMaterialsShowCount(FString::FromInt(Data->M_Count > 9999 ? 9999 : Data->M_Count));
	this->SetButtonStyle(Grid->GetButton(), Data->ItemTexturePath.ToString());
	Grid->GetButton()->SetIsEnabled(true);

	if (this->M_CurrentSelectType == EItemType::E_MaterialItem)
	{
		if (_Index == this->M_CurrentSelectIndex)
		{
			Grid->AddExchangeSplit_SplitSlot();
		}
	}
}

//分解区-卡片 
UWidget* UUI_ExchangeAndSplit::WidgetCreate_CardInit(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this, LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));

	Grid->SetUI_Other(this);

	Grid->SetRenderScale(FVector2D(0.8f));

	Grid->GetButton()->OnClicked.AddDynamic(Grid, &UUI_PlayerBagCardGrid::AddExchangeSplit_SplitSlot);

	this->SetSplitCards(_Data, _Index, Cast<UWidget>(Grid));

	return Grid;
}

void UUI_ExchangeAndSplit::WidgetRefresh_CardUpdate(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetSplitCards(_Data, _Index, _UWidget);
}

void UUI_ExchangeAndSplit::SetSplitCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_PlayerBagCardGrid* const Grid = Cast<UUI_PlayerBagCardGrid>(_UWidget);
	FItemCard* const Data = (FItemCard*)_Data->GetValue();

	Grid->SetFItemCardData(Data);
	Grid->SetUIIndex(_Index);
	Grid->UpdateButtonTexture(FString::FromInt(Data->M_CardPrice));
	this->SetButtonStyle(Grid->GetButton(), Data->ItemTexturePath.ToString());
	Grid->GetButton()->SetIsEnabled(true);

	if (this->M_CurrentSelectType == EItemType::E_Card)
	{
		if (_Index == this->M_CurrentSelectIndex)
		{
			Grid->AddExchangeSplit_SplitSlot();
		}
	}
}

void UUI_ExchangeAndSplit::SetTicketsByTreviFountain(const TArray<FTreviFountainItemStruct>& _Input, TArray<FItemTicketPrice>& _Out)
{
	for (const auto& _Item : _Input)
	{
		FItemTicketPrice PriceObj{
		  _Item.M_ItemName,
		  EItemType::E_Card,
		  _Item.M_ItemCount,
		  _Item.M_ItemHeadPath.ToString(),
		  this->NeedResourceTexturePath,
		  TEXT("重要的资源之一"),
		  this->NeedResourceName,
		  _Item.M_TransPrice,
		  true
		};

		switch (_Item.M_ItemType)
		{
		case ETreviFountainItemType::TT_Card: {
			PriceObj.M_ItemType = EItemType::E_Card;
			FItemCard CData;
			if (UCardBaseStruct::SearchCardFromDataTable(_Item.M_ItemName, CData))
				PriceObj.M_ItemDescirbe = CData.ItemDescrible.ToString();
		}break;
		case ETreviFountainItemType::TT_Equipment: {
			PriceObj.M_ItemType = EItemType::E_Equipment;
			FEquipmentBase EData;
			if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(_Item.M_ItemName, EData))
				PriceObj.M_ItemDescirbe = EData.ItemDescrible.ToString();
		}break;
		case ETreviFountainItemType::TT_Material: {
			PriceObj.M_ItemType = EItemType::E_MaterialItem;
			FMaterialBase MData;
			if (UMaterialBaseStruct::SearchMaterailFromDataTable(_Item.M_ItemName, MData))
				PriceObj.M_ItemDescirbe = MData.ItemDescrible.ToString();
		}break;
		}

		//数量没有转换
		_Out.Emplace(PriceObj);
	}
}

void UUI_ExchangeAndSplit::SetTicketsTransformByTreviFountain(const TArray<FTreviFountainItemStruct>& _Input)
{
	for (auto& Data : _Input)
	{
		switch (Data.M_ItemType)
		{
		case ETreviFountainItemType::TT_Equipment:
		{
			if (Data.M_ItemName.Contains(TEXT("转")) || Data.M_ItemName.Contains(TEXT("进化凭证")))
			{
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_CardChangePosition);
			}
			else if (Data.M_ItemName.Contains(TEXT("技能书")))
			{
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_CardBooks);
			}
			else {
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_OtherItems);
			}
		}break;
		case ETreviFountainItemType::TT_Card:
		{
			this->SetTicketsByTreviFountain(TArray{ Data }, this->M_Cards);
		}break;
		case ETreviFountainItemType::TT_Material:
		{
			if (Data.M_ItemName.Contains(TEXT("转")) || Data.M_ItemName.Contains(TEXT("进化凭证")))
			{
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_CardChangePosition);
			}
			else if (Data.M_ItemName.Contains(TEXT("技能书")))
			{
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_CardBooks);
			}
			else {
				this->SetTicketsByTreviFountain(TArray{ Data }, this->M_OtherItems);
			}
		}break;
		}
	}
}

void UUI_ExchangeAndSplit::SetTicketsNative(const FSoftObjectPath& _DataTabel, TArray<FItemTicketPrice>& _Out)
{
	TArray<FItem_TicketPrice_Data> Datas;
	UGameSystemFunction::GetDataTableRows<FItem_TicketPrice_Data>(_DataTabel.ToString(), Datas);
	for (const auto& Data : Datas)
		_Out.Emplace(Data.M_FItemPrice);
}

void UUI_ExchangeAndSplit::SetTicketName(const FString& _TicketName, const FString& _TicketTexturePath)
{
	this->NeedResourceName = _TicketName;
	this->NeedResourceTexturePath = _TicketTexturePath;
	this->SetImageBrush(this->M_ResourceHead, _TicketTexturePath);
	this->SetImageBrush(this->M_Split_Ticket_LeftImg, _TicketTexturePath);
	this->SetImageBrush(this->M_Split_Ticket_RightImg, _TicketTexturePath);

	this->M_Split_Ticket_LeftImg->SetVisibility(ESlateVisibility::Collapsed);
	this->M_Split_Ticket_RightImg->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_ExchangeAndSplit::SetKeywordsByMaterials(const TArray<FString>& _Keys, const TArray<FString>& _KeyIgnore)
{
	this->M_MaterialsKeywords.Empty();
	this->M_MaterialsKeywords_Ignore.Empty();
	this->M_MaterialsKeywords.Append(_Keys);

	if (_KeyIgnore.Num() != 0)
		this->M_MaterialsKeywords_Ignore.Append(_KeyIgnore);
}

void UUI_ExchangeAndSplit::SetKeywordsByCards(const TArray<FString>& _Keys, const TArray<FString>& _KeyIgnore)
{
	this->M_CardsKeywords.Empty();
	this->M_CardsKeywords_Ignore.Empty();
	this->M_CardsKeywords.Append(_Keys);

	if (_KeyIgnore.Num() != 0)
		this->M_CardsKeywords_Ignore.Append(_KeyIgnore);
}

void UUI_ExchangeAndSplit::SetCardsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel)
{
	this->SetTicketsNative(_DataTabel.ToSoftObjectPath(), this->M_Cards);
}

void UUI_ExchangeAndSplit::SetSkillBooksTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel)
{
	this->SetTicketsNative(_DataTabel.ToSoftObjectPath(), this->M_CardBooks);
}

void UUI_ExchangeAndSplit::SetChangeJobMaterialsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel)
{
	this->SetTicketsNative(_DataTabel.ToSoftObjectPath(), this->M_CardChangePosition);
}

void UUI_ExchangeAndSplit::SetOtherMaterialsTicketByDatas(const TSoftObjectPtr<UDataTable>& _DataTabel)
{
	this->SetTicketsNative(_DataTabel.ToSoftObjectPath(), this->M_OtherItems);
}

void UUI_ExchangeAndSplit::SetTicketByTreviFountain(const TSoftObjectPtr<UDataTable>& _DataTabel)
{
	TArray<FTreviFountainStructData> Datas;
	TArray<FTreviFountainItemStruct> Out;
	UGameSystemFunction::GetDataTableRows<FTreviFountainStructData>(_DataTabel.ToSoftObjectPath().ToString(), Datas);
	for (const auto& Data : Datas)
		Out.Emplace(Data.M_FTreviFountainStruct);

	this->SetTicketsTransformByTreviFountain(Out);
}

const int32 UUI_ExchangeAndSplit::GetCurrentSplitItemCountByName(const FString& _ItemName) const
{
	TArray<FItemTicketPrice> All;
	All.Append(this->M_Cards);
	All.Append(this->M_CardBooks);
	All.Append(this->M_CardChangePosition);
	All.Append(this->M_OtherItems);

	for (const auto& Ldata : All)
		if (Ldata.M_ItemName.Equals(_ItemName))
			return (int32)(Ldata.M_TicketCount / 2);

	return int32(0);
}

void UUI_ExchangeAndSplit::SetSplitEnable(bool _State)
{
	if (_State)
	{
		this->M_NeedSplitCard_Butt->SetVisibility(ESlateVisibility::Visible);
		this->M_SplitCard_Butt->SetIsEnabled(true);
		this->M_Split_Ticket_LeftImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_Split_Ticket_RightImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_Split_Generate_ResourceTicketCount_Left_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_Split_Generate_ResourceTicketCount_Right_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_Split_Generate_ResourceTicket_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->M_NeedSplitCard_Butt->SetVisibility(ESlateVisibility::Collapsed);
		this->M_SplitCard_Butt->SetIsEnabled(false);
		this->M_Split_Ticket_LeftImg->SetVisibility(ESlateVisibility::Collapsed);
		this->M_Split_Ticket_RightImg->SetVisibility(ESlateVisibility::Collapsed);
		this->M_Split_Generate_ResourceTicketCount_Left_Text->SetVisibility(ESlateVisibility::Collapsed);
		this->M_Split_Generate_ResourceTicketCount_Right_Text->SetVisibility(ESlateVisibility::Collapsed);
		this->M_Split_Generate_ResourceTicket_Text->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_ExchangeAndSplit::SetCurrentSlipItem(const EItemType& _CurrentSelectType, const int32& _Index, UItemDataTable* const _OutSelectData, UButton* const _CurrentSelectButt)
{
	this->M_CurrentSelectType = _CurrentSelectType;
	this->M_CurrentSelectIndex = _Index;
	this->M_CurrentSelectButt = _CurrentSelectButt;

	if (IsValid(this->M_CurrentSelect) && IsValid(_OutSelectData))
	{
		this->M_CurrentSelect->SetValue(_OutSelectData->GetValue());
	}
	else {
		if (!IsValid(this->M_CurrentSelect))
		{
			this->M_CurrentSelect = NewObject<UItemDataTable>(this, TEXT("ExchangeSplit_Iten_CurrentSelect"));
		}

		if (IsValid(_OutSelectData))
		{
			this->M_CurrentSelect->SetValue(_OutSelectData->GetValue());
		}
		else {
			this->M_CurrentSelect->SetValue(nullptr);
		}
	}

	if (IsValid(this->M_CurrentSelect) && this->M_CurrentSelect->GetValue() && this->M_CurrentSelectIndex != -1)
	{
		this->M_CurrentSplitCount = this->GetCurrentSplitItemCountByName(
		((FItemBase*)(this->M_CurrentSelect->GetValue()))->ItemName.ToString()
		);
		this->SetSplitItemAddedRate(this->M_RandomAddedRate);
		this->SetButtonStyle(this->M_NeedSplitCard_Butt, ((FItemBase*)(this->M_CurrentSelect->GetValue()))->ItemTexturePath.ToString(), true);
		this->SetSplitEnable(true);
	}
	else
	{
		this->M_CurrentSplitCount = 0;
		this->SetSplitEnable(false);
	}

}

void UUI_ExchangeAndSplit::SetSplitItemAddedRate(float _Value)
{
	this->M_RandomAddedRate = _Value;

	if (this->M_RandomAddedRate < 0.f)
		this->M_RandomAddedRate = 0.f;

	//设置额外产出
	this->M_CurrentOutSplitCount = this->M_CurrentSplitCount * this->M_RandomAddedRate;

	//设置产出的文字
	this->M_Split_Generate_ResourceTicketCount_Left_Text->SetText(FText::FromString(TEXT("x") + FString::FromInt(this->M_CurrentSplitCount)));
	this->M_Split_Generate_ResourceTicketCount_Right_Text->SetText(FText::FromString(TEXT("x") + FString::FromInt(this->M_CurrentOutSplitCount)));

	//设置百分比
	this->M_Split_Generate_ResourceTicket_Text->SetText(FText::FromString(FString::FromInt(this->M_RandomAddedRate * 100) + TEXT("%")));
}

float UUI_ExchangeAndSplit::GetSplitItemAddedRate()
{
	return this->M_RandomAddedRate;
}

UButton* UUI_ExchangeAndSplit::GetCurrentSelectButt()
{
	return this->M_CurrentSelectButt;
}

void UUI_ExchangeAndSplit::UpdateListByExchange()
{
	this->M_Exchange_Loadding->SetCurrentPage(0);
	this->M_Exchange_Loadding->SetResetScrollBoxOffset();
	this->M_Exchange_Loadding->ContinueRun();
}

void UUI_ExchangeAndSplit::UpdateResourceText()
{
	//更新点券
	this->M_ResourceD->SetText(FText::FromString(FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2)));

	//更新券
	this->TicketPayFinishOver();
}

void UUI_ExchangeAndSplit::PayFinish()
{
	this->UpdateResourceText();
}

void UUI_ExchangeAndSplit::TicketPayFinishOver()
{
	this->M_ResourceTicket->SetText(FText::FromString(TEXT("0")));
	for (const auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
	{
		if (Data.ItemName.ToString().Equals(this->NeedResourceName) && Data.M_Count > 0)
		{
			this->M_ResourceTicket->SetText(FText::FromString(FString::FromInt(Data.M_Count)));
			break;
		}
	}
}

void UUI_ExchangeAndSplit::ShowAll()
{
	this->M_AllItems.Empty();
	this->M_AllItems.Append(this->M_Cards);
	this->M_AllItems.Append(this->M_CardBooks);
	this->M_AllItems.Append(this->M_CardChangePosition);
	this->M_AllItems.Append(this->M_OtherItems);

	this->M_Exchange_Loadding->UpdateDatatable(this->M_AllItems);
	this->M_Exchange_Loadding->SetLoadItemMaxCount(this->M_AllItems.Num());
	this->UpdateListByExchange();
}

void UUI_ExchangeAndSplit::ShowCard()
{
	this->M_Exchange_Loadding->UpdateDatatable(this->M_Cards);
	this->M_Exchange_Loadding->SetLoadItemMaxCount(this->M_Cards.Num());
	this->UpdateListByExchange();
}

void UUI_ExchangeAndSplit::ShowSkillBooks()
{
	this->M_Exchange_Loadding->UpdateDatatable(this->M_CardBooks);
	this->M_Exchange_Loadding->SetLoadItemMaxCount(this->M_CardBooks.Num());
	this->UpdateListByExchange();
}

void UUI_ExchangeAndSplit::ShowChangeJobMaterials()
{
	this->M_Exchange_Loadding->UpdateDatatable(this->M_CardChangePosition);
	this->M_Exchange_Loadding->SetLoadItemMaxCount(this->M_CardChangePosition.Num());
	this->UpdateListByExchange();
}

bool UUI_ExchangeAndSplit::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Exchange_Scroll = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_19"));
	this->M_Exchange_Uniform = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_56"));

	this->M_SplitMaterial_Scroll = this->GetWidgetComponent<UScrollBox>(this, TEXT("Scroll_Materials"));
	this->M_SplitMaterial_Uniform = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_54"));

	this->M_Card_Scroll = this->GetWidgetComponent<UScrollBox>(this, TEXT("Scroll_Cards"));
	this->M_Card_Uniform = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_91"));

	//初始化资源图片头像组件
	this->M_ResourceHead = this->GetWidgetComponent<UImage>(this, TEXT("ResourceHead"));
	this->M_Split_Ticket_LeftImg = this->GetWidgetComponent<UImage>(this, TEXT("Out_Left"));
	this->M_Split_Ticket_RightImg = this->GetWidgetComponent<UImage>(this, TEXT("Out_Left_1"));

	//角色的货币资源文本
	this->M_ResourceD = this->GetWidgetComponent<UTextBlock>(this, TEXT("SourceCount_1"));
	this->M_ResourceTicket = this->GetWidgetComponent<UTextBlock>(this, TEXT("SourceCount"));
	this->M_Split_Generate_ResourceTicketCount_Left_Text = this->GetWidgetComponent<UTextBlock>(this, TEXT("OutText_Left_1"));
	this->M_Split_Generate_ResourceTicketCount_Right_Text = this->GetWidgetComponent<UTextBlock>(this, TEXT("OutText_Right_1"));
	this->M_Split_Generate_ResourceTicket_Text = this->GetWidgetComponent<UTextBlock>(this, TEXT("OutText_Right"));

	//卡片分解
	this->M_NeedSplitCard_Butt = this->GetWidgetComponent<UButton>(this, TEXT("SplitItem"));
	this->M_SplitCard_Butt = this->GetWidgetComponent<UButton>(this, TEXT("SplitBut"));

	return true;
}

void UUI_ExchangeAndSplit::InitPlayerMaterialData(bool _IsCard)
{
	if (_IsCard)
	{
		for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card)
			Data.M_ItemID = -1;
	}
	else {
		for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
			Data.M_ItemID = -1;
	}
}

void UUI_ExchangeAndSplit::InitPlayerMaterialsList()
{
	if (this->M_SplitMaterial_Loadding)
	{
		this->InitPlayerMaterialData(false);

		TArray<FMaterialBase*> PlayerDatas;

		//等待排除的结果
		bool IgnoreResult = false;
		for (const auto& Keyword : this->M_MaterialsKeywords)
		{
			for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
			{
				if (Data.M_ItemID != 0)
					if (Data.ItemName.ToString().Contains(Keyword))
					{
						//查询排除
						for (const auto& Keyword_Ignore : this->M_MaterialsKeywords_Ignore)
						{
							if (Data.ItemName.ToString().Contains(Keyword_Ignore))
							{
								IgnoreResult = true;
								break;
							}
						}

						//不被排除则不用
						if (!IgnoreResult)
						{
							//检测是否可以被分解
							if (this->GetCurrentSplitItemCountByName(Data.ItemName.ToString()) != 0)
							{
								Data.M_ItemID = 0;
								PlayerDatas.Emplace(&Data);
							}
						}
						else {
							//恢复排除结果
							IgnoreResult = false;
						}
					}
			}
		}


		this->M_SplitMaterial_Loadding->UpdateDatatable(PlayerDatas);
		this->M_SplitMaterial_Loadding->SetLoadItemMaxCount(PlayerDatas.Num());
		this->M_SplitMaterial_Loadding->SetCurrentPage(0);

		if (PlayerDatas.Num() <= 18)
		{
			this->M_SplitMaterial_Loadding->SetItemMargin(FVector2D(-3.f, 5.f));
		}
		else {
			this->M_SplitMaterial_Loadding->SetItemMargin(FVector2D(2.f, 5.f));
		}



		this->M_SplitMaterial_Loadding->SetResetScrollBoxOffset();
		this->M_SplitMaterial_Loadding->ContinueRun();
	}
}

void UUI_ExchangeAndSplit::InitPlayerCardsList()
{
	if (this->M_Card_Loadding)
	{
		this->InitPlayerMaterialData(true);

		TArray<FItemCard*> CardDatas;

		//等待排除的结果
		bool IgnoreResult = false;
		for (const auto& Keyword : this->M_CardsKeywords)
		{
			for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card)
			{
				if (Data.M_ItemID != 0)
					if (Data.ItemName.ToString().Contains(Keyword))
					{
						//查询排除
						for (const auto& Keyword_Ignore : this->M_CardsKeywords_Ignore)
						{
							if (Data.ItemName.ToString().Contains(Keyword_Ignore))
							{
								IgnoreResult = true;
								break;
							}
						}

						//不被排除则不用
						if (!IgnoreResult)
						{
							//检测是否可以被分解
							if (this->GetCurrentSplitItemCountByName(Data.ItemName.ToString()) != 0)
							{
								Data.M_ItemID = 0;
								CardDatas.Emplace(&Data);
							}
						}
						else {
							//恢复排除结果
							IgnoreResult = false;
						}
					}
			}
		}

		this->M_Card_Loadding->UpdateDatatable(CardDatas);
		this->M_Card_Loadding->SetLoadItemMaxCount(CardDatas.Num());

		if (CardDatas.Num() <= 18)
		{
			this->M_Card_Loadding->SetLoadRowAndCol(3, 5);
			this->M_Card_Loadding->SetItemMargin(FVector2D(-2.f, -28.f));
		}
		else {
			this->M_Card_Loadding->SetLoadRowAndCol(3, 6);
			this->M_Card_Loadding->SetItemMargin(FVector2D(-18.f, -28.f));
		}

		this->M_Card_Loadding->SetCurrentPage(0);
		this->M_Card_Loadding->SetResetScrollBoxOffset();
		this->M_Card_Loadding->ContinueRun();
	}
}

void UUI_ExchangeAndSplit::InitPlayerData(FString _Title)
{
	this->M_Title = _Title;

	FPlayerExchangeAndSplit* Data = UFVMGameInstance::GetPlayerStructManager_Static()
		->M_FPlayerExchangeAndSplitInfor.Find(_Title);

	//如果数查询成功才会初始化，否则不会发生任何事情
	if (Data)
	{
		this->OnInitPlayerData(*Data);
	}
}

void UUI_ExchangeAndSplit::SaveCurInfor(FPlayerExchangeAndSplit Data)
{
	UFVMGameInstance::GetPlayerStructManager_Static()
		->M_FPlayerExchangeAndSplitInfor.Emplace(this->M_Title, Data);

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("兑换与分解{切换运势}")));
}

void UUI_ExchangeAndSplit::SplitCurSelectItem()
{
	//【0】如果当前分解的数量为0 表示分解的道具不合适
	if (this->M_CurrentSplitCount == 0)
	{
		UWidgetBase::CreateTipWidget(TEXT("当前道具不适合分解"));
		return;
	}

	//【1】判断当前分解的道具是否有效
	if (!this->M_CurrentSelect->GetValue())
	{
		UWidgetBase::CreateTipWidget(TEXT("当前道具-无效，请重启游戏"));
		return;
	}

	//分解道具的数量缓存
	int32 SplitCountCache = this->M_CurrentSplitCount + this->M_CurrentOutSplitCount;

	//【2】判断当前道具的类型【材料】【防御卡】
	if (this->M_CurrentSelectType == EItemType::E_Card)
	{
		FItemCard* LocalCard = (FItemCard*)this->M_CurrentSelect->GetValue();

		//标记移除属性
		LocalCard->bWaitRemove = true;

		//清理数据
		UGameSystemFunction::ClearWaitingItems(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card);

		//增加对应资源的数量
		UGameSystemFunction::SendMaterialToPlayerBag(this->NeedResourceName, SplitCountCache, true);

		//初始化选择的道具
		this->SetCurrentSlipItem(EItemType::E_Equipment, -1, nullptr, nullptr);

	}


	//【3】当前道具是材料
	if (this->M_CurrentSelectType == EItemType::E_MaterialItem)
	{
		FMaterialBase* LocalMaterialBase = (FMaterialBase*)this->M_CurrentSelect->GetValue();

		LocalMaterialBase->M_Count--;

		if (LocalMaterialBase->M_Count <= 0)
		{
			LocalMaterialBase->bWaitRemove = true;

			//清理数据
			UGameSystemFunction::ClearWaitingItems(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material);

			//初始化选择的道具
			this->SetCurrentSlipItem(EItemType::E_Equipment, -1, nullptr, nullptr);
		}
		else {
			this->SetCurrentSlipItem(this->M_CurrentSelectType, this->M_CurrentSelectIndex, nullptr, nullptr);
		}

		//增加对应资源的数量
		UGameSystemFunction::SendMaterialToPlayerBag(this->NeedResourceName, SplitCountCache, true);
	}

	//更新货币资源
	this->UpdateResourceText();

	UWidgetBase::CreateTipWidget(FString(TEXT("获得【") + this->NeedResourceName + TEXT("】 x") + FString::FromInt(SplitCountCache)));
}


void UUI_ExchangeAndSplit::GameInit()
{
	if (!IsValid(this->M_Exchange_Loadding))
	{
		this->M_Exchange_Loadding = NewObject<UItemLoadManager>(this, TEXT("Loadder_Exchange_Loadding"));
		this->M_SplitMaterial_Loadding = NewObject<UItemLoadManager>(this, TEXT("Loadder_SplitMaterial_Loadding"));
		this->M_Card_Loadding = NewObject<UItemLoadManager>(this, TEXT("Loadder_Card_Loadding"));

		this->M_Exchange_Loadding->InitWidget_First(this->M_Exchange_Uniform, this->M_Exchange_Scroll);
		this->M_Exchange_Loadding->InitRange_Second(4, 4, 100, 400.f, 170.f);
		this->M_Exchange_Loadding->SetItemMargin(FVector2D(40.f, 0.f));
		//this->M_Exchange_Loadding->SetMainScrollTransfrom(FVector2D(0.f, 50.f));
		this->M_Exchange_Loadding->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_ExchangeInit"));
		this->M_Exchange_Loadding->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_ExchangeUpdate"));

		this->M_SplitMaterial_Loadding->InitWidget_First(this->M_SplitMaterial_Uniform, this->M_SplitMaterial_Scroll);
		this->M_SplitMaterial_Loadding->InitRange_Second(3, 6, 100, 100, 100);
		this->M_SplitMaterial_Loadding->SetMainScrollTransfrom(FVector2D(0.f, 5.f));
		this->M_SplitMaterial_Loadding->SetItemMargin(FVector2D(2.f, 5.f));
		this->M_SplitMaterial_Loadding->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_SplitMaterialInit"));
		this->M_SplitMaterial_Loadding->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_SplitMaterialUpdate"));

		this->M_Card_Loadding->InitWidget_First(this->M_Card_Uniform, this->M_Card_Scroll);
		this->M_Card_Loadding->InitRange_Second(3, 6, 100, 120, 152);
		this->M_Card_Loadding->SetMainScrollTransfrom(FVector2D(0.f, 7.f));
		this->M_Card_Loadding->SetItemMargin(FVector2D(-18.f, -28.f));
		this->M_Card_Loadding->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_CardInit"));
		this->M_Card_Loadding->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_CardUpdate"));
		//初始化状态
		this->SetSplitEnable(false);
	}
}

void UUI_ExchangeAndSplit::GameClose()
{
	this->InitPlayerMaterialData(true);
	this->InitPlayerMaterialData(false);

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关闭{兑换与分解}界面")));
}
