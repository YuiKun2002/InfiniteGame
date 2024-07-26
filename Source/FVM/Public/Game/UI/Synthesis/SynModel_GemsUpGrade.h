// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "Game/UI/Synthesis/SynModel_Structures.h"
#include "SynModel_GemsUpGrade.generated.h"

/**
 *  宝石强化
 */

class UUI_PlayerSynthesis;
class UImage;
class UButton;
class UTextBlock;

UCLASS()
class FVM_API USynModel_GemsUpGrade : public UObject
{
	GENERATED_BODY()
private:
	//宝石强化概率
	const TArray<int32> GemsValue = { 100,88,75,63,50,38,26,19,13,10,9,8,7,6,5,0 };
	//需要的水晶
	const TArray<int32> NeedCrystalNums = { 5,10,15,25,35,55,75,100,130,160,200,245,300,355,420,666 };
	//需要的保险
	const TArray<int32> NeedInsuranceNums = { 0,0,0,0,0,0,400,500,600,2500,3000,3500,4000,4500,5000,0 };
private:
	//合成屋UI
	UPROPERTY()
		UUI_PlayerSynthesis* M_SynUi = nullptr;
private:
	//武器1槽
	UPROPERTY()
		FSynModel_GemSlot Slot_0;
	//武器2槽
	UPROPERTY()
		FSynModel_GemSlot Slot_1;
	//武器3槽
	UPROPERTY()
		FSynModel_GemSlot Slot_2;
	//武器4槽
	UPROPERTY()
		FSynModel_GemSlot Slot_3;
	//武器头像
	UPROPERTY()
		UImage* M_GemUp_WepaonHead = nullptr;
	//水晶头像
	UPROPERTY()
		UImage* M_GemUp_GemHead = nullptr;
	//需要强化水晶的概率文字
	UPROPERTY()
		UTextBlock* M_GemUp_RateText = nullptr;
	//需要水晶的数量显示文字
	UPROPERTY()
		UTextBlock* M_GemUp_GemCountText = nullptr;
	//选择武器时显示的武器文字
	UPROPERTY()
		UTextBlock* M_GemUp_SelectWepaonText = nullptr;
	//选择宝石时显示的宝石文字
	UPROPERTY()
		UTextBlock* M_GemUp_SelectGemText = nullptr;
	//宝石强化按钮
	UPROPERTY()
		UButton* M_GemUp_Butt = nullptr;
private:
	/*-----------------------------------强化水晶------------------------------------*/
	//普通强化水晶
	UPROPERTY()
		UItemDataTable* M_GU_NormalCrystal = nullptr;
	//高级强化水晶
	UPROPERTY()
		UItemDataTable* M_GU_AdvancedCrystal = nullptr;
	/*-----------------------------------强化水晶------------------------------------*/

	/*-----------------------------------武器------------------------------------*/
	//当前选择的武器数据
	UPROPERTY()
		UItemDataTable* M_GU_CurrentSelectWeapon = nullptr;
	//宝石数据
	UPROPERTY()
		TArray<UItemDataTable*> M_GU_WepaonGemsData;
	//当前选择的武器索引
	UPROPERTY()
		int32 M_GU_CurrentSelectWeaponIndex = -1;
	//槽索引
	UPROPERTY()
		int32 M_GU_GemSlotIndex = -1;
	//武器ID
	UPROPERTY()
		int32 M_GU_WepaonID = -1;

	/*-----------------------------------宝石------------------------------------*/

	//当前选择的宝石数据
	UPROPERTY()
		UItemDataTable* M_GU_CurrentSelectGem = nullptr;
	//当前选择的宝石索引
	UPROPERTY()
		int32 M_GU_CurrentSelectGemIndex = -1;
	/*-----------------------------------宝石------------------------------------*/

	/*-----------------------------------四叶草------------------------------------*/
	//当前选择的四叶草数据
	UPROPERTY()
		UItemDataTable* M_GU_CurrentSelectClover = nullptr;

