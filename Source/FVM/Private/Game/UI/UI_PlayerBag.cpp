// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/UI/UI_PlayerBag.h"

#include "GameSystem/FVMGameInstance.h"

#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_CardSkillTable.h"
#include "Game/UI/UI_PlayerBagEquipmentGrid.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Data/EquipmentDataSturct.h"

#include "Game/UI/UI_PlayerBagRemove.h"

#include <Components/UniformGridPanel.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>

#include "Game/UI/Tips/UI_ShopPayTip.h"
#include <Components/EditableTextBox.h>
#include <Components/VerticalBox.h>

#include <Kismet/KismetSystemLibrary.h>

//角色形象
#include "Game/UI/UI_PlayerInformationShow.h"




//------------------------------------------------------------------------------------------------------------------------

//选项卡按钮功能
//UUI_PlayerBag::LoadCardSkillBookTabs_DefCategory()   关联函数

void UUI_PlayerBag::Open_SkillBooks_Normal()
{
	this->PlayOperateAudio();
	UGameSystemFunction::TabSelect(this->M_SkillBookButtons, FString(TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/")), this->M_SkillBookDefStyle, this->M_SkillBookClickStyle, 0);
	this->LoadCardSkillBookTabs(ESkillBookTipCategory::S_Normal);
}

void UUI_PlayerBag::Open_SkillBooks_Style0()
{
	this->PlayOperateAudio();
	UGameSystemFunction::TabSelect(this->M_SkillBookButtons, FString(TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/")), this->M_SkillBookDefStyle, this->M_SkillBookClickStyle, 1);
	this->LoadCardSkillBookTabs(ESkillBookTipCategory::S_Style_1);
}

void UUI_PlayerBag::Open_SkillBooks_Style1()
{
	this->PlayOperateAudio();
	UGameSystemFunction::TabSelect(this->M_SkillBookButtons, FString(TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/")), this->M_SkillBookDefStyle, this->M_SkillBookClickStyle, 2);
	this->LoadCardSkillBookTabs(ESkillBookTipCategory::S_Style_2);
}

void UUI_PlayerBag::Open_SkillBooks_Style2()
{
	this->PlayOperateAudio();
	UGameSystemFunction::TabSelect(this->M_SkillBookButtons, FString(TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/")), this->M_SkillBookDefStyle, this->M_SkillBookClickStyle, 3);
	this->LoadCardSkillBookTabs(ESkillBookTipCategory::S_Style_3);
}
//------------------------------------------------------------------------------------------------------------------------


void UUI_PlayerBag::RefreshPlayerSuit()
{
	this->M_PlayerInformationShow->SetCurrentPlayer(UFVMGameInstance::GetPlayerStructManager_Static());
	this->M_PlayerInformationShow->RefreshPlayerSuit();
}

bool UUI_PlayerBag::Initialize()
{
	//if (!Super::Initialize())
	//	return false;

	/*

	//角色形象----------------------------------------------------------------------------------------------------

	//获取角色形象界面
	this->M_PlayerPoint = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerPoint");
	if (!this->M_PlayerInformationShow)
	{
		this->M_PlayerInformationShow = CreateWidget<UUI_PlayerInformationShow>(this, UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/BPUI_PlayerInforPanel.BPUI_PlayerInforPanel_C'")))));
		this->M_PlayerPoint->AddChildToCanvas(this->M_PlayerInformationShow);
	}

	if (!this->M_PlayerInformationShow->OnLoaddingEnd.IsBound())
		this->M_PlayerInformationShow->OnLoaddingEnd.BindUFunction(this, TEXT("LoadItemsData"));

	this->M_PlayerBagContent = this->GetWidgetComponent<UUniformGridPanel>(this, "Bag_Content");
	this->M_PlayerBagSelectPanel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerBagSelectPanel");

	//加载背包格子的图片
	this->M_BagCountImage_1 = this->GetWidgetComponent<UImage>(this, "Image");
	this->M_BagCountImage_2 = this->GetWidgetComponent<UImage>(this, "Image_1");
	this->M_BagCountImage_3 = this->GetWidgetComponent<UImage>(this, "Image_2");
	this->M_BagCountImage_4 = this->GetWidgetComponent<UImage>(this, "Image_3");

	//加载背包格子的按钮
	this->M_BagGridButton_1 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_1");
	this->M_BagGridButton_2 = this->GetWidgetComponent<UButton>(this, "Bag_Grid");
	this->M_BagGridButton_3 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_2");
	this->M_BagGridButton_4 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_3");

	//加背包物品删除按钮
	this->M_BagItemUniformRemoveButton = this->GetWidgetComponent<UButton>(this, "RemoveItems");

	if (this->M_PlayerItemPanels.Num() == 0)
	{
		this->M_PlayerItemPanels.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "PlayerItemList"));
		this->M_PlayerItemPanels.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "PlayerItemList_1"));
		this->M_PlayerItemPanels.Emplace(this->GetWidgetComponent<UUniformGridPanel>(this, "PlayerItemList_2"));

		//背包物品删除按钮绑定
		this->M_BagItemUniformRemoveButton->OnClicked.AddDynamic(this, &UUI_PlayerBag::OpenRemovePanel);

		//背包格子的按钮绑定
		this->M_BagGridButton_1->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_1_Func);
		this->M_BagGridButton_2->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_2_Func);
		this->M_BagGridButton_3->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_3_Func);
		this->M_BagGridButton_4->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_4_Func);
	}

	//---------------------------------------------------------------------------------------------------------------------
	//    技能书区域
	//---------------------------------------------------------------------------------------------------------------------
	this->M_CardSkillBook_Scroll = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_100");
	this->M_CardSkillBook_Panel = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_70");

	if (this->M_SkillBookButtons.Num() == 0)
	{
		//绑定(选项卡)按钮组件
		this->M_SkillBookButtons.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_1")));
		this->M_SkillBookButtons.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_2")));
		this->M_SkillBookButtons.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_3")));
		this->M_SkillBookButtons.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_4")));
		//绑定对应功能
		this->M_SkillBookButtons[0]->OnClicked.AddDynamic(this, &UUI_PlayerBag::Open_SkillBooks_Normal);
		this->M_SkillBookButtons[1]->OnClicked.AddDynamic(this, &UUI_PlayerBag::Open_SkillBooks_Style0);
		this->M_SkillBookButtons[2]->OnClicked.AddDynamic(this, &UUI_PlayerBag::Open_SkillBooks_Style1);
		this->M_SkillBookButtons[3]->OnClicked.AddDynamic(this, &UUI_PlayerBag::Open_SkillBooks_Style2);
		//添加对应样式
		this->M_SkillBookDefStyle = { FString(TEXT("T_PB_22")),FString(TEXT("T_PB_27")) ,FString(TEXT("T_PB_29")) ,FString(TEXT("T_PB_31")) };
		this->M_SkillBookClickStyle = { FString(TEXT("T_PB_21")),FString(TEXT("T_PB_26")) ,FString(TEXT("T_PB_28")) ,FString(TEXT("T_PB_30")) };


		//---------------------------------------------------------------------------------------------------------------------
		//    背包道具选择区域（装备，防御卡，材料）
		//---------------------------------------------------------------------------------------------------------------------
		//添加背包选项卡-对应图片
		this->M_BagTableDefImg = { FString(TEXT("66")),FString(TEXT("71")) ,FString(TEXT("76")) };
		this->M_BagTableSelectImg = { FString(TEXT("63")),FString(TEXT("69")) ,FString(TEXT("74")) };
	}*/

	Super::Initialize();

	return true;
}

int32 UUI_PlayerBag::SetBagGridStyle(int32 M_Count, const TArray<FPlayerBagGird>& _BagGrid)
{

	this->SetImageBrush(this->M_BagCountImage_1, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_2, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_3, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_4, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");

	this->M_BagCount_Text_1 = FString();
	this->M_BagCount_Text_2 = FString();
	this->M_BagCount_Text_3 = FString();
	this->M_BagCount_Text_4 = FString();

	int32 GridCount = 150;

	if (M_Count > 0)
	{
		this->M_BagCount_Text_1 = FString::FromInt(_BagGrid[0].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_1, _BagGrid[0].M_GridImagePath);
		GridCount += _BagGrid[0].M_GridCount;
	}
	if (M_Count > 1)
	{
		this->M_BagCount_Text_2 = FString::FromInt(_BagGrid[1].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_2, _BagGrid[1].M_GridImagePath);
		GridCount += _BagGrid[1].M_GridCount;
	}
	if (M_Count > 2)
	{
		this->M_BagCount_Text_3 = FString::FromInt(_BagGrid[2].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_3, _BagGrid[2].M_GridImagePath);
		GridCount += _BagGrid[2].M_GridCount;
	}
	if (M_Count > 3)
	{
		this->M_BagCount_Text_4 = FString::FromInt(_BagGrid[3].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_4, _BagGrid[3].M_GridImagePath);
		GridCount += _BagGrid[3].M_GridCount;
	}
	return GridCount;
}

void UUI_PlayerBag::Clear()
{
	//清除定时器
	this->TimeLoad_PlayerBags.Stop();

	//界面索引
	this->M_Panel_Index = 0;
}

void UUI_PlayerBag::LoaderInit()
{
	//装备加载器
	//this->TimeLoad_Equipment_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Equipment_Manager_T"));
	//this->TimeLoad_Equipment_Manager->InitWidget_First(this->M_PlayerItemPanels[0], Cast<UScrollBox>(this->M_PlayerItemPanels[0]->GetParent()), 0.001f);
	//this->TimeLoad_Equipment_Manager->InitRange_Second(6, 7, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0), 100.f, 100.f);
	//this->TimeLoad_Equipment_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Equipment"));
	//this->TimeLoad_Equipment_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Equipment"));

	//卡片加载器
	/*this->TimeLoad_Cards_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Cards_Manager_T"));
	this->TimeLoad_Cards_Manager->InitWidget_First(
		this->M_PlayerItemPanels[1],
		Cast<UScrollBox>(this->M_PlayerItemPanels[1]->GetParent()),
		0.001f);
	this->TimeLoad_Cards_Manager->InitRange_Second(4, 6,
		9999999,
		120.f,
		150.f);
	this->TimeLoad_Cards_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Cards"));
	this->TimeLoad_Cards_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Cards"));*/

	//材料加载器
	//this->TimeLoad_Materials_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Materials_Manager_T"));
	//this->TimeLoad_Materials_Manager->InitWidget_First(this->M_PlayerItemPanels[2], Cast<UScrollBox>(this->M_PlayerItemPanels[2]->GetParent()), 0.001f);
	//this->TimeLoad_Materials_Manager->InitRange_Second(6, 7, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(2), 100.f, 100.f);
	//this->TimeLoad_Materials_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Materials"));
	//this->TimeLoad_Materials_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Materials"));

	//技能书加载器
	//this->TimeLoad_CardSkillBooks_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_SkillBooks_Manager_T"));
	//this->TimeLoad_CardSkillBooks_Manager->InitWidget_First(this->M_CardSkillBook_Panel, this->M_CardSkillBook_Scroll, 0.001f);
	//this->TimeLoad_CardSkillBooks_Manager->InitRange_Second(7, 1, 999, 790.f, 100.f);
	//this->TimeLoad_CardSkillBooks_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_CardSkillBooks"));
	//this->TimeLoad_CardSkillBooks_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_CardSkillBooks"));
}

UWidget* UUI_PlayerBag::WidgetCreate_Equipment(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagEquipmentGrid* Grid = CreateWidget<UUI_PlayerBagEquipmentGrid>(this->GetWorld(), UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagEquipmentGrid.BPUI_PlayerBagEquipmentGrid_C'")))));

	this->SetEquipmentAttribute(Grid, _Data, _Index);

	this->UpdateBagCount(0u, this->TimeLoad_Equipment_Manager->GetCount());

	return Grid;
}

void UUI_PlayerBag::WidgetRefresh_Equipment(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetEquipmentAttribute(Cast<UUI_PlayerBagEquipmentGrid>(_UWidget), _Data, _Index);

	this->UpdateBagCount(0u, this->TimeLoad_Equipment_Manager->GetCount());
}



//创建防御卡技能书道具界面
UWidget* UUI_PlayerBag::WidgetCreate_CardSkillBooks(UItemDataTable* _Data, int32 _Index)
{
	UUI_CardSkillTable* Skill_Table = CreateWidget<UUI_CardSkillTable>(this->GetWorld(), UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerCardSkills_Table.BPUI_PlayerCardSkills_Table_C'")))));

	Skill_Table->InitData(*((FSkillBookData*)(_Data->GetValue())));

	//Skill_Table->PlayAnimation_1();

	return Skill_Table;
}
//刷新防御卡技能书道具界面
void UUI_PlayerBag::WidgetRefresh_CardSkillBooks(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_CardSkillTable>(_UWidget)->InitData(*((FSkillBookData*)(_Data->GetValue())));

	//Cast<UUI_CardSkillTable>(_UWidget)->PlayAnimation_1();
}

void UUI_PlayerBag::LoadPlayerBags(UUI_PlayerBag* _Class, TArray<FEquipmentBase*>& _Items_, int32 _Index)
{

	auto SetBag = [&](UUI_PlayerBagEquipmentGrid* _Grid)->void {
		//让结构指向背包中的数据
		_Grid->M_EuipmentData = _Items_[_Index];

		//当前界面赋予格子
		_Grid->M_BagUI = _Class;

		UWidgetBase::SetButtonStyle(_Grid->GetButton(), _Items_[_Index]->ItemTexturePath.ToString());

		if (_Grid->GetButton()->OnClicked.IsBound())
			_Grid->GetButton()->OnClicked.Clear();

		//绑定事件
		FScriptDelegate Func, ClickAudio;
		Func.BindUFunction(_Grid, "AttachToBag");
		ClickAudio.BindUFunction(_Grid, "PlayOperateAudioDef");
		_Grid->GetButton()->OnClicked.Add(Func);
		_Grid->GetButton()->OnClicked.Add(ClickAudio);

		if (_Items_[_Index]->M_IsOverlap)
			_Grid->SetItemCountText(FString::FromInt(_Items_[_Index]->M_Count), true);
		else
			_Grid->SetItemCountText(FString(), false);
		};

	if (_Index < _Class->M_PlayerBagContent->GetAllChildren().Num())
	{
		_Class->M_PlayerBagContent->GetAllChildren()[_Index]->SetVisibility(ESlateVisibility::Visible);

		SetBag(Cast<UUI_PlayerBagEquipmentGrid>(_Class->M_PlayerBagContent->GetAllChildren()[_Index]));
	}
	else {
		UUI_PlayerBagEquipmentGrid* Grid = CreateWidget<UUI_PlayerBagEquipmentGrid>(this->GetWorld(), UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagEquipmentGrid.BPUI_PlayerBagEquipmentGrid_C'")))));

		SetBag(Grid);

		_Class->M_PlayerBagContent->AddChildToUniformGrid(Grid, (_Index / 3), _Index - 3 * (_Index / 3));
	}

	//更新背包格子数量显示  如果加载的个数到达最大值，剩余的则不会加载
	if (this->UpdateBagCount(this->M_Panel_Index, _Index + 1) == _Index + 1)
		this->TimeLoad_PlayerBags.Stop();
}

void UUI_PlayerBag::LoadCardSkillBookTabs(ESkillBookTipCategory M_ESkillBookTipCategory)
{
	//设置分类选项
	this->M_CardSkillBooksCategory = M_ESkillBookTipCategory;

	//重置索引
	this->M_FItemData_Show_CardSkillBooks.Empty();

	//分类加载(如果同类则显示)
	for (auto& LBook : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
	{
		if (LBook.M_TabCategory == this->M_CardSkillBooksCategory)
			this->M_FItemData_Show_CardSkillBooks.Emplace(LBook);
	}

	this->TimeLoad_CardSkillBooks_Manager->UpdateDatatable(this->M_FItemData_Show_CardSkillBooks);
	this->TimeLoad_CardSkillBooks_Manager->SetLoadItemMaxCount(this->M_FItemData_Show_CardSkillBooks.Num());
	this->TimeLoad_CardSkillBooks_Manager->SetCurrentPage(0);
	this->TimeLoad_CardSkillBooks_Manager->SetResetScrollBoxOffset();
	this->TimeLoad_CardSkillBooks_Manager->ContinueRun();
}


void UUI_PlayerBag::LoadCardSkillBookTabs_DefCategory()
{
	this->LoadCardSkillBookTabs(this->M_CardSkillBooksCategory);

	switch (this->M_CardSkillBooksCategory)
	{
	case ESkillBookTipCategory::S_Normal:; break;
	case ESkillBookTipCategory::S_Style_1:; break;
	case ESkillBookTipCategory::S_Style_2:; break;
	case ESkillBookTipCategory::S_Style_3:; break;
	}
}

void UUI_PlayerBag::BagGridButton_BuyGrid(uint8 _Panel_Index, uint8 _Gird_Index, int32 _Price)
{
	FItemPrice Price;
	Price.M_ItemMoneyType = 1u;
	Price.M_ItemPrice = _Price;
	Price.M_ItemType = EItemType::E_BagGrid;
	Price.M_ItemMoneyTypeName = FString(TEXT("礼券"));

	//调出支付界面
	UUI_ShopPayTip* PayPanel = CreateWidget<UUI_ShopPayTip>(
		this,
		UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(
			FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_BuyItem_Tip.UI_BuyItem_Tip_C'")))
		));
	PayPanel->M_ItemDescribe = FString(TEXT("开启格子之后,你可以装备一个背包用于扩充你的存储格子"));
	PayPanel->M_ItemMoneyType = FString(TEXT("礼券"));
	PayPanel->M_ItemPirceText = FString(FString::FromInt(_Price));
	PayPanel->M_ItemName = FString(TEXT("开启背包格子第") + FString::FromInt(_Gird_Index) + TEXT("格"));
	PayPanel->M_EShopPayTipBuyType = EShopPayTipBuyType::E_DefShopBuy;
	PayPanel->M_FItemPrice = Price;
	//关闭输入
	PayPanel->GetEditableTextBox()->SetIsEnabled(false);
	PayPanel->GetInputTextButton()->SetIsEnabled(false);
	//得到对应界面索引
	PayPanel->GetEditableTextBox()->SetText(FText::FromString(FString::FromInt(_Panel_Index)));
	PayPanel->AddToViewport();
}


void UUI_PlayerBag::UseCurrentGrid(uint8 _PanelIndex, uint8 _BagNumber, int32 _Price)
{
	TArray<FPlayerBagGird>& Grid = this->LoadBagGridData(_PanelIndex);
	if (Grid.Num() > _BagNumber - 1)
	{
		//当前背包网格名称
		this->M_BagVectorName = FString(TEXT("PanelGridBag_")) + FString::FromInt(_PanelIndex) + FString::FromInt(_BagNumber);
		this->M_BagNumber = _BagNumber;

		//从背包中获取已经装备的背包
		TArray<FPlayerBagGird*> BagGrid = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetUseBagArrays();

		FString BagName;

		//从背包组中获取
		for (auto Bag : BagGrid)
		{
			if (Bag->M_VectorName.Equals(this->M_BagVectorName))
			{
				BagName = Bag->M_BagName;
				FPlayerBagGirdGroup::ClearData(*Bag);
			}
		}

		//从背包中寻找背包名称并且取消使用
		for (FEquipmentBase& Euipment : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
		{
			if (Euipment.ItemName.Equals(BagName) && Euipment.M_Used)
			{
				Euipment.M_Used = false;
				if (_PanelIndex == 0) this->LoadItemsData();
				if (_PanelIndex == 1) this->LoadCardData();
				if (_PanelIndex == 2) this->LoadMaterialData();
				return;
			}
		}

		//打开背包选择界面
		this->M_PlayerBagContent->ClearChildren();

		//背包道具缓冲区
		TArray<FEquipmentBase*> M_PlayerBagBuffer;

		for (FEquipmentBase& _Items_ : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
		{
			//加载背包数据
			if (_Items_.M_EquipmentType == EEquipment::E_Bag && !_Items_.M_Used)
				M_PlayerBagBuffer.Emplace(&_Items_);
		}


		for (auto Panel : this->M_PlayerBagContent->GetAllChildren())
		{
			Panel->SetVisibility(ESlateVisibility::Collapsed);
		}


		this->TimeLoad_PlayerBags.SetClass(this, &UUI_PlayerBag::LoadPlayerBags);
		this->TimeLoad_PlayerBags.SetDataList(M_PlayerBagBuffer);
		this->TimeLoad_PlayerBags.Run(0.001f);

		this->M_PlayerBagSelectPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		//判断前面的格子是否购买
		if (Grid.Num() == _BagNumber - 1)
			//调出支付界面
		{
			this->BagGridButton_BuyGrid(_PanelIndex, _BagNumber, _Price);
		}
	}
}

void UUI_PlayerBag::BagGridButton_1_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 1, 500);
}

void UUI_PlayerBag::BagGridButton_2_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 2, 1000);
}

void UUI_PlayerBag::BagGridButton_3_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 3, 2000);
}

void UUI_PlayerBag::BagGridButton_4_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 4, 5000);
}

TArray<FPlayerBagGird>& UUI_PlayerBag::LoadBagGridData(uint8 _PanelNumber)
{
	//判断当前格子是否拥有
	FPlayerBagGirdGroup& Grid = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

	switch (_PanelNumber)
	{
	case 0: return Grid.M_EquipmentGrid;
	case 2: return Grid.M_MaterialGrid;
	}
	return Grid.M_CardGrid;
}

void UUI_PlayerBag::LoadItemsData()
{
	//移除所有界面
	this->Clear();

	this->M_Panel_Index = 0;

	UGameSystemFunction::WidgetSelect(this->M_PlayerItemPanels, ESlateVisibility::Visible, 0);

	for (auto Panel : this->M_PlayerItemPanels[0]->GetAllChildren())
	{
		Panel->SetVisibility(ESlateVisibility::Collapsed);
	}

	this->TimeLoad_Equipment_Manager->UpdateDatatable(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);
	this->TimeLoad_Equipment_Manager->SetLoadItemMaxCount(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0));
	this->TimeLoad_Equipment_Manager->ContinueRun();
}

void UUI_PlayerBag::LoadItemsDataEnd(UUI_PlayerBag* _bag)
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("---------------玩家武器数据存储(开始)---------------"));
		//打印武器库索引
		for (const auto& Datas : UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("武器名称：") + Datas.ItemName + TEXT("武器ID：") + FString::FromInt(Datas.M_ItemID));
		}
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("---------------玩家武器数据存储(结束)---------------"));

		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("---------------玩家宝石数据存储(开始)---------------"));
		//打印武器库索引
		for (const auto& Datas : UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("宝石名称：") + Datas.ItemName + TEXT("宝石ID：") + FString::FromInt(Datas.M_ItemID));
		}
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("---------------玩家宝石数据存储(结束)---------------"));
	}

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("装备道具刷新完成")));
}




