// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>

bool UFlipbookBaseComponent::PlayAnimationEnd(const float& DeltaTime)
{
	const float TimelineLength = this->GetFlipbookLength();
	const float EffectiveDeltaTime = DeltaTime * (this->bReversePlayback ? (-this->PlayRate) : (this->PlayRate));

	float NewPosition = this->AccumulatedTime + EffectiveDeltaTime;

	if (EffectiveDeltaTime > 0.0f)
	{
		if (NewPosition > TimelineLength)
		{

			if (this->OnAnimationPlayEnd.IsBound())
			{
				this->OnAnimationPlayEnd.Execute();
			}

			if (this->OnDynamicAnimationEndDelegate.IsBound())
			{
				this->OnDynamicAnimationEndDelegate.Broadcast();
			}

			this->SetPlaybackPosition(0, true);

			return true;
		}
	}
	else
	{
		if (NewPosition < 0.0f)
		{

			if (this->OnAnimationPlayEnd.IsBound())
			{
				this->OnAnimationPlayEnd.Execute();
			}

			if (this->OnDynamicAnimationEndDelegate.IsBound())
			{
				this->OnDynamicAnimationEndDelegate.Broadcast();
			}


			this->SetPlaybackPosition(0, true);

			return true;
		}
	}

	this->SetPlaybackPosition(NewPosition, true);

	return false;
}

void UFlipbookBaseComponent::SetPlayAnimationOnce(UPaperFlipbook* _PlayAnim, UPaperFlipbook* _PlayEndAnim)
{
	this->M_UPaperFlipbook_PlayEnd = _PlayEndAnim;
	this->_bPlayOnce = true;
	this->SetFlipbook(_PlayAnim);
}

void UFlipbookBaseComponent::SetPlayAnimation(UPaperFlipbook* _PlayAnim, bool _bInfluence_AnimOnce)
{
	//如果之前开启了单次动画并且当前动画不影响之前的单次动画
	if (this->_bPlayOnce && !_bInfluence_AnimOnce)
	{
		//如果首次动画没有播放完毕，只会更换单次切换目标的动画
		if (IsValid(_PlayAnim))
		{
			this->M_UPaperFlipbook_PlayEnd = _PlayAnim;
		}

		return;
	}

	//关闭单次相应，并且开启了单次动画，并且当前动画等于单次最后的动画，直接跳出
	if (!_bInfluence_AnimOnce && (this->_bPlayOnce && _PlayAnim == this->M_UPaperFlipbook_PlayEnd))
	{
		return;
	}
	else {
		//强行关闭单次动画
		if (_bInfluence_AnimOnce && IsValid(this->M_UPaperFlipbook_PlayEnd))
		{
			this->M_UPaperFlipbook_PlayEnd = _PlayAnim;
			this->SetFlipbook(_PlayAnim);
		}
		else {
			this->SetFlipbook(_PlayAnim);
		}
	}

	//如果单次动画开启，并且又可以影响动画
	if (this->_bPlayOnce && _bInfluence_AnimOnce)
	{
		this->_bPlayOnce = false;//单次动画切换失效
	}
}

void UFlipbookBaseComponent::SetAnimationPlayEndDestroy()
{
	this->IsPlayEndOnDestroy = true;
}

void UFlipbookBaseComponent::CancelAnimationPlayEndDestroy()
{
	this->IsPlayEndOnDestroy = false;
}

void UFlipbookBaseComponent::SetTranslucency(const int32& _layer)
{
	this->_Layer = _layer;
	this->SetTranslucentSortPriority(_layer);
}

int32 UFlipbookBaseComponent::GetTranslucency()
{
	return this->_Layer;
}

void UFlipbookBaseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//开启动画单次切换
	if (this->_bPlayOnce)
	{
		if (this->PlayAnimationEnd(DeltaTime))
		{
			this->_bPlayOnce = false;
			this->SetPlayAnimation(this->M_UPaperFlipbook_PlayEnd);
			this->M_UPaperFlipbook_PlayEnd = nullptr;
		}
	}
	else {
		if (this->PlayAnimationEnd(DeltaTime))
		{
			//播放完成
			if (this->IsPlayEndOnDestroy)
			{
				if (this->OnAnimationComplete.IsBound())
				{
					this->OnAnimationComplete.Execute();
				}

				if (this->OnDynamicAnimationEndDelegate.IsBound())
				{
					this->OnDynamicAnimationEndDelegate.Broadcast();
				}

				return;
			}
		}
	}

	this->CalculateCurrentFrame();
}