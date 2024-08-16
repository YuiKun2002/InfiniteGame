// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>

//卡片制作
#include "Game/UI/Synthesis/SynModel_MakeCard.h"
//卡片强化
#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
//卡片转职
#include "Game/UI/Synthesis/SynModel_CardChange.h"
//金卡进化
#include "Game/UI/Synthesis/SynModel_GoldCardUp.h"
//武器开槽镶嵌
#include "Game/UI/Synthesis/SynModel_WepaonGems.h"
//宝石强化
#include "Game/UI/Synthesis/SynModel_GemsUpGrade.h"
//宝石分解
#include "Game/UI/Synthesis/SynModel_GemSplit.h"
//材料基础结构
#include "GameSystem/Item/Material/MaterialBaseStruct.h"

#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"

bool UUI_PlayerSynthesis::Initialize()
{
	if (!Super::Initialize())
		return false;

	//【卡片界面显示与隐藏，装备界面】
	this->M_ListDataPanel = this->GetWidgetComponent<UWidget>(this, "Cards_E_Panel");
	//保险金界面
	this->M_InsurancePanelPanel = this->GetWidgetComponent<UWidget>(this, "CanvasPanel_7");
	//四叶草界面
	this->M_ColoverPanel = this->GetWidgetComponent<UWidget>(this, "CanvasPanel_6");
	//文本
	this->Coin0Text = this->GetWidgetComponent<UTextBlock>(this, "Coin_0");
	this->Coin1Text = this->GetWidgetComponent<UTextBlock>(this, "Coin_2");
	this->InsuranceText = this->GetWidgetComponent<UTextBlock>(this, "TextBlock_107");

	/*----------------------------------------材料加载区----------------------------------------*/
	this->M_UScrollBox_Item_Material = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_77");
	this->M_Materials_2_BoxArr = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_60");
	/*----------------------------------------材料加载区----------------------------------------*/

	/*----------------------------------------卡片加载器----------------------------------------*/
	//加载卡片需要的界面
	this->M_UScrollBox_Card_Uniform = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_1");
	this->M_Uniform_CardGrid = this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_1");
	/*----------------------------------------卡片加载器----------------------------------------*/

	/*----------------------------------------共享功能----------------------------------------*/
	//保险按钮
	this->M_Insurance_Butt = this->GetWidgetComponent<UButton>(this, "Button_4");
	//四叶草按钮
	this->M_Clover_Butt = this->GetWidgetComponent<UButton>(this, "Button_3");
	/*----------------------------------------共享功能----------------------------------------*/



	/*----------------------------------------卡片制作----------------------------------------*/
	this->M_MakeCard_TabButt = this->GetWidgetComponent<UButton>(this, "MakeCard_butt");
	this->M_MakeCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Make_P");
	/*----------------------------------------卡片制作----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	this->M_CardUpgrade_TabButt = this->GetWidgetComponent<UButton>(this, "UpCard_butt");
	this->M_CardUpgradeP = this->GetWidgetComponent<UCanvasPanel>(this, "Up_P");
	/*----------------------------------------卡片强化----------------------------------------*/

	/*----------------------------------------卡片转职----------------------------------------*/
	this->M_CardChange_TabButt = this->GetWidgetComponent<UButton>(this, "ChangeCard_butt");
	this->M_CardChangeP = this->GetWidgetComponent<UCanvasPanel>(this, "Change_P");
	/*----------------------------------------卡片转职----------------------------------------*/


	/*----------------------------------------金卡进化----------------------------------------*/
	this->M_GoldCardUpHead = this->GetWidgetComponent<UImage>(this, "GoldCardUpHead");
	this->M_GoldCardUpResultHead = this->GetWidgetComponent<UImage>(this, "Image_164");
	this->M_GoldCardUp_Butt = this->GetWidgetComponent<UButton>(this, "GoldCardUp_Butt");

	this->M_GoldCardUp_TabButt = this->GetWidgetComponent<UButton>(this, "TabGoldCardUp_Butt");
	this->M_GoldCardUpP = this->GetWidgetComponent<UCanvasPanel>(this, "MainContent_2");
	/*----------------------------------------金卡进化----------------------------------------*/

	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	this->M_WepaonGem_TabButt = this->GetWidgetComponent<UButton>(this, "WeaponGem");
	this->M_WepaonGemP = this->GetWidgetComponent<UCanvasPanel>(this, "WeaponGem_P");
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
	this->M_GemUp_TabButt = this->GetWidgetComponent<UButton>(this, "GemUp");
	this->M_GemUpP = this->GetWidgetComponent<UCanvasPanel>(this, "GemUp_P");
	/*----------------------------------------宝石强化----------------------------------------*/

	/*----------------------------------------宝石分解----------------------------------------*/
	this->M_GemSplit_TabButt = this->GetWidgetComponent<UButton>(this, "GemSplit");
	this->M_GemSplitP = this->GetWidgetComponent<UCanvasPanel>(this, "GemSplit_P");
	/*----------------------------------------宝石分解----------------------------------------*/



	return true;
}

