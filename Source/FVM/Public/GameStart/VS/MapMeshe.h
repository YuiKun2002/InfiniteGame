// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/MapBaseType.h"
#include "MapMeshe.generated.h"

class UMapMesheWidgetComponent;
class UWidgetComponent;
class USphereComponent;
class UUI_MapMeshe;
class AMapMeshe;
class AMouseActor;
class ASpriteActor;

USTRUCT(BlueprintType)
struct FAMapMeshType {
	GENERATED_USTRUCT_BODY()
public:
	//当前线路
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_Line;
	//格子类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType;
};


//老鼠网格管理器【只会管理老鼠】
UCLASS()
class FVM_API AMapMouseMesheManager : public AActor
{
	GENERATED_BODY()
public:
	//盒体重叠开始
	UFUNCTION(BlueprintCallable)
		void BoxOverlapBegin(AActor* OtherActor);
	//盒体重叠结束
	UFUNCTION(BlueprintCallable)
		void BoxOverlapEnd(AActor* OtherActor);
public:
	const TMap<FString, AMouseActor*>& GetCurMouse() const;
public:
	//设置线路
	void SetLine(FLine Line);
private:
	//当前的线路
	UPROPERTY(EditAnywhere)
		FLine M_Line;
	//当前网格拥有的老鼠集合
	UPROPERTY(EditAnywhere)
		TMap<FString, AMouseActor*> M_CurMouses;
};

//单播委托
//【当卡片移动时】
DECLARE_MULTICAST_DELEGATE_FourParams(FOnCardMovedUpdate, const float& DeltaTime, const int32& _Layer, const FVector& _Location, const FLine& _Line);
//当卡片开始移动时
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCardMoveBegin, const int32& _Layer, const FLine& _Line);
//当卡片移动结束时
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCardMoveEnd, const int32& _Layer, const FLine& _Line);

UCLASS()
class FVM_API AMapMeshe : public AActor
{
	GENERATED_BODY()

		friend class UUI_MapMeshe;

public:
	//界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UMapMesheWidgetComponent* M_MapMesheWidgetComponent = nullptr;
	//坐标（点）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		USceneComponent* M_Point = nullptr;
	//一些属性类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		FAMapMeshType M_Type;
	//板块偏移量【水中偏移，陆地偏移，水上偏移等】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		FVector M_PlayerMesheOffset;
	//网格的背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
		ASpriteActor* M_MesheBG = nullptr;
	//盒体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereComp")
		USphereComponent* M_MesheBoxComp = nullptr;
	//格子的大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIMeshSize")
		FVector2D M_MeshSize = FVector2D::ZeroVector;
public:

	AMapMeshe();

