// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerBagCardDetail.h"
#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
#include "Game/UI/Synthesis/SynModel_CardChange.h"
#include "Game/UI/Synthesis/SynModel_GoldCardUp.h"
#include "Game/UI/UI_ExchangeAndSplit.h"
#include "Game/UI/UI_GamePrepare.h"
#include "Game/UI/UI_PlayerBag.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Math/Color.h>

bool UUI_PlayerBagCardGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_CardButton = this->GetWidgetComponent<UButton>(this, "Card");
	this->M_CardGradeImage = this->GetWidgetComponent<UImage>(this, "Grade");

	return true;
}

FOnButtonClickedEvent& UUI_PlayerBagCardGrid::GetButtonClickEvent()
{
	return this->M_CardButton->OnClicked;
}

void UUI_PlayerBagCardGrid::SetFItemCardData(FItemCard* const _FItemCardData)
{
	this->M_FItemCard = _FItemCardData;
	this->CopyData = *_FItemCardData;
}

void UUI_PlayerBagCardGrid::SetUI_PlayerSynthesis(UUI_PlayerSynthesis* const _PlayerSynthesis)
{
	this->M_UI_PlayerSynthesis = _PlayerSynthesis;
}

void UUI_PlayerBagCardGrid::SetUI_Other(UWidget* const _Widget)
{
	this->M_UI_Other = _Widget;
}

void UUI_PlayerBagCardGrid::SetCardIndex(const int32& _Value)
{
	this->M_CardIndex = _Value;
}

void UUI_PlayerBagCardGrid::SetUIIndex(const int32& _Value)
{
	this->M_UI_Index = _Value;
}

int32 UUI_PlayerBagCardGrid::GetCardIndex()
{
	return this->M_CardIndex;
}

int32 UUI_PlayerBagCardGrid::GetUIIndex()
{
	return this->M_UI_Index;
}

FItemCard* const UUI_PlayerBagCardGrid::GetFItemCardData()
{
	return this->M_FItemCard;
}

FString UUI_PlayerBagCardGrid::GetItemName()
{
	if (this->GetFItemCardData())
	{
		return this->GetFItemCardData()->ItemName;
	}

	return FString();
}

UButton* UUI_PlayerBagCardGrid::GetButton()
{
	return this->M_CardButton;
}

void UUI_PlayerBagCardGrid::ShowCardDetails()
{
	if (!this->GetFItemCardData())
		return;

	//细节界面
	UUI_PlayerBagCardDetail* M_UI_PlayerBagCardDetail = CreateWidget<UUI_PlayerBagCardDetail>(this, LoadClass<UUI_PlayerBagCardDetail>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardDetail.BPUI_PlayerBagCardDetail_C'")));
	M_UI_PlayerBagCardDetail->M_PlayerUIBag = this->M_PlayerUIBag;
	M_UI_PlayerBagCardDetail->M_FItemCard = this->M_FItemCard;
	M_UI_PlayerBagCardDetail->ItemName = M_FItemCard->ItemName;
	M_UI_PlayerBagCardDetail->ItemDescrible = M_FItemCard->ItemDescrible;
	M_UI_PlayerBagCardDetail->ItemDescrible_2 = M_FItemCard->ItemDescrible_2;
	M_UI_PlayerBagCardDetail->M_CardHP = FString(TEXT("生命值:")) + FString::FormatAsNumber(M_FItemCard->M_CardHP);
	M_UI_PlayerBagCardDetail->M_CardPrice = FString(TEXT("火苗:")) + FString::FormatAsNumber(M_FItemCard->M_CardPrice);
	M_UI_PlayerBagCardDetail->M_IsChange = M_FItemCard->M_FCardChangeJobs.M_IsChange ? FString(TEXT("转职:可以转职")) : FString(TEXT("转职:不可转职"));
	M_UI_PlayerBagCardDetail->M_ChangeMaterialsName = FString(TEXT("转职对象:")) + M_FItemCard->M_FCardChangeJobs.M_ChangeMaterialsName;
	M_UI_PlayerBagCardDetail->M_CardColdDown = FString(TEXT("冷却时间:")) + FString::SanitizeFloat(M_FItemCard->M_CardColdDown, 2);
	M_UI_PlayerBagCardDetail->AddToViewport();
}

void UUI_PlayerBagCardGrid::SelectCurrentCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		if (UUI_GamePrepare::M_GamePrepareStatic->GetSelectCardNums() > 17)
		{
			UWidgetBase::CreateTipWidget(TEXT("最多选择18张防御卡"));
			return;
		}

		UUI_GamePrepare::M_GamePrepareStatic->SelectCard(this->M_FItemCard->ItemName, *M_FItemCard);
		UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.Emplace(*this->M_FItemCard);
	}
}

