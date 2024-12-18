// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/GamePlayerBase.h"
#include "GamePlayer.generated.h"

class APlayerFirstWeapon;
class UUI_MapMeshe;
class AMapMeshe;
class AMouseActor;

UCLASS()
class FVM_API AGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()
private:
	//角色线路
	UPROPERTY()
	FLine M_PlayerLine;
	//角色主武器
	UPROPERTY()
	APlayerFirstWeapon* M_PlayerFirstWeapon = nullptr;
	//老鼠对象
	UPROPERTY()
	AMouseActor* M_MouseActor = nullptr;
	//当前网格UI
	UPROPERTY()
	UUI_MapMeshe* M_UUI_MapMeshe = nullptr;
	//对应的网格实例
	UPROPERTY()
	AMapMeshe* M_AMapMeshe = nullptr;
public:
	AGamePlayer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//初始化网格
	UFUNCTION(BlueprintCallable)
	void InitMeshe(UUI_MapMeshe* _UUI_MapMeshe, AMapMeshe* _AMapMeshe);
	//初始化指定套装外观【仅套装外观】
	UFUNCTION(BlueprintCallable)
	void SetPlayerSuit(FPlayerSuitItem SuitData);
	//初始化角色武器
	UFUNCTION(BlueprintCallable)
	void InitPlayerWeapon();
	//加载武器
	UFUNCTION(BlueprintCallable)
	void LoadPlayerWeapon(const FName& WeapinName, const FMainWeaponData& WeaponData);
public:
	//设置角色线路
	UFUNCTION(BlueprintCallable)
	void SetPlayerLine(const FLine& _Line);

	//设置角色渲染顺序
	void SetPlayerTranslucency(UUI_MapMeshe* _CurMeshe);
	//设置老鼠对象
	void SetCurrentMouse(AMouseActor* _AMapMeshe);
public:
	//获取角色线路
	UFUNCTION(BlueprintCallable)
	FLine GetLine() const;
	//获取玩家主武器
	UFUNCTION(BlueprintCallable)
	bool PlayerFirstWeaponIsValid() const;


	//获取当前老鼠对象
	AMouseActor* GetCurrentMouse();
	//获取网格
	AMapMeshe* const GetMapMeshe();
	//获取UI网格
	UUI_MapMeshe* const GetUIMapMeshe();
	//获取角色渲染层【卡片读取】
	int32 GetPlayerRenderLayerToCardLayer();
public:
	//角色默认动画
	void PlayerDef_Anim();
	//角色攻击动画
	void PlayerAttack_Anim(float TimeScale);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//角色位置更新
	void PlayerLocationUpdate(const float& DeltaTime);
	//武器瞄准更新
	void WepaonFirstMoveUpdate(const float& DeltaTime);
	//更新武器判断位置
	void UpdateWeaponLocation(const float& DeltaTime);
private:
	UPROPERTY()
	float time = 0.1f;
	UPROPERTY()
	float Ptime = 0.01f;

};
