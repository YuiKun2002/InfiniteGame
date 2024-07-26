// Fill out your copyright notice in the Description page of Prthis->M_MaterialBaseData->oject Settings.


#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "GameSystem/TaskSubsystem.h"

//卡片制作
#include "Game/UI/Synthesis/SynModel_MakeCard.h"
//卡片强化
#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
//武器宝石功能
#include "Game/UI/Synthesis/SynModel_WepaonGems.h"
//宝石强化功能
#include "Game/UI/Synthesis/SynModel_GemsUpGrade.h"
//宝石分割
#include "Game/UI/Synthesis/SynModel_GemSplit.h"
//兑换与分解
#include "Game/UI/UI_ExchangeAndSplit.h"


#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Tools/ItemDataTable.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"

#include "Data/MaterialDataStruct.h"

#include "Game/UI/Tips/UI_ItemDesTip.h"

#include "Game/UI/UI_PlayerSynthesis.h"
#include "Game/UI/UI_PlayerBag.h"


void UUI_PlayerBagMaterialGrid::SetIndex(int32 _Index)
{
	this->M_Index = _Index;
}

int32 UUI_PlayerBagMaterialGrid::GetIndex()
{
	return this->M_Index;
}

bool UUI_PlayerBagMaterialGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Button = this->GetWidgetComponent<UButton>(this, "Item_button");
	this->M_MaterialGrade = this->GetWidgetComponent<UImage>(this, "ItemGrade");
	this->M_CountText = this->GetWidgetComponent<UTextBlock>(this, "Count");

	return true;
}

void UUI_PlayerBagMaterialGrid::SetUI_PlayerSynthesis(UUI_PlayerSynthesis* _UUI_PlayerSynthesis)
{
	this->M_UUI_PlayerSynthesis = _UUI_PlayerSynthesis;
}

void UUI_PlayerBagMaterialGrid::SetUI_PlayerBag(UUI_PlayerBag* _UUI_PlayerBag)
{
	this->M_UUI_PlayerBag = _UUI_PlayerBag;
}

void UUI_PlayerBagMaterialGrid::SetUI_Other(UWidget* _Other_UI)
{
	this->M_Other_UI = _Other_UI;
}

void UUI_PlayerBagMaterialGrid::SetMaterialData(FMaterialBase* _Data)
{
	this->M_MaterialBaseData = _Data;
	this->MaterialBaseCopyData = *_Data;
	this->M_EquipmentBaseData = nullptr;
}

void UUI_PlayerBagMaterialGrid::SetEquipmentBase(FEquipmentBase* _Data)
{
	this->M_MaterialBaseData = nullptr;
	this->M_EquipmentBaseData = _Data;
	this->EquipmentBaseCopyData = *_Data;
}

void UUI_PlayerBagMaterialGrid::SetItemGrade(const FString& _GradePath)
{
	if (_GradePath.Equals(""))
		this->M_MaterialGrade->SetVisibility(ESlateVisibility::Collapsed);
	else
	{
		this->M_MaterialGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UWidgetBase::SetImageBrush(this->M_MaterialGrade, _GradePath);
	}
}

FMaterialBase* const UUI_PlayerBagMaterialGrid::GetMaterialData()
{
	return this->M_MaterialBaseData;
}

void UUI_PlayerBagMaterialGrid::PlayAnimation_1()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

UButton* UUI_PlayerBagMaterialGrid::GetButton()
{
	return this->M_Button;
}

FString UUI_PlayerBagMaterialGrid::GetItemName()
{
	if (this->GetMaterialData())
	{
		return this->GetMaterialData()->ItemName;
	}
	else if (this->M_EquipmentBaseData)
	{
		return this->M_EquipmentBaseData->ItemName;
	}

	return FString();
}

void UUI_PlayerBagMaterialGrid::UpdateMaterialsShowCount(FString  _CountText)
{
	this->M_CountText->SetText(FText::FromString(_CountText));
}

