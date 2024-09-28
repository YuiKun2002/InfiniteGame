// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameSystem/GameDataSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameUserInterfaceSubsystem.generated.h"

class UWidgetBase;
class UAssetCategoryName;


////UI集合结构体【UI界面映射】
//USTRUCT(BlueprintType)
//struct FGameUserInterfaceGroupTableRowBase : public FTableRowBase {
//	GENERATED_USTRUCT_BODY()
//public:
//	//UI的名称
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName UserInterName;
//
//	//UI的资产
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSoftClassPtr<UWidgetBase> UserInterSource;
//};


//UI结构体【UI配置表{项}】
USTRUCT(BlueprintType)
struct FGameUserInterfaceTableRowBase : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//更新的类条目【完全不建议后期修改名称】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftClassPtr<UWidgetBase>> UI;
};

//UI实例【一个实例包含多个UI界面】
UCLASS()
class FVM_API UGameUserInterfaceInstance : public UObject {
	GENERATED_BODY()
public:
	//UI List Source 对应UI的软类资产引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftClassPtr<UWidgetBase>> UI;
	//UI Insatnce 已经加载的UI实例对象集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UWidgetBase*> InstanceUI;
public:
	//新增临时UI，游戏结束释放，不会被保存
	UFUNCTION(BlueprintCallable, Category = "新增UI")
	bool AddTempNewUI(FName NewName, TSoftClassPtr<UWidgetBase> Resource);

	//通过名称获取UI
	UFUNCTION(BlueprintPure, Category = "获取UI")
	UWidgetBase* GetUI(FName Name);
	//通过名称获取UIClass对象
	UFUNCTION(BlueprintPure, Category = "获取UI")
	UClass* GetUIClass(FName Name);
	//通过名称获取已经被实例化的UI
	UFUNCTION(BlueprintPure, Category = "获取UI")
	UWidgetBase* GetInstanceUI(FName Name);
	//查询显示的UI
	UFUNCTION(BlueprintPure, Category = "获取UI")
	UWidgetBase* GetIsInViewportUI_ByName(FName Name);

	//通过名称默认【无函数调用】显示UI
	UFUNCTION(BlueprintCallable, Category = "显示UI")
	void ShowUI_ByName(FName Name, int32 ZOrder = 0);
	//通过对象直接显示UI
	UFUNCTION(BlueprintCallable, Category = "显示UI")
	bool ShowUI(UWidgetBase* WidgetInstance, int32 ZOrder = 0);

	//通过名称默认【无函数调用】移除显示UI
	UFUNCTION(BlueprintCallable, Category = "移除UI显示")
	void RemoveUI_ByName(FName Name);
	//通过对象直接移除显示UI
	UFUNCTION(BlueprintCallable, Category = "移除UI显示")
	void RemoveUI(UWidgetBase* WidgetInstance);

	/*
		通过名称直接释放UI
		@Param1 UI的唯一名称
		@Param2 不管是否在显示，强行释放
	*/
	UFUNCTION(BlueprintCallable, Category = "释放UI")
	bool UnloadUI_ByName(FName Name, bool bForceUnload = false);
	//释放所有没有显示的UI资源
	UFUNCTION(BlueprintCallable, Category = "释放UI")
	void UnloadAllNotShowResource();
	//强制释放全部的UI资源【如果有显示的UI也会被关闭】
	UFUNCTION(BlueprintCallable, Category = "释放UI")
	void ForceUnloadAllResource();

	//【系统结束】释放
	UFUNCTION()
	void Unload();
};

/**
 * 全局UI子系统
 */
UCLASS()
class FVM_API UGameUserInterfaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//数据初始化
	UFUNCTION()
	void InitializeUserInterData(class UGameDataAsset* DataAsset);

	//获取子系统
	UFUNCTION(BlueprintPure, Category = "UI子系统")
	static UGameUserInterfaceSubsystem* GetGameUserInterfaceSubsystemStatic();
	//获取名称
	UFUNCTION(BlueprintPure, Category = "UI子系统 | 获取UI")
	FName GetUserInterCategoryName(TSoftClassPtr<UAssetCategoryName> ObjectType);
	//通过Name获取UI实例【使用前请确保Name有效】
	UFUNCTION(BlueprintPure, Category = "UI子系统 | 获取UI")
	UGameUserInterfaceInstance* GetUserInterInstance(FName Name);
	//新增临时UI实例,游戏结束释放，不会被保存
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 新增UI")
	bool AddTempUserInterInstance(FName Name);
	//直接移除UI实例并且释放当前实例的所有UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 移除UI")
	bool RemoveUserInterInstance(FName Name);
	//直接释放全部界面并且未显示的UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 释放UI")
	void UnloadAllNotShowResource();
	//直接释放全部的界面
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 释放UI")
	void UnloadAllResource();
public:

	/*
		其他处理的UI，便捷调用函数
	*/

	//显示情报岛UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 情报岛UI")
	static void ShowGameInformationUISub(class UWidget* Widget);

	//显示任务UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 任务UI")
	static void ShowGameTaskUISub(class UWidget* Widget);
	//任务UI是否显示
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 任务UI")
	static bool GetGameTaskUIInViewportSub();
	//关闭任务UI的显示
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 任务UI")
	static void RemoveGameTaskUIViewportSub();
	//显示任务完成提示UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 任务UI")
	static void ShowTaskFinishTipUISub(class UWidget* Widget, class UGameInstance* Ins = nullptr);

	//显示新关卡UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 新增关卡提示UI")
	static void ShowNewLevelTipUISub(class UWidget* Widget);

	//显示新得到的物品UI提示
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 新增物品提示UI")
	static void ShowNewItemShowPanel(class UWidget* Widget, const TArray<FTaskItemStruct>& Data);

	//显示剧情进度UI
	UFUNCTION(BlueprintCallable, Category = "UI子系统 | 剧情进度UI")
	static void ShowDialogueInforUISub(class UWidget* Widget);

public:
	//初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//取消初始化
	virtual void Deinitialize() override;
public:
	//全部的UI资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UGameUserInterfaceInstance*> Resource;
};
