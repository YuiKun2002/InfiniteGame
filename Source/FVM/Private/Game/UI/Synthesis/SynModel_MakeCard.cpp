// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_MakeCard.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "GameSystem/TaskSubsystem.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"
#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/UniformGridPanel.h>
#include <Components/Image.h>


void FMakeCardBlueprintData::ShowStyle(USynModel_MakeCard* Class)
{
	//配方的显示
	if (this->PlayerBagIndex == -1)
	{
		/*UWidgetBase::SetButtonStyle(
			Class->GetBlueprintButt(),
			TEXT("")
		);*/

		Class->GetBlueprintButt()->SetVisibility(ESlateVisibility::Collapsed);

		for (const auto& BMGrid : Class->GetBlueprintMaterialGrid())
		{
			BMGrid.Reset();
		}
		return;
	}
	else {
		Class->GetBlueprintButt()->SetVisibility(ESlateVisibility::Visible);
		//设置外观
		UWidgetBase::SetButtonStyle(
			Class->GetBlueprintButt(),
			this->HeadPath
		);

		//绑定取消配方的委托
		Class->GetBlueprintButt()->OnClicked.Clear();
		Class->GetBlueprintButt()->OnClicked.AddDynamic(
			Class,
			&USynModel_MakeCard::CancelSelectBlueprint
		);
		Class->GetBlueprintButt()->OnClicked.AddDynamic(
			Class->GetPlayerSynthesis(), &UWidgetBase::PlayOperateAudioDef);
	}

	//对材料的显示与隐藏

	int32 i = 0;
	for (const auto& BMGrid : Class->GetBlueprintMaterialGrid())
	{

		if (this->Materials[i].Visible == ESlateVisibility::Collapsed)
		{
			BMGrid.Hidden();
			i++;
			continue;
		}

		if (this->Materials[i].PlayerBagIndex != -1)
		{
			BMGrid.Reset();
			BMGrid.SetImgHead(this->Materials[i].HeadPath, COLOR_WHITE);
		}
		else {
			BMGrid.Reset();
			BMGrid.SetImgHead(this->Materials[i].HeadPath, COLOR_DARK);
		}

		i++;
	}
}

void FMakeCardBlueprintData::CancelBlueprint()
{
	this->PlayerBagIndex = -1;

	if (IsValid(this->Grid))
	{
		this->Grid->GetButton()->SetIsEnabled(true);
	}
}

void FMakeCardBlueprintData::SetBlueprintMaterials(const TArray<FString>& CurMaterials)
{
	//重置材料
	for (auto& CurData : this->Materials)
	{
		CurData.Visible = ESlateVisibility::Collapsed;
		CurData.PlayerBagIndex = -1;
	}

	//设置新材料
	int32 i = 0;
	for (const FString& CurData : CurMaterials)
	{
		//安全检查，上限3个
		if (i > 2)
		{
			break;
		}

		//设置材料数据
		this->Materials[i].BluepName = CurData;

		FSoftObjectPath ImgPath;
		UItemBaseStruct::GetTextureResource(
			FCString::Atoi(*CurData),
			ImgPath
		);
		this->Materials[i].HeadPath = ImgPath.ToString();

		this->Materials[i].Visible = ESlateVisibility::SelfHitTestInvisible;
		this->Materials[i].PlayerBagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByID(FCString::Atoi(*CurData));

		i++;
	}
}

const TArray<FMakeCardBlueprintMaterialData>& FMakeCardBlueprintData::GetBlueprintMaterials()
{
	return this->Materials;
}

