// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "MagicMouse.generated.h"

/**
 * 魔法师鼠
 */

class UPaperFlipbook;
class UBoxComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct FMagicMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//存在小老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def1;
	//不存在小老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def2;
	//生成小老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Create;
	//发射小老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};

UCLASS()
class FVM_API AMagicMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMagicMouseResourceStruct Anim;
	//需要创建的小老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 需要创建的老鼠资源")
		TSoftClassPtr<class AMagicChildMouse> TargetCreateMouse;
	//开局移动格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 开局移动格子")
		int32 BeginMoveGrid = 1;
	//多少格子之后将不在发射
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 多少格子内允许发射老鼠")
		int32 MoveShootGrid = 6;
	//准备新老鼠的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 准备新老鼠的时间")
		float ReadyNewMouseTime = 3.f;
	//发射新老鼠的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性 | 发射新老鼠的时间")
		float ShootNewMouseTime = 3.f;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComp = nullptr;
public:
	AMagicMouse();
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
	void ShootMouse(const float& Hp,const FLine& CurLine);
private:
	//开局移动是否完成
	bool bBeginMove = false;
	//是否允许移动S
	bool bMove = false;
	//是否存在小老鼠
	bool bMouseChild = true;
	//准备时间
	float MouseTime = 0.f;
	//发射时间
	float ShootMouseTime = 0.f;
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
