// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "PlayerFirstWeapon.generated.h"

class UFirstWeaponProjectionComponent;
class UPlayerStructManager;
class UPaperFlipbook;
class UUI_MapMeshe;
class AMapMeshe;
class AMouseActor;
class AGamePlayer;

UCLASS()
class FVM_API APlayerFirstWeapon : public AGameActorFlipbookBase
{
	GENERATED_BODY()
private:
	//主武器数据
	UPROPERTY()
		FPlayerWeaponFirst M_FFPlayerWeaponFirstData;
	//主武器射线位置(已经计算过后)
	UPROPERTY()
		TArray<FLineTraceSetting> M_FirstWeaponLineTraceSettings;
	//角色
	UPROPERTY()
		AGamePlayer* M_AGamePlayer = nullptr;
	//对应的网格实例
	UPROPERTY()
		AMapMeshe* M_AMapMeshe = nullptr;
private:
	UPROPERTY()//主武器-默认动画
		UPaperFlipbook* M_Anim_FirstWeaponDef = nullptr;
	UPROPERTY()//主武器-攻击动画
		UPaperFlipbook* M_Anim_FirstWeaponAttack = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFirstWeaponProjectionComponent* M_UFirstWeaponProjectionComponent = nullptr;
	//子弹的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* BulletLocationComp = nullptr;
public:
	APlayerFirstWeapon();
public:
	//初始化武器数据
	void InitWeaponData(UPlayerStructManager* _Player, const FString& _WeaponName, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe);

	//获取主武器数据
	const FPlayerWeaponFirst& GetPlayerFirstWeaponData();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//板块移动
	void MeshMoveUpdate(float DeltaTime, UUI_MapMeshe* _UI_MapMeshe, AMapMeshe* _MapMeshe);
public:
	//设置主角
	void SetPlayeActor(AGamePlayer* _Player);
	//角色默认动画
	void PlayerDef_Anim();
	//角色攻击动画
	void PlayerAttack_Anim();
public:
	//获取子弹的位置
	USceneComponent* GetBulletLocationComp();
	//获取主角
	AGamePlayer* GetPlayerActor();
	//获取射线位置
	const TArray<FLineTraceSetting>& GetLineTraceSetting();
};
