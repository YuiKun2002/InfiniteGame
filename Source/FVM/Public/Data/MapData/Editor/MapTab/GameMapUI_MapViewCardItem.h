// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMapUI_MapViewCardItem.generated.h"


class UButton;
class UGameMapUI_MapViewEditor;
class UPaperFlipbook;

/**
 * 地图编辑器卡片展示对象
 *
 */

UCLASS()
class FVM_API UGameMapUI_MapViewCardViewPanel : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
		void CardViewPanelInit(const TSoftObjectPtr<UPaperFlipbook>& Anim);
};


/**
 * 地图编辑器卡片选项
 */
UCLASS()
class FVM_API UGameMapUI_MapViewCardItem : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MapViewEditor;

public:
	bool Initialize() override;

public:
	UPROPERTY()
		UButton* CurCardBut = nullptr;
public:
	void Init(const FString& CardName, const FSoftClassPath& ClassPath, const int32& CardLayer, const FSoftObjectPath& Card2dAnim, const FSoftObjectPath& Texture2dHead);
	const FString& GetCardName() const;
	const int32& GetCardLayer() const;
	const FSoftClassPath& GetCardClassPath() const;
	const FSoftObjectPath& GetCardHeadPath() const;
	const FSoftObjectPath& GetCard2DAnimPath() const;
public:
	UFUNCTION(BlueprintCallable)
		void Select();

	void Cancel();
private:
	//卡片名称
	UPROPERTY()
		FString MCardName;
	//实体类
	UPROPERTY()
		FSoftClassPath MClassPath;
	//卡片层级
	UPROPERTY()
		int32 MCardLayer;
	//动画路径
	UPROPERTY()
		FSoftObjectPath MCard2dAnim;
	//头像纹理
	UPROPERTY()
		FSoftObjectPath MTexture2dHead;
	UPROPERTY()
		UGameMapUI_MapViewEditor* GameMapUI_MapViewEditor = nullptr;
};