void UUI_PlayerBag::UpdateShowCoinText()
{
	this->M_Bag_Coin0_Count_Text = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0);
	this->M_Bag_Coin1_Count_Text = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_1);
	this->M_Bag_Coin2_Count_Text = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2);
}

uint8 UUI_PlayerBag::GetPanel_Index()
{
	return this->M_Panel_Index;
}

void UUI_PlayerBag::CloseSelectBagPanel()
{
	this->M_PlayerBagSelectPanel->SetVisibility(ESlateVisibility::Hidden);

	if (this->GetPanel_Index() == 0) this->LoadItemsData();
	if (this->GetPanel_Index() == 1) this->LoadCardData();
	if (this->GetPanel_Index() == 2) this->LoadMaterialData();
}

void UUI_PlayerBag::OpenRemovePanel()
{
	//初始化界面UI
	if (this->M_UUI_PlayerBagRemove_Panels.Num() != 3)
	{
		//绑定的函数
		FString LBindFunc[3] = { TEXT("SearchBag_Equipment"),TEXT("SearchBag_Card"),TEXT("SearchBag_Materials") };

		//初始化界面
		for (int32 i = 0; i < 3; i++)
		{
			this->M_UUI_PlayerBagRemove_Panels.Emplace(CreateWidget<UUI_PlayerBagRemove>(this, UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagRemoveItems.BP_PlayerBagRemoveItems_C'"))))));
		}

		//初始化界面数据据
		for (int32 i = 0; i < 3; i++)
		{
			//初始化背包UI
			this->M_UUI_PlayerBagRemove_Panels[i]->M_UIBag = this;
			//添加代理函数
			FScriptDelegate CallFunc;
			CallFunc.BindUFunction(this->M_UUI_PlayerBagRemove_Panels[i], *LBindFunc[i]);
			this->M_UUI_PlayerBagRemove_Panels[i]->GetSearchButton()->OnClicked.Add(CallFunc);
			//初始化加载器
			this->M_UUI_PlayerBagRemove_Panels[i]->InitLoader(i);
		}
	}

	//加载显示界面
	auto LoadWidget = [&](TArray<UUI_PlayerBagRemove*>& _UUI_PlayerBagRemove_Panels, int32 _Index) {
		_UUI_PlayerBagRemove_Panels[_Index]->SearchBag(_Index);
		_UUI_PlayerBagRemove_Panels[_Index]->AddToViewport();
		};

	//根据当前界面索引显示对应界面
	switch (this->GetPanel_Index())
	{
	case 0:LoadWidget(this->M_UUI_PlayerBagRemove_Panels, this->GetPanel_Index()); break;
	case 1:LoadWidget(this->M_UUI_PlayerBagRemove_Panels, this->GetPanel_Index()); break;
	case 2:LoadWidget(this->M_UUI_PlayerBagRemove_Panels, this->GetPanel_Index()); break;
	}
}

void UUI_PlayerBag::AddBagGrid(int32 _PanelNumber, int32 _BagNumber, TArray<FPlayerBagGird>& _Grid, FEquipmentBase* _Data)
{
	//将当前数据标记为已经使用
	_Data->M_Used = true;

	//加载数据表
	UDataTable* BagData = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/EquipmentBagData.EquipmentBagData'"));
	TArray<FTableRowBase*> BagDatas;
	BagData->GetAllRows("List", BagDatas);

	for (auto Items : BagDatas)
	{
		if ((*(FEquipment_Bag_Data*)(Items)).M_FEquipment.ItemName.Equals(_Data->ItemName))
		{
			_Grid[_BagNumber - 1].M_GridCount = (*(FEquipment_Bag_Data*)(Items)).M_FEquipment.M_ContentCount;
			_Grid[_BagNumber - 1].M_GridImagePath = _Data->ItemTexturePath.ToString();
			_Grid[_BagNumber - 1].M_PanelNumber = _PanelNumber;
			_Grid[_BagNumber - 1].M_BagNumber = _BagNumber - 1;
			_Grid[_BagNumber - 1].M_VectorName = this->M_BagVectorName;
			_Grid[_BagNumber - 1].M_BagName = _Data->ItemName;
			break;
		}
	}

	this->UpdateBagCount(_PanelNumber, 0);
}

int32 UUI_PlayerBag::UpdateBagCount(uint8 _UpdatePanel, int32 Count)
{
	//FPlayerBagGirdGroup& Bag = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

	//获取玩家购买的背包个数最多4个
	int32 _Count = this->LoadBagGridData(_UpdatePanel).Num();

	//获取背包总容量
	int32 _GridCount = this->SetBagGridStyle(_Count, this->LoadBagGridData(_UpdatePanel));

	//判断装备栏区域的ID是否需要更新
	if (_UpdatePanel == 0)
	{
		UFVMGameInstance::GetPlayerStructManager_Static()->UpdateEquipID(_GridCount);
	}

	//更新文字
	this->M_BagTotalCount_Text_1 = FString(FString::FromInt(Count) + " / " + FString::FromInt(_GridCount));

	//返回总容量
	return _GridCount;
}

void UUI_PlayerBag::CloseBagPanel()
{

	this->Clear();

	//保存游戏
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关闭背包界面")));

	this->RemoveFromParent();
}

void UUI_PlayerBag::SetEquipmentAttribute(class UUI_PlayerBagEquipmentGrid* _Grid, UItemDataTable* _Items_, int32 _Index)
{
	//设置名称
	_Grid->SetItemName(((FEquipmentBase*)(_Items_->GetValue()))->ItemName);
	//设置等级[空字符串，默认隐藏等级]
	_Grid->SetItemGrade("");
	//设置按钮显示图片
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), ((FEquipmentBase*)(_Items_->GetValue()))->ItemTexturePath.ToString());
	//取消按钮的事件绑定
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	/*----------------------------ID号判断----------------------------*/
	//如果ID等于-1【当前道具】
	if (((FEquipmentBase*)(_Items_->GetValue()))->M_ItemID == -1)
	{
		//申请ID号
		UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(
			*((FEquipmentBase*)(_Items_->GetValue()))
		);
	}

	/*----------------------------ID号判断----------------------------*/
	_Grid->M_EuipmentData = ((FEquipmentBase*)(_Items_->GetValue()));
	_Grid->M_BagUI = this;

	//是否可以重叠
	if (((FEquipmentBase*)(_Items_->GetValue()))->M_IsOverlap)
	{
		_Grid->SetItemCountText(FString::FromInt(((FEquipmentBase*)(_Items_->GetValue()))->M_Count), true);
	}
	else
	{
		_Grid->SetItemCountText(FString(), false);
	}

	//是否被使用
	if (((FEquipmentBase*)(_Items_->GetValue()))->M_Used)
	{
		_Grid->GetButton()->SetIsEnabled(false);
	}
	else {
		_Grid->GetButton()->SetIsEnabled(true);
	}


	//物品描述
	FString LDescribe;

	//更具类型做出选择
	switch (((FEquipmentBase*)(_Items_->GetValue()))->M_EquipmentType)
	{
	case EEquipment::E_Gift:
	{
		_Grid->M_BagUI = this;
		UWidgetBase::ButtonDelegateBind(_Grid->GetButton(), _Grid, "CreateNewGiftBox");
	}break;
	case EEquipment::E_Bag: {
		//如果在使用则关闭按钮
		if (!((FEquipmentBase*)(_Items_->GetValue()))->M_Used)
		{
			UGameSystemFunction::AddString(LDescribe, _Grid->M_EuipmentData->ItemDescrible);
			UGameSystemFunction::AddString(LDescribe, TEXT("\n\n背包使用方法：(可以点击下方的背包装备栏，即可装备此背包)\n\n (如果背包栏未解锁,点击灰色的锁即可购买解锁背包栏)"));
			UWidgetBase::ButtonDelegateBind(_Grid->GetButton(), _Grid, "ShowText");
		}
	}break;
	case EEquipment::E_PlayerEquipment: {
		UGameSystemFunction::AddString(LDescribe, _Grid->M_EuipmentData->ItemDescrible);
		UWidgetBase::ButtonDelegateBind(_Grid->GetButton(), _Grid, "ShowPlayerSuit");
	}break;
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper:
	{
		UGameSystemFunction::AddString(LDescribe, _Grid->M_EuipmentData->ItemDescrible);


		if (((FEquipmentBase*)(_Items_->GetValue()))->M_EquipmentType == EEquipment::E_PlayerWeaponFirst)
		{
			UGameSystemFunction::AddString(LDescribe, TEXT("\n\n武器类型：主武器"));
		}
		else if (((FEquipmentBase*)(_Items_->GetValue()))->M_EquipmentType == EEquipment::E_PlayerWeaponSecond)
		{
			UGameSystemFunction::AddString(LDescribe, TEXT("\n\n武器类型：副武器"));
		}
		else {
			UGameSystemFunction::AddString(LDescribe, TEXT("\n\n武器类型：超级武器"));
		}

		//追加字符串
		UGameSystemFunction::AddString(LDescribe, TEXT("\n\n道具使用方法：你可以选择装备到你的角色，当游戏开始的时候，就会得到展现"));

		//得到武器数据
		int32 LID = -1;
		if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(_Items_->GetValue()))->M_ItemID, LID) != -1)
		{
			//获取具体数据（武器）
			FPlayerWeaponBase* const Local = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID];

			UGameSystemFunction::AddString(LDescribe, TEXT("\n\n宝石："));
			for (const auto& Data : Local->M_WeaponGem)
			{
				if (Data.M_CurrentWeaponGemEnable && !Data.M_WepaonGemName.Equals(TEXT("")))
				{
					UGameSystemFunction::AddString(LDescribe,
						TEXT("\n【") + Data.M_WepaonGemName + TEXT("】等级：") + FString::FromInt(Data.M_WeaponGemGrade));
				}
			}
		}


		//绑定武器装备
		UWidgetBase::ButtonDelegateBind(_Grid->GetButton(), _Grid, "ShowPlayerWeapon");
	}break;
	case EEquipment::E_WeaponGem:
	{
		UGameSystemFunction::AddString(LDescribe, _Grid->M_EuipmentData->ItemDescrible);
		UGameSystemFunction::AddString(LDescribe, TEXT("\n\n宝石使用方法：(可以去合成屋的宝石强化进行提升宝石的等级)\n\n (或者武器开槽镶嵌中镶嵌宝石，让武器拥有宝石的能力)"));

		//设置等级
		_Grid->SetItemGrade(UGameSystemFunction::GetItemGradePath(_Grid->M_EuipmentData->M_EquipmentGrade));

		UWidgetBase::ButtonDelegateBind(_Grid->GetButton(), _Grid, "ShowText");
	}break;
	default:
	{
		UGameSystemFunction::AddString(LDescribe, _Grid->M_EuipmentData->ItemDescrible);
	}
	break;
	}

	//设置物品描述
	_Grid->SetItemDescribe(LDescribe);

	//绑定音效
	FScriptDelegate ClickAudio;
	ClickAudio.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(ClickAudio);

	//_Grid->PlayAnimation_1();
}





