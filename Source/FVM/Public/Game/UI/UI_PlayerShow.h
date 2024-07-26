// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerShow.generated.h"

/**
 * 角色形象显示界面
 */

class UCanvasPanel;
class UButton;
class UImage;

UCLASS()
class FVM_API UUI_PlayerShow : public UWidgetBase
{
	GENERATED_BODY()
private:
	//角色界面(基础着装和套装界面)
	UPROPERTY()
		TArray<UCanvasPanel*> M_PlayerSuitPanels;
	//-角色基础着装---------------------------------------------------------------------------------
	UPROPERTY()
		TArray<UImage*> M_PlayerBaseSuitImages;
	//-角色基础着装---------------------------------------------------------------------------------
	//-角色套装---------------------------------------------------------------------------------
	UPROPERTY()
		UImage* M_PlayerBaseSuitImage = nullptr;
	//-角色套装---------------------------------------------------------------------------------
private:
	//当前玩家存档
	UPROPERTY()
		UPlayerStructManager* M_CurrentPlayer = nullptr;
	//临时的服装数据据
	UPROPERTY()
		FPlayerSuit M_FPlayerSuitTemp;
	//如果为true无论怎么操作都不会影响角色存档的数据
	UPROPERTY()
		bool M_bConst = false;
public:
	//不修改玩家数据
	UFUNCTION()
		void SetConstCurrentPlayer();
	//设置当前玩家数据
	UFUNCTION(BlueprintCallable)
		void SetCurrentPlayer(UPlayerStructManager* _CurrentPlayer);
public:
	//获取服装的限定条件
	UFUNCTION(BlueprintCallable)
		bool GetPlayerSuitConst();
public:
	//获取服装数据
	FPlayerSuit* GetPlayerSuit();
public:
	//获取是否是限定数据
	UFUNCTION()
		bool GetbConst();
public:
	//初始化
	bool Initialize() override;
public:
	//初始化角色基础数据
	UFUNCTION(BlueprintCallable)
		void InitPlayerDataBase();
	//初始化角色形象
	UFUNCTION(BlueprintCallable)
		void InitPlayerSuit();
	//卸载角色着装
	UFUNCTION(BlueprintCallable)
		void UnInstallPlayerSuit(EPlayerEquipmentSlotPosition _LEPlayerEquipmentSlotPosition);
	//显示或者隐藏角色套装
	UFUNCTION(BlueprintCallable)
		void PlayerSuitHidden(bool _bShowSuit);
};