void UUI_PlayerBagMaterialGrid::ShowInformation()
{
	//创建物品提示按钮
	UUI_ItemDesTip* TipComp = CreateWidget<UUI_ItemDesTip>(this, LoadClass<UUI_ItemDesTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'")));
	TipComp->SetContentTitleText(this->M_MaterialBaseData->ItemName);
	TipComp->SetContentText(this->M_MaterialBaseData->ItemDescrible);
	TipComp->SetOkButtonTitleText(TEXT("了解"));
	TipComp->SetCancelButtonTitleText(TEXT("知道了"));
	TipComp->SetItemHeadPath(this->M_MaterialBaseData->ItemTexturePath.ToString());
	TipComp->AddToViewport(1);
}

FMaterialBase* GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName)
{
	for (FMaterialBase& Items : _Arrays)
	{
		if (Items.ItemName.Equals(_ItemName))
			return &Items;
	}
	return nullptr;
}

void UUI_PlayerBagMaterialGrid::AddSynthesisBlueprint()
{
	//启用之前的禁止按钮UI
	if (IsValid(this->M_UUI_PlayerSynthesis->GetMakeCardFunction()))
	{
		if (IsValid(this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetBlueprintData().Grid))
		{
			this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetBlueprintData().Grid->GetButton()->SetIsEnabled(true);
		}
	}

	//添加配方
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->
		GetBlueprintData().AddNewBlueprint(
			this->MaterialBaseCopyData.ItemName,
			this,
			this->GetIndex(),
			this->M_UUI_PlayerSynthesis->GetMakeCardFunction()
		);

	//禁用当前按钮
	this->GetButton()->SetIsEnabled(false);
	//检测制作
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->CheckMakeCard();
}

void UUI_PlayerBagMaterialGrid::AddSynthesisSpicesSlot()
{
	//判断当前香料有没有达到5个
	if (this->MaterialBaseCopyData.M_Count < 5)
	{
		UWidgetBase::CreateTipWidget(TEXT("至少需要5个"));
		return;
	}

	//启用之前的禁止按钮UI
	if (IsValid(this->M_UUI_PlayerSynthesis->GetMakeCardFunction()))
	{
		if (IsValid(this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesData().Grid))
		{
			this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesData().Grid->GetButton()->SetIsEnabled(true);
		}
	}

	FSpicesMaterial _Spices;
	if (!UMaterialBaseStruct::GetMaterialSourceData<FSpicesMaterial>(
		this->MaterialBaseCopyData.ItemName,
		_Spices,
		EMaterialType::E_Spices
		))
	{
		UWidgetBase::CreateTipWidget(FString(TEXT("[") + this->MaterialBaseCopyData.ItemName + TEXT("]查询失败!")));
		return;
	}

	//获取香料配置
	FMakeCardSpicesData& CurSpicesData = this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesData();

	//选择香料
	FMakeCardSpicesData NewData;
	NewData.Grid = this;
	NewData.MakeCardGrade = _Spices.M_MakeCardGrade;
	NewData.PlayerBagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->MaterialBaseCopyData.ItemName);
	NewData.SpicesName = this->MaterialBaseCopyData.ItemName;
	NewData.SpicesData = this->MaterialBaseCopyData;
	CurSpicesData.SetSelectSpices(NewData);

	if (NewData.PlayerBagIndex == -1)
	{
		UWidgetBase::CreateTipWidget(FString(TEXT("[") + this->MaterialBaseCopyData.ItemName + TEXT("]选择失败!")));
		return;
	}

	//禁用当前按钮
	this->GetButton()->SetIsEnabled(false);

	//检测制作
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->CheckMakeCard();

	//设置（合成屋香料槽）按钮样式
	UWidgetBase::SetButtonStyle(
		this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesButt(),
		this->MaterialBaseCopyData.ItemTexturePath.ToString()
	);

	//绑定取消香料的委托
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesButt()->OnClicked.Clear();
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesButt()->OnClicked.AddDynamic(
		this->M_UUI_PlayerSynthesis->GetMakeCardFunction(),
		&USynModel_MakeCard::CancelSelectSpices
	);
	this->M_UUI_PlayerSynthesis->GetMakeCardFunction()->GetSpicesButt()->OnClicked.AddDynamic(
		this->M_UUI_PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
}

