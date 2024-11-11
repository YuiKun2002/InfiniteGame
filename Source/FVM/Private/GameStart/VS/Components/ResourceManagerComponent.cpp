// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/UI/UI_FlameManager.h"


#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameMapStructManager.h"

#include "GameStart/Flipbook/GameActor/ResourceActor.h"
#include "GameStart/Flipbook/GameActor/FlameActor.h"



// Sets default values for this component's properties
UResourceManagerComponent::UResourceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UResourceManagerComponent::EnableAutoFlameSpawn(const float& _SpawnTime)
{
	if (_SpawnTime == 0.f)
	{
		this->bEnable = false;
	}
	else {
		this->bEnable = true;
	}

	this->CurSpawnTime = _SpawnTime;
	this->CurTime = 0;
}

void UResourceManagerComponent::AddFlameNum(const int32& _Value)
{
	if (this->M_FlameNum + _Value > 9999)
		this->M_FlameNum = 9999;
	else
		this->M_FlameNum += _Value;
}

void UResourceManagerComponent::SubtractFlameNum(const int32& _Value)
{
	if (this->M_FlameNum - _Value < 0)
		this->M_FlameNum = 0;
	else
		this->M_FlameNum -= _Value;
}

const int32& UResourceManagerComponent::GetCurrentFlameNum()
{
	return this->M_FlameNum;
}


bool UResourceManagerComponent::GetDay()
{
	return this->IsDayTime;
}

bool UResourceManagerComponent::AddNewStaticPool(FString ID, UObjectPoolManager* NewPool, bool bReplace)
{
	if (!bReplace)
	{
		if (IsValid(this->GetStaticPool(ID)))
		{
			return false;
		}
	}

	this->StaticPool.Emplace(ID, NewPool);

	return true;
}

UObjectPoolManager* UResourceManagerComponent::GetStaticPool(FString ID)
{
	UObjectPoolManager** Temp = this->StaticPool.Find(ID);

	if (Temp)
	{
		return *Temp;
	}

	return nullptr;
}

UObjectPoolManager* UResourceManagerComponent::GetStaticPoolOrAdd(FString ID, UObjectPoolManager* NewPool, bool bReplace /*= false*/)
{
	UObjectPoolManager* Temp = this->GetStaticPool(ID);
	if (IsValid(Temp))
	{
		return Temp;
	}

	if (this->AddNewStaticPool(ID, NewPool, bReplace))
	{
		return NewPool;
	}
	else {
		return this->GetStaticPool(ID);
	}

	return nullptr;
}

// Called every frame
void UResourceManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FVM_VS_GamePause())
		return;

	if (this->CurTime < this->CurSpawnTime)
	{
		this->CurTime += DeltaTime;
	}
	else {
		this->CurTime = 0.f;
		//如果是白天
		if (this->IsDayTime)
		{
			if (!FVM_VS_GamePause())
			{
				this->AddFlameNum(25);
			}
		}
	}
}

void UResourceManagerComponent::GameStart()
{
	//设置白天或者夜晚
	this->IsDayTime = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetGameMapStructManager()->LevelConfig.LevelIsDay;
	this->M_FlameNum = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetGameMapStructManager()->LevelConfig.InitEnergy;

	this->EnableAutoFlameSpawn(10.f);

	//初始化缓存火苗
	for (int32 i = 0; i < 25; i++)
	{
		AFlameActor* NewFlameActor = AGameMapInstance::GetGameMapInstance()->GetWorld()->SpawnActor<AFlameActor>(
			LoadClass<AFlameActor>(this,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Resource/MyFlameActor.MyFlameActor_C'"))
			);
		NewFlameActor->SetActorHiddenInGame(true);
		this->FlameActorResourcePool.Emplace(NewFlameActor);
	}
}

void UResourceManagerComponent::GameOver()
{
	this->EnableAutoFlameSpawn(0.f);



	//结算时间
	this->CurLevelTime = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->GetCurTime();

	this->FlameActorResourcePool.Empty();

	//清理对象池
	for (auto& CurPool : this->StaticPool)
	{
		if (IsValid(CurPool.Value))
		{
			CurPool.Value->ClearAllObjectActor();
			CurPool.Value = nullptr;
		}
	}
	this->StaticPool.Empty();
}

UResourceManagerComponent* UResourceManagerComponent::GetResourceManagerComponent()
{
	return AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;
}

void UResourceManagerComponent::AddResource(FString ResourceName, int32 ResourceCount)
{
	//查询道具是否存在
	int32* Value = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->CurResource.Find(ResourceName);
	if (Value)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("掉落资源重叠增加：") + ResourceName);
		}
		//覆写数据
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->CurResource.Emplace(ResourceName, ResourceCount + *Value);
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("掉落资源新增：") + ResourceName);
		}
		//新增数据
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->CurResource.Emplace(ResourceName, ResourceCount);
	}
}

TMap<FString, int32> UResourceManagerComponent::GetResource()
{
	return AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->CurResource;
}

