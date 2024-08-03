// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Tips/UI_ItemTitleTip.h"
#include "GameSystem/FVMGameInstance.h" 
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"

void UUI_ItemTitleTip::SetShow(bool _bShow, const FString& _Title)
{
	if (_bShow && this->M_bHidd)
	{
		this->M_Title = _Title;
		this->SetText(_Title);
		this->M_bHidd = false;
		this->AddToViewport(5);
		this->SetPadding(FMargin(-90.f, -60.f, 0.f, 0.f));
	}
	else {
		if (!_bShow)
		{
			this->M_bHidd = true;
			this->RemoveFromViewport();
		}
	}
}

void UUI_ItemTitleTip::CreateBaseTipWidget(const FString& _ShowTitle, bool _bShow)
{
	if (_bShow)
	{
		//获取鼠标位置
		FVector2D LPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(
			UFVMGameInstance::GetFVMGameInstance()->GetWorld()
		);
		//设置缩放
		LPosition *= UWidgetLayoutLibrary::GetViewportScale(UFVMGameInstance::GetFVMGameInstance()->GetWorld());
		//设置最终位置
		this->SetPositionInViewport(LPosition, true);
		//设置标题
		this->SetShow(true, _ShowTitle);
	}
	else {
		this->SetShow(false, TEXT(""));
	}
}