void UUI_PlayerBagMaterialGrid::AddUpGradeCardCloverSlot()
{
	if (!this->M_UUI_PlayerSynthesis)
	{
		return;
	}

	//启用之前的禁止按钮UI
	if (IsValid(this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()))
	{
		if (IsValid(this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()->GetSelectCloverUI()))
		{
			this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()->GetSelectCloverUI()->GetButton()->SetIsEnabled(true);
		}
	}

	FCloverMaterial _Clover;
	if (!UMaterialBaseStruct::GetMaterialSourceData<FCloverMaterial>(
		this->M_MaterialBaseData->ItemName,
		_Clover,
		EMaterialType::E_Clover
		))
	{
		UWidgetBase::CreateTipWidget(FString(TEXT("[") + this->M_MaterialBaseData->ItemName + TEXT("]查询失败!")));
		return;
	}

	//禁用当前选择的UI
	this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()->SetSelectCloverUI(this);

	this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()->SetSelectClover(
		this->M_MaterialBaseData->ItemName,
		UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->M_MaterialBaseData->ItemName),
		_Clover.M_UpGrateRate
	);

	this->GetButton()->SetIsEnabled(false);

	//设置按钮样式
	UWidgetBase::SetButtonStyle(this->M_UUI_PlayerSynthesis->GetCloverButton(),
		this->M_MaterialBaseData->ItemTexturePath.ToString()
	);

	//更新概率
	this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction()->GetUpGradeRate();

	//重新绑定四叶草的点击功能
	this->M_UUI_PlayerSynthesis->GetCloverButton()->OnClicked.Clear();
	this->M_UUI_PlayerSynthesis->GetCloverButton()->OnClicked.AddDynamic(
		this->M_UUI_PlayerSynthesis->GetCardUpgradeFunction(), &USynModel_CardUpgrade::CancelSelectClover);
	this->M_UUI_PlayerSynthesis->GetCloverButton()->OnClicked.AddDynamic(
		this->M_UUI_PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
}

void UUI_PlayerBagMaterialGrid::AddWepaonToSlot()
{
	if (USynModel_WepaonGems* LWepaonGems = this->M_UUI_PlayerSynthesis->GetWepaonGemsFunction())
	{
		LWepaonGems->WidgetReset();
		LWepaonGems->WidgetDataReload();

		TObjectPtr<UItemDataTable> LData = NewObject<UItemDataTable>(this);
		LData->SetValue((FTableRowBase*)(this->M_EquipmentBaseData));

		LWepaonGems->SetCurrentSelectWeapon(LData, this->GetIndex());

		LWepaonGems->EnableWepaonToSlot();

		this->M_Button->SetIsEnabled(false);
	}
}

void UUI_PlayerBagMaterialGrid::AddGemToSlot()
{
	if (USynModel_WepaonGems* LWepaonGems = this->M_UUI_PlayerSynthesis->GetWepaonGemsFunction())
	{
		//取消宝石数据选择
		LWepaonGems->SetCurrentSelectGem(nullptr, -1);
		//重新加载数据
		LWepaonGems->WidgetDataReload();

		//设置新的选择数据
		TObjectPtr<UItemDataTable> LData = NewObject<UItemDataTable>(this);
		LData->SetValue((FTableRowBase*)(this->M_EquipmentBaseData));

		LWepaonGems->SetCurrentSelectGem(LData, this->GetIndex());

		//解析宝石
		if (LWepaonGems->EnableGemToSlot())
		{
			this->M_Button->SetIsEnabled(false);
		}
		else {
			LWepaonGems->SetCurrentSelectGem(nullptr, -1);
		}
	}
}

void UUI_PlayerBagMaterialGrid::GemUp_AddWepaonToSlot()
{
	if (USynModel_GemsUpGrade* LGemUpGrade = this->M_UUI_PlayerSynthesis->GetGemsUpGradeFunction())
	{
		//设置武器和宝石的选择
		LGemUpGrade->SetCurrentSelectWeapon(nullptr, -1);
		LGemUpGrade->SetCurrentSelectGem(nullptr, -1);

		//重新加载数据
		LGemUpGrade->WidgetResetLoadData();

		//设置新的选择数据【武器】
		TObjectPtr<UItemDataTable> LData = NewObject<UItemDataTable>(this);
		LData->SetValue((FTableRowBase*)(this->M_EquipmentBaseData));

		//选择武器
		LGemUpGrade->SetCurrentSelectWeapon(LData, this->GetIndex());
		//添加武器
		LGemUpGrade->AddWeapon();

		//禁用按钮
		this->M_Button->SetIsEnabled(false);
	}
}