AActor* UResourceManagerComponent::CreateNewActor(FSoftClassPath Path)
{
	return	AGameMapInstance::GetGameMapInstance()->GetWorld()->SpawnActor<AActor>(
		TSoftClassPtr<AActor>(Path).LoadSynchronous()
		);
}

void UResourceManagerComponent::ResourceAddPlayCard(FString CardName)
{
	if (!IsValid(AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent))
	{
		return;
	}

	int32* CurCount = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->PlayCards.Find(CardName);
	if (CurCount)
	{
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->PlayCards.Emplace(CardName, (*CurCount + 1));
	}
	else {
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->PlayCards.Emplace(CardName, 1);
	}
}

void UResourceManagerComponent::ResourceAddKillMouse(FString MouseName)
{
	if (!IsValid(AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent))
	{
		return;
	}

	int32* CurCount = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->KillMouses.Find(MouseName);
	if (CurCount)
	{
		*CurCount = *CurCount + 1;
	}
	else {
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->KillMouses.Emplace(MouseName, 1);
	}
}

void UResourceManagerComponent::SpawnItemByName(FString ResourceName, FVector Location)
{
	if (!IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return;
	}

	if (!IsValid(AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent))
	{
		return;
	}

	UMouseStructManager* Manager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->M_UMouseStructManager;

	//获得关卡配置
	const FLevelConfig& LevelConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

	//得到动画
	const FSoftObjectPath* Path = LevelConfig.LevelItems.Find(ResourceName);
	if (Path)
	{
		//生成资源动画【默认生成的资源是 1】
		AResourceActor* Ac = AResourceActor::MakeResourceActor(ResourceName, *Path, 1 * LevelConfig.ItemsCountRate);
		Ac->SetActorLocation(Location);
		Ac->InitRotation();
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, ResourceName + FString(TEXT(":指定生成的资源无效，请检测关卡配置是否配置了忽略项目")));
		}
	}
}

void UResourceManagerComponent::SpawnItem(FVector Location)
{
	if (!IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return;
	}

	if (!IsValid(AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent))
	{
		return;
	}

	const FLevelConfig& LevelConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;
	//随机是否要生成
	if (UGameSystemFunction::GetRange(LevelConfig.ItemsValue, 150))
	{
		UMouseStructManager* Manager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->M_UMouseStructManager;

		if (!Manager)
		{
			return;
		}

		int32 RoundIndex = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->GetCurRound();

		FRondWidthMouseConfig Config;
		if (Manager->GetMouseRound(RoundIndex, Config))
		{
			if (Config.LevelItems.Num() > 0)
			{
				int32 Index = UGameSystemFunction::GetRandomRange(0, Config.LevelItems.Num() - 1);

				//得到动画
				const FSoftObjectPath* Path = LevelConfig.LevelItems.Find(Config.LevelItems[Index]);

				if (Path)
				{
					//生成资源动画【默认生成的资源是 1】
					AResourceActor* Ac = AResourceActor::MakeResourceActor(Config.LevelItems[Index], *Path, 1 * LevelConfig.ItemsCountRate);
					Ac->SetActorLocation(Location);
					Ac->InitRotation();
				}
				else {
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__, Config.LevelItems[Index] + FString(TEXT(":回合生成的资源无效，请检测关卡配置是否配置了忽略项目")));
					}
				}
			}
		}
	}
}

void UResourceManagerComponent::SpawnItemCoin(FVector Location)
{
	//随机是否要生成
	if (UGameSystemFunction::GetRange(25, 150))
	{
		AResourceActor* Ac = AResourceActor::MakeResourceActorCoin(TEXT("金币"), 0, 100);
		Ac->SetActorLocation(Location);
		Ac->InitRotation();
	}
}

void UResourceManagerComponent::ResourceAddEx(int32 Ex)
{
	UResourceManagerComponent::GetResourceManagerComponent()->CurEx += Ex;
}

void UResourceManagerComponent::ResourceAddScore(int32 Score)
{
	UResourceManagerComponent::GetResourceManagerComponent()->CurScore += Score;
}

void UResourceManagerComponent::ResourceAddBadCard()
{
	UResourceManagerComponent::GetResourceManagerComponent()->CurBadCardCount += 1;
}

AFlameActor* UResourceManagerComponent::GetFlame()
{
	for (AFlameActor* CurFlame : UResourceManagerComponent::GetResourceManagerComponent()->FlameActorResourcePool)
	{
		if (CurFlame->IsHidden())
		{
			CurFlame->SetActorHiddenInGame(false);
			return CurFlame;
		}
	}

	return nullptr;
}

void UResourceManagerComponent::AddFlame(AFlameActor* CurFlameActor)
{
	CurFlameActor->SetActorHiddenInGame(true);

	UResourceManagerComponent::GetResourceManagerComponent()->AddFlameNum(CurFlameActor->GetFlameValue());
}