	//当卡片更新移动时
	FOnCardMovedUpdate OnCardMovedUpdate;
	//当开始开始移动时
	FOnCardMoveBegin OnCardMoveBegin;
	//当卡片移动结束时
	FOnCardMoveEnd OnCardMoveEnd;
public:
	//初始化网格
	void InitUUI_MapMesh(const ELineType& _LineType, const int32& _Translucency);
	//获取排序优先级
	UFUNCTION(BlueprintCallable)
		int32 GetTranslucency() const;
	//设置排序优先级
	void SetTranslucency(const int32& _Translucency);
private:
	//是否可以使用
	UPROPERTY()
		bool bUse = true;
	//优先级
	UPROPERTY()
		int32 TranslucencyLayer = 0;
	//是否可以移动
	UPROPERTY()
		bool M_bMove = false;
	//移动时间
	UPROPERTY()
		float M_MoveingTime = 0.f;
	//移动总时间
	UPROPERTY()
		float M_CurrentMoveingTime = 0.f;
	//当前位置
	UPROPERTY()
		FLine M_CurrentLine;
	//移动偏移
	UPROPERTY()
		FLine M_MoveingOffSet;
	//移动冷却总时间
	UPROPERTY()
		float M_BeginMoveingCloudDownTime = 0.f;
	//当前移动冷却时间
	UPROPERTY()
		float M_CurrentMoveingCloudDownTime = 0.f;
	//是否已经开始
	UPROPERTY()
		bool M_bBegin = false;
	//当前坐标
	UPROPERTY()
		FVector M_CurrentLocation;
	//目标坐标
	UPROPERTY()
		FVector M_TargetLocation;
	//占用道具，禁用当前网格
	UPROPERTY()
		TMap<FString, UObject*> M_MesheDis;
public:
	//设置网格背景
	UFUNCTION(BlueprintCallable)
		void SetMesheBg(ASpriteActor* _Bg);
	//设置网格移动(每次移动的冷却时间，每次移动到目标的总时间，格子偏移，坐标)
	UFUNCTION(BlueprintCallable)
		void SetMesheMove(float _CloudDownTime, float _Time, FLine _Offset, FVector _TargetLocation);
	//设置路线
	UFUNCTION(BlueprintCallable)
		void SetLine(const FLine& _Line);
	//设置当前格子类型
	UFUNCTION(BlueprintCallable)
		void SetLineType(const ELineType _Type);
	//设置UI组件的位置
	UFUNCTION(BlueprintCallable)
		void SetWidgetComponentTransform(const FTransform& _ActorTransform);
	//设置UI组件旋转
	UFUNCTION(BlueprintCallable)
		void SetUUI_MapMeshRotatonPitchY(const float& _Angle);
	//设置预备放置卡片
	UFUNCTION(BlueprintCallable)
		void SetPrePlayCard(const TArray<FMesheColPrePlayCard>& Cards);
	//设置网格的使用
	UFUNCTION(BlueprintCallable)
		void SetMesheEnable(bool bEnable, UObject* CurObj);
public:
	//获取禁用的网格
	UFUNCTION(BlueprintCallable)
		TArray<UObject*> GetMesheEnableObjs();
	//获取当前板块是否可以使用
	UFUNCTION(BlueprintCallable)
		bool GetbUse();
	//获取板块是否在移动
	UFUNCTION(BlueprintCallable)
		bool GetMove();
	//获取路线
	UFUNCTION(BlueprintCallable)
		FLine GetLine();
	UFUNCTION(BlueprintCallable)
		ELineType GetLineType();
	//'获取UI组件的位置
	UFUNCTION(BlueprintCallable)
		FTransform GetWidgetComponentTransform();
	//获取UImapmeshe
	UFUNCTION(BlueprintCallable)
		UUI_MapMeshe* GetUIMapMeshe();
	//获取角色
	UFUNCTION(BlueprintCallable)
		bool GetPlayer();
	//游戏是否暂停
	UFUNCTION(BlueprintCallable)
		bool GamePause();
	//获取预备放置卡片
	UFUNCTION(BlueprintCallable)
		const TArray<FMesheColPrePlayCard>& GetPrePlayCard() const;
	//获取当前网格是否启用
	UFUNCTION(BlueprintCallable)
		bool GetMesheEnable();
public:
	//复制数据
	UFUNCTION(BlueprintCallable)
		void CopyUIMapMesheData(AMapMeshe* MapMeshe, UUI_MapMeshe* UIMapMeshe);
	//更新角色位置
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerLocation();
	//更新卡片位置
	UFUNCTION(BlueprintCallable)
		virtual	void UpdateCardsLocation(ACardActor* SingleCard = nullptr);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//开始网格移动
	void MesheMoveBegin();
	//开始网格移动
	void MesheMoveEnd();
public:
	//盒体重叠开始
	UFUNCTION()
		void BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//盒体重叠结束
	UFUNCTION()
		void BoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	//四舍五入
	int32 ValueRound(double n);


	//添加卡片移动模式绑定
	UFUNCTION()
		void AddCardMoveModelBind(ACardActor* _CardActor);
	//移除卡片
	UFUNCTION()
		void RemoveCard(ACardActor* _CardActor);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	//当前网格拥有的卡片集合
	UPROPERTY(EditAnywhere)
		TArray<ACardActor*> M_CurMeshCards;

	//网格列预放置卡片
	UPROPERTY()
		TArray<FMesheColPrePlayCard> MesheColPrePlayCard;
};
