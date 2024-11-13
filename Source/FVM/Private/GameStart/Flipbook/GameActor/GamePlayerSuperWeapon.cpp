// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/GamePlayerSuperWeapon.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

void AGamePlayerSuperWeapon::BeginDestroy()
{
	Super::BeginDestroy();

	//释放全部的子弹对象池

	for (auto& Cur : this->Bullets)
	{
		if (IsValid(Cur.Value))
		{
			Cur.Value->ClearAllObjectActor();
			Cur.Value = nullptr;
		}
	}

	this->Bullets.Empty();
}

void AGamePlayerSuperWeapon::AddNewBullet(
	const FString& NewTag,
	const TSoftClassPtr<AActor>& ResPath,
	int32 InitPoolCount
)
{
	//查询子弹是否存在
	UObjectPoolManager** PoolObj = this->Bullets.Find(NewTag);
	if (PoolObj && IsValid(*PoolObj))
	{
		return;
	}

	UObjectPoolManager* NewActorPool = UObjectPoolManager::MakePoolManager(this->GetWorld(), ResPath, InitPoolCount);
	this->Bullets.Emplace(NewTag, NewActorPool);
}

void AGamePlayerSuperWeapon::RemoveBulletByTag(
	const FString& NewTag
)
{
	this->Bullets.Remove(NewTag);
}

AFlyItemActor* AGamePlayerSuperWeapon::GetBulletByTag(
	const FString& NewTag
)
{
	//通过标签获取对应池中的子弹对象
	UObjectPoolManager** PoolObj = this->Bullets.Find(NewTag);
	if (PoolObj && IsValid(*PoolObj))
	{
		return Cast<AFlyItemActor>((*PoolObj)->GetObjectActor());
	}

	return nullptr;
}
