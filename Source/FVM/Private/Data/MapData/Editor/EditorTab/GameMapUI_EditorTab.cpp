// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/EditorTab/GameMapUI_EditorTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include "Data/CardData/MouseDataStruct.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

//---------------------------------------------------------------------------------------------------------------
// 编辑器选项卡组件：行列表项
//---------------------------------------------------------------------------------------------------------------

bool UFVMEditUI_EditRowTableItem::Initialize()
{
	Super::Initialize();

	this->Text_Title = Cast<UTextBlock>(this->GetWidgetFromName(FName(TEXT("RowNameText"))));

	return true;
}

void UFVMEditUI_EditRowTableItem::InitData(class UGameMapUI_EditorTab* _Class)
{
	this->Class = _Class;
}

void UFVMEditUI_EditRowTableItem::SetRowName(const FString& _RowName)
{
	this->RowName = FName(_RowName);

	if (this->Text_Title)
		Text_Title->SetText(FText::FromString(_RowName));
}



void UFVMEditUI_EditRowTableItem::Config()
{
	FString CurName = this->Class->GetGameMapEditor()->GetCurEditRowName().ToString();

	if (CurName.Equals(this->RowName.ToString()))
	{
		return;
	}

	this->Class->GetGameMapEditor()->SetEditorTitle(FText::FromName(this->RowName));
	this->Class->GetGameMapEditor()->InitCurEditConfig();
	this->Class->GetGameMapEditor()->LoadConfig();
}

bool UGameMapUI_EditorTab::Initialize()
{
	Super::Initialize();


	return true;
}

void UGameMapUI_EditorTab::InitEditorTab(UFVMEditUI_GameMapEdit* _Class)
{
	if (!this->FVMEditUI_GameMapEdit)
		this->FVMEditUI_GameMapEdit = _Class;

	this->EditorTab_ListViewPanel = Cast<UVerticalBox>(this->GetWidgetFromName(FName(TEXT("VerticalBox"))));
	this->EditorTab_NewRowInputText = Cast<UEditableTextBox>(this->GetWidgetFromName(FName(TEXT("EditorConfigRowName"))));
	this->EditorTab_NewRowSaveButt = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("EditorTabSaveConfig"))));
}

void UGameMapUI_EditorTab::InitEditorTabListItems()
{
	if (!this->EditorTab_ListViewPanel)
		return;

	int32 LocalIndex = 0;
	for (const FName& CurRowName : this->FVMEditUI_GameMapEdit->GetGameMapRowNames())
	{
		//判断列表数量
		if (LocalIndex >= this->EditorTab_ListViewPanel->GetAllChildren().Num())
		{
			//创建UI
			UFVMEditUI_EditRowTableItem* CurRow = CreateWidget<UFVMEditUI_EditRowTableItem>(
				this,
				LoadClass<UFVMEditUI_EditRowTableItem>(0,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/EditorTab/BP_EditorTabListItem.BP_EditorTabListItem_C'")
				)
			);
			//初始化
			CurRow->InitData(this);
			//设置数据
			CurRow->SetRowName(CurRowName.ToString());
			//添加一行数据
			this->EditorTab_ListViewPanel->AddChildToVerticalBox(CurRow);
		}
		else
		{
			//重新设置数据
			Cast<UFVMEditUI_EditRowTableItem>(this->EditorTab_ListViewPanel->GetChildAt(LocalIndex))
				->InitData(this);
			//初始化数据
			Cast<UFVMEditUI_EditRowTableItem>(this->EditorTab_ListViewPanel->GetChildAt(LocalIndex))
				->SetRowName(CurRowName.ToString());
		}

		LocalIndex++;
	}
}

UFVMEditUI_GameMapEdit* const UGameMapUI_EditorTab::GetGameMapEditor()
{
	return this->FVMEditUI_GameMapEdit;
}