	/*-----------------------------------四叶草------------------------------------*/



	/*-----------------------------------等待强化的宝石最大等级------------------------------------*/
	//强化宝石数据
	UPROPERTY()
		UItemDataTable* M_GU_CurrentGem = nullptr;
	//强化水晶数据
	UPROPERTY()
		UItemDataTable* M_GU_CurrentCrystal = nullptr;
	//最终确认使用的水晶类型
	UPROPERTY()
		bool M_GU_UseGemNormalCrytal = true;
	//当前宝石的最大等级【数据共享的->当选择单个宝石时】
	UPROPERTY()
		int32 M_GU_CurrentGemGradeMax = 10;
	//当前宝石等级
	UPROPERTY()
		int32 M_GU_CurrentGemGrade = 0;
	//当前选择的宝石槽位置索引
	UPROPERTY()
		int32 M_GU_CurrentSelectGemSlotIndex = 0;
	//当前宝石强化的等级
	UPROPERTY()
		int32 CurUpGemGrade = 0;


	/*-----------------------------------等待强化的宝石最大等级------------------------------------*/

public:
	void InitializeBySynthesis(UUI_PlayerSynthesis* _Parent_UI_Class);

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
	//设置当前选择的武器数据
	UFUNCTION()
		void SetCurrentSelectWeapon(UItemDataTable* _Value, const int32& _Index);
	//设置当前选择的宝石数据
	UFUNCTION()
		void SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index);
	//设置当前四叶草数据
	UFUNCTION()
		void SetCurrentSelectClover(UItemDataTable* _Value);
	//设置装备属性
	UFUNCTION()
		void SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
public:
	//设置水晶个数UI显示【开启水晶强化功能】
	UFUNCTION()
		void SetCrystalUIFunc(const int32& _GemGrade, const UItemDataTable* const _Crystal);
	//设置被强化的宝石数据[-1则表示单个宝石]
	UFUNCTION()
		void SetUpGradeGemData(const int32& _Index = -1);
public:
	//获取宝石的强化概率
	UFUNCTION()
		const TArray<int32>& GetGemsValue() const;
	//获取宝石需要的水晶数量
	UFUNCTION()
		const TArray<int32>& GetGemsNeedCrystalNums() const;
	//获取宝石需要的保险金数量
	UFUNCTION()
		const TArray<int32>& GetGemsNeedInsurance() const;
	//获取宝石的数据
	UFUNCTION()
		UItemDataTable* const GetCrystalData(const int32& _Grade);
	//获取当前强化的等级
	UFUNCTION(BlueprintCallable)
		int32 GetCurrentGemGrade();
public:
	//添加武器
	UFUNCTION()
		void AddWeapon();
	//添加宝石
	UFUNCTION()
		void AddGem();
	//添加水晶
	UFUNCTION()
		void AddCrystal(const FString& _GemName, const int32& _GemGrade);
	//添加强化水晶[根据宝石的等级]
	UFUNCTION()
		void AddUpGradeCrystal(const int32& _GemGrade);
public:
	//取消选择四叶草
	UFUNCTION(BlueprintCallable)
		void CancelClover();
public:
	//选择宝石
	UFUNCTION()
		void SelectGem(const int32& _Index);
	//选择或者取消
	UFUNCTION()
		void SelectAndCancel(const int32& _Index);
public:
	//搜索宝石数据
	UFUNCTION()
		void SearchCrystalData();
public:
	//更新概率
	UFUNCTION()
		const float UpdateRate() const;
public:
	//初始化槽
	void InitializeSlot(FSynModel_GemSlot& _Slot);
public:
	UFUNCTION()
		void Gem_0();
	UFUNCTION()
		void Gem_1();
	UFUNCTION()
		void Gem_2();
	UFUNCTION()
		void Gem_3();

	//宝石强化
	UFUNCTION()
		void GemUpGrade();
};
