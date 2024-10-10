// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "MagicMasterMouse.generated.h"

/**
 * 魔笛鼠
 */

 //所有资产路径
USTRUCT(BlueprintType)
struct FMagicMasterResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Idle;
	//正常形态(攻击)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Attack;
	//吹笛
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Use;
	//死亡形态(死亡)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Death;

	//吹笛动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> UseAnim;
	//加血动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> AddHPAnim;
};

//加血动画
UCLASS()
class FVM_API AMagicMasterHpAddtionBuff : public ASpineActor {
	GENERATED_BODY()
public:
	void Init(AMouseActor* MouseActor);
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY()
	AMouseActor* CurMouse = nullptr;

	float CTime = 0.2f;
	float time = 0.f;
};


//治疗外星人
UCLASS()
class FVM_API AMagicMasterMouse : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	//回血动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	TSoftClassPtr<AMagicMasterHpAddtionBuff> HpAddtionBuffAnim;
	//使用魔笛武器的冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	float fUseWeaponTime = 15.f;
	//多少范围内的老鼠生效【半径】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	float fWeaponRangeRadius = 90.f;
	//回复自身血量的百分比
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	float fReplySelfHpRate = 0.3f;
	//可以攻击卡片的什么类型【攻击力0】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "治疗外星人")
	ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	//初始化
	virtual void BeginPlay() override;
	virtual void MouseInit() override;
	virtual void MouseTick(const float& DeltaTime) override;
	//动画播放完毕
	UFUNCTION()
	void AnimationPlayEnd(class UTrackEntry* Track);
public:
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
private:
	//是否启动攻击线
	bool bEnableAttakLine = true;
	//是否处于创建动画状态
	bool bUse = false;

	//使用魔笛的延迟
	float fUseDelay = 0.f;
	float fWeaponRangeRadius_ = 90.f;
	float fReplySelfHpRate_ = 0.3f;
};