void FMakeCardBlueprintData::AddNewBlueprint(const FCardBlueprint& BlueprintData, UUI_PlayerBagMaterialGrid* BGrid, int32 BUIGridIndex, USynModel_MakeCard* BClass)
{
	////原始数据
	//FCardBlueprint _CardBlueprint;
	//if (!UMaterialBaseStruct::GetMaterialSourceData<FCardBlueprint>(
	//	BlueprintName,
	//	_CardBlueprint,
	//	EMaterialType::E_Blueprint
	//))
	//{
	//	UWidgetBase::CreateTipWidget(FString(TEXT("[") + BlueprintName + TEXT("]查询失败!")));
	//	return;
	//}

	this->Bp = BlueprintData;

	TArray<FString> IDs;
	for (const FString& D : BlueprintData.M_Materials)
	{
		if (!D.IsEmpty())
		{
			IDs.Emplace(D);
		}
	}


	//配方出错
	if (IDs.Num() < 2)
	{
		return;
	}


	this->Grid = BGrid;
	this->UIGridIndex = BUIGridIndex;



	this->TargetCardName = BlueprintData.M_SynthesisCardName;
	this->TargetCardType = BlueprintData.M_SynthesisType;

	//图片
	this->HeadPath = BlueprintData.ItemTexturePath.ToString();
	//设置必要数据【当前配方的ID】
	this->BluepName = FString::FromInt(BlueprintData.M_ItemID);
	//需要合成的目标卡片
	this->TargetCardID = FString::FromInt(BlueprintData.M_ItemID);
	//设置配方的ID，从背包得到位置，检索玩家背包是否存在配方
	this->PlayerBagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByID(BlueprintData.M_ItemID);
	//设置消耗材料的ID
	this->SetBlueprintMaterials(IDs);

	//刷新外观
	this->ShowStyle(BClass);
}

void FMakeCardBlueprintMaterialGrid::Init(UUI_PlayerSynthesis* Class, const FString& BgName, const FString& HeadName, const FString& HeadPath)
{
	this->Bg = Class->GetWidgetComponent<UImage>(Class, BgName);
	this->Head = Class->GetWidgetComponent<UImage>(Class, HeadName);
	this->CurHeadPath = HeadPath;
}

void FMakeCardBlueprintMaterialGrid::SetImgHead(const FString& HeadPath, const FVector& Color) const
{
	UWidgetBase::SetImageBrush(this->Head, HeadPath, Color);
}

void FMakeCardBlueprintMaterialGrid::Reset() const
{
	this->Bg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->Head->SetVisibility(ESlateVisibility::Collapsed);
	//UWidgetBase::SetImageBrush(this->Head, this->CurHeadPath);
}

void FMakeCardBlueprintMaterialGrid::Hidden() const
{
	this->Bg->SetVisibility(ESlateVisibility::Collapsed);
	this->Head->SetVisibility(ESlateVisibility::Collapsed);
}


void FMakeCardSpicesData::SetSelectSpices(FMakeCardSpicesData NewData)
{
	this->Grid = NewData.Grid;
	this->MakeCardGrade = NewData.MakeCardGrade;
	this->PlayerBagIndex = NewData.PlayerBagIndex;
	this->SpicesName = NewData.SpicesName;
	this->SpicesData = NewData.SpicesData;
}

void FMakeCardSpicesData::CancelSelectSpices(USynModel_MakeCard* Class)
{
	this->PlayerBagIndex = -1;

	//还原样式
	/*UWidgetBase::SetButtonStyle(
		Class->GetSpicesButt(),
		TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_18.T_PS_18'")
	);*/
	Class->GetSpicesButt()->SetVisibility(ESlateVisibility::Collapsed);
	//解除绑定
	Class->GetSpicesButt()->OnClicked.Clear();
	//重新加载香料区
	Class->LoadSpicesToMakeCard(
		{ FMaterialsSerachTypeBind(EMaterialType::E_Spices,{"AddSynthesisSpicesSlot"}) }, FMaterialsSerachKeyWordsIgnore()
	);
}

void FMakeCardSpicesData::Use()
{

}

bool FMakeCardSpicesData::bSelectSpices()
{
	return this->PlayerBagIndex != -1 ? true : false;
}

int32 FMakeCardSpicesData::GetMakeCardGrade()
{
	return this->MakeCardGrade;
}






