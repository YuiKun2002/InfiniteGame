// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "MatrixMainMouse.generated.h"

/**
 * 忍者首领
 */

class AMatrixChildMouse;
class UPaperFlipbook;


//所有资产路径
USTRUCT(BlueprintType)
struct FMatrixMainResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//奔跑
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Run;
	//奔跑残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RunLow;

	//召唤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Create;
	//召唤残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> CreateLow;

	//走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Walk;
	//走残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> WalkLow;

	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Attack;
	//攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> AttackLow;

	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleDeath;

	//被召唤的忍者鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AMatrixChildMouse> ChildMouse;
};

UENUM()
enum class EMatrixGroupTag : uint8 {
	Main,
	Top,
	Bottom,
	Left,
	Right
};

UCLASS()
class FVM_API UMatrixGroup : public UObject
{
	GENERATED_BODY()

public:
	//搭建
	void Setup(const EMatrixGroupTag& Tag, class AMatrixGroupMouse* NewMouse);
	//是否需要创建老鼠
	bool GetNeedCreate(TArray<EMatrixGroupTag>& OutTag);
	//是否可以移动
	bool GetMove();
	//更新自身状态
	void UpdateState(class AMatrixGroupMouse* CallObj);
	//移动禁止
	void MoveStop();
	//获取移动
	const bool& GetbMove();
	//开始攻击
	void SetAtk(bool bCurAtk);
	//获取攻击
	bool GetAtk();
	//获取攻击
	const bool& GetbAtk();
	//获取创建老鼠
	bool GetbCreate();
	//老鼠死亡
	void DeathMouse(class AMatrixGroupMouse* NewMouse, bool MainMouseDeathed = false);
private:
	UPROPERTY()
		TMap<EMatrixGroupTag, class AMatrixGroupMouse*> GroupMouse;
	UPROPERTY()
		bool bMove = true;
	UPROPERTY()
		bool bAtk = false;
	UPROPERTY()
		bool bMainMouseDeathed = false;

};


//矩阵组合
UCLASS()
class FVM_API AMatrixGroupMouse : public ANormalBase
{
	GENERATED_BODY()

public:
	void SetupMatrixGroup(const EMatrixGroupTag& Tag, UMatrixGroup* matrixGroup);
	UMatrixGroup* GetMatrixGroup();

public:
	virtual void MoveingUpdate(float DeltaTime) override;
	virtual void MoveFunc(const float& DeltaTime);

	virtual void UpdateAnim();

	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop() override;
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin() override;

	bool GetbCreate();
protected:
	UPROPERTY()
		bool bCreateMouse = false;
private:
	UPROPERTY()
		UMatrixGroup* MatrixGroup = nullptr;
};


UCLASS()
class FVM_API AMatrixMainMouse : public AMatrixGroupMouse
{
	GENERATED_BODY()

public:
	AMatrixMainMouse();
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//移动
	virtual void MoveFunc(const float& DeltaTime) override;
	//开始攻击
	virtual void AttackedBegin() override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimPlayEnd();
	//移动
	void CheckMoveFunc(const float& DeltaTime);
	//更新状态
	virtual void UpdateAnim() override;
	void UpdateAnim(bool bState, const TSoftObjectPtr<UPaperFlipbook>& CurAnim, const TSoftObjectPtr<UPaperFlipbook>& CurAnimLow);
	//创建新的小弟
	AMouseActor* CreateNewChildMouse(const FVector& Loca, const FLine& NewLine);
	//功能奔跑
public:
	//卡片检测
	void UpdateRunCheckCard(const float& DeltaTime);

	//创建功能
public:
	//创建新成员
	void CreateNewChild();
	//成员创建完毕
	bool CreateChildEnd();
	//创建小弟
	void CreateChildMouse();
	//其他动作更新
public:
	void UpdateWalk(const float& DeltaTime);

	bool UpdateAttackAnim();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMatrixMainResourceStruct Anim;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//网格检测碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* MesheComp = nullptr;
	//老鼠本体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionComp = nullptr;
private:
	//奔跑
	UPROPERTY()
		bool bRun = true;
	//奔跑检测延迟
	UPROPERTY()
		float RunDelay = 0.2f;
	//最长奔跑时间
	UPROPERTY()
		float RunMaxTime = 3.f;

	//创建
	UPROPERTY()
		bool bCreate = false;
	//速度还原
	UPROPERTY()
		bool bSpeedBack = false;


	//启用攻击
	UPROPERTY()
		bool bAttack = false;
	//奔跑检测延迟
	UPROPERTY()
		float AttackDelay = 0.1f;
	//创建小弟的延迟
	UPROPERTY()
		float bCreateDelay = 5.f;
};
