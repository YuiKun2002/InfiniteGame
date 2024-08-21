// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "UI_PlayerBagMaterialGrid.generated.h"

/**
 *
 */

class UTextBlock;
class UButton;
class UUI_PlayerSynthesis;
class UUI_PlayerBag;

UCLASS()
class FVM_API UUI_PlayerBagMaterialGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//背包UI
	UPROPERTY()
	UUI_PlayerBag* M_UUI_PlayerBag = nullptr;
	//合成屋的UI
	UPROPERTY()
	UUI_PlayerSynthesis* M_UUI_PlayerSynthesis = nullptr;
	//任意UI
	UPROPERTY()
	UWidget* M_Other_UI = nullptr;
private:
	//物品数据(指向玩家背包里面的数据)
	UPROPERTY()
	FMaterialBase MaterialBaseCopyData;
	UPROPERTY()
	FEquipmentBase EquipmentBaseCopyData;
private:
	//索引(可以是UI索引可以是物品数据索引)
	int32 M_Index = -1;
private:
	//当前按钮点击
	UPROPERTY()
	UButton* M_Button = nullptr;
	//图片等级->表示材料的等级
	UPROPERTY()
	UImage* M_MaterialGrade = nullptr;
public:
	void SetIndex(int32 _Index);
public:
	int32 GetIndex();
public:
	//显示的道具数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* M_CountText = nullptr;
public:
	virtual bool Initialize() override;
	//设置合成屋UI的指向
	void SetUI_PlayerSynthesis(UUI_PlayerSynthesis* _UUI_PlayerSynthesis);
	//设置背包UI的指向
	void SetUI_PlayerBag(UUI_PlayerBag* _UUI_PlayerBag);
	//设置任意UI的指向
	void SetUI_Other(UWidget* _Other_UI);
	//设置物品基本数据
	void SetMaterialData(FMaterialBase _Data);
	void SetEquipmentBase(FEquipmentBase _Data);

	//设置物品等级
	UFUNCTION(BlueprintCallable)
	void SetItemGrade(const FString& _GradePath);
public:
	//获取物品基本数据
	FMaterialBase* const GetMaterialData();
	//播放动画
	void PlayAnimation_1();
public:
	//获取按钮
	UFUNCTION(BlueprintCallable)
	UButton* GetButton();
	//获取物品名称
	//UFUNCTION(BlueprintCallable)
	//	FString GetItemName();
	virtual FString ToString_Implementation() override;
public:
	//更新物品显示的数量
	UFUNCTION(BlueprintCallable)
	void  UpdateMaterialsShowCount(FString _CountText);
public:

	/*----------------------------------------道具-描述----------------------------------------*/
	UFUNCTION()
	void ShowInformation();
	/*----------------------------------------道具-描述----------------------------------------*/


	/*----------------------------------------卡片制作----------------------------------------*/
	//将合成配方的信息存储，并且修改合成界面的某些显示图片
	UFUNCTION()
	void AddSynthesisBlueprint();
	//添加到合成屋香料槽位
	UFUNCTION()
	void AddSynthesisSpicesSlot();
	/*----------------------------------------卡片制作----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	//添加到卡片强化四叶草槽位
	UFUNCTION()
	void AddUpGradeCardCloverSlot();
	/*----------------------------------------卡片强化----------------------------------------*/



	/*----------------------------------------添加武器-宝石到槽【武器宝石镶嵌】----------------------------------------*/
	//选择当前武器到槽
	UFUNCTION()
	void AddWepaonToSlot();
	//选择当前宝石到槽
	UFUNCTION()
	void AddGemToSlot();
	/*----------------------------------------添加武器-宝石到槽【武器宝石镶嵌】----------------------------------------*/


	/*----------------------------------------添加武器-宝石到槽【宝石强化】----------------------------------------*/
	//选择当前武器到槽
	UFUNCTION()
	void GemUp_AddWepaonToSlot();
	//选择当前宝石到槽
	UFUNCTION()
	void GemUp_AddGemToSlot();
	//选择四叶草
	UFUNCTION()
	void GemUp_AddColoverRate();
	/*----------------------------------------添加武器-宝石到槽【宝石强化】----------------------------------------*/

	/*----------------------------------------添加宝石-宝石到槽【宝石分解】----------------------------------------*/
	//添加宝石到槽
	UFUNCTION()
	void GemSlip_AddGemToSlot();
	/*----------------------------------------添加宝石-宝石到槽【宝石分解】----------------------------------------*/

	/*----------------------------------------（兑换和分解UI）添加当前分解的道具----------------------------------------*/
	//添加到分解槽
	UFUNCTION()
	void AddExchangeSplit_SplitSlot();
	/*----------------------------------------（兑换和分解UI）添加当前分解的道具----------------------------------------*/


	/*----------------------------------------道具使用-防御卡技能书----------------------------------------*/
public:
	//弹出提示框（询问是否使用）
	UFUNCTION()
	void UseCardSkillBook();
	//使用技能书->并且重新加载列表
	UFUNCTION()
	void UseSkillBook();
public:
	//使用得到的技能书数据
	bool UseSkillBookData(FCardSkillBookMaterial _Data, int32 _BooksCount);
public:
	/*----------------------------------------道具使用-防御卡技能书----------------------------------------*/
};
