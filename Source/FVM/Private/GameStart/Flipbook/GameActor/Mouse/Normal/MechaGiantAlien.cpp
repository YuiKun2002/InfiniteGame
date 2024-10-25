﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MechaGiantAlien.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

void AMechaGiantAlien::OnAnimationPlayEnd2(class UTrackEntry* Track)
{
	if (!this->GetMouseIsDeath())
	{
		this->AddAttackCardUpdate();

		this->CheckCard();
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

	if (IsValid(this->GetCurrentAttackCard())
		&&
		this->GetCurrentAttackCard()->GetCurrentHP() > 0.f
		)
	{
		if (!this->GetbShootEnabled())
		{
			if (this->GetbIsAttack())
			{
				return;
			}
		}
	}

	for (const FGunAlienCheckLineTraceBaseConfig& Config : this->CheckLineTraceConfig)
	{
		if (this->AddAttackLineFunc(
			UGameSystemFunction::GetCardCollisionBoxType(
				Config.CheckCardType
			), 100.f, true
		))
		{
			this->MoveStop();
			this->PlayAttack2Anim();
			this->SetShootEnable(false);
			this->SetMouseATK(this->MeleeCombatATK);
			return;
		}
	}


	this->SetShootEnable(true);
	Super::CheckCard();
}