//初始化所有的加载器
void UUI_PlayerSynthesis::Loader_Init()
{
	if (this->M_TabWidgets_Panels.Num())
	{
		return;
	}
	/*----------------------------------------[卡片界面主容器的子界面【卡片界面，装备界面】]----------------------------------------*/
	this->M_TabWidgets_Panels.Emplace(this->GetWidgetComponent<UWidget>(this, "ScrollBox_1"));
	this->M_TabWidgets_Panels.Emplace(this->GetWidgetComponent<UWidget>(this, "ScrollBox_2"));
	/*----------------------------------------[卡片界面主容器的子界面【卡片界面，装备界面】]----------------------------------------*/

	/*----------------------------------------保险金----------------------------------------*/
	this->M_Insurance_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::SelectInsurance);
	/*----------------------------------------保险金----------------------------------------*/

	/*----------------------------------------卡片制作----------------------------------------*/
	//this->M_MakeCard_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowMakeCard);
	if (!this->M_USynModel_MakeCard)
	{
	//	this->M_USynModel_MakeCard = NewObject<USynModel_MakeCard>(this, TEXT("Synthesis_USynModel_MakeCard"));
	//	this->M_USynModel_MakeCard->InitializeBySynthesis(this);
	}
	/*----------------------------------------卡片制作----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	//this->M_CardUpgrade_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowCardUpgrade);
	
	/*----------------------------------------卡片强化----------------------------------------*/


	/*----------------------------------------卡片转职----------------------------------------*/
	this->M_CardChange_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowChangeCard);
	if (!this->M_USynModel_CardChange)
	{
		this->M_USynModel_CardChange = NewObject<USynModel_CardChange>(this, TEXT("Synthesis_USynModel_CardChange"));
		this->M_USynModel_CardChange->InitializeBySynthesis(this);
	}
	/*----------------------------------------卡片转职----------------------------------------*/


	/*----------------------------------------金卡进化----------------------------------------*/
	this->M_GoldCardUp_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowGoldCardUpPanel);
	if (!this->M_USynModel_GoldCardUp)
	{
		this->M_USynModel_GoldCardUp = NewObject<USynModel_GoldCardUp>(this, TEXT("Synthesis_USynModel_GoldCardUp"));
		this->M_USynModel_GoldCardUp->InitializeBySynthesis(this);
	}
	/*----------------------------------------金卡进化----------------------------------------*/

	//-----------------------------------------------
	//记录卡片状态是否存放了卡片（名称为""表示没有存放卡片）
	this->M_GoldCard_Name_State.Emplace("");
	this->M_GoldCard_Name_State.Emplace("");
	//需要消耗掉的卡片
	this->M_NeedCards.Emplace("");
	this->M_NeedCards.Emplace("");
	//需要消耗掉的卡片等级
	this->M_NeedGradeCards.Emplace(0);
	this->M_NeedGradeCards.Emplace(0);
	//对应卡片的UI索引
	this->M_NeedCard_UI_Index.Emplace(-1);
	this->M_NeedCard_UI_Index.Emplace(-1);
	//-----------------------------------------------

	this->M_GoldCard_Select_Button.Emplace(this->GetWidgetComponent<UButton>(this, "Button_143"));
	this->M_GoldCard_Select_Button.Emplace(this->GetWidgetComponent<UButton>(this, "Button_7"));

	//选项卡切换功能(宝石卡片，神级进化)
	this->M_TabSelectPanels.Emplace(this->GetWidgetComponent<UWidget>(this, "PanelChange"));
	this->M_TabSelectPanels.Emplace(this->GetWidgetComponent<UWidget>(this, "PanelChange2"));

	//卡片展示界面集合
	this->M_Cards2_Box.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_1"));
	this->M_Cards2_Box.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_2"));

	/*----------------------------------------武器开槽镶嵌----------------------------------------*/
	this->M_WepaonGem_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowWeaponGem);
	//初始化武器开槽镶嵌功能
	if (!this->M_USynModel_WepaonGems)
	{
		this->M_USynModel_WepaonGems = NewObject<USynModel_WepaonGems>(this, TEXT("Synthesis_USynModel_WepaonGems"));
		this->M_USynModel_WepaonGems->InitializeBySynthesis(this);
	}
	/*----------------------------------------武器开槽镶嵌----------------------------------------*/

	/*----------------------------------------宝石强化----------------------------------------*/
	this->M_GemUp_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowGemUp);
	if (!this->M_USynModel_GemsUpGrade)
	{
		this->M_USynModel_GemsUpGrade = NewObject<USynModel_GemsUpGrade>(this, TEXT("Synthesis_USynModel_GemsUpGrade"));
		this->M_USynModel_GemsUpGrade->InitializeBySynthesis(this);
	}
	/*----------------------------------------宝石强化----------------------------------------*/

	/*----------------------------------------宝石分解----------------------------------------*/
	this->M_GemSplit_TabButt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowGemSplit);
	if (!this->M_USynModel_GemSplit)
	{
		this->M_USynModel_GemSplit = NewObject<USynModel_GemSplit>(this, TEXT("Synthesis_USynModel_GemSplit"));
		this->M_USynModel_GemSplit->InitializeBySynthesis(this);
	}
	/*----------------------------------------宝石分解----------------------------------------*/

	//材料加载器
	this->M_UItemLoadManager_Item_Material = NewObject<UItemLoadManager>(this, TEXT("Material_Load"));
	this->M_UItemLoadManager_Item_Material->InitWidget_First(this->M_Materials_2_BoxArr, this->M_UScrollBox_Item_Material);
	this->M_UItemLoadManager_Item_Material->InitRange_Second(1, 10, 800, 100.f, 100.f, true);
	this->M_UItemLoadManager_Item_Material->SetMainScrollTransfrom(FVector2D(30.f, 0.f), FVector2D(1.05f, 1.f));
	this->M_UItemLoadManager_Item_Material->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMaterial"));
	this->M_UItemLoadManager_Item_Material->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMaterial"));

	//卡片加载器
	this->M_UItemLoadManager_Card = NewObject<UItemLoadManager>(this, TEXT("CardLoad"));
	this->M_UItemLoadManager_Card->InitWidget_First(this->M_Uniform_CardGrid, this->M_UScrollBox_Card_Uniform);
	this->M_UItemLoadManager_Card->InitRange_Second(5, 6, 800, 120.f, 150.f);
	this->M_UItemLoadManager_Card->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitCards"));
	this->M_UItemLoadManager_Card->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateCards"));
}

