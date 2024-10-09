// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/MapLauncher/MapLauncherBase.h"
#include "MeshesLauncherComponent.generated.h"

class AMapMeshe;
class ASpineActor;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class UPaperSprite;
class AMapLauncherBase;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMeshesLauncherComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//需要被发射的网格
	UPROPERTY()
	TMap<AMapMeshe*, ASpineActor*> M_Meshes;
	//动画资源
	UPROPERTY()
	UClass* M_AnimRes = nullptr;
	//动画
	UPROPERTY()
	TArray<class AMesheLauncherActor*> MesheLauncherActors;
private:
	//是否提交申请
	UPROPERTY()
	bool M_IsbApply = false;
	//发射器
	UPROPERTY()
	AMapLauncherBase* M_Owner = nullptr;
public:
	// Sets default values for this component's properties
	UMeshesLauncherComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//设置网格坐标
	UFUNCTION(BlueprintCallable)
	void SetMeshs(const TArray<FLine>& _Lines);
	//设置动画资源
	UFUNCTION(BlueprintCallable)
	void SetAnimationResPath_C(TSoftClassPtr<ASpineActor> _AnimRes);
	//应用设置
	UFUNCTION(BlueprintCallable)
	void ApplySetting();
};
