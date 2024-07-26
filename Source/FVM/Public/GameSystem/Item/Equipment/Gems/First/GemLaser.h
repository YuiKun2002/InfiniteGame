// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Item/Equipment/Gems/GemFirstBase.h"
#include "GemLaser.generated.h"

//道具->宝石装备->主动技能宝石->激光宝石类
USTRUCT(BlueprintType)
struct FGemLaserType : public FGemFirst {
	GENERATED_USTRUCT_BODY()
public:
	FGemLaserType() { this->M_GemTag = TEXT("Laser"); }
public:
	//发动技能时->需要播放的UI动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_LaserUITitleAnimationClassPath;
	//海螺攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_LaerAttackAnimationClassPath;
	//动画播放完毕->子弹的路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_LaserProjectileClassPath;
	//每次升2级追加一个弹幕
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_LaserUpGradeAddCount = 2;
	//基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_LaserBaseATK = 50;
};


//装备-玩家宝石数据->激光宝石数据
USTRUCT(BlueprintType)
struct FEquipment_GemLaser_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FGemLaserType M_FEquipment;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FEquipment;
		return true;
	}
};

UCLASS()
class FVM_API UGemLaser : public UGemFirstBase
{
	GENERATED_BODY()
public:
	//路径
	static FString DataTablePath;
public:

};