//创建-材料区域-材料UI界面
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagMaterialGrid> LocalUIClass = Cast <UClass>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")))));
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LocalUIClass.Get());

	this->SetMaterialsData(Grid, _Data, _Index, this->M_BindFunctionName_Materials);

	return Grid;
}
//刷新-材料区域-材料UI界面
void UUI_PlayerSynthesis::WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMaterialsData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index, this->M_BindFunctionName_Materials);
}
//卡片制作界面-设置（香料，四叶草，等等）UI界面
void UUI_PlayerSynthesis::SetMaterialsData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index, const TArray<FMaterialsSerachTypeBind>& _BindFuncName)
{
	//设置材料索引
	_Grid->SetIndex(_Index);
	//设置材料按钮表示可以选择
	_Grid->GetButton()->SetIsEnabled(true);
	//设置材料的数量
	int32 CurCount = ((FMaterialBase*)(_CardData->GetValue()))->M_Count;
	//设置显示上限
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));
	//设置材料数据
	_Grid->SetMaterialData(((FMaterialBase*)(_CardData->GetValue())));
	//设置材料的外观
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), ((FMaterialBase*)(_CardData->GetValue()))->ItemTexturePath.ToString());

	//判断当前道具类型(是否被玩家选中)
	//switch (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType)
	//{
	//case EMaterialType::E_Spices://判断当前UI是否被玩家选中->如果选中则禁用按钮
	//	if (this->M_MakeCard_CurrentSelect_Index == _Index) { _Grid->GetButton()->SetIsEnabled(false); this->M_MakeCard_CurrentSelectData = ((FMaterialBase*)(_CardData->GetValue())); } break;
	//}

	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	//绑定函数
	for (auto& LBind : _BindFuncName)
	{
		if (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType == LBind.M_Type)
		{
			//设置指向合成屋的UI
			_Grid->SetUI_PlayerSynthesis(this);
			for (const auto& FnName : LBind.M_BindFnName)
			{
				//如果是空的则不绑定
				if (FnName.IsEqual(""))
				{
					continue;
				}

				//绑定
				FScriptDelegate AddFunc;
				AddFunc.BindUFunction(_Grid, FnName);
				_Grid->GetButton()->OnClicked.Add(AddFunc);
			}

			break;
		}
	}

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}
/*加载--卡片界面（卡片制作，金卡进化，卡片转职）显示区域--开始*/
//加载卡片UI界面
UWidget* UUI_PlayerSynthesis::WidgetCreate_InitCards(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this,
		LoadClass<UUI_PlayerBagCardGrid>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));

	this->SetCardData(Grid, _Data, _Index);

	return Grid;
}
//更新卡片UI界面
void UUI_PlayerSynthesis::WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetCardData(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);
}
//设置卡片UI的数据（需要加载卡片的所有界面-不包括卡片制作界面）
void UUI_PlayerSynthesis::SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index)
{
	//设置纹理
	_Grid->M_CardTexturePath = ((FItemCard*)(_Data->GetValue()))->ItemTexturePath.ToString();
	//设置数据
	_Grid->SetFItemCardData(((FItemCard*)(_Data->GetValue())));
	//设置指向
	_Grid->SetUI_PlayerSynthesis(this);
	//启动按钮
	_Grid->GetButton()->SetIsEnabled(true);

	//设置索引
	_Grid->SetUIIndex(_Index);
	_Grid->SetCardIndex(_Index);

	//更新文本
	_Grid->UpdateButtonTexture(FString::FromInt(((FItemCard*)(_Data->GetValue()))->M_CardPrice));

	//清理绑定
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	this->OnCardLoad.ExecuteIfBound(_Grid, _Data, _Index);

	//绑定音效
	FScriptDelegate AddFunc_;
	AddFunc_.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc_);
}
/*加载卡片界面--结束*/

