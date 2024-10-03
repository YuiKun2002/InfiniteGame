// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MouseTab/MouseViewSelectMousePanel.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseViewEditor.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>

bool UMouseViewSelectMouseItem::Initialize()
{
	Super::Initialize();

	this->CurButton = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("CurButtonItem"))));

	return true;
}

void UMouseViewSelectMouseItem::InitSelectMouseItem(UGameMapUI_MouseTab* MouseTab, UMouseViewSelectMousePanel* SelectMousePanel, const FMouse_Data& Data)
{
	this->CurSelectMousePanel = SelectMousePanel;
	this->CurMouseTab = MouseTab;
	this->MouseData = Data;

	FButtonStyle Style;
	Style.Normal.SetResourceObject(Data.M_Mouse.MouseHead.TryLoad());
	Style.Normal.TintColor = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Hovered.SetResourceObject(Data.M_Mouse.MouseHead.TryLoad());
	Style.Hovered.TintColor = FLinearColor(FVector(7.f, 7.f, 7.f));
	Style.Pressed.SetResourceObject(Data.M_Mouse.MouseHead.TryLoad());
	Style.Pressed.TintColor = FLinearColor(FVector(5.f, 5.f, 5.f));

	Style.Disabled.SetResourceObject(Data.M_Mouse.MouseHead.TryLoad());
	this->CurButton->SetStyle(Style);
}

void UMouseViewSelectMouseItem::Select()
{
	if (this->CurSelectMousePanel->CurSelectMouseItem == this)
	{
		this->Cancel();
		this->CurSelectMousePanel->OnCancel();

		return;
	}

	if (IsValid(this->CurSelectMousePanel->CurSelectMouseItem))
	{
		this->CurSelectMousePanel->CurSelectMouseItem->Cancel();
	}

	this->OnSelect(this->MouseData);

	this->CurSelectMousePanel->CurSelectMouseItem = this;

	FMouseConfigNode NewMouseNode;
	NewMouseNode.CurMouseName = this->MouseData.M_Mouse.M_MouseName;
	this->CurSelectMousePanel->CurUnifromMouseNodeSetting = NewMouseNode;
	this->CurSelectMousePanel->OnSelect(NewMouseNode);
}

void UMouseViewSelectMouseItem::Cancel()
{
	if (IsValid(this->CurSelectMousePanel->CurSelectMouseItem))
	{
		this->CurSelectMousePanel->CurSelectMouseItem = nullptr;
		this->CurSelectMousePanel->OnCancel();
	}

	this->OnCancel();
}

bool UMouseViewSelectMousePanel::Initialize()
{
	Super::Initialize();

	this->UniformGridPanel = Cast<UUniformGridPanel>(this->GetWidgetFromName(FName(TEXT("UniformGridPanel_124"))));

	return true;
}

void UMouseViewSelectMousePanel::InitMouseViewSelectMousePanel()
{
	//初始化老鼠数据表
	this->AllMouseDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"));

	//显示默认类型
	this->ShowType(TEXT("老鼠.陆地"));
}

void UMouseViewSelectMousePanel::ShowType(const FString& _Type)
{
	//折叠所有项
	for (auto Item : this->UniformGridPanel->GetAllChildren())
	{
		Item->SetVisibility(ESlateVisibility::Collapsed);
		(Cast<UMouseViewSelectMouseItem>(Item))->Cancel();
	}

	//获取数据
	TArray<FMouse_Data*> CurData;
	this->AllMouseDataTable->GetAllRows<FMouse_Data>(TEXT("AllMouseData"), CurData);

	//筛选符合条件的数据
	TArray<FMouse_Data> CurValidData;
	for (FMouse_Data* Data : CurData)
	{
		if (Data->M_Mouse.MouseCategory.GetTagName().ToString().Equals(_Type))
		{
			CurValidData.Emplace(*Data);
		}
	}

	for (int32 i = 0; i < CurValidData.Num(); i++)
	{
		if (i + 1 <= this->UniformGridPanel->GetAllChildren().Num())
		{
			//得到项
			UMouseViewSelectMouseItem* CurViewItem = Cast<UMouseViewSelectMouseItem>(this->UniformGridPanel->GetChildAt(i));
			CurViewItem->InitSelectMouseItem(GameMapUI_MouseViewEditor->GetMouseTab(), this, CurValidData[i]);
			CurViewItem->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			UMouseViewSelectMouseItem* CurViewItem = CreateWidget<UMouseViewSelectMouseItem>(this,
				LoadClass<UMouseViewSelectMouseItem>(nullptr,
					TEXT("WidgetBlueprint'/Game/Resource/BP/Data/MapData/MouseConfig/BPUI_MouseViewSelectItem.BPUI_MouseViewSelectItem_C'")));
			CurViewItem->InitSelectMouseItem(GameMapUI_MouseViewEditor->GetMouseTab(), this, CurValidData[i]);
			this->UniformGridPanel->AddChildToUniformGrid(CurViewItem, i / 3, i % 3);
		}
	}
}

void UMouseViewSelectMousePanel::SetNewMouseNode(FMouseConfigNode MouseNode)
{
	this->CurUnifromMouseNodeSetting.IgnoreRows = MouseNode.IgnoreRows;
}

FMouseConfigNode UMouseViewSelectMousePanel::GetMouseNode()
{
	return this->CurUnifromMouseNodeSetting;
}

UMouseViewSelectMouseItem* UMouseViewSelectMousePanel::GetMouseViewSelectMouseItem()
{
	return this->CurSelectMouseItem;
}
