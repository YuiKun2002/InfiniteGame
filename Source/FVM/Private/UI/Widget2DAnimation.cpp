// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "UI/Widget2DAnimation.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include <Components/Image.h>

UWidget2DAnimation* UWidget2DAnimation::MakeWidget2DAnimation(TSoftObjectPtr<UPaperFlipbook> FlipResource, UImage* ShowImage)
{
	UWidget2DAnimation* NewAnimObject = NewObject<UWidget2DAnimation>();
	NewAnimObject->NewFlipbook = FlipResource;
	NewAnimObject->ShowView = ShowImage;
	return NewAnimObject;
}

void UWidget2DAnimation::Update(const float& time)
{
	if (this->FlipAnim && this->ShowView)
	{
		UPaperSprite* pf = nullptr;

		auto AnimLength = this->FlipAnim->GetTotalDuration();
		if (TimeNow > AnimLength) {
			TimeNow -= AnimLength;
		}
		pf = this->FlipAnim->GetSpriteAtTime(TimeNow);
		TimeNow += time;

		if (pf) {
			FSlateBrush sb;
			sb.SetResourceObject(pf);
			this->ShowView->SetBrush(sb);
		}
	}
	else {
		if (!this->FlipAnim)
		{
			this->NewFlipbook.LoadSynchronous();
			this->FlipAnim = this->NewFlipbook.Get();
		}
	}
}