void UUI_PlayerBagMaterialGrid::GemUp_AddGemToSlot()
{
	if (USynModel_GemsUpGrade* LGemUpGrade = this->M_UUI_PlayerSynthesis->GetGemsUpGradeFunction())
	{
		//设置武器和宝石的选择
		LGemUpGrade->SetCurrentSelectWeapon(nullptr, -1);
		LGemUpGrade->SetCurrentSelectGem(nullptr, -1);

		//重新加载数据
		LGemUpGrade->WidgetResetLoadData();
		LGemUpGrade->WidgetReset();

		//设置新的选择数据【宝石】
		TObjectPtr<UItemDataTable> LData = NewObject<UItemDataTable>(this);
		LData->SetValue((FTableRowBase*)(this->M_EquipmentBaseData));

		//选择宝石
		LGemUpGrade->SetCurrentSelectGem(LData, this->GetIndex());
		//添加宝石
		LGemUpGrade->AddGem();

		//禁用按钮
		this->M_Button->SetIsEnabled(false);
	}
}

void UUI_PlayerBagMaterialGrid::GemUp_AddColoverRate()
{
	if (USynModel_GemsUpGrade* LGemUpGrade = this->M_UUI_PlayerSynthesis->GetGemsUpGradeFunction())
	{
		//重新加载材料
		LGemUpGrade->LoadMaterials();

		//禁用按钮
		this->M_Button->SetIsEnabled(false);

		//设置新的选择数据【宝石】
		TObjectPtr<UItemDataTable> LData = NewObject<UItemDataTable>(this);
		LData->SetValue((FTableRowBase*)(this->M_MaterialBaseData));

		//设置值
		LGemUpGrade->SetCurrentSelectClover(LData);

		//设置外观
		this->M_UUI_PlayerSynthesis->SetButtonStyle(this->M_UUI_PlayerSynthesis->GetCloverButton(), this->M_MaterialBaseData->ItemTexturePath.ToString());

		//更新概率
		LGemUpGrade->UpdateRate();
	}
}

void UUI_PlayerBagMaterialGrid::GemSlip_AddGemToSlot()
{
	if (USynModel_GemSplit* LGemSplit = this->M_UUI_PlayerSynthesis->GetGemSplitFunction())
	{
		LGemSplit->SetCurrentSelectGem(nullptr, -1);

		//禁用按钮
		this->M_Button->SetIsEnabled(false);

		//设置新的选择数据【宝石】
		TObjectPtr<UItemDataTable> Ldata = NewObject<UItemDataTable>(this);
		Ldata->SetValue((FTableRowBase*)(this->M_MaterialBaseData));

		//设置值
		LGemSplit->SetCurrentSelectGem(Ldata, this->GetIndex());

		LGemSplit->LoadEquipment();

		LGemSplit->AddGem();
	}
}

void UUI_PlayerBagMaterialGrid::AddExchangeSplit_SplitSlot()
{
	if (IsValid(this->M_Other_UI))
	{
		if (UUI_ExchangeAndSplit* const LExUI = Cast<UUI_ExchangeAndSplit>(this->M_Other_UI))
		{
			if (IsValid(LExUI->GetCurrentSelectButt()))
				LExUI->GetCurrentSelectButt()->SetIsEnabled(true);

			UItemDataTable* Data = NewObject<UItemDataTable>(this);
			Data->SetValue((FTableRowBase*)(this->M_MaterialBaseData));
			LExUI->SetCurrentSlipItem(EItemType::E_MaterialItem, this->GetIndex(), Data, this->GetButton());
			this->GetButton()->SetIsEnabled(false);
		}
	}
}

void UUI_PlayerBagMaterialGrid::UseCardSkillBook()
{
	FString Content = FString(TEXT("是否要使用:") + FString(this->M_MaterialBaseData->ItemName) + TEXT("[使用之后可以为该防御卡提升技能经验值，一定经验值之后可以提升技能等级。]"));

	//创建物品提示按钮
	UUI_ItemDesTip* TipComp = CreateWidget<UUI_ItemDesTip>(this,
		LoadClass<UUI_ItemDesTip>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_ItemDesTip.BPUI_ItemDesTip_C'"))
		);

	TipComp->SetContentTitleText(this->M_MaterialBaseData->ItemName);
	TipComp->SetContentText(Content);
	TipComp->SetOkButtonTitleText(TEXT("使用全部"));
	TipComp->SetCancelButtonTitleText(TEXT("关闭"));
	TipComp->SetItemHeadPath(this->M_MaterialBaseData->ItemTexturePath.ToString());

	FScriptDelegate Add;
	Add.BindUFunction(this, TEXT("UseSkillBook"));
	TipComp->GetOkButtonComponent()->OnClicked.Add(Add);
	TipComp->AddToViewport(1);
}

