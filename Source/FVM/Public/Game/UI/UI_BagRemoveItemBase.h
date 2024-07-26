// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_BagRemoveItemBase.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UPlayerStructManager;
class UUI_PlayerBagRemove;

UCLASS()
class FVM_API UUI_BagRemoveItemBase : public UWidgetBase
{
	GENERATED_BODY()
private:
	//父类UI
	UPROPERTY()
		UUI_PlayerBagRemove* M_ParentUI_Remove = nullptr;
	//选择或者取消按钮
	UPROPERTY()
		UButton* M_SelectButton = nullptr;
	//显示选择的状态
	UPROPERTY()
		UImage* M_ShowSelectTag = nullptr;
private:
	//物品名称
	FString M_ItemName;
	//物品索引
	int32 M_ItemIndex;
	//界面索引->决定表示对象从背包中的装备 卡片 材料的那种数组选择
	uint8 M_PanelNumber;
	//是否选择
	bool M_bIsSelectItem = true;
public:
	//设置选择的对象
	UFUNCTION()
		void SelectItems(bool _Tag);
public:
	//设置批量删除的UI
	UFUNCTION()
		void SetUIPlayerBagRemove(UUI_PlayerBagRemove* _Class);
public:
	//设置物品名称
	virtual void SetItemName(const FString& _ItemName);
public:
	//更新
	UFUNCTION()
		void Update();
public:
	//按钮的显示文字
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_SelectButtonText;
public:
	template<class ItemType>
	void SelectItemTag(UPlayerStructManager* _PlayerData, bool _Tag, TArray<ItemType>& _Datas)
	{
		int32 LIndex = UPlayerStructManager::SerachEquipDataByID(_PlayerData->M_PlayerItems_Equipment[M_ItemIndex].M_ItemID, _Datas);

		if (LIndex != -1)
		{
			_Datas[LIndex].bWaitRemove = _Tag;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("道具库:") + _PlayerData->M_PlayerItems_Equipment[M_ItemIndex].ItemName + (_Tag ? TEXT("标记") : TEXT("取消标记")));
			}
		}
	}
public:
	UFUNCTION()
		void Select();
public:
	virtual bool Initialize() override;
public:
	//设置被标记的物品
	void SetTagItems(int32 _ItemIndex, uint8 _PanelNumber);
	//设置选择UI
	void SetSelect();
	//设置取消
	void SetCancel();
public:
	//获取选择按钮
	UButton* GetSelectButton();
	//获取当前选择状态
	bool GetCurrentSelect();
public:
	//绑定按钮功能
	void BindSelectFunc();
};
