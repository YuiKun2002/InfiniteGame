// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "Game/UI/Synthesis/SynModel_Structures.h"
#include "SynModel_WepaonGems.generated.h"

class UUI_PlayerSynthesis;

//当装备加载刷新的的时候触发[单播]
DECLARE_DELEGATE_ThreeParams(FSynModelEquipmentLoad, UItemDataTable*, int32, UWidget*);


UCLASS()
class FVM_API USynModel_WepaonGems : public UObject
{
	GENERATED_BODY()
private:
	//装备
	UPROPERTY()
		UItemLoadManager* M_Equipment_Loadder = nullptr;
	UPROPERTY()
		class UScrollBox* M_Equipment_Scroll = nullptr;
	UPROPERTY()
		class UUniformGridPanel* M_Equipment_Gird = nullptr;
private:
	//当道具加载的时候触发
	FSynModelEquipmentLoad OnEquipmentLoad;
private:
	//合成屋UI
	UPROPERTY()
		UUI_PlayerSynthesis* M_UUI_PlayerSynthesis = nullptr;
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
		class UImage* M_WepaonHead = nullptr;
	//镶嵌
	UPROPERTY()
		class UButton* M_GemAttachmentButt = nullptr;
private:
	//武器标题名称
	UPROPERTY()
		class UTextBlock* M_WeaponNameText = nullptr;
	//宝石标题名称
	UPROPERTY()
		class UTextBlock* M_GemNameText = nullptr;
private:
	/*-----------------------------------武器------------------------------------*/
	//当前选择的武器数据
	UPROPERTY()
		UItemDataTable* M_CurrentSelectWeapon = nullptr;
	//当前选择的武器索引
	UPROPERTY()
		int32 M_CurrentSelectWeaponIndex = -1;
	//材料名称
	UPROPERTY()
		FString M_NeedMaterialName = TEXT("");
	//槽索引
	UPROPERTY()
		int32 M_GemSlotIndex = -1;
	//武器ID
	UPROPERTY()
		int32 M_WepaonID = -1;

	/*-----------------------------------宝石------------------------------------*/

	//当前选择的宝石数据
	UPROPERTY()
		UItemDataTable* M_CurrentSelectGem = nullptr;
	//当前选择的宝石索引
	UPROPERTY()
		int32 M_CurrentSelectGemIndex = -1;
	//等宝石待嵌入名称
	UPROPERTY()
		FString M_WaitAttachGemName = TEXT("");
	//宝石槽索引
	UPROPERTY()
		int32 M_CurrentGemSlotIndex = -1;

	/*-----------------------------------拆卸改锥数据------------------------------------*/
	//当前选择的拆卸改锥数据
	UPROPERTY()
		UItemDataTable* M_CurrentGemUninstallBit = nullptr;
	//当前需要从此武器数据中拆卸宝石数据
	UPROPERTY()
		UItemDataTable* M_CurrentGemUninstallWepaonData = nullptr;
	//卸载槽索引
	UPROPERTY()
		int32 M_CurrentUninstallGemSlotIndex = -1;
public:
	//初始化
	void InitializeBySynthesis(UUI_PlayerSynthesis* _UI);
	//初始化装备宝石加载器
	void InitEquipmentLoadder();
	//重新绑定
	FSynModelEquipmentLoad& GetEquipmentLoadBind();
	//获取装备宝石加载器
	UItemLoadManager*& GetEquipmentLoadder();
public:
	//重新绑定装备加载对象
	template<class ClassType>
	void SetEquipmentLoadRebind(ClassType* _Class, const FString& _FuncName)
	{
		if (this->OnEquipmentLoad.IsBound())
			this->OnEquipmentLoad.Unbind();

		this->OnEquipmentLoad.BindUFunction(_Class, *_FuncName);
	}
public:
	//重置界面
	UFUNCTION(BlueprintCallable)
		void WidgetReset();
	//材料重新加载
	UFUNCTION(BlueprintCallable)
		void WidgetDataReload();
	//加载材料-钻头-水晶
	UFUNCTION(BlueprintCallable)
		void LoadMaterials();


	//加载装备-宝石
	UFUNCTION(BlueprintCallable)
		void LoadEquipment();
	//宝石加载刷新
	void LoadEquipmentRefresh(TArray<FEquipmentBase*>& _datas);


	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_InitEquipment(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_UpdateEquipment(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
public:
	//初始化槽
	void InitializeSlot(FSynModel_GemSlot& _Slot);
public:
	//设置装备属性
	UFUNCTION()
		void SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//设置当前选择的武器数据
	UFUNCTION()
		void SetCurrentSelectWeapon(UItemDataTable* _Value, const int32& _Index);
	//设置当前选择的宝石数据
	UFUNCTION()
		void SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index);
	//开启武器解析【当选择武器的时候】
	UFUNCTION()
		void EnableWepaonToSlot();
	//开始宝石添加解析【当武器被选择好了，有多余空位】
	UFUNCTION()
		bool EnableGemToSlot();
public:
	//武器开槽或者卸载宝石
	UFUNCTION()
		void OpenSlotOrUnInstall_0();
	//武器开槽或者卸载宝石
	UFUNCTION()
		void OpenSlotOrUnInstall_1();
	//武器开槽或者卸载宝石
	UFUNCTION()
		void OpenSlotOrUnInstall_2();
	//武器开槽或者卸载宝石
	UFUNCTION()
		void OpenSlotOrUnInstall_3();
	//宝石开启
	UFUNCTION()
		void GemEnable();

	//宝石解析
	UFUNCTION()
		bool GemEnableFunc(FSynModel_GemSlot& _Slot, const int32& _Index, const FPlayerWeaponBase& _WepaonData);
	//移除宝石占位【未镶嵌的宝石】
	UFUNCTION()
		void RemoveGemToSlot();

	//移除并销毁宝石
	UFUNCTION()
		void DirectRemoveGemDestory();
	//卸载宝石并且返回背包
	UFUNCTION()
		void DirectRemoveGemReturnToBag();

	//宝石槽
	UFUNCTION()
		void GemsSlot(const int32& _Index);
	//宝石槽位开启
	UFUNCTION()
		void GemSlotOpen(const int32& _Index);
	//宝石槽位卸载宝石
	UFUNCTION()
		void GemSlotUninstall(const int32& _Index);
	//宝石镶嵌
	UFUNCTION()
		void GemAttach();

};