void UUI_PlayerBagMaterialGrid::UseSkillBook()
{
	UDataTable* Table = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardSkillBookData/CardSkillBooks.CardSkillBooks'")
		);

	for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
	{
		if (Item.M_Name.Equals(this->M_MaterialBaseData->ItemDescrible))
		{
			//获取技能书信息
			TArray<FCardSkillBooks_Data*> Datas;

			Table->GetAllRows<FCardSkillBooks_Data>(TEXT("List"), Datas);

			//获取数据
			for (auto LItem : Datas)
			{
				//成功拿到数据
				if (LItem->M_FMaterial.ItemName.Equals(this->M_MaterialBaseData->ItemName))
				{
					this->UseSkillBookData(LItem->M_FMaterial, this->M_MaterialBaseData->M_Count);
					break;
				}
			}
			break;
		}
	}

	//加载列表
	this->M_UUI_PlayerBag->LoadMaterialData();
	this->M_UUI_PlayerBag->LoadCardSkillBookTabs_DefCategory();

}

bool UUI_PlayerBagMaterialGrid::UseSkillBookData(FCardSkillBookMaterial _Data, int32 _BooksCount)
{
	//从玩家存档的背包中拿取源数据
	TArray<FSkillBookData>& BooksData = UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas;
	TArray<FMaterialBase>& MaterialsData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material;

	auto Func = [](int32 _UpLV, int32& _Lv, int32& _CurrentEx, int32& _CurrentTotalEx, int32& _ItemCount, float ExAddRate, int32 ExLevel, int32 ExAdd)->bool {
		//突破等级功能
		if (_Lv == 0 && ExLevel == 1)
		{
			//等级增加
			_Lv++;
			//经验赋值
			_CurrentEx = ExAdd;
			//道具数量减少
			_ItemCount--;
			return true;
		}
		//从[_UpLV]级突破到[_UpLV + 1]级(经验相等)
		else if (_Lv == _UpLV - 1 && ExLevel == _UpLV && _CurrentEx == _CurrentTotalEx)
		{
			_Lv++;
			_ItemCount--;
			_CurrentEx = ExAdd;
			//计算下一个等级需要的经验值
			_CurrentTotalEx = _CurrentTotalEx * (ExAddRate + 1);
			return true;
		}
		else if (_Lv < ExLevel) {
			FString Content = FString(TEXT("经验书等级过高，请提升防御卡技能书等级"));
			UWidgetBase::CreateTipWidget(Content);
		}



		return false;
	};

	auto DataUpdate = [](TArray<FMaterialBase>& _MDatas, FMaterialBase& _MB, int32 _Count, FSkillBookData& _SkillItem, int32 _Lv, int32 _Ex, int32 _ExTotal) {
		//清理道具
		for (auto& Item : _MDatas)
		{
			if (Item.ItemName.Equals(_MB.ItemName))
			{
				//道具个数赋值
				Item.M_Count = _Count;
				//为0则清除
				if (Item.M_Count <= 0)
					Item.bWaitRemove = true;

				break;
			}
		}

		//更新数据
		_SkillItem.M_LV = _Lv;
		_SkillItem.M_CurrentEx = _Ex;
		_SkillItem.M_CurrentTotalEx = _ExTotal;
	};

	bool _Result = false;

	//等级判断
	for (auto& BookItem : BooksData)
	{
		if (BookItem.M_Name.Equals(_Data.ItemDescrible))
		{

			//是否满级
			if (BookItem.M_IsMax)
			{
				FString Content = FString(BookItem.M_Name + TEXT("已经满级"));
				UWidgetBase::CreateTipWidget(Content);
				return false;
			}

			int32 LV = BookItem.M_LV;
			int32 CurrentEx = BookItem.M_CurrentEx;
			int32 CurrentTotalEx = BookItem.M_CurrentTotalEx;
			int32 ItemCount = _BooksCount;

			//判断是否需要突破（4级以下需要突破）
			if (CurrentEx == CurrentTotalEx && LV < 4 || LV == 0)
			{

				if (Func(LV + 1, LV, CurrentEx, CurrentTotalEx, ItemCount, BookItem.M_NextAddTotalEx, _Data.M_CardSkillBookMaterial_ExLevel, _Data.M_CardSkillBookMaterial_ExAdd))
				{
					//突破成功 更新数据
					DataUpdate(MaterialsData, _Data, ItemCount, BookItem, LV, CurrentEx, CurrentTotalEx);

					_Result = true;

					FString Content = FString(TEXT("[") + BookItem.M_Name + TEXT("技能书]已经突破等级至") +
						FString::FromInt(LV) + TEXT("级"));
					UWidgetBase::CreateTipWidget(Content);

					//添加历史记录
					UPlayerRecord::AddDayCardSkillBook(BookItem.M_Name);

					UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

					break;
				}
			}

			//技能书经验使用(小于4经验值满了则需要停止使用)[技能书不能跨级使用比如：3级使用经验增加1级]
			if (CurrentEx != CurrentTotalEx && LV < 4 && _Data.M_CardSkillBookMaterial_ExLevel <= LV)
			{
				//存储道具个数->最终确认道具最终个数
				int32 Temp_ItemCount = ItemCount;
				//使用道具个数
				for (int32 i = 1; i <= ItemCount; i++, Temp_ItemCount--)
				{
					CurrentEx += _Data.M_CardSkillBookMaterial_ExAdd;
					//经验值满了-停止使用
					if (CurrentEx >= CurrentTotalEx)
					{
						//使用完成
						CurrentEx = CurrentTotalEx;
						break;
					}
				}

				FString Content = FString(TEXT("[") + BookItem.M_Name + TEXT("技能书]的经验值增加了"));
				UWidgetBase::CreateTipWidget(Content);

				//添加历史记录
				UPlayerRecord::AddDayCardSkillBook(BookItem.M_Name);

				UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

				//使用完成->操作道具个数以及更新数据
				DataUpdate(MaterialsData, _Data, Temp_ItemCount, BookItem, LV, CurrentEx, CurrentTotalEx);
			}
			//大于等于4则可以直接升级
			else if (CurrentEx != CurrentTotalEx && LV >= 4 && _Data.M_CardSkillBookMaterial_ExLevel <= LV)
			{

				int32 TempCount = ItemCount;
				//使用道具个数
				for (int32 i = 1; i <= ItemCount; i++, TempCount--)
				{
					CurrentEx += _Data.M_CardSkillBookMaterial_ExAdd;
					//经验值满了->提升等级
					if (CurrentEx >= CurrentTotalEx)
					{
						//使用完成
						LV++;
						CurrentEx = CurrentEx - CurrentTotalEx;
						CurrentTotalEx = CurrentTotalEx * (BookItem.M_NextAddTotalEx + 1);

						if (LV >= 9)
						{
							CurrentEx = CurrentTotalEx;
							BookItem.M_IsMax = true;
							break;
						}

					}
				}

				FString Content = FString(TEXT("[") + BookItem.M_Name + TEXT("技能书]的经验值增加了"));
				UWidgetBase::CreateTipWidget(Content);

				//添加历史记录
				UPlayerRecord::AddDayCardSkillBook(BookItem.M_Name);

				UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

				//使用完成->操作道具个数以及更新数据
				DataUpdate(MaterialsData, _Data, TempCount, BookItem, LV, CurrentEx, CurrentTotalEx);
			}
			else {
				if (BookItem.M_CurrentEx == BookItem.M_CurrentTotalEx && BookItem.M_LV >= _Data.M_CardSkillBookMaterial_ExLevel)
				{
					FString Content = FString(TEXT("请使用[高等级]技能书突破等级"));
					UWidgetBase::CreateTipWidget(Content);
				}
				else {
					FString Content = FString(TEXT("经验书等级过高，请提升防御卡技能书等级"));
					UWidgetBase::CreateTipWidget(Content);
				}
			}

		}
	}

	UGameSystemFunction::ClearWaitingItems(MaterialsData);
	UGameSystemFunction::SaveCurrentPlayerData();

	return _Result;
}



