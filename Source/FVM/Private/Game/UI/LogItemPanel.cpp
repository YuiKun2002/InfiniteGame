// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/LogItemPanel.h"
#include <Components/RichTextBlock.h>

bool ULogItemPanel::Initialize()
{
	
	Super::Initialize();

	this->CurText = this->GetWidgetComponent<URichTextBlock>(this,TEXT("Content"));

	return true;
}

void ULogItemPanel::SetText(const FString& Content)
{
	if (IsValid(this->CurText))
	{
		this->CurText->SetText(FText::FromString(Content));
	}
}
