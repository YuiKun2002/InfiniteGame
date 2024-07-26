// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameMapUI_LevelItemsTab.generated.h"

/**
 *
 */

class UFVMEditUI_GameMapEdit;
class UGameMapUI_LevelItemsTab;
class UUniformGridPanel;
class UVerticalBox;
class UButton;
class UTextBlock;
class UImage;

//操作界面选项物品
UCLASS()
class FVM_API UGameMapUI_LevelItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(class UGameMapUI_LevelItemsPanel* Class, const FString& CName, const FSoftObjectPath& CHeadPath);
private:
	UPROPERTY()
		class UGameMapUI_LevelItemsPanel* CurClass = nullptr;
	UPROPERTY()
		FString Name;
	UPROPERTY()
		FSoftObjectPath HeadPath;
private:
	UPROPERTY()
		UImage* ShowHead = nullptr;
	UPROPERTY()
		UTextBlock* ShowTex = nullptr;
	UPROPERTY()
		UButton* But = nullptr;
	UPROPERTY()
		UTextBlock* ShowButTex = nullptr;
protected:
	UFUNCTION()
		void Select();
	UFUNCTION()
		void Update();
};

//操作界面选项卡
UCLASS()
class FVM_API UGameMapUI_LevelItemsTabButt : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(class UGameMapUI_LevelItemsPanel* Class, const FString& Name, UDataTable* NewDataTable);

	UFUNCTION(BlueprintCallable)
		void Show();
private:
	//显示标题
	UPROPERTY()
		UTextBlock* ShowTextComp = nullptr;
	//对应数据
	UPROPERTY()
		UDataTable* CurData = nullptr;
	//类
	UPROPERTY()
		class UGameMapUI_LevelItemsPanel* CurClass = nullptr;
};

//操作界面
UCLASS()
class FVM_API UGameMapUI_LevelItemsPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void InitLevelTab(UGameMapUI_LevelItemsTab* Class);
	UFUNCTION(BlueprintCallable)
		UGameMapUI_LevelItemsTab* GetCurClass();
	UFUNCTION()
		UUniformGridPanel* GetDataRange();
public:
	//所有的数据表格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, TSoftObjectPtr<UDataTable>> Tabs;
private:
	UPROPERTY()
		UGameMapUI_LevelItemsTab* CurClass = nullptr;
	//显示的所有数据
	UPROPERTY()
		UUniformGridPanel* DataRange = nullptr;
	//选项卡列表
	UPROPERTY()
		UVerticalBox* DataList = nullptr;
};

//打开界面的按钮
UCLASS()
class FVM_API UGameMapUI_LevelItemsTab : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitLevelTab(UFVMEditUI_GameMapEdit* Class);

	UFUNCTION(BlueprintCallable)
		UFVMEditUI_GameMapEdit* GetCurClass();
private:
	UPROPERTY()
		UFVMEditUI_GameMapEdit* CurClass = nullptr;
};
