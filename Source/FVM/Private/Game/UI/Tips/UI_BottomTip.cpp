// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Tips/UI_BottomTip.h"

void UUI_BottomTip::SetTitle(const FString& _Title)
{
	this->M_Title = _Title;
}

void UUI_BottomTip::PlayAnim()
{	
	this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

void UUI_BottomTip::SetTitleAndPlay(const FString& _Title)
{
	this->SetTitle(_Title);
	this->PlayAnim();
}

void UUI_BottomTip::RemoveThisWidget()
{
	this->RemoveFromViewport();
}

bool UUI_BottomTip::Initialize()
{
	if (!Super::Initialize())
		return false;



	return true;
}