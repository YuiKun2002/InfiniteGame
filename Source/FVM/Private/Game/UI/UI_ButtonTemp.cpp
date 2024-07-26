// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_ButtonTemp.h"
#include "Game/UI/UI_Notice.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

bool UUI_ButtonTemp::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_ButtonComp = GetWidgetComponent<UButton>(this, TEXT("Tab_Button"));
	this->M_TitleText = GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_37"));

	//绑定点击事件
	FScriptDelegate Add_Func;
	Add_Func.BindUFunction(this, TEXT("Show"));
	this->M_ButtonComp->OnClicked.Add(Add_Func);

	return true;
}

void UUI_ButtonTemp::SetTitle(const FString& _title)
{
	this->M_TitleText->SetText(FText::FromString(_title));
}

void UUI_ButtonTemp::SetIndex(int32 _index)
{
	this->M_Button_Index = _index;
}

void UUI_ButtonTemp::SetUINotice(UUI_Notice* _UUI_Notice)
{
	this->M_UUI_Notice = _UUI_Notice;
}

FString UUI_ButtonTemp::GetTitle()
{
	return this->M_TitleText->GetText().ToString();
}

int32 UUI_ButtonTemp::GetIndex()
{
	return this->M_Button_Index;
}

UButton* UUI_ButtonTemp::GetButton()
{
	return this->M_ButtonComp;
}

void UUI_ButtonTemp::Show()
{
	if (this->M_UUI_Notice)
		this->M_UUI_Notice->ClickTab(this->M_Button_Index);
}
