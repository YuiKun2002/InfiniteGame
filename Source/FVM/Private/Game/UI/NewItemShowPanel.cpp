// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/NewItemShowPanel.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include <Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>

bool UNewItemShowPanel::Initialize()
{
	Super::Initialize();

	this->ListScroll = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_21"));
	this->ListUniform = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_28"));

	return true;
}

void UNewItemShowPanel::Load(const TArray<FTaskItemStruct>& Data)
{
	this->Datas.Empty();
	this->Datas.Append(Data);

	//初始化任务显示
	if (!IsValid(this->TimeLoadManager))
	{
		this->TimeLoadManager = NewObject<UItemLoadManager>(this, TEXT("ItemTimeLoadManager_T"));
		this->TimeLoadManager->InitWidget_First(
			this->ListUniform,
			this->ListScroll,
			0.01f);
		this->TimeLoadManager->InitRange_Second(2, 5,
			9999,
			240.f, 300.f);
		this->TimeLoadManager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreateList"));
		this->TimeLoadManager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshList"));
	}

	this->TimeLoadManager->UpdateDatatable(this->Datas);
	this->TimeLoadManager->SetLoadItemMaxCount(this->Datas.Num());
	this->TimeLoadManager->ContinueRun();
}

UWidget* UNewItemShowPanel::WidgetCreateList(UItemDataTable* _Data, int32 _Index)
{
	UNewItemShow* NewUI = CreateWidget<UNewItemShow>(
		this, LoadClass<UNewItemShow>(nullptr,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BP_NewItemTip.BP_NewItemTip_C'"))
		);

	FTaskItemStruct* CurData = ((FTaskItemStruct*)(_Data->GetValue()));
	NewUI->Set(CurData->ItemName, CurData->ImagePath);

	return NewUI;
}

void UNewItemShowPanel::WidgetRefreshList(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UNewItemShow* NewUI = Cast<UNewItemShow>(_UWidget);
	FTaskItemStruct* CurData = ((FTaskItemStruct*)(_Data->GetValue()));
	NewUI->Set(CurData->ItemName, CurData->ImagePath);
}

bool UNewItemShow::Initialize()
{
	Super::Initialize();

	this->Name = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_42"));
	this->Bg = this->GetWidgetComponent<UImage>(this, TEXT("Image_71"));

	return true;
}

void UNewItemShow::Set(const FString& ItemName, const FSoftObjectPath& ImgPath)
{
	this->Name->SetText(FText::FromString(ItemName));
	UWidgetBase::SetImageBrush(this->Bg,
		ImgPath.ToString(),
		FVector(1.f), 1.f, true);
}
