// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData/MapDataStruct.h"
#include "FVMEditUI_GameMapEdit.generated.h"

class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;


//关卡配置选项
class UGameMapUI_LevelTab;
//关卡掉落物配置
class UGameMapUI_LevelItemsTab;
//地图配置
class UGameMapUI_MapTab;
//老鼠配置选项
class UGameMapUI_MouseTab;
//编辑器选项
class UGameMapUI_EditorTab;


//实时编辑游戏地图
UCLASS(BlueprintType)
class FVM_API UFVMEditUI_GameMapEdit : public UUserWidget
{
	GENERATED_BODY()

		friend class UFVMEditUI_EditRowTableItem;

public:
	/*
		初始化
	*/
	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	//初始化编辑器配置【右边列表所有的项】
	UFUNCTION(BlueprintCallable)
		void InitEditorConfig();

	//初始化地图配置数据表
	void InitGameMapData();
	//初始化数据列表
	void InitGameMapListItems();
	//初始化编辑配置数据
	void InitCurEditConfig();
private:
	//加载配置
	UFUNCTION(BlueprintCallable)
		void LoadConfig();
	//保存配置
	UFUNCTION(BlueprintCallable)
		void SaveConfig();
public:
	//保存实例对象
	UFUNCTION(BlueprintImplementableEvent)
		void SaveCurPakage();
public:
	//获取数据库表
	UDataTable* const GetGameMapData();
	//获取所有的行名称
	const TArray<FName>& GetGameMapRowNames();
	//获取正在编辑的行名称
	const FName& GetCurEditRowName();
	//获取正在编辑的数据
	FGameMapData& GetCurEditData();

	//获取正在编辑的关卡数据
	FLevelConfig& GetCurEditLevelData();

	//设置编辑器的标题
	void SetEditorTitle(const FText& _Title);
private:
	//地图配置数据表【左侧所有】
	UPROPERTY()
		UDataTable* GameMapData = nullptr;
	//数据表数据行名称
	UPROPERTY()
		TArray<FName> GameMapDataRowNames;
	//正在编辑的行名称
	UPROPERTY()
		FName CurEditRowName;
	//当前正在编辑的数据表行数据
	UPROPERTY()
		FGameMapData CurEditData;
private:
	//显示当前正在编辑的关卡数据名称
	UPROPERTY()
		UTextBlock* Text_CurEditTitle = nullptr;
private:
	//关卡配置选项
	UPROPERTY()
		UGameMapUI_LevelTab* GameMapUI_LevelTab = nullptr;
	//关卡掉落物配置选项
	UPROPERTY()
		UGameMapUI_LevelItemsTab* GameMapUI_LevelItemsTab = nullptr;
	//地图配置选项
	UPROPERTY()
		UGameMapUI_MapTab* GameMapUI_MapTab = nullptr;
	//老鼠配置选项
	UPROPERTY()
		UGameMapUI_MouseTab* GameMapUI_MouseTab = nullptr;
	//编辑器选项
	UPROPERTY()
		UGameMapUI_EditorTab* GameMapUI_EditorTab = nullptr;
};
