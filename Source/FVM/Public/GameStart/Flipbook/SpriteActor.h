// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "SpriteActor.generated.h"

UCLASS()
class FVM_API ASpriteActor : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Position = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperSpriteComponent* M_SpriteComp = nullptr;
private:
	//偏移
	UPROPERTY()
		FVector M_Offset;
public:
	// Sets default values for this actor's properties
	ASpriteActor();
public:
	//设置精灵
	UFUNCTION(BlueprintCallable)
		void SetSprite(UPaperSprite* _NewSprite);
	//设置精灵的平面旋转
	UFUNCTION(BlueprintCallable)
		void SetSpritePitchRotation(float _Angle);
	//设置精灵的偏移
	UFUNCTION(BlueprintCallable)
		void SetSpriteOffset(const FVector& _Offset);
	//设置精灵的坐标(第二个参数：是否额外添加偏移)
	UFUNCTION(BlueprintCallable)
		void SetSpriteLocation(const FVector& _Location, bool _bAddOffset = false);
	//设置精灵的透明度
	UFUNCTION(BlueprintCallable)
		void SetSpriteColorOpacity(float _Alpha);
public:
	//获取精灵的坐标
	UFUNCTION(BlueprintCallable)
		FVector GetSpriteLocation();
	//获取精灵的不透明度
	UFUNCTION(BlueprintCallable)
		float GetSpriteColorOpacity();
	//获取精灵的偏移
	UFUNCTION(BlueprintCallable)
		const FVector& GetSpriteOffset();
public:
	//获取排序优先级
	const int32 GetTranslucency() const;
	//设置排序优先级
	void SetTranslucency(const int32& _Translucency);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int32 Translucency = 0;

};