void UUI_PlayerBagCardGrid::RemoveCurrentSelectCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		this->RemoveFromParent();

		//调用代理
		this->OnRemoveCurrentSelectCard();

		for (auto FCardDataPP = UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.CreateIterator(); FCardDataPP; FCardDataPP++)
		{
			if (this->M_FItemCard->ItemName.Equals((*FCardDataPP).ItemName))
			{
				FCardDataPP.RemoveCurrent();
				break;
			}
		}

		TArray<FString> Names = { M_FItemCard->ItemName };
		UUI_GamePrepare::M_GamePrepareStatic->SetCardEnable(Names, true);
		UUI_GamePrepare::M_GamePrepareStatic->CancelCardNum();
	}
}

void UUI_PlayerBagCardGrid::AddUpGradeFunc()
{
	if (IsValid(this->M_UI_PlayerSynthesis->GetCardUpgradeFunction()))
	{
		FSynModelCardUpgradeData Data;
		Data.PlayerBagIndex = M_CardIndex;
		Data.CardGrid = this;
		Data.CardData = this->CopyData;
		this->M_UI_PlayerSynthesis->GetCardUpgradeFunction()->AddCardDataMain(Data);
	}
}

void UUI_PlayerBagCardGrid::AddSynthesisChangeJobs()
{
	if (!this->M_UI_PlayerSynthesis)
	{
		return;
	}

	if (!IsValid(this->M_UI_PlayerSynthesis->GetCardChangeFunction()))
	{
		return;
	}

	//数据
	FMainCardChangeType& CardData = this->M_UI_PlayerSynthesis->GetCardChangeFunction()->GetFMainCardChangeData();

	//设置UI
	if (IsValid(CardData.Grid))
	{
		CardData.Grid->GetButton()->SetIsEnabled(true);
	}
	CardData.Grid = this;
	CardData.UIGrideIndex = this->GetUIIndex();
	this->GetButton()->SetIsEnabled(false);

	CardData.ChangeCardName = this->CopyData.ItemName;
	CardData.ChangeCardHeadPath = this->CopyData.ItemTexturePath.ToString();
	CardData.TargetChangeCardName = this->CopyData.M_FCardChangeJobs.M_ChangeMaterialsName;
	CardData.TargetType = this->CopyData.M_FCardChangeJobs.M_ChangeType;
	CardData.CardGrade = this->CopyData.M_FCardChangeJobs.M_ChangeGrade;
	//赋值索引
	const TMap<int32, FItemCard>& Cards = UFVMGameInstance::GetPlayerStructManager_Static()->FindCardByName(this->CopyData.ItemName);
	for (const auto& CurData : Cards)
	{
		if (CurData.Value.M_CardGrade == this->CopyData.M_CardGrade)
		{
			CardData.PlayerBagIndex = CurData.Key;
			break;
		}
	}

	//拿到转职关键词
	FString ChangeKeyName = this->CopyData.M_FCardChangeJobs.M_ChangeKeyName;
	//拿到转职等级
	FString ChangeGradeKeyName = this->CopyData.M_FCardChangeJobs.M_ChangeGradeKeyName;
	//转职顺序字母
	TArray<FString> GradeNum = { TEXT("A"),TEXT("B"),TEXT("C") };

	//设置材料
	auto AddMaterials = [&](const int32& i) {
		FCardChangeMaterialType& CurData = CardData.FCardChangeMaterials[i];
		FCardChangeJobMaterial _Material;
		if (!UMaterialBaseStruct::GetMaterialSourceData<FCardChangeJobMaterial>(
			FString(ChangeKeyName + TEXT("[") + ChangeGradeKeyName + TEXT("转") + GradeNum[i] + TEXT("]")),
			_Material,
			EMaterialType::E_CardChangeJobMaterial
			))
		{
			CurData.Visible = ESlateVisibility::Collapsed;
		}
		else {
			//查询成功
			CurData.Visible = ESlateVisibility::SelfHitTestInvisible;
			CurData.ChangeMaterialHeadPath = _Material.ItemTexturePath.ToString();
			CurData.ChangeMaterialName = FString(ChangeKeyName + TEXT("[") + ChangeGradeKeyName + TEXT("转") + GradeNum[i] + TEXT("]"));
			CurData.PlayerBagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(
				CurData.ChangeMaterialName
			);
		}
	};

	//更具等级决定材料数量
	if (this->CopyData.M_FCardChangeJobs.M_ChangeGrade > 9)
	{
		//三种
		for (int32 i = 0; i < 3; i++)
		{
			AddMaterials(i);
		}
	}
	else {
		//两种
		CardData.FCardChangeMaterials[2].Visible = ESlateVisibility::Collapsed;
		for (int32 i = 0; i < 2; i++)
		{
			AddMaterials(i);
		}
	}

	//更新显示
	CardData.ShowStyle(this->M_UI_PlayerSynthesis->GetCardChangeFunction());
}

