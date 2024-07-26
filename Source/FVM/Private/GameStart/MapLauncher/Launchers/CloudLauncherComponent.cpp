// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/MapLauncher/Launchers/CloudLauncherComponent.h"

#include "GameStart/MapLauncher/MapLauncherBase.h"
#include "GameStart/Flipbook/SpriteActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/MapMeshe_Cloud.h"
#include "GameStart/VS/GameMapInstance.h"

// Sets default values for this component's properties
UCloudLauncherComponent::UCloudLauncherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCloudLauncherComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCloudLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//云朵发射器
void UCloudLauncherComponent::Init(class AMapLauncherBase* _Launcher, UClass* _Cloud_Res)
{
	this->M_LauncherBase = _Launcher;
	this->M_Cloud_Res = _Cloud_Res;
}

void UCloudLauncherComponent::SetCloudResPath(FString _Cloud_0_Path, FString _Cloud_1_Path)
{
	this->M_Cloud_0_Path = _Cloud_0_Path;
	this->M_Cloud_1_Path = _Cloud_1_Path;
}

void UCloudLauncherComponent::SetCloudMoveSpeed(float _Speed)
{
	this->M_CloudSpeed = _Speed;
}

void UCloudLauncherComponent::SetCloudEndOffset(FVector2D _CloudEndOffset)
{
	this->M_CloudEndOffset = _CloudEndOffset;
}

void UCloudLauncherComponent::SetCloudMaxDistance(float _Value)
{
	this->M_CloudMaxDistance = _Value;
}

void UCloudLauncherComponent::SetSpawnSkillCloudTimes(float _Times)
{
	this->M_SpawnSkillCloudTimes = _Times;
}

void UCloudLauncherComponent::SetUpdateTimeRate(float _Rate)
{
	this->M_DeltaTime_Rate = _Rate;
}

void UCloudLauncherComponent::SpawnCloud(int32 _Row, int32 _Col_Begin, int32 _Col_End, FVector _UniformOffset, FVector2D _UniformMargin)
{
	if (_Row >= this->M_LauncherBase->GetMapMeshLine().Row)
	{
		if (_Col_End >= this->M_LauncherBase->GetMapMeshLine().Col)
			return;
	}

	for (int32 Col = _Col_Begin; Col < _Col_End; Col++)
	{
		//生成一个云朵
		ASpriteActor* Cloud = Cast<ASpriteActor>(this->M_LauncherBase->GetWorld()->SpawnActor(M_Cloud_Res));
		//设置背景
		Cloud->SetTranslucency(TranslucencyConst::GetLayer(TranslucencyConst::ETranslucency::EBgBlock));

		if (Col % 2 == 0)
			Cloud->SetSprite(LoadObject<UPaperSprite>(0, *this->M_Cloud_1_Path));
		else
			Cloud->SetSprite(LoadObject<UPaperSprite>(0, *this->M_Cloud_0_Path));

		Cloud->SetSpriteLocation(this->M_LauncherBase->GetMapMeshes(FLine(_Row, Col))->GetActorLocation() + _UniformOffset + FVector(0.f, (_UniformMargin * Col).X, (_UniformMargin * Col).Y));

		//添加数据
		this->M_Clouds.M_Clouds.Emplace(Cloud);
		this->M_Clouds.EndLocation.Emplace(this->M_LauncherBase->GetMapMeshes(FLine(_Row, _Col_End - 1))->GetActorLocation() + _UniformOffset);
	}
}

void UCloudLauncherComponent::Update(float DeltaTime)
{
	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
	{
		return;
	}

	this->M_Time += DeltaTime * this->M_DeltaTime_Rate;


	if (this->M_bCreate)
	{
		this->M_KillCarClouds.Empty();
		this->M_bCreate = false;
	}

	//云朵
	for (auto Cloud = this->M_Clouds.M_Clouds.CreateIterator(); Cloud; Cloud++)
	{
		(*Cloud)->AddActorLocalOffset(FVector(0.f, this->M_CloudSpeed * DeltaTime * this->M_DeltaTime_Rate, 0.f));

		//当距离目标点还有一掉段距离时，进行等比缩小
		if ((*Cloud)->GetSpriteLocation().Y - 12.f < this->M_CloudMaxDistance)
		{
			if ((*Cloud)->GetActorScale3D().X > 0.1f)
			{
				(*Cloud)->SetActorScale3D((*Cloud)->GetActorScale3D() - FVector(0.1f * DeltaTime * 6.f));
				//(*Cloud)->SetSpriteColorOpacity((*Cloud)->GetSpriteColorOpacity() - 0.1f * DeltaTime * 2.f);
			}
		}
		else if ((*Cloud)->GetSpriteLocation().Y > this->M_CloudMaxDistance)
		{
			if ((*Cloud)->GetActorScale3D().X < 1.f)
			{
				(*Cloud)->SetActorScale3D((*Cloud)->GetActorScale3D() + FVector(0.1f * DeltaTime * 7.f));
				//(*Cloud)->SetSpriteColorOpacity((*Cloud)->GetSpriteColorOpacity() + 0.1f * DeltaTime * 2.f);
			}
		}

		if ((*Cloud)->GetSpriteLocation().Y < this->M_CloudMaxDistance)
		{
			(*Cloud)->SetSpriteLocation(this->M_Clouds.EndLocation[Cloud.GetIndex()] + FVector(0.f, this->M_CloudEndOffset.X, this->M_CloudEndOffset.Y));

			this->M_KillCarClouds.Emplace((*Cloud));

			this->M_bCreate = true;
		}
	}

	if (this->M_Time > this->M_SpawnSkillCloudTimes)
	{
		this->CreateSKillCardCloud();
	}
}

void UCloudLauncherComponent::CreateSKillCardCloud()
{
	if (!this->M_bCreate)
		return;

	if (this->M_KillCarClouds.Num() == 0)
		return;

	//清除时间
	this->M_Time = 0.f;

	int32 Index = UGameSystemFunction::GetRandomRange(0, this->M_KillCarClouds.Num() - 1);

	//创建一个云朵卡片清除器
	AMapMeshe_Cloud* CloudActor = Cast<AMapMeshe_Cloud>(
		this->M_LauncherBase->GetWorld()->SpawnActor<AMapMeshe_Cloud>(
			LoadClass<AMapMeshe_Cloud>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/CustomMeshe/custom/AMapMeshe_Cloud.AMapMeshe_Cloud_C'"))));
	CloudActor->SetCloudTag(this->M_KillCarClouds[Index]);
}
//云朵发射器