UWidget* UUI_PlayerBag::WidgetCreate_Cards(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_BAGUINAME, TEXT("Card"))
	);

	this->SetCardAttribute(M_TempCardGrid, _Data, _Index);

	//this->UpdateBagCount(1u, this->TimeLoad_Cards_Manager->GetCount());

	return M_TempCardGrid;
}

void UUI_PlayerBag::WidgetRefresh_Cards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetCardAttribute(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);

	//this->UpdateBagCount(1u, this->TimeLoad_Cards_Manager->GetCount());
}

void UUI_PlayerBag::SetCardAttribute(UUI_PlayerBagCardGrid* _TempCardGrid, UItemDataTable* _Items_, int32 _Index)
{
	if (!IsValid(_TempCardGrid))
		return;

	_TempCardGrid->M_PlayerUIBag = this;
	_TempCardGrid->M_CardTexturePath = ((FItemCard*)_Items_->GetValue())->ItemTexturePath.ToString();
	_TempCardGrid->SetFItemCardData(((FItemCard*)_Items_->GetValue()));
	_TempCardGrid->UpdateButtonTexture(FString::FromInt(((FItemCard*)_Items_->GetValue())->M_CardPrice));

	//如果有绑定则清楚绑定
	if (_TempCardGrid->GetButtonClickEvent().IsBound())
		_TempCardGrid->GetButtonClickEvent().Clear();

	//绑定函数 点击之后显示细节面板
	FScriptDelegate CallFunc, ClickAudio;
	CallFunc.BindUFunction(_TempCardGrid, "ShowCardDetails");
	ClickAudio.BindUFunction(_TempCardGrid, "PlayOperateAudioDef");
	_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
	_TempCardGrid->GetButtonClickEvent().Add(ClickAudio);
}