void USynModel_MakeCard::InitializeBySynthesis(UUI_PlayerSynthesis* Class)
{
	this->PlayerSynthesis = Class;

	//初始化香料的加载界面
	this->ScrollBox_Spice = this->PlayerSynthesis->GetWidgetComponent<UScrollBox>(
		this->PlayerSynthesis, TEXT("ScrollBox_UniformMater"));
	this->UniformGridPanel_Spice = this->PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(
		this->PlayerSynthesis,
		TEXT("Items_Spice_UniformMater")
	);

	//初始化配方，配方原材料加载界面
	this->M_Materials_Box = this->PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(
		this->PlayerSynthesis, TEXT("Items_Materials"));
	this->M_UScrollBox_MakeCard_Material = this->PlayerSynthesis->GetWidgetComponent<UScrollBox>(
		this->PlayerSynthesis, TEXT("ScrollBox_Materials"));

	/*
	this->M_Cards_Box = this->PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(this->PlayerSynthesis, "Cards_Box");
	this->M_UScrollBox_MakeCard = this->PlayerSynthesis->GetWidgetComponent<UScrollBox>(this->PlayerSynthesis, "MakeCard_ScrollBox");

	//初始化卡片制作加载器
	this->M_UItemLoadManager_MakeCard = NewObject<UItemLoadManager>(this, TEXT("MakeCardLoad"));
	this->M_UItemLoadManager_MakeCard->InitWidget_First(this->M_Cards_Box, this->M_UScrollBox_MakeCard);
	this->M_UItemLoadManager_MakeCard->InitRange_Second(3, 7, 800, 120.f, 150.f);
	this->M_UItemLoadManager_MakeCard->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMakeCard"));
	this->M_UItemLoadManager_MakeCard->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMakeCard"));
	*/

	//初始化格子【配方材料】
	FMakeCardBlueprintMaterialGrid Grid1;
	Grid1.Init(this->PlayerSynthesis, TEXT("Image_1"), TEXT("Material_0"),
		TEXT(""));

	FMakeCardBlueprintMaterialGrid Grid2;
	Grid2.Init(this->PlayerSynthesis, TEXT("Image_2"), TEXT("Material_2"),
		TEXT(""));

	FMakeCardBlueprintMaterialGrid Grid3;
	Grid3.Init(this->PlayerSynthesis, TEXT("Image"), TEXT("Material_1"),
		TEXT(""));

	this->BlueprintMaterialGrid.Append({ Grid1,Grid2,Grid3 });
	//初始化配方按钮
	this->Blueprint_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Main_Material_Butt"));
	//香料按钮
	this->Spices_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Material_3_Butt"));
	//卡片制作按钮
	this->MakeCard_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Butt_MakeCard"));
	this->MakeCard_Butt->OnClicked.AddDynamic(this, &USynModel_MakeCard::MakeCard);

}

void USynModel_MakeCard::WidgetReset()
{
	//取消对香料的选择
	this->CancelSelectSpices();
	//取消对配方的选择
	this->CancelSelectBlueprint();
	//检测
	this->CheckMakeCard();
}

void USynModel_MakeCard::WidgetResetLoadData()
{
	this->LoadMaterialsToMakeCard();
	//this->LoadCardsToMakeCard();

	this->LoadSpicesToMakeCard(
		{ FMaterialsSerachTypeBind(EMaterialType::E_Spices,{"AddSynthesisSpicesSlot"}) }, FMaterialsSerachKeyWordsIgnore()
	);
}

/*
//创建界面-卡片制作-卡片显示区域
UWidget* USynModel_MakeCard::WidgetCreate_InitMakeCard(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagCardGrid> LocalUIClass = Cast<UClass>(
		UAssetManager::GetStreamableManager().LoadSynchronous(
			FSoftClassPath(FString(
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")
			))));

	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this->PlayerSynthesis, LocalUIClass.Get());

	Grid->SetUIIndex(_Index);

	//this->SetMakeCardPanelData(Grid, _Data, _Index);

	return Grid;
}

*/

//更新界面-卡片制作-卡片显示区域
//void USynModel_MakeCard::WidgetRefresh_UpdateMakeCard(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
//{
	//this->SetMakeCardPanelData(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);
//}

/*
//卡片制作界面-设置卡片UI界面【显示，一般不需要】
void USynModel_MakeCard::SetMakeCardPanelData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _CardData, int32 _Index)
{
	//设置材料数据
	_Grid->SetFItemCardData(((FItemCard*)(_CardData->GetValue())));
	_Grid->M_CardTexturePath = ((FItemCard*)(_CardData->GetValue()))->ItemTexturePath.ToString();
	_Grid->UpdateButtonTexture(FString::FromInt(((FItemCard*)(_CardData->GetValue()))->M_CardPrice));
	//_Grid->PlayBeginAnim();

	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}

*/


