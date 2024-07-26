// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Data/EquipmentDataSturct.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerInformationShow.generated.h"

/**
 * 角色信息展示界面（装备，服装，名称，等）
 */

 //当服装加载完毕时调用
DECLARE_DELEGATE(FOnLoaddingEndDelegate)

class UCanvasPanel;
class UButton;
class UUI_PlayerShow;

UCLASS()
class FVM_API UUI_PlayerInformationShow : public UWidgetBase
{
	GENERATED_BODY()
public:
	bool Initialize() override;
public:
	FOnLoaddingEndDelegate OnLoaddingEnd;
private:
	//角色套装展示背景(默认 流星)
	UPROPERTY()
		UImage* M_PlayerSuitBG_Img = nullptr;
	//角色套装背景
	UPROPERTY()
		UCanvasPanel* M_PlayerSuitBG_Panel = nullptr;
	//角色主武器背景
	UPROPERTY()
		UCanvasPanel* M_PlayerFristWeaponBG_Panel = nullptr;
	//角色副武器背景
	UPROPERTY()
		UCanvasPanel* M_PlayerSecondWeaponBG_Panel = nullptr;
	//角色超级武器背景
	UPROPERTY()
		UCanvasPanel* M_PlayerSuperWeaponBG_Panel = nullptr;
public:
	//------------------------------------------------------------------------------------------------------------------------
	//   角色形象开始
	//------------------------------------------------------------------------------------------------------------------------	

	//-角色基础着装---------------------------------------------------------------------------------

	//角色创建点位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvasPanel* M_PlayerPoint = nullptr;

	//角色着装道具按钮->用于卸载着装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UButton*> M_PlayerBaseSuitButtons;
	//-角色基础着装---------------------------------------------------------------------------------

	//-角色套装---------------------------------------------------------------------------------
	//角色着装道具按钮->用于卸载着装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_PlayerBaseSuitButton = nullptr;
	//-角色套装---------------------------------------------------------------------------------


	//角色形象界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_PlayerShow* M_UUI_PlayerShow = nullptr;

public:
	//初始化角色基本数据
	UFUNCTION(BlueprintCallable)
		void InitPlayerBaseData();
	//设置限定数据(设置之后不可修改)
	UFUNCTION(BlueprintCallable)
		void SetPlayerSuitConst();
	//初始化角色形象
	UFUNCTION(BlueprintCallable)
		void InitPlayerSuit();
	//刷新角色显示
	UFUNCTION(BlueprintCallable)
		void RefreshPlayerSuit();
	//卸载角色着装
	UFUNCTION(BlueprintCallable)
		void UnInstallPlayerSuit(EPlayerEquipmentSlotPosition _LEPlayerEquipmentSlotPosition);
	//卸载角色主器
	UFUNCTION(BlueprintCallable)
		void UnEquip_Weapon(EEquipment _LEEquipment);
	//显示或者隐藏角色套装
	UFUNCTION(BlueprintCallable)
		void PlayerSuitHidden(class UButton* _Button_);
	//初始化按钮的显示状态
	UFUNCTION(BlueprintCallable)
		void InitPlayerSuitButtonStyle(class UButton* _Button_);
	//设置当前玩家数据
	UFUNCTION(BlueprintCallable)
		void SetCurrentPlayer(UPlayerStructManager* _CurrentPlayer);
public:
	//获取玩家数据是否被限制
	UFUNCTION(BlueprintCallable)
		bool GetPlayerSuitConst();
	//------------------------------------------------------------------------------------------------------------------------	
	//   角色形象结束
	//------------------------------------------------------------------------------------------------------------------------
};