class USynModel_MakeCard* UUI_PlayerSynthesis::GetMakeCardFunction()
{
	return this->M_USynModel_MakeCard;
}

class USynModel_CardUpgrade* UUI_PlayerSynthesis::GetCardUpgradeFunction()
{
	return this->M_USynModel_CardUpgrade;
}

class USynModel_CardChange* UUI_PlayerSynthesis::GetCardChangeFunction()
{
	return this->M_USynModel_CardChange;
}

//【金卡进化】加载金卡进化
void UUI_PlayerSynthesis::LoadGoldCard_Card()
{
	//设置金卡进化界面
	this->M_Uniform_LoadCardData_Panel_Index = 2;

	//背包容量
	this->M_CardBagCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, this->M_CardBagCount);

	//this->M_UItemLoadManager_CardUpGrade->UpdateDatatable(Datas);
	//this->M_UItemLoadManager_CardUpGrade->SetLoadItemMaxCount(Datas.Num());
	//this->M_UItemLoadManager_CardUpGrade->ContinueRun();
}

//设置特定的界面显示
void UUI_PlayerSynthesis::ShowCardGemPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index)
{
	UWidgetBase::SetButtonStyle(this->M_MakeCard_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_8.T_PS_8'");
	UWidgetBase::SetButtonStyle(this->M_CardUpgrade_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_10.T_PS_10'");
	UWidgetBase::SetButtonStyle(this->M_CardChange_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_12.T_PS_12'");
	UWidgetBase::SetButtonStyle(this->M_WepaonGem_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/665.665'");
	UWidgetBase::SetButtonStyle(this->M_GemUp_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/670.670'");
	UWidgetBase::SetButtonStyle(this->M_GemSplit_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/675.675'");

	//卡片制作界面
	this->M_MakeCardP->SetVisibility(ESlateVisibility::Collapsed);
	//卡片强化界面
	this->M_CardUpgradeP->SetVisibility(ESlateVisibility::Collapsed);
	//......
	this->M_CardChangeP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemSplitP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUpP->SetVisibility(ESlateVisibility::Collapsed);
	this->M_WepaonGemP->SetVisibility(ESlateVisibility::Collapsed);

	switch (_Index) {
	case 1:UWidgetBase::SetButtonStyle(this->M_CardUpgrade_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_11.T_PS_11'"); break;
	case 2:UWidgetBase::SetButtonStyle(this->M_CardChange_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_13.T_PS_13'"); break;
	case 3:UWidgetBase::SetButtonStyle(this->M_WepaonGem_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/662.662'"); break;
	case 4:UWidgetBase::SetButtonStyle(this->M_GemUp_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/668.668'"); break;
	case 5:UWidgetBase::SetButtonStyle(this->M_GemSplit_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/673.673'"); break;
	default:UWidgetBase::SetButtonStyle(this->M_MakeCard_TabButt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_9.T_PS_9'"); break;
	}

	_UCanvasPanel->SetVisibility(ESlateVisibility::Visible);
}

//显示卡片制作界面
void UUI_PlayerSynthesis::ShowMakeCard()
{
	if (!IsValid(this->M_USynModel_MakeCard))
	{
		this->M_USynModel_MakeCard = NewObject<USynModel_MakeCard>(this, TEXT("Synthesis_USynModel_MakeCard"));
		this->M_USynModel_MakeCard->InitializeBySynthesis(this);
	}
	this->M_USynModel_MakeCard->WidgetReset();
	this->M_USynModel_MakeCard->WidgetResetLoadData();
}

void UUI_PlayerSynthesis::ShowCardUpgrade()
{
	if (!this->M_USynModel_CardUpgrade)
	{
		this->M_USynModel_CardUpgrade = NewObject<USynModel_CardUpgrade>(this, TEXT("Synthesis_USynModel_CardUpgrade"));
		this->M_USynModel_CardUpgrade->InitializeBySynthesis(this);
	}
	this->M_USynModel_CardUpgrade->WidgetReset();
	this->M_USynModel_CardUpgrade->WidgetResetLoadData();
}
//显示卡片转职界面
void UUI_PlayerSynthesis::ShowChangeCard()
{
	this->ShowCardGemPanel(this->M_CardChangeP, 2);
	this->M_USynModel_CardChange->WidgetReset();
	this->M_USynModel_CardChange->WidgetResetLoadData();
	this->ShowDataPanel(ESlateVisibility::Visible, 0, 0, ESlateVisibility::Visible, ESlateVisibility::Visible);
}
//显示武器宝石界面
void UUI_PlayerSynthesis::ShowWeaponGem()
{
	this->ShowCardGemPanel(this->M_WepaonGemP, 3);

	this->M_USynModel_WepaonGems->WidgetReset();
	this->M_USynModel_WepaonGems->LoadEquipment();
	this->M_USynModel_WepaonGems->LoadMaterials();

	this->ShowDataPanel(ESlateVisibility::Visible, 1, 0, ESlateVisibility::Collapsed);
}
//显示宝石强化界面
void UUI_PlayerSynthesis::ShowGemUp()
{
	this->ShowCardGemPanel(this->M_GemUpP, 4);

	this->M_USynModel_GemsUpGrade->WidgetReset();
	this->M_USynModel_GemsUpGrade->WidgetResetLoadData();

	this->ShowDataPanel(ESlateVisibility::Visible, 1, 0, ESlateVisibility::Visible, ESlateVisibility::Visible);

}
//显示宝石分解界面
void UUI_PlayerSynthesis::ShowGemSplit()
{
	this->ShowCardGemPanel(this->M_GemSplitP, 5);

	this->M_USynModel_GemSplit->WidgetReset();
	this->M_USynModel_GemSplit->WidgetResetLoadData();

	this->ShowDataPanel(ESlateVisibility::Visible, 1, 0, ESlateVisibility::Collapsed);

}
//【金卡进化】显示金卡进化界面
void UUI_PlayerSynthesis::ShowGoldCardUpPanel()
{
	this->ShowDataPanel(ESlateVisibility::Visible, 0, 1, ESlateVisibility::Collapsed);

	this->M_USynModel_GoldCardUp->WidgetReset();
	this->M_USynModel_GoldCardUp->WidgetResetLoadData();

	/*
	this->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);

	for (auto LBut : this->M_GoldCard_Select_Button)
	{
		LBut->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (auto& LButState : this->M_GoldCard_Name_State)
	{
		LButState.Empty();
	}

	this->M_GoldCardUp_Butt->SetIsEnabled(false);

	*/
}
//关闭合成屋界面
void UUI_PlayerSynthesis::ClearPanel()
{
	//关闭随机更新
	UGameSystemFunction::CloseUpdate();

	this->RemoveFromParent();

	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关闭合成屋界面")));
}

UButton* UUI_PlayerSynthesis::GetCloverButton()
{
	return this->M_Clover_Butt;
}

//【共享界面】保险金的选择
void UUI_PlayerSynthesis::SelectInsurance()
{
	if (this->M_UpGrade_bSelectInsurance)
	{
		this->M_UpGrade_bSelectInsurance = false;
		UWidgetBase::SetButtonStyle(
			this->M_Insurance_Butt,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_26.T_PS_26'");
	}
	else {
		this->M_UpGrade_bSelectInsurance = true;
		UWidgetBase::SetButtonStyle(
			this->M_Insurance_Butt,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_27.T_PS_27'"
		);
	}
}

//【金卡进化】将一张被选择的卡片从金卡进化槽里面移出
void UUI_PlayerSynthesis::RemoveCardSlot(int32 _Index)
{
	if (_Index < 0 || _Index > 2)
		return;

	this->M_NeedCards[_Index] = "";
	this->M_GoldCard_Name_State[_Index] = "";
	this->M_NeedCard_UI_Index[_Index] = -1;
	this->M_GoldCard_Select_Button[_Index]->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUpResultHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GoldCardUp_Butt->SetIsEnabled(false);

	this->LoadGoldCard_Card();
}
//【金卡进化】金卡进化 进化 功能
void UUI_PlayerSynthesis::GoldCardUp()
{
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		//搜寻进化凭证
		if (FMaterialBase* GoldCardUpItemData = Player->GetMaterialByName(this->M_GoldCardUp_ItemName))
		{
			GoldCardUpItemData->M_Count--;
			if (GoldCardUpItemData->M_Count <= 0)
			{
				GoldCardUpItemData->bWaitRemove = true;
			}

			//删除道具
			UGameSystemFunction::ClearWaitingItems(Player->M_PlayerItems_Material);
			//保存数据
			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("金卡进化操作")));
		}

		//搜寻卡片
		for (auto CardPP = this->M_NeedCards.CreateConstIterator(); CardPP; ++CardPP)
		{
			//搜索对应卡片
			for (auto SourceCardData = Player->M_PlayerItems_Card.CreateIterator(); SourceCardData; ++SourceCardData)
			{
				if (
					(*SourceCardData).ItemName.ToString().Equals((*CardPP))
					&&
					(*SourceCardData).M_CardGrade == this->M_NeedGradeCards[CardPP.GetIndex()]
					)
				{
					//直接移除当前元素
					SourceCardData.RemoveCurrent();
					//跳出
					break;
				}
			}
		}

		UWidgetBase::CreateTipWidget(TEXT("进化成功"));

		//直接发送新的卡片到背包
		UGameSystemFunction::SendCardToPlayerBag(this->M_GoldCardUp_CardName, this->M_NeedGradeCards[0] > this->M_NeedGradeCards[1] ? this->M_NeedGradeCards[0] : this->M_NeedGradeCards[1]);

		//取消卡片的选择
		this->M_NeedCard_UI_Index[0] = -1;
		this->M_NeedCard_UI_Index[1] = -1;

		//重新加载一次金卡进化
		this->ShowGoldCardUpPanel();
		this->LoadGoldCard_Card();
	}
}

class USynModel_GoldCardUp* UUI_PlayerSynthesis::GetGoldCardUpFunction()
{
	return this->M_USynModel_GoldCardUp;
}

USynModel_WepaonGems* UUI_PlayerSynthesis::GetWepaonGemsFunction()
{
	return this->M_USynModel_WepaonGems;
}

USynModel_GemsUpGrade* UUI_PlayerSynthesis::GetGemsUpGradeFunction()
{
	return this->M_USynModel_GemsUpGrade;
}

USynModel_GemSplit* UUI_PlayerSynthesis::GetGemSplitFunction()
{
	return this->M_USynModel_GemSplit;
}

//【选项卡选择界面】宝石强化选项卡
void UUI_PlayerSynthesis::Select_GemCardPanel()
{
	this->ShowDataPanel(ESlateVisibility::Collapsed, 0, 0, ESlateVisibility::Collapsed);
	this->ShowMakeCard();
}
//【选项卡选择界面】金卡进化选项卡
void UUI_PlayerSynthesis::Select_GoldCardUpPanel()
{
	this->ShowDataPanel(ESlateVisibility::Visible, 0, 1, ESlateVisibility::Collapsed);
	this->ShowGoldCardUpPanel();
}
//【更新显示】更新金币的文本
void UUI_PlayerSynthesis::Update_CoinText()
{
	this->Coin0Text->SetText(
		FText::FromString(FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0))
	);
	this->Coin1Text->SetText(
		FText::FromString(FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2))
	);
}
//【界面显示】显示保险金UI界面【卡片界面显示(武器的显示界面，卡片的显示界面)】
void UUI_PlayerSynthesis::ShowDataPanel(
	ESlateVisibility _ListDataPanel,
	uint8 _ListData_ShowIndex,
	uint8 _SynthesisPanel_ShowIndex,
	ESlateVisibility _InsurancePanel_State2,
	ESlateVisibility _ColoverPanel_State
)
{
	//列表界面
	this->M_ListDataPanel->SetVisibility(_ListDataPanel);
	//容器界面选择
	UGameSystemFunction::WidgetSelect<UWidget>(
		this->M_TabWidgets_Panels,
		ESlateVisibility::Visible,
		_ListData_ShowIndex
	);
	//大界面选择
	UGameSystemFunction::WidgetSelect<UWidget>(
		this->M_TabSelectPanels,
		ESlateVisibility::Visible,
		_SynthesisPanel_ShowIndex
	);
	//
	this->M_InsurancePanelPanel->SetVisibility(_InsurancePanel_State2);
	this->M_ColoverPanel->SetVisibility(_ColoverPanel_State);
}

void UUI_PlayerSynthesis::SetInsuranceText(const FString& _Text)
{
	this->InsuranceText->SetText(FText::FromString(_Text));
}

bool UUI_PlayerSynthesis::GetSelectInsurance()
{
	return this->M_UpGrade_bSelectInsurance;
}

void UUI_PlayerSynthesis::LoadCards(const TArray<FItemCard*>& CardsData, const int32& LoadCount)
{
	this->M_UItemLoadManager_Card->UpdateDatatable(CardsData);
	this->M_UItemLoadManager_Card->SetLoadItemMaxCount(LoadCount);
	this->M_UItemLoadManager_Card->ContinueRun();
}

void UUI_PlayerSynthesis::LoadMaterials(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords)
{
	this->M_BindFunctionName_Materials.Empty();

	//赋值绑定数据
	this->M_BindFunctionName_Materials.Append(_BindFuncName);

	//材料数据(获取指定的材料数据)		
	TArray<FMaterialBase*> TempArrays;
	//材料数据(获取指定的材料数据)		
	TArray<FMaterialBase*> TargetArrays;

	//获取类型
	TArray<EMaterialType> Types;
	//按照顺序添加类型
	for (const auto& _Type : _BindFuncName)
	{
		Types.Emplace(_Type.M_Type);
	}

	//添加材料
	UGameSystemFunction::GetMaterialsArrayByType(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material,
		Types,
		TempArrays
	);

	//排除数据
	if (IgnoreKeyWords.GetKeyWords().Num())
	{
		for (FMaterialBase* CurData : TempArrays)
		{
			for (const FString& CurIgnoreKeyWords : IgnoreKeyWords.GetKeyWords())
			{
				if (CurData->ItemName.ToString().Contains(CurIgnoreKeyWords))
				{
					continue;
				}
				else
				{
					TargetArrays.Emplace(CurData);
				}
			}
		}

		this->M_UItemLoadManager_Item_Material->UpdateDatatable(TargetArrays);
		this->M_UItemLoadManager_Item_Material->SetLoadItemMaxCount(TargetArrays.Num());
	}
	else {
		this->M_UItemLoadManager_Item_Material->UpdateDatatable(TempArrays);
		this->M_UItemLoadManager_Item_Material->SetLoadItemMaxCount(TempArrays.Num());
	}

	this->M_UItemLoadManager_Item_Material->SetCurrentPage(0);
	this->M_UItemLoadManager_Item_Material->SetResetScrollBoxOffset();
	this->M_UItemLoadManager_Item_Material->ContinueRun();
}

void UUI_PlayerSynthesis::LoadMaterials(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const TArray<FString>& IgnoreKeyWords)
{
	this->LoadMaterials(_BindFuncName, FMaterialsSerachKeyWordsIgnore(IgnoreKeyWords));
}
