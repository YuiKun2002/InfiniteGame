// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Card/RangeATKCardActor.h"

const FItemCardRangeATK& ARangeATKCardActor::GetRangeATKCardData()
{
	return this->ItemCardRangeATK;
}

void ARangeATKCardActor::BeginPlay()
{
	//初始化必要数据
	this->ItemCardRangeATK = this->GetCardDataComponent()->GetCardRangeATKData(this->CardActor_Name);

	//设置基础攻击力
	this->SetATK(this->ItemCardRangeATK.M_CardATKBase);
	//设置当前攻击力
	this->SetCurATK(this->ItemCardRangeATK.M_CardATKBase);
	//设置攻击速度
	this->SetAttackSpeed(this->ItemCardRangeATK.M_ShootSpeed);
	this->SetCurAttackSpeed(this->ItemCardRangeATK.M_ShootSpeed);
	//设置攻击次数
	this->SetAttackCount(this->ItemCardRangeATK.M_ProjectionItemCount);
	//设置投射时间
	this->SetAttackDelay(this->ItemCardRangeATK.M_FirstShootSpeed, this->ItemCardRangeATK.M_ShootDelay);

	//判断是否有攻击力提升功能
	if (this->ItemCardRangeATK.M_ECardUpGradeType == ECardUpGradeType::E_Atk_Up)
	{
		//攻击力计算
		this->SetCurATK(
			this->GetATK() +
			(this->GetATK() / 2.f) * this->ItemCardRangeATK.M_CardATKRate *
			((float)(this->GetCardGrade(this->ItemCardRangeATK.ItemName))) *
			this->ItemCardRangeATK.M_M_ECardUpGradeUpRate
		);

		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[CardActor.cpp ACardActor::SetCardData_ATK]攻击卡片强化技能:|| [最终攻击力](%.2f) || 计算:[BaseATK (%.2f)] + [BaseATK / 2 (%.2f)] * [ATKRate (%.2f)] * [Grade (%d)] * [UpGradeRate (%.2f)]"),
				this->GetCurATK(),
				this->GetATK(),
				(this->GetATK() / 2.f),
				this->ItemCardRangeATK.M_CardATKRate,
				this->GetCardGrade(this->ItemCardRangeATK.ItemName),
				this->ItemCardRangeATK.M_M_ECardUpGradeUpRate);
		}
	}

	//攻击速度提升
	if (this->ItemCardRangeATK.M_ECardSkillType == ECardSkillType::E_ATTACKSPEED)
	{
		this->SetCurAttackSpeed(
			this->GetAttackSpeed() -
			this->GetAttackSpeed() * UGameSystemFunction::GetPlayerSkillBooksLv(this->ItemCardRangeATK.M_CardSkillBookName) *
			0.1f);

		if (UFVMGameInstance::GetDebug())
		{
			FString Content = FString(TEXT("当前防御卡[") +
				this->ItemCardRangeATK.ItemName + TEXT("]提升攻击速度=>") +
				FString::SanitizeFloat(this->GetCurAttackSpeed()));

			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}
	}

	this->SetCardActor(this->ItemCardRangeATK);

	Super::BeginPlay();
}

void ARangeATKCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