void UUI_PlayerBagCardGrid::AddSynthesisGoldCardUp()
{
	if (!IsValid(this->M_UI_PlayerSynthesis))
	{
		return;
	}

	//获取金卡进化界面
	USynModel_GoldCardUp* Class = this->M_UI_PlayerSynthesis->GetGoldCardUpFunction();
	if (!IsValid(Class))
	{
		return;
	}

	FGoldCardUpCardType Data;
	Data.Grid = this;
	Data.UIGridIndex = this->GetUIIndex();
	Data.CardName = this->CopyData.ItemName;
	Data.CardHeadPath = this->CopyData.ItemTexturePath.ToString();
	Data.CardGrade = this->CopyData.M_CardGrade;

	const TMap<int32, FItemCard>& Cards = UFVMGameInstance::GetPlayerStructManager_Static()->FindCardByName(this->CopyData.ItemName);
	for (const auto& Card : Cards)
	{
		if (Card.Value.M_CardGrade == this->CopyData.M_CardGrade)
		{
			Data.PlayerBagIndex  = Card.Key;
			break;
		}
	}

	if (!Class->AddCardData(Data))
	{
		return;
	}
	this->GetButton()->SetIsEnabled(false);
	Class->UpdateListData();

	/*
		查询卡槽是否有空位

		int32 LResult = 0;
	//判断卡片名字是否已经填写
	for (auto& State : this->M_UI_PlayerSynthesis->M_GoldCard_Name_State)
	{
		if (!State.Equals(""))
		{
			LResult++;
		}
	}

	if (LResult >= 2)
	{
		UWidgetBase::CreateTipWidget(TEXT("无法选择,卡槽已满"));
		return;
	}

	//隐藏显示所有的空卡槽位置
	for (auto State = this->M_UI_PlayerSynthesis->M_GoldCard_Name_State.CreateConstIterator(); State; ++State)
	{
		if ((*State).Equals(""))
		{
			this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//将当前卡片设置为被选中状态
	this->GetButton()->SetIsEnabled(false);
	//隐藏其他的道具(因为材料不齐全所以需要隐藏)
	this->M_UI_PlayerSynthesis->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);
	//关闭进化按钮(因为材料不齐全所以需要关闭进化按钮)
	this->M_UI_PlayerSynthesis->M_GoldCardUp_Butt->SetIsEnabled(false);


	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前金卡进化防御卡卡槽名称："));
	}

	//设置卡片卡槽名字
	for (auto State = this->M_UI_PlayerSynthesis->M_GoldCard_Name_State.CreateConstIterator(); State; ++State)
	{
		//判断当前卡槽是否是空的
		if (this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()].Equals(""))
		{
			//添加名字
			this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()] = this->GetFItemCardData()->ItemName;
			//设置卡槽
			UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()], this->GetFItemCardData()->ItemTexturePath.ToString());
			//开启卡槽
			this->M_UI_PlayerSynthesis->M_GoldCard_Select_Button[State.GetIndex()]->SetVisibility(ESlateVisibility::Visible);
			//添加被消耗的卡片
			this->M_UI_PlayerSynthesis->M_NeedCards[State.GetIndex()] = this->GetFItemCardData()->ItemName;
			//添加卡片等级
			this->M_UI_PlayerSynthesis->M_NeedGradeCards[State.GetIndex()] = this->GetFItemCardData()->M_CardGrade;
			//添加卡片对应的UI索引
			this->M_UI_PlayerSynthesis->M_NeedCard_UI_Index[State.GetIndex()] = this->GetUIIndex();

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, this->M_UI_PlayerSynthesis->M_GoldCard_Name_State[State.GetIndex()]);
			}

			break;
		}
	}

	//卡片名称合并查询->进化凭证
	TArray<FMaterial_GoldCardUp_Data> Datas;
	UGameSystemFunction::GetDataTableRows(
		TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/GoldCardUpData.GoldCardUpData'"),
		Datas
	);

	//最后从背包搜寻的金卡转职凭证
	FString LResultItemName = TEXT("");
	//对应的卡片名称
	FString LResultCardName = TEXT("");

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前金卡进化查询进化凭证："));
	}

	for (auto Data = Datas.CreateConstIterator(); Data; ++Data)
	{
		bool TLResult = true;

		FString StateResult = TEXT("");

		//搜寻凭证
		for (auto& State : this->M_UI_PlayerSynthesis->M_GoldCard_Name_State)
		{
			StateResult += State;
		}

		if (UFVMGameInstance::GetDebug())
		{
			FString Content;
			Content += TEXT("当前凭证名称:");
			Content += (*Data).M_GoldCardItemName;
			Content += TEXT(" 当前合成名字结果:");
			Content += StateResult;
			Content += TEXT(" 目标合成名称结果:");
			Content += (*Data).M_AddCardName;
			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}

		//查询匹配最终结果
		if ((*Data).M_AddCardName.Equals(StateResult))
		{
			LResultItemName = (*Data).M_GoldCardItemName;
			LResultCardName = (*Data).M_GoldCardName;

			break;
		}
	}

	//没有任何结果，返回
	if (LResultItemName.Equals(""))
	{
		return;
	}

	//从背包搜索->玩家是否含有进化凭证
	if (FMaterialBase* LMaterialData = UFVMGameInstance::GetPlayerStructManager_Static()->GetMaterialByName(LResultItemName))
	{
		//如果从背包种查询失败，则直接返回
		if (!LMaterialData)
		{
			return;
		}

		//查询卡片【对应进化凭证的卡片】
		FItemCard CardData;
		//如果查询成功开启进化按钮
		if (UCardBaseStruct::SearchCardFromDataTable(LResultCardName, CardData))
		{
			UWidgetBase::SetImageBrush(this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead, CardData.ItemTexturePath.ToString());
			this->M_UI_PlayerSynthesis->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Visible);

			UWidgetBase::SetImageBrush(this->M_UI_PlayerSynthesis->M_GoldCardUpHead, LMaterialData->ItemTexturePath.ToString());

			this->M_UI_PlayerSynthesis->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Visible);

			//开启进化按钮
			this->M_UI_PlayerSynthesis->M_GoldCardUp_Butt->SetIsEnabled(true);

			//赋予对应的数值
			this->M_UI_PlayerSynthesis->M_GoldCardUp_ItemName = LResultItemName;
			this->M_UI_PlayerSynthesis->M_GoldCardUp_CardName = LResultCardName;
		}
	}
	*/

}

