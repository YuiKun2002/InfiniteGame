// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/CardSkillBookStruct.h"
#include "UI_CardSkillTable.generated.h"

/**
 *
 */

class UImage;

UCLASS()
class FVM_API UUI_CardSkillTable : public UWidgetBase
{
	GENERATED_BODY()
private:
	//头像
	UPROPERTY()
		UImage* M_Head = nullptr;
	//进度条
	UPROPERTY()
		UImage* M_Pro = nullptr;
public:
	//名称
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Name = FString(TEXT(""));
	//提示
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Tip = FString(TEXT(""));
	//等级
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Lv = FString(TEXT(""));
	//经验
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_Ex = FString(TEXT(""));
public:
	//初始化
	bool Initialize() override;
	//初始数据
	void InitData(FSkillBookData _Data);
	//动画
	void PlayAnimation_1();
public:
	//加载数据
	UFUNCTION(BlueprintCallable)
		void LoadData();
};
