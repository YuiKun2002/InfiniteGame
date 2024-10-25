// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/GameMapStructManager.h"
#include "UI_RoomCreate.generated.h"

/**
 * 房间创建
 */

class UImage;
class UTextBlock;
class UVerticalBox;

DECLARE_DELEGATE(FSelctDelegate);

UCLASS()
class FVM_API UUI_RoomCreateRoomListItem : public UWidgetBase
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	void Init(class UUI_RoomCreate* NewCurRoom);

	void SetData(FString SNumber, FString SRoomName, int32 ISex);

	UFUNCTION(BlueprintCallable)
	void Select();

	void Cancel();
public:

	FSelctDelegate OnSelctDelegate;

private:
	//当前角色性别
	UPROPERTY()
	UImage* SexImg = nullptr;
	//选择框
	UPROPERTY()
	UImage* SelectBoxImg = nullptr;
	//房间号
	UPROPERTY()
	UTextBlock* RoomNumberText = nullptr;
	//房间名称
	UPROPERTY()
	UTextBlock* RoomNameText = nullptr;
	//房间对象
	UPROPERTY()
	class UUI_RoomCreate* CurRoom = nullptr;
};

UCLASS()
class FVM_API UUI_RoomCreate : public UWidgetBase
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void Init(FString MapName);
private:
	//地图背景
	UPROPERTY()
	UImage* MapBg = nullptr;
	//默认老鼠显示
	UPROPERTY()
	UImage* MouseDefImg = nullptr;
public:
	
	//地图标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MapTitle;

	//简单选项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Easy;

	//困难选项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Hard;

	UPROPERTY()
	TArray<UUI_RoomCreateRoomListItem*> Alls;
	UPROPERTY()
	FLevelConfig SourceData;
public:
	//房间1
	UFUNCTION(BlueprintCallable)
	void Room1();
	//房间2
	UFUNCTION(BlueprintCallable)
	void Room4();
};
