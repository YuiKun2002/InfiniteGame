// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameSystem/Item/PlayerStructBase.h"
#include "Components/ActorComponent.h"
#include "VSManagerComponent.generated.h"

class UUI_GameOver;
class UUI_VSCountDown;
class UUI_BottomTip;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UVSManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//是否是胜利
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Win = true;
	//是否使用过钥匙或者徽章
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bUseKey = false;
public:
	// Sets default values for this component's properties
	UVSManagerComponent();
public:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	UFUNCTION(BlueprintCallable)
		static bool CreatePlayer(
			bool bMainPlayer,
			FString PlayerName,
			int32 Row,
			int32 Col,
			int32 PlayerGrade,
			FSoftObjectPath PlayerHead,
			FPlayerSuitItem NewSuit,
			FString FirstWeaponName
		);

	UFUNCTION(BlueprintCallable)
		static bool CreatePlayerByData(
			FString PlayerName,
			int32 Row,
			int32 Col,
			int32 PlayerGrade,
			TSoftObjectPtr<UDataTable> PlayerDataPtr
		);

	//游戏暂停
	UFUNCTION(BlueprintCallable)
		void SetGamePause();

	//游戏继续
	UFUNCTION(BlueprintCallable)
		void SetGameContinue();
public:
	//开始游戏
	UFUNCTION(BlueprintCallable)
		void GameStart();

	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
public:
	//显示文字到底部
	UFUNCTION(BlueprintCallable)
		void ShowText(const FString& _Title);
public:
	//直接游戏胜利，结算
	UFUNCTION(BlueprintCallable)
		static void GameWin();
	//直接游戏失败，结算
	UFUNCTION(BlueprintCallable)
		static void GameFailed();
private:
	//游戏结束UI
	UPROPERTY(EditAnywhere)
		UUI_GameOver* M_UI_GameOver = nullptr;
	//游戏底部提示
	UPROPERTY(EditAnywhere)
		UUI_BottomTip* M_UIBottomTip = nullptr;
	//游戏倒计时
	UPROPERTY(EditAnywhere)
		UUI_VSCountDown* UI_VSCountDown = nullptr;
};
