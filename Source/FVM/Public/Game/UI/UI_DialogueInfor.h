// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/DataTable.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_DialogueInfor.generated.h"


USTRUCT()
struct FDialogueShowInforDataTable : public FTableRowBase {
	GENERATED_BODY()
public:
	//剧情标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DialogueTitle;
	//剧情目标位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DialogueTarget;
public:
	FDialogueShowInforDataTable() {}
	FDialogueShowInforDataTable(const FString& S1, const FString& S2)
		:DialogueTitle(S1), DialogueTarget(S2)
	{
	}
};

/**
 * 对话显示信息列表项
 */
UCLASS()
class FVM_API UUI_DialogueListItem : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
private:
	UPROPERTY()
		class UTextBlock* Title = nullptr;
	UPROPERTY()
		class UTextBlock* Content = nullptr;
public:

	void Init(const FString& T1, const FString& T2, class UUI_DialogueInfor* TUI);

	UFUNCTION(BlueprintCallable)
		void Cancel();

	//支线背景
	UFUNCTION(BlueprintImplementableEvent)
		void MainBG();
	//主线背景
	UFUNCTION(BlueprintImplementableEvent)
		void PlayerMainBG();
	//支线背景
	UFUNCTION(BlueprintImplementableEvent)
		void OtherBG();
private:
	UPROPERTY()
		class UUI_DialogueInfor* UI = nullptr;
};

/**
 * 对话显示信息
 */
UCLASS()
class FVM_API UUI_DialogueInfor : public UWidgetBase
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
		void Load();
private:
	//加载器
	UPROPERTY()
		class UItemLoadManager* TimeLoad_Dia_Manager = nullptr;
	//
	UPROPERTY()
		class UUniformGridPanel* UniformDia = nullptr;
	//
	UPROPERTY()
		class UScrollBox* ScrollBoxDia = nullptr;
protected:
	//创建界面
	UFUNCTION()
		UWidget* WidgetCreate_Dia(UItemDataTable* _Data, int32 _Index);
	//刷新卡片界面
	UFUNCTION()
		void WidgetRefreshDia(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
private:
	UPROPERTY()
		TArray<FDialogueShowInforDataTable> DialogueShowInforDataTable;
};
