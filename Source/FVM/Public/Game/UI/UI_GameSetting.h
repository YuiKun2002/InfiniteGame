#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_GameSetting.generated.h"

//结构-设置配置映射
USTRUCT(BlueprintType)
struct FSetting_DataTabel : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//选项卡名称
	UPROPERTY(EditDefaultsOnly)
		FString M_TabName = TEXT("媒体设置");
	//对应的蓝图引用路径
	UPROPERTY(EditDefaultsOnly)
		FString M_TargetBlueprintRefPath = TEXT("");
};

class UUI_SettingButtonTab;
class UScrollBox;

UCLASS()
class FVM_API UUI_GameSetting : public UWidgetBase
{
	GENERATED_BODY()
private:
	//左边列表
	UPROPERTY()
		UScrollBox* M_UScrollBox_Content = nullptr;
	//主要的显示界面
	UPROPERTY()
		UScrollBox* M_UScrollBox_MainContent = nullptr;
private:
	FTimerLoad<UUI_GameSetting, FSetting_DataTabel> WidgetLoadding;
public:
	//界面加载
	void LoaddingFunc(UUI_GameSetting* _Widget, TArray<FSetting_DataTabel>& _Datas, int32 _Index);
	//加载结束
	void LoaddingEnd(UUI_GameSetting* _Widget);
private:
	//按钮集合
	UPROPERTY()
		TArray<UUI_SettingButtonTab*> M_ButtTab;
public:
	//初始化
	bool Initialize() override;
public:
	//加载
	UFUNCTION(BlueprintCallable)
		void LoadWidgets();
	//刷新界面
	UFUNCTION(BlueprintCallable)
		void Refresh();
};
