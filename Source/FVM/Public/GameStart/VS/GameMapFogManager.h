// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameMapFogManager.generated.h"

/**
 * 迷雾管理器
 */

class AGameMapInstance;

//迷雾块
UCLASS()
class FVM_API AGameMapFogBlock : public AActor
{
	GENERATED_BODY()
public:
	//显示事件
	UFUNCTION(BlueprintImplementableEvent)
		void Dispaly(bool Value);
	//时间迷雾消失[强行消失一段时间]
	UFUNCTION(BlueprintImplementableEvent)
		void TimeDispaly(float time);
	//设置渲染层
	UFUNCTION(BlueprintImplementableEvent)
		void SetTrans(int32 Trans);
	//获取显示结果
	UFUNCTION(BlueprintCallable)
		bool GetDisplayResult();
public:
	void SetObj(UObject* NewObj, bool bAdd);
	void TimeHidd(float time);
private:
	UPROPERTY()
		TArray<UObject*> Objs;
	UPROPERTY()
		bool bInit = false;
};

//迷雾管理器
UCLASS()
class FVM_API UGameMapFogManager : public UObject
{
	GENERATED_BODY()
public:
	UGameMapFogManager();

	//初始化全部迷雾块，
	UFUNCTION(BlueprintCallable)
		void InitAllBlock();
	//显示单个迷雾
	UFUNCTION(BlueprintCallable)
		void SingleBlock(UObject* NewObj, bool bShow, int32 RowIndex, int32 ColIndex);
	//范围显示[Row,Col]
	UFUNCTION(BlueprintCallable)
		void RangeBlock(UObject* NewObj, bool bShow, FLine RangeRow, FLine RangeCol);
	//全屏设置
	UFUNCTION(BlueprintCallable)
		void FullFogBlock(UObject* NewObj, bool bShow);
	//全屏时间消失
	UFUNCTION(BlueprintCallable)
		void FullTimeHiddBlock(float time);
private:
	//所有存在的迷雾块
	UPROPERTY()
		TArray<AGameMapFogBlock*> AllFogBlocks;
	//当前行列
	UPROPERTY()
		FLine CurRowAndCol;
	//初始化
	UPROPERTY()
		bool bInit = false;
};