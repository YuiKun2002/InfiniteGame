// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStart/VS/MapBaseType.h"
#include "CustomMapMeshe.generated.h"

UCLASS()
class FVM_API ACustomMapMeshe : public AActor
{
	GENERATED_BODY()

		friend class UUI_CustomMapMeshe;

public:
	// Sets default values for this actor's properties
	ACustomMapMeshe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	//设置路线
	UFUNCTION(BlueprintCallable)
		class UWidgetComponent* GetWidget();

	//设置路线
	UFUNCTION(BlueprintCallable)
		void SetLine(FLine NewLine);

	//获取路线
	UFUNCTION(BlueprintCallable)
		bool GetLine(FLine& OutLine);
private:
	UPROPERTY()
		bool bInit = false;
	UPROPERTY()
		bool bSetLine = false;
	UPROPERTY()
		FLine CurLine;

};
