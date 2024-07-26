// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/DataTable.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "UI_GameTask.generated.h"

/**
 * 任务系统
 */

class UImage;
class UTextBlock;
class UScrollBox;
class URichTextBlock;
class UUniformGridPanel;


// WidgetBlueprint'/Game/Resource/BP/Game/UI/GameTask/BP_TaskItem.BP_TaskItem'

//奖励条目
UCLASS()
class FVM_API UUI_GameTaskItemShow : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化组件
	virtual bool Initialize() override;
	//设置参数
	void Set(const FSoftObjectPath& ImagePath, int32 ItemCount, FString ItemName);
private:
	UPROPERTY()
		UImage* BgImg = nullptr;
	UPROPERTY()
		UTextBlock* NameText = nullptr;
	UPROPERTY()
		UTextBlock* CountText = nullptr;
};


//任务条目
UCLASS()
class FVM_API UUI_GameTaskItem : public UWidgetBase
{
	GENERATED_BODY()

		friend class UUI_GameTask;

public:
	//初始化组件
	virtual bool Initialize() override;
	//设置状态
	void SetTaskState(UUI_GameTask* CurTaskUI, FTaskIndex LTaskIndex, uint8 LTaskState, FString STaskName, FString STaskOtherTitle);
	//选择
	UFUNCTION(BlueprintCallable)
		void Select();
private:
	//主线图标
	UPROPERTY()
		UImage* MainTaskImg = nullptr;
	//任务状态
	UPROPERTY()
		UImage* TaskStateImg = nullptr;
	//任务标题
	UPROPERTY()
		UTextBlock* TaskTitle = nullptr;
	//任务副标题
	UPROPERTY()
		UTextBlock* TaskOtherTitle = nullptr;
protected:
	UPROPERTY()
		FTaskIndex TaskIndex;
	UPROPERTY()
		UUI_GameTask* TaskUI = nullptr;
};

//任务
UCLASS()
class FVM_API UUI_GameTask : public UWidgetBase
{
	GENERATED_BODY()

		friend class UUI_GameTaskItem;

public:
	UPROPERTY()
		UTextBlock* TTaskTitle = nullptr;
	UPROPERTY()
		UTextBlock* TTaskContent = nullptr;
	UPROPERTY()
		UImage* UTaskState = nullptr;
	UPROPERTY()
		UImage* UTaskFinishStateImage = nullptr;
	UPROPERTY()
		URichTextBlock* TTaskTarget = nullptr;
	UPROPERTY()
		UTextBlock* TTaskEx = nullptr;
	UPROPERTY()
		UTextBlock* TTaskCoin = nullptr;
	UPROPERTY()
		UButton* TaskFinishBut = nullptr;
public:
	UPROPERTY()
		int32 PlayerEx = 0;
	UPROPERTY()
		int32 PlayerCoin = 0;
public:
	UPROPERTY()
		class UUI_GameTaskItem* CurSelect = nullptr;
	UPROPERTY()
		class UUI_GameTaskItem* CurListSelect = nullptr;
public:
	//初始化组件
	virtual bool Initialize() override;
	//初始化内容
	void InitContent();

	//领取奖励
	UFUNCTION(BlueprintCallable)
		void GetTaskItems();
protected:
	//任务
	UPROPERTY()
		TArray<FTaskBase> TaskItems;
	UPROPERTY()
		UItemLoadManager* TaskTimeLoadManager = nullptr;
	UPROPERTY()
		UScrollBox* TaskListScroll = nullptr;
	UPROPERTY()
		UUniformGridPanel* TaskListUniform = nullptr;
	//创建任务列表
	UFUNCTION()
		UWidget* WidgetCreateTaskList(UItemDataTable* _Data, int32 _Index);
	//刷新任务列表
	UFUNCTION()
		void WidgetRefreshTaskList(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//任务奖励
	UPROPERTY()
		TArray<FTaskItemStruct> TaskItemDatas;
	//执行的任务函数
	UPROPERTY()
		TSoftClassPtr<UTaskFuncObjectExe> TaskFuncObjectExe;
	UPROPERTY()
		UItemLoadManager* TaskItemTimeLoadManager = nullptr;
	UPROPERTY()
		UScrollBox* TaskItemListScroll = nullptr;
	UPROPERTY()
		UUniformGridPanel* TaskItemListUniform = nullptr;
	//创建任务列表
	UFUNCTION()
		UWidget* WidgetCreateTaskItemList(UItemDataTable* _Data, int32 _Index);
	//刷新任务列表
	UFUNCTION()
		void WidgetRefreshTaskItemList(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	UFUNCTION()
		void LoadItems(const TArray<FTaskItemStruct>& Items);
};
