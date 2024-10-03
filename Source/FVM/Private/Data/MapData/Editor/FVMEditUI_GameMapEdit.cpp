// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include "Data/MapData/Editor/LevelTab/GameMapUI_LevelTab.h"
#include "Data/MapData/Editor/MapTab/GameMapUI_MapTab.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseTab.h"
#include "Data/MapData/Editor/EditorTab/GameMapUI_EditorTab.h"
#include "Data/MapData/Editor/LevelTab/GameMapUI_LevelItemsTab.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#endif

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>



//---------------------------------------------------------------------------------------------------------------
// 编辑器
//---------------------------------------------------------------------------------------------------------------

bool UFVMEditUI_GameMapEdit::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化【显示当前正在编辑的关卡数据名称】
	this->Text_CurEditTitle = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("CurEditName"))));


	//初始化关卡配置选项
	this->GameMapUI_LevelTab = Cast<UGameMapUI_LevelTab>(this->GetWidgetFromName(FName(TEXT("BP_LevelTab"))));
	//初始化关卡掉落物配置选项
	this->GameMapUI_LevelItemsTab = Cast<UGameMapUI_LevelItemsTab>(this->GetWidgetFromName(FName(TEXT("BP_LevelItemsBut"))));
	//初始化地图配置选项
	this->GameMapUI_MapTab = Cast<UGameMapUI_MapTab>(this->GetWidgetFromName(FName(TEXT("BP_MapTab"))));
	//初始化老鼠配置选项
	this->GameMapUI_MouseTab = Cast<UGameMapUI_MouseTab>(this->GetWidgetFromName(FName(TEXT("BP_MouseTab"))));
	//初始化编辑器
	this->GameMapUI_EditorTab = Cast<UGameMapUI_EditorTab>(this->GetWidgetFromName(FName(TEXT("BP_EditorTab"))));

	//初始化数据表数据
	this->InitGameMapData();

	return true;
}

void UFVMEditUI_GameMapEdit::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UFVMEditUI_GameMapEdit::NativeDestruct()
{
	Super::NativeDestruct();

}

void UFVMEditUI_GameMapEdit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UFVMEditUI_GameMapEdit::InitGameMapData()
{
	//初始化标题
	this->SetEditorTitle(FText::FromString(TEXT("美味岛")));

	//加载数据表
	this->GameMapData = LoadObject<UDataTable>(
		this,
		TEXT("DataTable'/Game/Resource/BP/Data/MapData/DT_GameMapDataList.DT_GameMapDataList'")
	);

	//初始化列表数据
	this->InitGameMapListItems();

	//初始化当前正在编辑的数据
	this->CurEditRowName = FName();
	this->CurEditData = {};
}

void UFVMEditUI_GameMapEdit::InitGameMapListItems()
{
	//清空数据
	this->GameMapDataRowNames.Empty();
	this->GameMapDataRowNames = this->GameMapData->GetRowNames();
}

void UFVMEditUI_GameMapEdit::InitEditorConfig()
{
	/*
		关卡选项【初始化】
	*/
	this->GameMapUI_LevelTab->InitLevelTab(this);
	/*
		关卡掉落物选项【初始化】
	*/
	this->GameMapUI_LevelItemsTab->InitLevelTab(this);
	/*
		地图选项【初始化】
	*/
	this->GameMapUI_MapTab->InitMapTab(this);
	/*
		老鼠选项【初始化】
	*/
	this->GameMapUI_MouseTab->InitMouseTab(this);
	/*
		编辑器选项【初始化】
	*/
	//初始化编辑器选项
	this->GameMapUI_EditorTab->InitEditorTab(this);
	//初始化列表项
	this->GameMapUI_EditorTab->InitEditorTabListItems();
}

void UFVMEditUI_GameMapEdit::InitCurEditConfig()
{
	FGameMapDataList* const Cur = this->GameMapData->FindRow<FGameMapDataList>(this->CurEditRowName, TEXT("GameMapData"));
	UMapDataStructAsset* Data = Cur->MapDataTable.LoadSynchronous();
	if (IsValid(Data))
	{
		this->CurEditData = Data->Data;
	}
}

void UFVMEditUI_GameMapEdit::LoadConfig()
{
	this->GameMapUI_LevelTab->InitLevelTab(this);
	this->GameMapUI_MapTab->InitMapTab(this);
	this->GameMapUI_MouseTab->InitMouseTab(this);
}

void UFVMEditUI_GameMapEdit::SaveConfig()
{
	//获取关卡配置表
	FLevelConfig LevelConfig = this->GameMapUI_LevelTab->GetConfig();
	//获取地图配置表
	FGameMapStruct MapConfig = this->GameMapUI_MapTab->GetConfig();
	//获取老鼠配置表
	FMouseConfig MouseConfig = this->GameMapUI_MouseTab->GetConfig();

	/*得到总配置*/

	//覆盖关卡配置的数据
	this->CurEditData.M_FLevelConfig = LevelConfig;
	this->CurEditData.M_FGameMapStruct = MapConfig;
	this->CurEditData.M_FMouseConfig = MouseConfig;

	/*保存配置*/
	FGameMapDataList* const CurRow = this->GameMapData->FindRow<FGameMapDataList>(this->CurEditRowName, TEXT("GameMapData"));
	if (CurRow)
	{
		UMapDataStructAsset* Data = CurRow->MapDataTable.LoadSynchronous();
		if (IsValid(Data))
		{
			Data->Data = this->CurEditData;

		}
		UPackage* Package = FindPackage(nullptr,
			*FPackageName::FilenameToLongPackageName(CurRow->MapDataTable->GetPathName())
		);
		if (Package)
		{
			#if WITH_EDITOR
			Package->MarkPackageDirty();
			Package->SetDirtyFlag(true);
			#endif
		}
	}
	this->SaveCurPakage();
	//缓存配置
	this->LoadConfig();
}

UDataTable* const UFVMEditUI_GameMapEdit::GetGameMapData()
{
	return this->GameMapData;
}

const TArray<FName>& UFVMEditUI_GameMapEdit::GetGameMapRowNames()
{
	return this->GameMapDataRowNames;
}

const FName& UFVMEditUI_GameMapEdit::GetCurEditRowName()
{
	return this->CurEditRowName;
}

FGameMapData& UFVMEditUI_GameMapEdit::GetCurEditData()
{
	return this->CurEditData;
}


FLevelConfig& UFVMEditUI_GameMapEdit::GetCurEditLevelData()
{
	return this->GameMapUI_LevelTab->GetConfig();
}

void UFVMEditUI_GameMapEdit::SetEditorTitle(const FText& _Title)
{
	this->Text_CurEditTitle->SetText(_Title);
	this->CurEditRowName = FName(_Title.ToString());
}

