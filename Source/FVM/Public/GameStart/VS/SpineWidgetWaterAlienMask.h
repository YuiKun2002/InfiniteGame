// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Components/ActorComponent.h"
#include "SpineWidgetWaterAlienMask.generated.h"

class UWorld;
class UCanvasPanel;
class USpineWidget;
class UWidgetComponent;

/**
 * 解决陆地外星人入水问题
 */
UCLASS(Blueprintable)
class FVM_API USpineWidgetWaterAlienMask : public UWidgetBase
{
	GENERATED_BODY()
public:
	//设置水面位置
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetWaterViewTransform(FWidgetTransform Trans);
	//设置入水位置
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetInWaterViewTransform(FWidgetTransform Trans);
	//设置遮罩对象位置
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskObjectTransform(FWidgetTransform Trans);
	//获取遮罩对象位置
	UFUNCTION(BlueprintPure, Category = SpineWidgetWaterAlienMask)
	void GetMaskObjectTransform(FWidgetTransform& OutTrans);
	//设置遮罩对象旋转
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskObjectRotation(float Angle);
	//设置遮罩最大高度
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskWidgetMaxHight(float MaxHight);
	//设置遮罩值
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskValue(float Value);
	//设置自动遮罩值
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskAutoValue();
	//设置最大遮罩值
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskMaxValue(float Value, bool bPreview = false);
	//设置遮罩对象颜色
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	void SetMaskObjectRenderColor(FLinearColor NewColor);
public:
	//获取遮罩对象
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	USpineWidget* GetMaskObject();
public:
	//预览
	UFUNCTION(BlueprintCallable, Category = SpineWidgetWaterAlienMask)
	static void PreViewMaskObject(UWidgetComponent* Comp);
	//播放入水动画
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = SpineWidgetWaterAlienMask)
	void PlayInWaterAnimation();
	//播放水动画
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = SpineWidgetWaterAlienMask)
	void PlayWaterAnimation(bool bBegin);
	//播放遮罩对象动画
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = SpineWidgetWaterAlienMask)
	class UTrackEntry* PlayMaskObjectAnimation(const FString& AnimationName);
public:
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:

	//遮罩对象缩放比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	float MaskObjectScale = 1.f;

	//遮罩对象图形数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpineWidgetWaterAlienMask)
	class USpineAtlasAsset* MaskObjectAtlas = nullptr;

	//遮罩对象骨骼数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpineWidgetWaterAlienMask)
	class USpineSkeletonDataAsset* MaskObjectSkeletonData = nullptr;

	//遮罩层
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	UCanvasPanel* MaskWidget = nullptr;

	//遮罩对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	USpineWidget* MaskCharView = nullptr;

	//水动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	USpineWidget* WaterView = nullptr;

	//入水动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	USpineWidget* InWaterView = nullptr;
protected:
	//播放水动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = SpineWidgetWaterAlienMask)
	bool bPlayWaterAnim = false;
private:
	//水动画位置
	UPROPERTY()
	FWidgetTransform CurWaterViewTrans;
	//入水动画位置
	UPROPERTY()
	FWidgetTransform CurInWaterViewTrans;
	//遮罩对象位置
	UPROPERTY()
	FWidgetTransform CurMaskObjectTrans;
	//遮罩最大高度
	UPROPERTY()
	float MaskMaxHight = 0.f;
	//遮罩最大值
	UPROPERTY()
	float MaskMaxValue = 1.f;
};
