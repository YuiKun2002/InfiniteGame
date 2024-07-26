// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "FunctionCardActor.generated.h"

/**
 * 功能卡片
 */

class AFlyItemActor;
class UBoxComponent;
class UCardFunctionComponent;


//------------------------------------------------------------------------------
//功能卡片属性接口
UINTERFACE(MinimalAPI, BlueprintType)
class UCardFunctionInterface : public UInterface {
	GENERATED_BODY()
};
class FVM_API ICardFunctionInterface
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() = 0;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp) = 0;
	//重叠
	virtual bool OnOverlapBegin(class UCardFunctionComponent* CardFuncComp, AActor* OverlapObj) = 0;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp) = 0;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) = 0;
	//卡片死亡
	virtual void OnDeath(class UCardFunctionComponent* CardFuncComp) = 0;
};
//卡片功能基类【空类，【不建议直接选择】】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionBase : public UObject, public ICardFunctionInterface
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp) override;
	//重叠
	virtual bool OnOverlapBegin(class UCardFunctionComponent* CardFuncComp, AActor* OverlapObj) override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp) override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
	//卡片死亡
	virtual void OnDeath(class UCardFunctionComponent* CardFuncComp) override;
};
//------------------------------------------------------------------------------

//功能对象
UCLASS()
class FVM_API AFunctionActor : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit(class AFunctionCardActor* CurCardActor);
};

UCLASS()
class FVM_API AFunctionCardActor : public ACardActor
{
	GENERATED_BODY()
public:
	//获取功能卡片数据
	const FItemCardFunction& GetFunctionCardData();
	//获取重叠盒体
	UBoxComponent* GetOverlapBoxComponent();
public:
	//设置重叠
	UFUNCTION(BlueprintCallable)
		void SetOverlapBoxComponent(UBoxComponent* Box);
	//设置功能组件
	UFUNCTION(BlueprintCallable)
		void SetFunctionComponent(UCardFunctionComponent* CardFuncComp);


	//运行卡片功能类
	UFUNCTION(BlueprintCallable)
		void ExecuteCardFuncClassByCardFunction(UCardFunctionComponent* CardComp);
	//卡片功能重叠
	UFUNCTION(BlueprintCallable)
		void ExecuteCardFuncClassByCardFunctionOnOverlapBegin(UCardFunctionComponent* CardFuncComp, AActor* OverlapObj);
	//卡片功能动画播放完毕
	UFUNCTION(BlueprintCallable)
		void ExecuteCardFuncClassByCardFunctionOnAnimPlayEnd(UCardFunctionComponent* CardFuncComp);
	//卡片功能更新
	UFUNCTION(BlueprintCallable)
		void ExecuteCardFuncClassByCardFunctionUpdate(UCardFunctionComponent* CardFuncComp, const float& DeltaTime);
	//卡片死亡
	UFUNCTION()
		void ExecuteCardFuncDeath();
	//获取当前正在执行的功能
	UFUNCTION(BlueprintCallable)
		UCardFunctionBase* GetCurrentExecuteCardFuncClass();


public:
	//默认动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "卡片预设属性 | 动画")
		TSoftObjectPtr<UPaperFlipbook> CardActor_DefAniml;
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	//功能卡片数据
	UPROPERTY()
		FItemCardFunction ItemCardFunction;
	//碰撞盒体
	UPROPERTY()
		UBoxComponent* OverlapBoxComponent = nullptr;
	//功能组件
	UPROPERTY()
		class UCardFunctionComponent* CardFuncComponent = nullptr;
	//当前卡片的功能
	UPROPERTY()
		TSet<TSoftClassPtr<class UCardFunctionBase>> CardFunctionClass;
	//当前卡片的功能实例队列
	UPROPERTY()
		TArray<class UCardFunctionBase*> CardFunctionClassInstanceQueue;
	//当前卡片功能触发死亡
	UPROPERTY()
		TArray<class UCardFunctionBase*> CardFunctionClassInstanceOnDeathQueue;
};
