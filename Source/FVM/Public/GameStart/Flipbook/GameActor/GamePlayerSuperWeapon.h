// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GamePlayerSuperWeapon.generated.h"


class AFlyItemActor;

/**
 * 角色超级武器
 */
UCLASS()
class FVM_API AGamePlayerSuperWeapon : public ASpineActor
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;
	//移除子弹
	UFUNCTION(BlueprintCallable)
	void RemoveBulletByTag(
		const FString& NewTag
	);
	//添加新的子弹
	UFUNCTION(BlueprintCallable)
	void AddNewBullet(
		const FString& NewTag,
		const TSoftClassPtr<AActor>& ResPath,
		int32 InitPoolCount = 1
	);
	//获取子弹
	UFUNCTION(BlueprintPure)
	AFlyItemActor* GetBulletByTag(
		const FString& NewTag
	);
private:
	//多个不同子弹的对象池
	UPROPERTY()
	TMap<FString, UObjectPoolManager*> Bullets;
};