void UUI_PlayerBag::LoadCardData()
{
	if (!IsValid(this->TimeLoad_Cards_Manager))
	{
		this->TimeLoad_Cards_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Cards_Manager_T"));
		this->TimeLoad_Cards_Manager->InitWidget_First(this->ItemList_Cards, this->ScrollBox_Cards, 0.001f);
		this->TimeLoad_Cards_Manager->InitRange_Second(3, 7, 9999999, 260.f, 230.f);
		this->TimeLoad_Cards_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Cards"));
		this->TimeLoad_Cards_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Cards"));
	}

	this->TimeLoad_Cards_Manager->UpdateDatatable(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card
	);
	this->TimeLoad_Cards_Manager->SetLoadItemMaxCount(9999999);
	this->TimeLoad_Cards_Manager->ContinueRun();
}


UWidget* UUI_PlayerBag::WidgetCreate_Materials(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(
		this, UGameSystemFunction::GetUserInterClassByName(UI_BAGUINAME, TEXT("Material"))
	);

	this->SetMaterialAttribute(Grid, _Data, _Index);

	return Grid;
}
void UUI_PlayerBag::WidgetRefresh_Materials(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMaterialAttribute(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index);
}

void UUI_PlayerBag::SetMaterialAttribute(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _Items_, int32 _Index)
{
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), ((FMaterialBase*)(_Items_->GetValue()))->ItemTexturePath.ToString());
	int32 CurCount = ((FMaterialBase*)(_Items_->GetValue()))->M_Count;
	_Grid->UpdateMaterialsShowCount(FString::FromInt(CurCount > 9999 ? 9999 : CurCount));

	auto LDelegateBind = [&](UUI_PlayerBagMaterialGrid* _LGrid, UObject* DelegateClass, const FString& _DelegateFuncName) {
		//赋值数据
		_LGrid->SetMaterialData(((FMaterialBase*)(_Items_->GetValue())));
		_LGrid->SetUI_PlayerBag(this);

		if (_LGrid->GetButton()->OnClicked.IsBound())
			_LGrid->GetButton()->OnClicked.Clear();

		FScriptDelegate Add, ClickAudio;
		Add.BindUFunction(DelegateClass, *_DelegateFuncName);
		ClickAudio.BindUFunction(_LGrid, "PlayOperateAudioDef");
		_LGrid->GetButton()->OnClicked.Add(Add);
		_LGrid->GetButton()->OnClicked.Add(ClickAudio);
		};

	//根据材料类型添加对应界面
	if (((FMaterialBase*)(_Items_->GetValue()))->M_MaterialType == EMaterialType::E_CardSkillBook)
	{
		LDelegateBind(_Grid, _Grid, TEXT("UseCardSkillBook"));
	}
	else {
		LDelegateBind(_Grid, _Grid, TEXT("ShowInformation"));
	}
}

void UUI_PlayerBag::LoadMaterialData()
{
	if (!IsValid(this->TimeLoad_Materials_Manager))
	{
		this->TimeLoad_Materials_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Materials_Manager_T"));
		this->TimeLoad_Materials_Manager->InitWidget_First(this->ItemList_Materials, this->ScrollBox_Materials, 0.001f);
		this->TimeLoad_Materials_Manager->InitRange_Second(5, 10, 99999, 180.f, 140.f);
		this->TimeLoad_Materials_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Materials"));
		this->TimeLoad_Materials_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Materials"));
	}
	this->TimeLoad_Materials_Manager->UpdateDatatable(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material
	);
	this->TimeLoad_Materials_Manager->SetLoadItemMaxCount(99999);
	this->TimeLoad_Materials_Manager->ContinueRun();
}

