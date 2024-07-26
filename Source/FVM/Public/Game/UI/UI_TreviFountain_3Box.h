// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Game/UI/UI_TreviFountainItemBoxBase.h"
#include "UI_TreviFountain_3Box.generated.h"

/**
 *
 */

class UImage;

UCLASS()
class FVM_API UUI_TreviFountain_3Box : public UUI_TreviFountainItemBoxBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UImage* M_Bg = nullptr;
	UPROPERTY()
		UImage* M_Head = nullptr;
public:
	//界面初始化
	bool Initialize() override;
	//设置背景
	void SetBgStyle(uint8 _value);
	//设置头像
	void SetHead(const FString& _path);
};
