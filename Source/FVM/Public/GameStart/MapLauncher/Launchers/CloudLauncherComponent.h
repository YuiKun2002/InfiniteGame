// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloudLauncherComponent.generated.h"

//地图发射器是争对每一张地图所使用的
class AMapMeshe;
class ASpriteActor;

//云朵
USTRUCT(BlueprintType)
struct FCloudPosition {
	GENERATED_USTRUCT_BODY()
public:
	//云朵集合
	UPROPERTY()
		TArray<ASpriteActor*> M_Clouds;
	//最后一个云朵的位置
	UPROPERTY()
		TArray<FVector> EndLocation;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCloudLauncherComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//每多少秒创建一个销毁卡片的云朵
	float M_Time = 0.f;
	//是否已经添加了云朵
	bool M_bCreate = false;
private:
	UPROPERTY()
		class AMapLauncherBase* M_LauncherBase = nullptr;
	//资源云朵
	UPROPERTY()
		UClass* M_Cloud_Res = nullptr;
	//云朵集合
	UPROPERTY()
		FCloudPosition M_Clouds;
	//创建可能会被选为销毁卡片的云朵
	UPROPERTY()
		TArray<ASpriteActor*> M_KillCarClouds;
private:
	UPROPERTY()
		FString M_Cloud_0_Path = FString(TEXT("PaperSprite'/Game/Resource/Texture/Sprite/VS/Map/4/8/T_1_Sprite.T_1_Sprite'"));
	UPROPERTY()
		FString M_Cloud_1_Path = FString(TEXT("PaperSprite'/Game/Resource/Texture/Sprite/VS/Map/4/8/T_1_Sprite.T_1_Sprite'"));
private:
	//云朵的移动速度
	UPROPERTY()
		float M_CloudSpeed = -9.f;
	//云朵重置到最后一个的偏移位置
	UPROPERTY()
		FVector2D M_CloudEndOffset = FVector2D(35.f, 0.f);
	//云朵最大距离
	UPROPERTY()
		float M_CloudMaxDistance = -115.f;
	//多少秒生成一个云朵卡片销毁器
	UPROPERTY()
		float M_SpawnSkillCloudTimes = 40.f;
	//更新时间的倍率
	UPROPERTY()
		float M_DeltaTime_Rate = 1.f;
public:
	//初始化
	UFUNCTION(BlueprintCallable)
		void Init(class AMapLauncherBase* _Launcher, UClass* _Cloud_Res);
	//设置云朵资源
	UFUNCTION(BlueprintCallable)
		void SetCloudResPath(FString _Cloud_0_Path, FString _Cloud_1_Path);
	//设置云朵移动速度
	UFUNCTION(BlueprintCallable)
		void SetCloudMoveSpeed(float _Speed = -9.f);
	//设置云朵重置到最后一格的偏移位置
	UFUNCTION(BlueprintCallable)
		void SetCloudEndOffset(FVector2D _CloudEndOffset = FVector2D(35.f, 0.f));
	//设置云朵移动的最大距离
	UFUNCTION(BlueprintCallable)
		void SetCloudMaxDistance(float _Value = -115.f);
	//设置销毁器生成时间
	UFUNCTION(BlueprintCallable)
		void SetSpawnSkillCloudTimes(float _Times = 40.f);
	//设置时间更新倍率
	UFUNCTION(BlueprintCallable)
		void SetUpdateTimeRate(float _Rate = 1.f);

public:
	//创建云朵缓冲
	UFUNCTION(BlueprintCallable)
		void SpawnCloud(int32 _Row, int32 _Col_Begin, int32 _Col_End, FVector _UniformOffset = FVector(10.f, 10.f, 0.f), FVector2D _UniformMargin = FVector2D(0.f, 0.f));
	//更新
	UFUNCTION(BlueprintCallable)
		void Update(float DeltaTime);
public:
	//创建销毁卡片的云朵
	void CreateSKillCardCloud();
public:
	// Sets default values for this component's properties
	UCloudLauncherComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
