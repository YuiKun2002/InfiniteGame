// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/MapBaseType.h"
#include "Components/ActorComponent.h"
#include "MesheControllComponent.generated.h"

class UGameMapStructManager;
class AMapLauncherBase;
class AMapMouseMesheManager;
class AMapMeshe;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMesheControllComponent :public UActorComponent
{
	GENERATED_BODY()

public:

	UMesheControllComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//设置游戏地图配置
	UFUNCTION(BlueprintCallable)
	UGameMapStructManager* SetGameMapStructManager(UGameMapStructManager* _GameMapStructManager);
	//获取游戏地图配置
	UFUNCTION(BlueprintCallable)
	UGameMapStructManager* const GetGameMapStructManager();
	//删除所有的网格
	UFUNCTION(BlueprintCallable)
	void ClearAllMeshes();
	//生成网格
	UFUNCTION(BlueprintCallable)
	void SpawnMeshes();

	//获取所有的网格
	TArray<AActor*> GetAllMapMeshes();

	//设置网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	void SetMapMesh(AMapMeshe* NewMapMeshe, int32 _Row, int32 _Col);
	//获取网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMesh(int32 _Row, int32 _Col);
	//通过线路获取网格忽略被禁用的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMeshByLine(int32 _Row, int32 _Col);
	//获取当前网格的最顶部网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetMapMeshCurrentRight(int32 Row);
	//获取当前行，含有卡片的网格
	UFUNCTION(BlueprintCallable, Category = "AMapMesh")
	AMapMeshe* GetHasCardMeshe(int32 Row);

	//获取老鼠网格管理器
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	AMapMouseMesheManager* GetMapMouseMesh(int32 _Row, int32 _Col);
	//获取当前网格的最顶部网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	AMapMouseMesheManager* GetMapMouseMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	AMapMouseMesheManager* GetMapMouseMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	AMapMouseMesheManager* GetMapMouseMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	AMapMouseMesheManager* GetMapMouseMeshCurrentRight(int32 Row);
	//创建的网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	static AMapMeshe* CreateNewMapMeshe(
		UMesheControllComponent* Comp, //网格控制组件
		FTransform Trans,//网格位置
		ELineType LineType,//网格线路类型
		int32 Translucy,//网格渲染层
		FLine Line,//线路
		const TArray<FMesheColPrePlayCard>& PlayCard,//预放置卡片
		bool bCreateBG,//是否创建背景
		FSoftObjectPath BgPath,//背景路径
		FVector BgOffset,//背景偏移
		bool bInitMeshe = true//是否初始化到网格控制器组件中
	);
	//替换网格类型
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	static AMapMeshe* ReplaceMapMeshe(FLine Line, ELineType LineType);
	//替换所有网格
	UFUNCTION(BlueprintCallable, Category = "MouseMeshManager")
	static void ReplaceAllMapMeshe(ELineType CurLineType, ELineType TargetLineType);

	//获取地图发射器
	UFUNCTION(BlueprintCallable)
	AMapLauncherBase* GetMapLauncher();
public:
	//初始化网格地图
	UFUNCTION()
	void InitMeshesMap(const int32& _Row, const int32& _Col);
	//初始化网格
	UFUNCTION()
	void InitMesh(AActor* _Mesh, const FVector& _MeshLocation);
	//获取网格
	UFUNCTION(BlueprintCallable)
	AMapMeshe* GetMesh(const int32& _Row, const int32& _Col);
	//获取网格的坐标
	UFUNCTION(BlueprintCallable)
	FVector GetMapMeshLocation(const int32& _Row, const int32& _Col);
	//获取地图网格的总行和总列
	UFUNCTION(BlueprintCallable)
	FLine GetMapMeshRowAndCol();
	//获取网格数量
	UFUNCTION(BlueprintCallable)
	int32 GetMesheCount() const;
	//检测网格
	UFUNCTION(BlueprintCallable)
	bool CheckMesheIsValid(const int32& _Row, const int32& _Col);
	//显示提示网格
	UFUNCTION(BlueprintCallable)
	void ShowTipMeshe(ELineType LineType);
	//关闭提示网格
	UFUNCTION(BlueprintCallable)
	void CloseTipMeshe();
public:
	//每一行的最后一个的位置
	UPROPERTY(EditAnywhere)
	TArray<FVector> M_LineLastLocation;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//初始化地图
	void InitGameMapMeshe();
private:
	//地图配置管理器
	UPROPERTY(EditAnywhere)
	UGameMapStructManager* M_GameMapStructManager = nullptr;
	//地图的网格
	UPROPERTY(EditAnywhere)
	UClass* M_AMapMeshe = nullptr;
	//地图的发射器
	UPROPERTY(EditAnywhere)
	AMapLauncherBase* M_MapLauncher = nullptr;
private:
	//当前地图的总行和总列
	UPROPERTY()
	FLine M_CurMapLine;
	//当前所有网格合集
	UPROPERTY()
	TArray<AActor*> M_CurMeshes;
	//当前所有网格的位置
	UPROPERTY()
	TArray<FVector> M_CurMeshesLocation;
	//当前所有的老鼠网格管理器
	UPROPERTY()
	TArray<AMapMouseMesheManager*> M_CurMapMouseMesheManager;
	//提示网格
	UPROPERTY()
	TArray<AActor*> M_CurTipMeshes;
	//是否显示
	UPROPERTY()
	bool bShowTip = false;
	//当前提示的网格类型
	UPROPERTY()
	ELineType ShowTipLineType = ELineType::DisEnable;
};
