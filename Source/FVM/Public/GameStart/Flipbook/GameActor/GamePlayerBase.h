// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpineActor.h"
#include "GameSystem/PlayerStructManager.h"
#include "GamePlayerBase.generated.h"

class UPaperFlipbook;
class USceneComponent;
class UFlipbookBaseComponent;
class AGameActorFlipbookBase;

UCLASS()
class FVM_API AGamePlayerBase : public ASpineActor
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void LoadAnimation(
			const FPlayerSuitItem& _Value, //角色服装数据
			bool _bUseFirst,	//服装位置【前，后】
			UFlipbookBaseComponent* _Position,//服装动画组件
			UPaperFlipbook*& _Def,//默认动画
			UPaperFlipbook*& _Attack,//攻击动画
			EPlayerEquipmentSlotPosition _Type//服装类型
		);
public:
	// Sets default values for this actor's properties
	AGamePlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//初始化角色基本数据
	void InitPlayerData();

};