//创建界面-卡片制作-防御卡合成配方材料显示区域
UWidget* USynModel_MakeCard::WidgetCreate_InitMakeCard_Material(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this->PlayerSynthesis,
		UGameSystemFunction::GetUserInterClassByName(UI_LABSUINAME, TEXT("Material")));
	this->SetMakeCard_Material_PanelData(Grid, _Data, _Index);
	return Grid;
}

//更新界面-卡片制作-防御卡合成配方材料显示区域【显示，一般不需要】
void USynModel_MakeCard::WidgetRefresh_UpdateMakeCard_Material(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMakeCard_Material_PanelData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index);
}

void USynModel_MakeCard::OnLoadBlueprint(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index)
{
	if (this->BlueprintData.PlayerBagIndex != -1 && this->BlueprintData.UIGridIndex == _Index)
	{
		_Grid->GetButton()->SetIsEnabled(false);
	}
}

//卡片制作界面-设置材料配方，配方材料UI界面
void USynModel_MakeCard::SetMakeCard_Material_PanelData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index)
{
	//启用按钮
	_Grid->GetButton()->SetIsEnabled(true);
	//设置材料数据
	_Grid->SetMaterialData(_CardData->GetTransValue<FMaterialBase>());
	//设置材料索引
	_Grid->SetIndex(_Index);
	//设置材料个数
	int32 CurCount = _Grid->GetMaterialData()->M_Count;
	//设置显示个数
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));
	//设置材料按钮
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), _Grid->GetMaterialData()->ItemTexturePath.ToString());
	//清除材料的事件绑定并且恢复选择状态
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}
	//绑定事件(卡片制作材料（配方）)
	if (_Grid->GetMaterialData()->M_MaterialType == EMaterialType::E_Blueprint)
	{
		_Grid->SetUI_PlayerSynthesis(this->PlayerSynthesis);
		this->OnLoadBlueprint(_Grid, _CardData, _Index);
		FScriptDelegate AddFunc;
		AddFunc.BindUFunction(_Grid, "AddSynthesisBlueprint");
		_Grid->GetButton()->OnClicked.Add(AddFunc);
	}
	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}

//加载材料-卡片制作界面（防御卡配方-配方材料-卡片制作界面）
void USynModel_MakeCard::LoadMaterialsToMakeCard()
{
	if (!IsValid(this->M_UItemLoadManager_MakeCard_Materials))
	{
		//初始化卡片制作-材料区域-加载器
		this->M_UItemLoadManager_MakeCard_Materials = NewObject<UItemLoadManager>(this, TEXT("MakeCard_Material_Load"));
		this->M_UItemLoadManager_MakeCard_Materials->InitWidget_First(
			this->M_Materials_Box,
			this->M_UScrollBox_MakeCard_Material
		);
		this->M_UItemLoadManager_MakeCard_Materials->InitRange_Second(5, 5, 800, 132.f, 132.f);
		//this->M_UItemLoadManager_MakeCard_Materials->SetMainScrollTransfrom(FVector2D(0.f, 8.f));
		//this->M_UItemLoadManager_MakeCard_Materials->SetItemMargin(FVector2D(5.f, 15.f));
		this->M_UItemLoadManager_MakeCard_Materials->OnCreateItem.BindUFunction(this,
			TEXT("WidgetCreate_InitMakeCard_Material"));
		this->M_UItemLoadManager_MakeCard_Materials->OnRefreshItem.BindUFunction(this,
			TEXT("WidgetRefresh_UpdateMakeCard_Material"));
	}

	//材料数据
	TArray<FMaterialBase*> Items;
	UGameSystemFunction::GetMaterialsArrayByType
	(
		UFVMGameInstance::GetFVMGameInstance()->
		GetPlayerStructManager()->
		M_PlayerItems_Material,
		{ EMaterialType::E_Blueprint,EMaterialType::E_CardSynthesisMaterial },
		Items
	);
	this->M_UItemLoadManager_MakeCard_Materials->UpdateDatatable(Items);
	this->M_UItemLoadManager_MakeCard_Materials->SetLoadItemMaxCount(Items.Num());
	this->M_UItemLoadManager_MakeCard_Materials->ContinueRun();
}
////加载卡片-卡片制作界面（防御卡-卡片制作界面）
//void USynModel_MakeCard::LoadCardsToMakeCard()
//{
//	this->M_UItemLoadManager_MakeCard->UpdateDatatable(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card);
//	this->M_UItemLoadManager_MakeCard->SetLoadItemMaxCount(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1));
//	this->M_UItemLoadManager_MakeCard->ContinueRun();
//}
//加载香料-卡片制作界面（材料区域-香料加载-卡片制作界面）
void USynModel_MakeCard::LoadSpicesToMakeCard(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords)
{
	//this->PlayerSynthesis->LoadMaterials({ FMaterialsSerachTypeBind(EMaterialType::E_Spices,{"AddSynthesisSpicesSlot"}) });
	if (!IsValid(this->ItemLoadManager_Spice))
	{
		this->ItemLoadManager_Spice = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Spice_Make"));
		this->ItemLoadManager_Spice->InitWidget_First(this->UniformGridPanel_Spice, this->ScrollBox_Spice);
		this->ItemLoadManager_Spice->InitRange_Second(1, 4, 800, 142.f, 132.f, true);
		this->ItemLoadManager_Spice->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMaterial"));
		this->ItemLoadManager_Spice->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMaterial"));
	}

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

		this->ItemLoadManager_Spice->UpdateDatatable(TargetArrays);
		this->ItemLoadManager_Spice->SetLoadItemMaxCount(TargetArrays.Num());
	}
	else {
		this->ItemLoadManager_Spice->UpdateDatatable(TempArrays);
		this->ItemLoadManager_Spice->SetLoadItemMaxCount(TempArrays.Num());
	}

	this->ItemLoadManager_Spice->SetCurrentPage(0);
	this->ItemLoadManager_Spice->SetResetScrollBoxOffset();
	this->ItemLoadManager_Spice->ContinueRun();
}

