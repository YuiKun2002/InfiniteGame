// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/TreviFountainStruct.h"
#include "UI_TreviFountain2.generated.h"

/**
 * //法老抽奖
 */


class UUniformGridPanel;
class UUI_TreviFountain2Box;
class UUI_TreviFountain2Panel;
class UUI_TreviFountain2BoxSelect;
class UTextBlock;
class UButton;

UCLASS()
class FVM_API UUI_TreviFountain2 : public UWidgetBase
{
	GENERATED_BODY()
public:
	//奖池
	UPROPERTY()
		FTreviFountainRandomCategoyStruct M_FTreviFountainRandomCategoyStruct;
	//奖池奖励
	UPROPERTY()
		TArray<FTreviFountainItemStruct> M_FTreviFountainItemStruct;
private:
	//承载抽奖盒子的容器
	UPROPERTY()
		UUniformGridPanel* M_UUniformGridPanel = nullptr;
	//承载自选道具抽奖盒子的容器
	UPROPERTY()
		UUniformGridPanel* M_UUniformGrid_CustomSelect_Panel = nullptr;
public:
	//自选卡片文字描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CustomSelect;
private:
	bool M_bHidd = false;
private:
	//当前自选道具名称
	UPROPERTY()
		FString M_CustonSelectItemName;
	//当前自选道具次数
	UPROPERTY()
		int32 M_CustonSelectItemCount = 0;
	//是否开启自选
	UPROPERTY()
		bool M_bCustonSelectItemEnable = false;
private:
	//8连抽奖界面
	UPROPERTY()
		UUI_TreviFountain2Panel* M_UI_TreviFountain2Panel = nullptr;
	UPROPERTY()
		UButton* M_UButton_Get = nullptr;
	UPROPERTY()
		UTextBlock* M_UTextBlock = nullptr;
	//抽奖次数显示的文字
	UPROPERTY()
		UTextBlock* M_GetItemsCountText = nullptr;

	//抽奖显示的展示盒子
	UPROPERTY()
		TArray<UUI_TreviFountain2Box*> M_Boxs;
	//自选奖励盒子
	UPROPERTY()
		TArray<UUI_TreviFountain2BoxSelect*> M_CustonSelectBoxs;
private:
	//8连抽的默认时候的图片
	UPROPERTY()
		FString M_8GetDefTexture2DPath;
	//8连抽的打开时候的图片
	UPROPERTY()
		FString M_8GetOpenTexture2DPath;
public:
	//界面初始化
	bool Initialize() override;
public:
	//内容初始化(宝箱盒子的路径，奖池的数据表)
	UFUNCTION(BlueprintCallable)
		void ContentInit(const FString& _BoxPath, const FString& _ItemsDataTablePath);
	//初始化抽奖按钮(8连抽的默认时候的图片,8连抽的打开时候的图片)
	UFUNCTION(BlueprintCallable)
		void Init8GetButtonStyle(const FString& _DefPath, const FString& _OpenPath);
public:
	//设置自选道具名称
	UFUNCTION(BlueprintCallable)
		void SetCustonSelectItemName(const FString& _Name);
	//设置自选道具次数
	UFUNCTION(BlueprintCallable)
		void SetCustonSelectItemCount(int32 _Count);
	//设置自选道具是否开启
	UFUNCTION(BlueprintCallable)
		void SetCustonSelectItemEnable(bool _bActive);
public:
	//获取所有的自选礼盒选项卡
	UFUNCTION(BlueprintCallable)
		const TArray<UUI_TreviFountain2BoxSelect*>& GetCustomBoxSelectItems();
	//直接获取保底道具
	UFUNCTION(BlueprintCallable)
		FString GetCustomSelectItem();
	//直接获取保底道具名称
	UFUNCTION(BlueprintCallable)
		FString GetCustomSelectItemName();
public:
	//更新文字描述（需求填入当前抽到的道具名称，触发大保底返回true）
	UFUNCTION(BlueprintCallable)
		bool UpdateCustomSelectText(const FString& _CurrentItemName);	  
public:
	//刷新
	UFUNCTION()
		ETreviFountainItemQualityType GetItems_1(TArray<FTreviFountainItemStruct>& _OutResults);
	//8连抽
	UFUNCTION()
		void GetItems_8();
	//重置
	UFUNCTION()
		void Reset();
	//隐藏
	UFUNCTION()
		void Hidden();
};
