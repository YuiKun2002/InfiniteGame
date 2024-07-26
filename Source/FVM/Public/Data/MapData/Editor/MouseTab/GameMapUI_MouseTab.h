// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameMapUI_MouseTab.generated.h"

/**
 * 老鼠选项卡【编辑老鼠出场】
 */

class UFVMEditUI_GameMapEdit;
class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;

UCLASS()
class FVM_API UGameMapUI_MouseTab : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	//初始化老鼠选项
	UFUNCTION(BlueprintCallable)
		void InitMouseTab(UFVMEditUI_GameMapEdit* Class);

	//初始化配置
	UFUNCTION(BlueprintImplementableEvent)
		void InitData(FMouseConfig MouseConfig);

	//设置配置
	UFUNCTION(BlueprintCallable)
		void SetConfig(FMouseConfig MouseConfig);

	//获取配置
	UFUNCTION(BlueprintCallable)
		FMouseConfig GetConfig();

	//获取配置引用
	FMouseConfig& GetConfigRef();
	//更新视图
	void UpdateView();
public:

	//获取编辑器
	UFUNCTION(BlueprintCallable)
		UFVMEditUI_GameMapEdit* GetEditor();

	//获取老鼠的数据表
	UFUNCTION(BlueprintCallable)
		UDataTable* GetMouseData();

private:
	//编辑器
	UPROPERTY()
		UFVMEditUI_GameMapEdit* FVMEditUI_GameMapEdit = nullptr;
	//老鼠配置缓存
	UPROPERTY()
		FMouseConfig MouseConfigBuffer;
	//所有的老鼠
	UPROPERTY()
		UDataTable* MouseData = nullptr;
};