//创建-材料区域-材料UI界面
UWidget* USynModel_MakeCard::WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this->PlayerSynthesis,
		UGameSystemFunction::GetUserInterClassByName(UI_LABSUINAME, TEXT("Material"))
	);
	this->SetMaterialsData(Grid, _Data, _Index, this->M_BindFunctionName_Materials);

	return Grid;
}
//刷新-材料区域-材料UI界面
void USynModel_MakeCard::WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMaterialsData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index, this->M_BindFunctionName_Materials);
}
//卡片制作界面-设置（香料，四叶草，等等）UI界面
void USynModel_MakeCard::SetMaterialsData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index, const TArray<FMaterialsSerachTypeBind>& _BindFuncName)
{
	//设置材料索引
	_Grid->SetIndex(_Index);
	//设置材料按钮表示可以选择
	_Grid->GetButton()->SetIsEnabled(true);
	//设置材料的数量
	int32 CurCount = _Grid->GetMaterialData()->M_Count;
	//设置显示上限
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));
	//设置材料数据
	_Grid->SetMaterialData(_CardData->GetTransValue<FMaterialBase>());
	//设置材料的外观
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), _Grid->GetMaterialData()->ItemTexturePath.ToString());

	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	//绑定函数
	for (auto& LBind : _BindFuncName)
	{
		if (_Grid->GetMaterialData()->M_MaterialType == LBind.M_Type)
		{
			//设置指向合成屋的UI
			_Grid->SetUI_PlayerSynthesis(this->PlayerSynthesis);
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

UButton* USynModel_MakeCard::GetSpicesButt()
{
	return this->Spices_Butt;
}

FMakeCardSpicesData& USynModel_MakeCard::GetSpicesData()
{
	return this->SpicesData;
}

void USynModel_MakeCard::CancelSelectSpices()
{
	this->SpicesData.CancelSelectSpices(this);
}

FString USynModel_MakeCard::GetCurrentCardName()
{
	return this->CardName;
}

void USynModel_MakeCard::CancelSelectBlueprint()
{
	this->BlueprintData.CancelBlueprint();
	this->GetBlueprintButt()->OnClicked.Clear();
	this->BlueprintData.ShowStyle(this);
	this->CheckMakeCard();
}

void USynModel_MakeCard::MakeCard()
{
	if (this->BlueprintData.PlayerBagIndex == -1)
	{
		return;
	}

	////香料
	//int32 TargetCardGrade = 0;
	//if (this->SpicesData.PlayerBagIndex != -1)
	//{
	//	TargetCardGrade = this->SpicesData.MakeCardGrade;
	//}

	if (this->SpicesData.PlayerBagIndex == -1)
	{
		this->PlayerSynthesis->OnSelectMakeCardRequest(
			this,
			TEXT("-1"),
			this->GetBlueprintData().Bp.BagID
		);
	}
	else {
		this->PlayerSynthesis->OnSelectMakeCardRequest(
			this,
			this->SpicesData.SpicesData.BagID,
			this->GetBlueprintData().Bp.BagID
		);
	}



	//查询背包空间
	/*if (
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetBagNum(1)
		==
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetBagMaxCount(1))
	{
		UWidgetBase::CreateTipWidget(TEXT("背包空间不足"));
		return;
	}*/

	////支付金币200
	//if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0))
	//{
	//	UWidgetBase::CreateTipWidget(TEXT("金币不足"));
	//	return;
	//}

	////香料
	//int32 TargetCardGrade = 0;
	//if (this->SpicesData.PlayerBagIndex != -1)
	//{
	//	TargetCardGrade = this->SpicesData.MakeCardGrade;
	//}

	////制作卡片
	////获取制作成功的卡片数据
	//FItemCard _Card;
	//if (!UCardBaseStruct::SearchCardFromDataTable(
	//	this->BlueprintData.TargetCardName, _Card, true, this->BlueprintData.TargetCardType, TargetCardGrade
	//))
	//{
	//	UWidgetBase::CreateTipWidget(
	//		TEXT("制作失败!") + FString(TEXT("目标卡片：") + this->BlueprintData.TargetCardName) +
	//		FString(TEXT("目标等级：") + FString::FromInt(TargetCardGrade))
	//	);

	//	return;
	//}
	////赋予等级
	//_Card.M_CardGrade = TargetCardGrade;

	////添加到背包【末尾或者已经存在的卡片后面】
	//int32 PlayIndex = -1;
	//for (
	//	auto CardPP = UFVMGameInstance::GetFVMGameInstance()->
	//	GetPlayerStructManager()->M_PlayerItems_Card.CreateConstIterator();
	//	CardPP;
	//	++CardPP
	//	)
	//{
	//	if ((*CardPP).ItemName.EqualTo(_Card.ItemName))
	//	{
	//		PlayIndex = CardPP.GetIndex();
	//		break;
	//	}
	//}

	//if (PlayIndex == -1)
	//{
	//	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(_Card);
	//}
	//else
	//{
	//	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->
	//		M_PlayerItems_Card.EmplaceAt(PlayIndex, _Card);
	//}



	////消耗香料
	//if (this->SpicesData.PlayerBagIndex != -1)
	//{
	//	UFVMGameInstance::GetPlayerStructManager_Static()->UseMaterial(
	//		this->SpicesData.PlayerBagIndex,
	//		this->SpicesData.SpicesName,
	//		5, false
	//	);
	//}

	////消耗材料
	//for (const auto& CurMaterial : this->BlueprintData.GetBlueprintMaterials())
	//{
	//	if (CurMaterial.PlayerBagIndex != -1)
	//	{
	//		UFVMGameInstance::GetPlayerStructManager_Static()->UseMaterial(
	//			CurMaterial.PlayerBagIndex,
	//			CurMaterial.BluepName,
	//			1, false
	//		);
	//	}
	//}

	////消耗配方【并且保持数据】
	//UFVMGameInstance::GetPlayerStructManager_Static()->UseMaterial(
	//	this->BlueprintData.PlayerBagIndex,
	//	this->BlueprintData.BluepName,
	//	1, true
	//);


	////查询香料是否存在【重新选择或者取消】
	//this->SpicesData.PlayerBagIndex =
	//	UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->SpicesData.SpicesName);
	//if (
	//	//香料无效
	//	this->SpicesData.PlayerBagIndex == -1
	//	||
	//	//香料数量不足
	//	(this->SpicesData.PlayerBagIndex != -1
	//		&&
	//		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[this->SpicesData.PlayerBagIndex].M_Count < 5
	//		))
	//{
	//	//取消对香料的选择
	//	this->CancelSelectSpices();
	//}




	//this->CardName = _Card.ItemName.ToString();

	////添加历史记录
	//UPlayerRecord::AddDayCardMakeCount(this->CardName);

	////执行任务
	//UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);


	//UWidgetBase::CreateTipWidget(TEXT("制作成功!"));


	//检测制作
	//this->CheckMakeCard();
	//重新加载列表
	//this->WidgetResetLoadData();
}

bool USynModel_MakeCard::CheckBlueprint()
{
	//查询配方【重新选择或者取消】
	this->BlueprintData.PlayerBagIndex =
		UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByID(this->BlueprintData.Bp.M_ItemID);
	//不存在则取消选择
	if (this->BlueprintData.PlayerBagIndex == -1)
	{
		this->CancelSelectBlueprint();

		return false;
	}
	else {
		////重新添加配方
		//this->BlueprintData.AddNewBlueprint(
		//	this->BlueprintData.Bp,
		//	this->BlueprintData.Grid,
		//	this->BlueprintData.UIGridIndex,
		//	this
		//);

		this->PlayerSynthesis->OnSelectCardBlueprintDataRequest(
			this,
			this->BlueprintData.Bp,
			this->BlueprintData.Grid,
			this->BlueprintData.UIGridIndex
		);

		return true;
	}

	return false;
}

void USynModel_MakeCard::CheckSpices()
{
	//查询香料是否存在【重新选择或者取消】
	this->SpicesData.PlayerBagIndex =
		UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByID(this->SpicesData.SpicesData.M_ItemID);
	if (
		//香料无效
		this->SpicesData.PlayerBagIndex == -1
		||
		//香料数量不足
		(this->SpicesData.PlayerBagIndex != -1
			&&
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[
				this->SpicesData.PlayerBagIndex
			].M_Count < 5
			))
	{
		//取消对香料的选择
		this->CancelSelectSpices();
	}
}

void USynModel_MakeCard::CheckMakeCard()
{
	this->GetMakeCardButt()->SetIsEnabled(true);

	bool bResult = false;

	//配方检测
	if (this->BlueprintData.PlayerBagIndex == -1)
	{
		bResult = true;
	}

	//材料检测
	if (this->BlueprintData.PlayerBagIndex != -1)
	{
		for (const auto& CurMa : this->BlueprintData.GetBlueprintMaterials())
		{
			if (CurMa.Visible == ESlateVisibility::Collapsed)
			{
				continue;
			}

			if (CurMa.PlayerBagIndex == -1)
			{
				bResult = true;
				break;
			}
		}
	}

	if (bResult)
	{
		this->GetMakeCardButt()->SetIsEnabled(false);
	}
}

UButton* USynModel_MakeCard::GetMakeCardButt()
{
	return this->MakeCard_Butt;
}

UButton* USynModel_MakeCard::GetBlueprintButt()
{
	return this->Blueprint_Butt;
}

FMakeCardBlueprintData& USynModel_MakeCard::GetBlueprintData()
{
	return this->BlueprintData;
}

void USynModel_MakeCard::AddNewBlueprint(const FCardBlueprint& LBlueprintData, UUI_PlayerBagMaterialGrid* BGrid, int32 BUIGridIndex, USynModel_MakeCard* BClass)
{
	FString Log;
	for (const auto& Data : LBlueprintData.M_Materials)
	{
		Log += TEXT("配方材料ID：") + Data;
	}
	UGameSystemFunction::FVMLog(__FUNCTION__,
		TEXT("配方ID：") + LBlueprintData.BagID +
		TEXT("		材料集合：") +
		Log
	);
	this->GetBlueprintData().AddNewBlueprint(LBlueprintData, BGrid, BUIGridIndex, this);
}

UUI_PlayerSynthesis* USynModel_MakeCard::GetPlayerSynthesis()
{
	return this->PlayerSynthesis;
}

const TArray<FMakeCardBlueprintMaterialGrid>& USynModel_MakeCard::GetBlueprintMaterialGrid() const
{
	return this->BlueprintMaterialGrid;
}
