// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Tips/UI_ItemTitleTip.h"

bool UUI_ItemTitleTip::Initialize()
{
	if (!Super::Initialize())
		return false;





	return true;
}


void UUI_ItemTitleTip::SetShow(bool _bShow, const FString& _Title)
{
	if (_bShow && this->M_bHidd)
	{
		this->M_Title = _Title;
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