void UUI_PlayerBagCardGrid::AddExchangeSplit_SplitSlot()
{
	if (IsValid(this->M_UI_Other))
	{
		if (UUI_ExchangeAndSplit* const LExUI = Cast<UUI_ExchangeAndSplit>(this->M_UI_Other))
		{
			if (IsValid(LExUI->GetCurrentSelectButt()))
			{
				LExUI->GetCurrentSelectButt()->SetIsEnabled(true);
			}

			UItemDataTable* Data = NewObject<UItemDataTable>(this);
			Data->SetValue((FTableRowBase*)(this->M_FItemCard));
			LExUI->SetCurrentSlipItem(EItemType::E_Card, this->GetUIIndex(), Data, this->GetButton());
			this->GetButton()->SetIsEnabled(false);
		}
	}
}

void UUI_PlayerBagCardGrid::UpdateButtonTexture(const FString& _Price)
{
	//等级资源
	if (this->M_FItemCard->M_CardGrade != 0)
	{
		FSlateBrush GradeBrush;
		GradeBrush.SetResourceObject(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(FString("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FromInt(this->M_FItemCard->M_CardGrade) + (".") + FString::FormatAsNumber(this->M_FItemCard->M_CardGrade) + ("'"))));
		GradeBrush.DrawAs = ESlateBrushDrawType::Image;
		this->M_CardGradeImage->SetBrush(GradeBrush);
	}

	//资源
	FSlateBrush Brush;
	Brush.SetResourceObject(this->LoadTexture2D(this->M_CardTexturePath));
	//设置按钮不同事件时图片颜色深浅
	FButtonStyle Style;
	Style.SetNormal(Brush);

	Brush.TintColor = FLinearColor(FVector4(.7f, .7f, .7f, 1.f));
	Style.SetHovered(Brush);

	Brush.TintColor = FLinearColor(FVector4(.5f, .5f, .5f, 1.f));
	Style.SetPressed(Brush);

	this->M_CardButton->SetStyle(Style);
	this->M_CardPriceText = M_FItemCard->M_CardPriceAutoUp ? FText::FromString(_Price + "+") : FText::FromString(_Price);

	//如果卡片等级为0则不显示
	if (this->M_FItemCard->M_CardGrade != 0)
	{
		this->M_CardGradeImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->M_CardGradeImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

UTexture2D* UUI_PlayerBagCardGrid::GetTexture()
{
	return Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(this->M_CardTexturePath)));
}

void UUI_PlayerBagCardGrid::PlayBeginAnim()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num())
	{
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
	}
}
