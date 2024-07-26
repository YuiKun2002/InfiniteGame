// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/LogPanel.h"
#include "Data/GameLogSubsystem.h"
#include <Components/Image.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <Components/ScrollBox.h>
#include "Game/UI/LogItemPanel.h"
#include <Kismet/GameplayStatics.h>

bool ULogPanel::Initialize()
{
	Super::Initialize();

	//初始化窗口信息列表
	this->CurLogList = this->GetWidgetComponent<UVerticalBox>(this, TEXT("LogList"));
	this->CurScrollBox = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_46"));
	//初始化信息组件
	for (int32 i = 0; i < 7; i++)
	{
		ULogItemPanel* CurItem = nullptr;
		CurItem = CreateWidget<ULogItemPanel>(this, LoadClass<ULogItemPanel>(
			this,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/LogPanel/BP_LogItem.BP_LogItem_C'")
			));

		this->CurLogList->AddChildToVerticalBox(CurItem);
		CurItem->SetVisibility(ESlateVisibility::Collapsed);
		CurLogItems.Emplace(CurItem);
	}

	this->bCur = false;
	this->CurLogCount = 0;
	this->PrivetLogCount = 0;

	return true;
}

void ULogPanel::OpenCurrent()
{
	if (this->bCur)
	{
		return;
	}

	this->bCur = true;

	this->CurPage = 0;

	this->UpdateContent();
}

void ULogPanel::OpenPrivate()
{
	if (!this->bCur)
	{
		return;
	}

	this->bCur = false;

	this->CurPage = 0;

	this->UpdateContent();
}

void ULogPanel::UpdateContent()
{
	if (!IsValid(this->CurSystem))
	{
		this->CurSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UGameLogSubsystem>();
	}

	TArray<FString> CurContent;
	if (this->bCur)
	{
		this->CurSystem->GetCurrent(CurContent);
	}
	else {
		this->CurSystem->GetPrivate(CurContent);
	}

	int32 CurIndex = this->CurPage;
	int32 TargetIndex = CurIndex + 7;
	int32 ShowIndex = 0;
	for (; CurIndex < TargetIndex; CurIndex++)
	{
		if (CurIndex < CurContent.Num())
		{
			ULogItemPanel* CurPanel = Cast<ULogItemPanel>(this->CurLogList->GetChildAt(ShowIndex));
			CurPanel->SetText(CurContent[CurIndex]);
			CurPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			ULogItemPanel* CurPanel = Cast<ULogItemPanel>(this->CurLogList->GetChildAt(ShowIndex));
			CurPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
		ShowIndex++;
	}

	if (IsValid(this->CurScrollBox))
	{
		this->CurScrollBox->SetScrollOffset(this->CurLogList->GetDesiredSize().Y);
	}
}

void ULogPanel::LastPage()
{
	this->CurPage -= 7;
	if (this->CurPage < 0)
	{
		this->CurPage = 0;
	}

	this->UpdateContent();
}

void ULogPanel::NextPage()
{
	int32 CurNum = 0;
	if (this->bCur)
	{
		CurNum = this->CurSystem->GetCurrentCount();
	}
	else {
		CurNum = this->CurSystem->GetPrivateCount();
	}

	if (CurNum % 7 == 0)
	{
		return;
	}

	int32 MaxPageNum = CurNum / 7;
	this->CurPage += 7;
	if (this->CurPage > MaxPageNum * 7)
	{
		if (MaxPageNum != 0)
		{
			this->CurPage = MaxPageNum * 7;
		}
		else {
			this->CurPage = 0;
		}
	}

	this->UpdateContent();
}

void ULogPanel::EndPage()
{
	int32 CurNum = 0;
	if (this->bCur)
	{
		CurNum = this->CurSystem->GetCurrentCount();
		if (this->CurLogCount == CurNum)
		{
			//无需更新
			return;
		}
		else {
			this->CurLogCount = CurNum;
		}
	}
	else {
		CurNum = this->CurSystem->GetPrivateCount();
		if (this->PrivetLogCount == CurNum)
		{
			//无需更新
			return;
		}
		else {
			this->PrivetLogCount = CurNum;
		}
	}

	if (CurNum % 7 == 0)
	{
		return;
	}

	int32 MaxPageNum = CurNum / 7;
	if (MaxPageNum != 0)
	{
		this->CurPage = MaxPageNum * 7;
	}
	else {
		this->CurPage = 0;
	}

	this->UpdateContent();
}
