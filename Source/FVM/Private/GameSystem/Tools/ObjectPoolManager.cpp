// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/ObjectPoolManager.h"

AObjectPoolClass::AObjectPoolClass()
{
	this->SetActorTickEnabled(false);
}

void AObjectPoolClass::PoolCreate(class UObjectPoolManager* PoolManager)
{
	this->CurManager = PoolManager;
	this->SetActorHiddenInGame(true);
	this->SetActorTickEnabled(false);
}

void AObjectPoolClass::PoolInit(UObjectPoolManager* PoolManager)
{
	this->SetActorHiddenInGame(false);
	this->SetActorTickEnabled(true);
}

bool AObjectPoolClass::ReturnPool()
{
	this->SetActorHiddenInGame(true);
	this->SetActorTickEnabled(false);

	//归还资源
	if (IsValid(this->CurManager))
	{
		return this->CurManager->ReturnObjectActor(this);
	}
	else {
		this->Destroy();
	}

	return false;
}

UObjectPoolManager* UObjectPoolManager::MakePoolManager(
	UWorld* World,
	TSoftClassPtr<AActor> ResPath,
	int32 InitCount
)
{
	if (!IsValid(World))
	{
		return nullptr;
	}

	UObjectPoolManager* CurPool = NewObject<UObjectPoolManager>();
	//初始化资源内容
	CurPool->CurRes = ResPath;
	CurPool->CurWorld = World;
	//初始化资源
	UClass* LCurRes = CurPool->GenClass();

	if (!IsValid(LCurRes))
	{
		return nullptr;
	}

	for (int32 i = 0; i < InitCount; i++)
	{
		AActor* NewActor = World->SpawnActor<AActor>(
			TSubclassOf<AActor>(LCurRes)
			);

		if (IsValid(NewActor))
		{
			NewActor->SetLifeSpan(0.f);
			Cast<IObjectPoolInterface>(NewActor)->PoolCreate(CurPool);
			CurPool->CanUsePool.Emplace(NewActor);
		}
	}

	CurPool->CurCount = InitCount;

	return CurPool;
}

AActor* UObjectPoolManager::GetObjectActor()
{


	AActor* CurObj = nullptr;

	if (this->CanUsePool.Num())
	{
		CurObj = this->CanUsePool[0];
		if (IsValid(CurObj) && Cast<IObjectPoolInterface>(CurObj))
		{
			//移除元素
			this->CanUsePool.RemoveAtSwap(0);
			//添加元素
			//this->CurUsePool.Emplace(CurObj);
			//初始化
			Cast<IObjectPoolInterface>(CurObj)->PoolInit(this);

			return CurObj;
		}
	}

	if (this->UseCount > 0 && CurCount >= this->UseCount)
	{
		return nullptr;
	}

	//创建新资源
	CurObj = this->CurWorld->SpawnActor<AActor>(
		TSubclassOf<AActor>(this->GenClass())
		);

	IObjectPoolInterface* CurObjInterface = nullptr;

	CurObjInterface = Cast<IObjectPoolInterface>(CurObj);

	CurObj->SetLifeSpan(0.f);
	//创建
	CurObjInterface->PoolCreate(this);
	//初始化
	CurObjInterface->PoolInit(this);
	//放入正在使用的池子
	this->CurUsePool.Emplace(CurObj);
	//更新数量
	this->CurCount = this->CurUsePool.Num();

	return CurObj;
}

bool UObjectPoolManager::ReturnObjectActor(AActor* CurNewReturn)
{
	//归还资源
	if (IsValid(CurNewReturn))
	{
		/*int32 I = this->CurUsePool.Find(CurNewReturn);
		if (I > -1)
		{
			this->CurUsePool.RemoveAtSwap(I);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("查询失败"));
		}*/

		//对象添加
		this->CanUsePool.Emplace(CurNewReturn);

		return true;
	}

	return false;
}

bool UObjectPoolManager::InitComplete()
{
	if (IsValid(this->GenClass()))
	{
		return true;
	}
	return false;
}

void UObjectPoolManager::ClearAllObjectActor()
{
	for (auto& Cur : this->CanUsePool)
	{
		if (IsValid(Cur))
		{
			Cur->Destroy();
		}
	}

	this->CanUsePool.Empty();
}

void UObjectPoolManager::SetObjectPoolMaxCount(int32 Count /*= 0*/)
{
	this->UseCount = Count;
}

UClass* UObjectPoolManager::GenClass()
{
	//初始化资源
	UClass* TCurRes = nullptr;
	if (!this->CurRes.Get())
	{
		TCurRes = this->CurRes.LoadSynchronous();
		if (!IsValid(TCurRes))
		{
			return nullptr;
		}
	}
	else {
		TCurRes = this->CurRes.Get();
	}

	return TCurRes;
}