void UGameMapUI_EditorTab::LoadRowDatas(FName RowName)
{

	//如果加载的行名称为空或者为相同名称则不会继续加载
	if (
		RowName.IsEqual(FName(TEXT("")))
		||
		this->FVMEditUI_GameMapEdit->GetCurEditRowName().IsEqual(RowName)
		)
	{
		return;
	}

	if (this->FVMEditUI_GameMapEdit->GetGameMapRowNames().Find(RowName) != INDEX_NONE)
	{
		//this->CurEditRowName = RowName;
		//this->CurEditData = *((FGameMapData*)(this->GameMapData->FindRow<FGameMapData>(RowName, TEXT("GameMapDatas"))));

		//初始化列表项
		//this->InitEditorTabListItems();
	}

}

void UGameMapUI_EditorTab::AddNewRow(FName NewRowName)
{
	//如果加载的行名称为空或者为相同名称则不会继续加载
	if (
		NewRowName.IsEqual(FName(TEXT("")))
		||
		this->FVMEditUI_GameMapEdit->GetCurEditRowName().IsEqual(NewRowName)
		)
	{
		return;
	}

	if (this->FVMEditUI_GameMapEdit->GetGameMapRowNames().Find(NewRowName) == INDEX_NONE)
	{
		//添加新行

		UMapDataStructAsset* Data = UMapDataStruct::CreateMapDataAsset(NewRowName.ToString());
		Data->Data.M_FLevelConfig.LevelName = NewRowName.ToString();

		FGameMapDataList DataList;
		DataList.MapDataTable = TSoftObjectPtr<UMapDataStructAsset>(Data->GetPathName());

		this->FVMEditUI_GameMapEdit->GetGameMapData()->AddRow(NewRowName, DataList);

		//刷新行
		this->FVMEditUI_GameMapEdit->InitGameMapListItems();
		this->InitEditorTabListItems();
	}
}

void UGameMapUI_EditorTab::EditAlienName(const FString& SourceName, const FString& TargetName)
{
	// Map DataTable 
	// DataTable'/Game/Resource/BP/Data/MapData/DT_GameMapDataList.DT_GameMapDataList'

	// Alien DataTable
	// DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'


	UDataTable* Map = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Resource/BP/Data/MapData/DT_GameMapDataList.DT_GameMapDataList'"));
	UDataTable* Alien = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"));

	TArray<FMouse_Data*> AlienList;
	Alien->GetAllRows<FMouse_Data>(TEXT("AlienLists"), AlienList);
	for (FMouse_Data* list : AlienList)
	{
		if (list->M_Mouse.M_MouseName.Equals(SourceName))
		{
			list->M_Mouse.M_MouseName = TargetName;
			break;
		}
	}

	//修改所有地图带有当前名称的外星人
	TArray<FGameMapDataList*> List;
	Map->GetAllRows<FGameMapDataList>(TEXT("maps"), List);
	for (FGameMapDataList* list : List)
	{
		UMapDataStructAsset* Aseet = list->MapDataTable.LoadSynchronous();
		if (IsValid(Aseet))
		{
			int32* ID = Aseet->Data.M_FMouseConfig.AllMouseListMap.Find(SourceName);
			if (ID)
			{
				int32 LID = *ID;
				Aseet->Data.M_FMouseConfig.AllMouseKeyListMap.Emplace(LID, TargetName);
				Aseet->Data.M_FMouseConfig.AllMouseListMap.Remove(SourceName);
				Aseet->Data.M_FMouseConfig.AllMouseListMap.Emplace(TargetName, LID);
			}
		}
	}

#if WITH_EDITOR
	UPackage* Package = FindPackage(nullptr,
		*FPackageName::FilenameToLongPackageName(Map->GetPathName())
	);
	if (Package)
	{
		Package->MarkPackageDirty();
		Package->SetDirtyFlag(true);
	}

	UPackage* PackageA = FindPackage(nullptr,
		*FPackageName::FilenameToLongPackageName(Alien->GetPathName())
	);
	if (PackageA)
	{
		PackageA->MarkPackageDirty();
		PackageA->SetDirtyFlag(true);
	}
#endif
}
