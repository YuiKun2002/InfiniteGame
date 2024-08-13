// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Card/SpawnCardActor.h"

const FItemCardSpawn& ASpawnCardActor::GetSpawnCardData()
{
	return this->ItemCardSpawnData;
}

void ASpawnCardActor::BeginPlay()
{
	//初始化必要数据
	this->ItemCardSpawnData = this->GetCardDataComponent()->GetCardSpawnData(this->CardActor_Name);

	//判断是否有生产速度功能
	if (this->ItemCardSpawnData.M_ECardUpGradeType == ECardUpGradeType::E_SpawnSpeed_Up)
	{
		float SourceValue = this->ItemCardSpawnData.M_SpawnSpeed;
		this->ItemCardSpawnData.M_SpawnSpeed =
			this->ItemCardSpawnData.M_SpawnSpeed -
			this->ItemCardSpawnData.M_SpawnSpeed *
			this->ItemCardSpawnData.M_SpawnSpeedRate *
			this->GetCardGrade(this->ItemCardSpawnData.ItemName.ToString()) / 10.f *
			this->ItemCardSpawnData.M_M_ECardUpGradeUpRate;

		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp,
				Error,
				TEXT("[CardActor.cpp ACardActor::SetCardData_Spawn]生产卡片强化技能: [初始时间]%.2f  [提升后的时间]%.2f"),
				SourceValue,
				this->ItemCardSpawnData.M_SpawnSpeed
			);
		}
	}

	//产量提升
	if (this->ItemCardSpawnData.M_ECardSkillType == ECardSkillType::E_SpawnFlameNumUp)
	{
		this->ItemCardSpawnData.M_FlameNum = this->ItemCardSpawnData.M_FlameNum + this->ItemCardSpawnData.M_FlameNum * UGameSystemFunction::GetPlayerSkillBooksLv(this->ItemCardSpawnData.M_CardSkillBookName) * 0.25f;
		if (UFVMGameInstance::GetDebug())
		{
			FString Content =
				FString(TEXT("当前防御卡[") +
					this->ItemCardSpawnData.ItemName.ToString() +
					TEXT("]产量提升至=>") +
					FString::FromInt(this->ItemCardSpawnData.M_FlameNum));

			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}
	}

	//赋值基础属性
	this->SetCardActor(this->ItemCardSpawnData);

	//初始化父类【以及组件】
	Super::BeginPlay();
}

void ASpawnCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}
