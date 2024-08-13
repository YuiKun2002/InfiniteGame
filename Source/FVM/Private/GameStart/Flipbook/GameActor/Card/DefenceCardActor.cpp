// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Card/DefenceCardActor.h"

const FItemCardDefence& ADefenceCardActor::GetDefenceCardData()
{
	return this->ItemCardDefence;
}

void ADefenceCardActor::BeginPlay()
{
	//初始化必要数据
	this->ItemCardDefence = this->GetCardDataComponent()->GetCardDefenceData(this->CardActor_Name);

	if (this->ItemCardDefence.M_ECardUpGradeType == ECardUpGradeType::E_Defence_Up)
	{
		this->ItemCardDefence.M_CardHP =
			this->ItemCardDefence.M_CardHP +
			this->ItemCardDefence.M_CardHP * this->ItemCardDefence.M_DefenceUpRate *
			this->GetCardGrade(this->ItemCardDefence.ItemName.ToString()) * this->ItemCardDefence.M_M_ECardUpGradeUpRate;
	}

	this->SetCardActor(this->ItemCardDefence);

	Super::BeginPlay();
}

void ADefenceCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
