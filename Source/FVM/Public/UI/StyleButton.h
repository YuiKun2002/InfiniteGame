// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "StyleButton.generated.h"

/**
 * 样式按钮
 */
UCLASS()
class FVM_API UStyleButton : public UButton
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	float ScaleRate = 0.03f;
protected:
	FReply StyleSlateHandleClicked();
	void StyleSlateHandlePressed();
	void StyleSlateHandleReleased();
	void StyleSlateHandleHovered();
	void StyleSlateHandleUnhovered();
protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
private:
	UPROPERTY()
	bool PressedState = false;
	UPROPERTY()
	bool HoveredState = false;
};
