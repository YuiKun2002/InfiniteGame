// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "EngineeringMouse.generated.h"

/**
 * 工程车鼠
 */

class UPaperFlipbook;
class UBoxComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct FEngineeringMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//移动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def1;
	//等待
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Wait;
	//发射
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};


//工程师鼠投掷的子弹
UCLASS()
class FVM_API AEngineeringPrjBullet : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	//初始化
	UFUNCTION(BlueprintImplementableEvent)
		void Init(const FVector& TargetLocation, const TArray<class ACardActor*>& TargetHitCard);
	//更新
	UFUNCTION(BlueprintImplementableEvent)
		void Update(const float& DeltaTime);
public:
	AEngineeringPrjBullet();
	void CInit(const FVector& TargetLocation, const TArray<class ACardActor*>& TargetHitCard);
	virtual void Tick(float DeltaTime) override;
private:
	bool bInit = false;
};


UCLASS()
class FVM_API AEngineeringMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEngineeringMouseResourceStruct Anim;
	//需要创建的子弹
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 需要创建的子弹资源")
		TSoftClassPtr<class AEngineeringPrjBullet> TargetCreateBullet;
	//开局移动格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 开局移动格子")
		int32 BeginMoveGrid = 1;
	//多少格子之后将不在发射
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 多少格子内允许发射子弹")
		int32 MoveShootGrid = 6;
	//子弹出现的延迟时间([0.1-1])
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 子弹出现的延迟时间")
		float ProjectileConstDelayTime = 0.4f;
	//准备新子弹的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 准备新子弹的时间")
		float ReadyNewBulletTime = 3.f;
	//发射新子弹的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 发射新子弹的时间")
		float ShootNewBulletTime = 3.f;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
public:
	AEngineeringMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimPlayEnd();

	//射击
	void ProjectileBullet(const FLine& CurLine);
private:
	//开局移动是否完成
	bool bBeginMove = false;
	//是否允许移动S
	bool bMove = false;
	//是否存在小老鼠
	bool bValidBullet = true;
	//准备时间
	float ReadyBulletTime = 0.f;
	//发射时间
	float ProjectileBulletTime = 0.f;
	//发射延迟
	bool bProjStart = false;
	float ProjectileDelay = 0.f;
	//是否允许发射
	bool bShoot = true;
	//是否发射
	bool bCurShoot = false;
	//攻击间隔
	float AtkDelay = 0.2f;
	//是否检测
	bool bCheck = true;

	UPROPERTY()
		class UMesheControllComponent* Meshe = nullptr;
};
