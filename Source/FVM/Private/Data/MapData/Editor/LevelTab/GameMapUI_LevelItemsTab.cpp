// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/LevelTab/GameMapUI_LevelItemsTab.h"
#include "Data/MapData/Editor/FVMEditUI_GameMapEdit.h"
#include "Data/MapData/Editor/LevelTab/GameMapUI_LevelTab.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/UniformGridPanel.h>
#include "UI/WidgetBase.h"


void UGameMapUI_LevelItem::Init(class UGameMapUI_LevelItemsPanel* Class, const FString& CName, const FSoftObjectPath& CHeadPath)
{
	this->CurClass = Class;

	this->ShowHead = Cast<UImage>(this->GetWidgetFromName(TEXT("Image_83")));
	//初始化组件
	this->ShowTex = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("TextBlock_73")));
	//初始化组件
	this->ShowButTex = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("TextBlock_104")));
	//初始化组件
	this->But = Cast<UButton>(this->GetWidgetFromName(TEXT("Button_42")));

	this->Name = CName;
	this->HeadPath = CHeadPath;
	this->ShowTex->SetText(FText::FromString(this->Name));

	this->Update();

	this->But->OnClicked.AddDynamic(this, &UGameMapUI_LevelItem::Select);
}

void UGameMapUI_LevelItem::Select()
{
	FLevelConfig& CurData = this->CurClass->GetCurClass()->GetCurClass()->GetCurEditLevelData();

	FSoftObjectPath* Obj = CurData.LevelItems.Find(this->Name);
	if (Obj)
	{
		CurData.LevelItems.Remove(this->Name);
	}
	else {
		CurData.LevelItems.Emplace(
			this->Name,
			this->HeadPath
		);
	}

	this->Update();
}

void UGameMapUI_LevelItem::Update()
{
	//当前关卡配置数据
	FLevelConfig& CurData = this->CurClass->GetCurClass()->GetCurClass()->GetCurEditLevelData();
	FSoftObjectPath* Cur = CurData.LevelItems.Find(this->Name);
	if (Cur)
	{
		this->ShowButTex->SetText(FText::FromString(TEXT("取消")));
		this->ShowButTex->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f)));
		//设置图片
		UWidgetBase::SetImageBrush(
			this->ShowHead, this->HeadPath.ToString(), FVector(0.3f), 1.f, true, FVector(0.8f)
		);
	}
	else {
		this->ShowButTex->SetText(FText::FromString(TEXT("选择")));
		this->ShowButTex->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f)));
		//设置图片
		UWidgetBase::SetImageBrush(
			this->ShowHead, this->HeadPath.ToString(), FVector(1.f), 1.f, true, FVector(0.8f)
		);
	}
}

void UGameMapUI_LevelItemsTabButt::Init(class UGameMapUI_LevelItemsPanel* Class, const FString& Name, UDataTable* NewDataTable)
{
	//初始化组件
	this->ShowTextComp = Cast<UTextBlock>(this->GetWidgetFromName(TEXT("TextBlock_40")));
	//设置标题
	this->ShowTextComp->SetText(FText::FromString(Name));
	//设置数据表
	this->CurData = NewDataTable;
	this->CurClass = Class;
}

void UGameMapUI_LevelItemsTabButt::Show()
{
	this->CurClass->GetDataRange()->ClearChildren();

	int32 i = 0;
	for (const FName& CurRow : this->CurData->GetRowNames())
	{
		FItemBaseStructData* TCurData = this->CurData->FindRow<FItemBaseStructData>(CurRow, TEXT("CurData"));
		FItemBase NewData;
		if (TCurData && TCurData->GetSourceData(NewData))
		{

			//加载界面
			UGameMapUI_LevelItem* Cur =
				CreateWidget<UGameMapUI_LevelItem>(this,
					LoadClass<UGameMapUI_LevelItem>(0,
						TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/LevelItems/BP_LevelItem.BP_LevelItem_C'")));


			Cur->Init(this->CurClass, NewData.ItemName.ToString(), NewData.ItemTexturePath);
			this->CurClass->GetDataRange()->AddChildToUniformGrid(
				Cur, i / 8, i % 8
			);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("加载失败!"));
		}
		i++;
	}
}

void UGameMapUI_LevelItemsPanel::InitLevelTab(UGameMapUI_LevelItemsTab* Class)
{
	this->CurClass = Class;

	//初始化组件
	this->DataRange = Cast<UUniformGridPanel>(this->GetWidgetFromName(TEXT("DataRange")));
	this->DataList = Cast<UVerticalBox>(this->GetWidgetFromName(TEXT("TabList")));

	//初始化列表数据
	for (const auto& Data : this->Tabs)
	{
		//加载界面
		UGameMapUI_LevelItemsTabButt* Cur =
			CreateWidget<UGameMapUI_LevelItemsTabButt>(this,
				LoadClass<UGameMapUI_LevelItemsTabButt>(0,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/LevelItems/BP_LevelItemsSelectButt.BP_LevelItemsSelectButt_C'")));

		//加载对应数据
		if (Data.Value.LoadSynchronous())
		{
			UDataTable* CurTable = Data.Value.Get();

			Cur->Init(this, Data.Key, CurTable);

			//添加到UI
			this->DataList->AddChildToVerticalBox(Cur);
		}
	}
}

UGameMapUI_LevelItemsTab* UGameMapUI_LevelItemsPanel::GetCurClass()
{
	return this->CurClass;
}

UUniformGridPanel* UGameMapUI_LevelItemsPanel::GetDataRange()
{
	return this->DataRange;
}

void UGameMapUI_LevelItemsTab::InitLevelTab(UFVMEditUI_GameMapEdit* Class)
{
	this->CurClass = Class;
}

UFVMEditUI_GameMapEdit* UGameMapUI_LevelItemsTab::GetCurClass()
{
	return  this->CurClass;
}


