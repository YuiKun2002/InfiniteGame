// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_GamePlayerHead.generated.h"

/**
 * 角色显示头像
 */

class UImage;
class UCanvasPanel;
class UTextBlock;

UCLASS()
class FVM_API UUI_GamePlayerHead : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void Init(bool bMainPlayer, const FSoftObjectPath& OtherHead, int32 Grade, const FString& PlayerName);

	UFUNCTION(BlueprintImplementableEvent)
		void OnMainPlayer();
private:
	UPROPERTY()
		UCanvasPanel* MainCharPoint = nullptr;
	UPROPERTY()
		UImage* OtherCharHead = nullptr;
	UPROPERTY()
		UImage* CharGradeHead = nullptr;
	UPROPERTY()
		UTextBlock* CharName = nullptr;
};
