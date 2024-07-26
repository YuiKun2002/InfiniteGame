// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameMapUI_LevelTab.generated.h"

class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;

//主界面
class UFVMEditUI_GameMapEdit;

UCLASS()
class FVM_API UGameMapUI_LevelTab : public UUserWidget
{
	GENERATED_BODY()

	friend class UFVMEditUI_GameMapEdit;

public:

	//初始化
	virtual bool Initialize() override;

	//初始化
	void InitLevelTab(UFVMEditUI_GameMapEdit* Class);

public:
	//获取配置数据
	FLevelConfig& GetConfig();

public:
	//初始化数据表
	UFUNCTION(BlueprintImplementableEvent)
		void InitData(FLevelConfig Config);
	//修改数据表
	UFUNCTION(BlueprintCallable)
		void LevelConfigChanged(FLevelConfig Config);
private:
	//关卡配置缓存
	UPROPERTY()
		FLevelConfig LevelConfigBuffer;

private:
	//编辑器
	UPROPERTY()
		UFVMEditUI_GameMapEdit* FVMEditUI_GameMapEdit = nullptr;
};
