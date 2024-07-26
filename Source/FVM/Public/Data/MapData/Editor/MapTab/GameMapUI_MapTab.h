// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameMapUI_MapTab.generated.h"

/**
 *
 */

class UGameMapUI_LevelTab;
class UFVMEditUI_GameMapEdit;

class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;


UCLASS()
class FVM_API UGameMapUI_MapTab : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MapViewEditor;

public:

	virtual bool Initialize() override;

	void InitMapTab(UFVMEditUI_GameMapEdit* Class);

	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();

public:

	//获取编辑器
	UFUNCTION(BlueprintCallable)
		UFVMEditUI_GameMapEdit* GetEditor();
	//获取地图选项卡
	UFUNCTION(BlueprintCallable)
		UGameMapUI_MapTab* GetMapTab();

	//更新配置
	UFUNCTION(BlueprintCallable)
		void UpdateConfig(FGameMapStruct GameMapStruct);

	//获取配置
	UFUNCTION(BlueprintCallable)
		FGameMapStruct GetConfig();

public:

	//获取配置引用
	FGameMapStruct& GetConfigRef();

private:
	//编辑器
	UPROPERTY()
		UFVMEditUI_GameMapEdit* EditorClass = nullptr;
	//地图配置缓存
	UPROPERTY()
		FGameMapStruct CurGameMapStructBuffer;
};
