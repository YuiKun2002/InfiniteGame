// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MechaGiantAlien.h"
#include "GameSystem/Tools/GameSystemFunction.h"
void AMechaGiantAlien::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);

	if (this->bEnableAttackLine && !this->GetbShoot())
	{
		for (const FGunAlienCheckLineTraceBaseConfig& Config : this->CheckLineTraceConfig)
		{
			if (this->AddAttackLineFunc(
				UGameSystemFunction::GetCardCollisionBoxType(
					Config.CheckCardType
				), DeltaTime, true
			))
			{
				this->bEnableAttackLine = false;
				this->SetShootEnable(false);
				return;
			}
		}

	}
}

void AMechaGiantAlien::OnAnimationPlayEnd2(class UTrackEntry* Track)
{
	if (!this->GetMouseIsDeath())
	{
		this->AddAttackCardUpdate();

		if (!this->GetbIsAttack())
		{
			this->PlayMoveAnim();
			this->bEnableAttackLine = true;
			this->SetShootEnable(true);
		}
	}
}

void AMechaGiantAlien::PlayAttack2Anim()
{
	if (this->State != 3)
	{
		//攻击状态
		this->State = 3;

		//播放攻击动画
		UTrackEntry* Track = this->SetAnimation(0,
			this->M_DefAnim_Anim.AttackAnimDamageRes.GetDefaultObject()->GetCategoryName().ToString(),
			true);
		BINDANIMATION(Track, this, &AMechaGiantAlien::OnAnimationPlayEnd2);
		this->SetTrackEntry(Track);
	}
}

void AMechaGiantAlien::CheckCard()
{
	for (const FGunAlienCheckLineTraceBaseConfig& Config : this->CheckLineTraceConfig)
	{
		if (this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(
				Config.CheckCardType
			), 100.f, true
		))
		{
			this->bEnableAttackLine = false;
			this->SetShootEnable(false);
			return;
		}
	}

	Super::CheckCard();
}
