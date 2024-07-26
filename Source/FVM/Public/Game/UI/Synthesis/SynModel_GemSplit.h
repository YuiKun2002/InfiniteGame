// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SynModel_GemSplit.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UTextBlock;
class UUI_PlayerSynthesis;

UCLASS()
class FVM_API USynModel_GemSplit : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UUI_PlayerSynthesis* M_SynUi = nullptr;
private:
	//宝石头像
	UPROPERTY()
		UImage* M_Head = nullptr;
	//宝石等级
	UPROPERTY()
		UImage* M_Grade = nullptr;
	//水晶头像
	UPROPERTY()
		UImage* M_GemHead = nullptr;
	//水晶数量的文字显示
	UPROPERTY()
		UTextBlock* M_GemCountText = nullptr;
	//分解按钮
	UPROPERTY()
	   UButton* M_GemSplit = nullptr;
private:
	//当前选择的宝石数据
	UPROPERTY()
		UItemDataTable* M_GS_CurrentSelectGem = nullptr;
	//当前选择的宝石索引
	UPROPERTY()
		int32 M_GS_CurrentSelectGemIndex = -1;
public:
	void InitializeBySynthesis(UUI_PlayerSynthesis* _Parent_UI_Class);
public:
	//设置当前选择的宝石
	UFUNCTION()
		void SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index);
public:
	//界面重置
	UFUNCTION(BlueprintCallable)
		void WidgetReset();
	//重新加载数据表
	UFUNCTION(BlueprintCallable)
		void WidgetResetLoadData();
public:
	//加载装备
	UFUNCTION(BlueprintCallable)
		void LoadEquipment();
	//加载材料
	UFUNCTION(BlueprintCallable)
		void LoadMaterials();
public:
	//添加宝石
	UFUNCTION(BlueprintCallable)
		void AddGem();
	//宝石分解
	UFUNCTION(BlueprintCallable)
	    void GemSplit();
public:
	//设置装备属性
	UFUNCTION()
		void SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
};
