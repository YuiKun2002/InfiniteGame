// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "GameSystem/Tools/GameSystemFunction.h"

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

//动画
#include <Paper2D/Classes/PaperFlipbook.h>

//数据加载
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

//数据表
#include "Engine/Classes/Engine/DataTable.h"
//游戏实例
#include "GameSystem/FVMGameInstance.h"
//地图结构
#include "GameSystem/GameMapStructManager.h"
//地图里面的UI网格
#include "GameStart/VS/UI/UI_MapMeshe.h"
//游戏配置
#include "GameSystem/GameConfigManager.h"
//游戏地图基础结构
#include "GameStart/VS/MapBaseType.h"
//老鼠结构
#include "GameSystem/MouseStructManager.h"
//老鼠对象
#include "GameStart/Flipbook/GameActor/MouseActor.h"
//UI主框架
#include "Game/UI/UI_MainFramework.h"
//物品结构
#include "GameSystem/Item/ItemStruct.h"
//商城道具购买结构
#include "Data/ShopItemPriceStruct.h"
//商城道具格子
#include "Game/UI/UI_TicketShopGrid.h"

#include "Components/TimelineComponent.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

//  UI
#include <Components/UniformGridPanel.h>
#include <Components/ScrollBox.h>

#ifndef PLAYER
#include "GameSystem/PlayerStructManager.h"
#endif

#define F __FUNCTION__

#include "GameSystemFunction.generated.h"

//信息
inline FString FVMAddLogPanel(const FString& Content, const FString& Flag = FString(TEXT("t"))) {
	return FString(FString(TEXT("<")) + Flag + FString(TEXT(">")) + Content + FString(TEXT("</>")));
};


#define FALD(A) FVMAddLogPanel(A)
#define FAL(A,B) FVMAddLogPanel(A,B)


//老鼠z轴偏移
#define MOUSE_Z_OFFSET (-15.f)
//boss鼠的Y轴默认位置
#define MOUSE_BOSS_Y_LOCATION 430.f

/**
 * 游戏系统的一下函数
 */

class UButton;
class UWidget;

//时间分段结构
struct FTimeClip {
public:
	FTimerHandle M_FTimerHandle;
	int32 M_Count = 0;
public:
	//每获取一次将自动增加一次
	int32 Update() {
		return this->M_Count++;
	}
};

UCLASS()
class FVM_API UGameSystemFunction : public UObject
{
	GENERATED_BODY()
public:
	static FDateTime GameSystem_Time;
	static FDateTime GameSystem_Time_Now;
	static FRandomStream Random_Now;
	static bool bEnable;
public:
	//更新时间
	UFUNCTION(BlueprintCallable)
	static void UpdateTime();
	//开启时间更新
	UFUNCTION(BlueprintCallable)
	static void OpenUpdate();
	//关闭更新
	UFUNCTION(BlueprintCallable)
	static void CloseUpdate();
public:
	//发送邮件给玩家
	static void SendMailToCurrentPlayer(FMail& _Mail);
	//清除分时器
	static void ClearTimeClip(FTimerManager& _TimeManager, FTimeClip& _Clip);
public:
	//保持当前角色存档（使用前必须保证角色存档已经被加载过）
	UFUNCTION(BlueprintCallable)
	static void SaveCurrentPlayerData(const FString& SaveLogMsg);
	//读取当前角色存档（使用前必须保证角色存档已经被加载过）
	UFUNCTION(BlueprintCallable)
	static UPlayerStructManager* LoadCurrentPlayerData(const FString& LoadLogMsg);
	//保持当前配置存档（使用前必须保证配置存档已经被加载过）
	UFUNCTION(BlueprintCallable)
	static void SaveCurrentGameConfigData();
public:
	//从背包获取材料数据指针，通过名称
	static FMaterialBase* GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName);
	//返回材料数组
	static void GetMaterialsArrayByType(
		TArray<FMaterialBase>& _ArraysSource,
		const TArray<EMaterialType>& _Type,
		TArray<FMaterialBase*>& OutArrays
	);
	//返回卡片等级图片
	static FString GetCardGradeImagePath(int32 _Grade);

	//返回概率结果是否中标 _ValueMaxRange >= [1,Max]
	UFUNCTION(BlueprintCallable)
	static bool GetRange(int32 _ValueMaxRange, int32 _EndMaxRange);

	//返回概率结果随机数[1,Max]
	static int32 GetRange_Int(int32 _ValueMaxRange, int32 _EndMaxRange);
	//返回一个概率结果[Min,Max]
	static int32 GetRandomRange(int32 Random_Min = 0, int32 RandomNums = 100);
	//返回一个概率结果[Min,Max]
	static float GetRandomRangeFloat(float Random_Min = 0.f, float RandomNums = 100.f);
public:
	//选项卡选择功能
	UFUNCTION(BlueprintCallable)
	static void TabSelect(
		const TArray<UButton*>& _ButtonArrays,
		const  FString& _UniformPath,
		const  TArray<FString>& _DefButtonStyle,
		const  TArray<FString>& _ClickButtonStyle,
		int32 _Index
	);

	//界面选择功能
	UFUNCTION(BlueprintCallable)
	static void WidgetsSelect(const TArray<UWidget*>& _Widgets, ESlateVisibility _ShowType, int32 _Index);

	//界面选择功能
	template<class T = UWidget>
	static void WidgetSelect(const TArray<T*>& _Widgets, ESlateVisibility _ShowType, int32 _Index)
	{
		for (auto& _Widget : _Widgets)
		{
			_Widget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (_Index >= 0 && _Index < _Widgets.Num())
		{
			_Widgets[_Index]->SetVisibility(_ShowType);
		}
	}

	//资源加载
	template<class T>
	static T* LoadRes(const TSoftObjectPtr<T>& Res)
	{
		if (!IsValid(Res.Get()))
		{
			Res.LoadSynchronous();
		}

		return Res.Get();
	}

	template<class T>
	static UClass* LoadRes(const TSoftClassPtr<T>& Res)
	{
		if (!IsValid(Res.Get()))
		{
			Res.LoadSynchronous();
		}

		return Res.Get();
	}
public:
	//打印字符串
	static void FVMLog(const FString& _FuncName, const FString& _Content);

	//添加游戏信息
	UFUNCTION(BlueprintCallable)
	static void AddGameLog(const UObject* WorldContext, const FString& _Content);
	//添加debug信息
	UFUNCTION(BlueprintCallable)
	static void AddDebugLog(const UObject* WorldContext, const FString& _Content);
	//添加指令
	UFUNCTION(BlueprintCallable)
	static bool AddGameinstructions(const UObject* WorldContext, const FString& InputContent, bool bTip = true);

	//追加聊天信息都字符串
	UFUNCTION(BlueprintCallable)
	static FString BlueprintAddLog(const FString& Content, const FString& Flag = FString(TEXT("t")));
public:
	//物体创建--------------------------------------------------------------------------开始
	//创建球体碰撞（14 卡片对象   通道3）
	static bool AddSphereTraceMulti(
		AActor* M_Owner,
		TArray<FHitResult>& OutActors,
		const FVector& _BeginOffset,
		const FVector& _EndOffset,
		float _Radius,
		ETraceTypeQuery _Type
	);
	//创建球体碰撞单通道
	static bool AddSphereTrace(
		AActor* M_Owner,
		FHitResult& OutActor,
		const FVector& _BeginOffset,
		const FVector& _EndOffset,
		float _Radius,
		ETraceTypeQuery _Type
	);
	static bool AddLineTrance(
		const UWorld* World,
		const FVector& CurLoca,
		const FVector& BeginOffset,
		const FVector& EndOffset,
		ECollisionChannel CC,
		UObject* InputObj,
		TFunctionRef<void(UObject* Obj, AActor* CurHit)> Func
	);
	static bool AddLineTranceMulti(
		const UWorld* World,
		const FVector& BeginLocation,
		const FVector& EndLocation,
		const TArray<TEnumAsByte<EObjectTypeQuery>> CC,
		UObject* InputObj,
		TFunctionRef<void(UObject* Obj, AActor* CurHit)> Func
	);
	//初始化老鼠的检测盒子的宽高
	static void InitMouseMeshe(class UBoxComponent* Box, class USceneComponent* BoxChildComp, FVector2D BoxRange = FVector2D(20.f), FVector2D Offset = FVector2D(0.f, 17.f), bool bBack = false);
	static void InintMouseCollisonByBox(class UBoxComponent* Box, FVector2D BoxRange = FVector2D(20.f), bool bBack = false);
	static void InintMouseCollisonBySphere(class USphereComponent* Sphere, float Radius = 32.f);
	//创建一个冰块图层
	static AActor* CreateIceBlock(AActor* M_Owner, const FVector& _Offset, const int32& _Layer, float _Time);
	//创建有关于地图网格的矩阵动画
	static void CreateMapMatrix(FLine _Line, int32 _RowCount, int32 _ColCount, FVector _Offset, FString _FlipBookAnimPath);
	//物体创建--------------------------------------------------------------------------结束
public:

	//通道获取---------------------------------------开始
	//将直接给指定老鼠造成伤害(必中)
	static bool HitMouse(UObject* CurHitMouseObj, float _HitValue, AMouseActor* _Mouse, FItem_Buff& _Buff, EFlyItemAttackType AttackType);
	//创建一个老鼠检测
	static AActor* CreateCheckMouseLineTrace(class UWorld* World, const TArray<FLineTraceSetting>& InputLineTraceSetting, ELineTraceType& OutCheckLineType);
	//线路检测
	static bool CheckMouseLineType(class UWorld* World, const FVector& BeginLoc, const FVector& EndLoc, const ECollisionChannel& Type, AActor*& OutMouse);
	//获取老鼠的射线碰撞通道
	static ETraceTypeQuery GetMouseCollisionTraceType(EMouseCollisionType _EMouseCollisionType);
	//通过通用路线类型获取碰撞类型
	static ECollisionChannel GetMouseCollisionBoxTypeByELineType(ELineType _EMouseLineType);
	static ETraceTypeQuery GetMouseCollisionTraceTypeByELineType(ELineType _EMouseLineType);
	//通过线路类型获取老鼠的碰撞通道
	static ECollisionChannel GetMouseCollisionBoxTypeByLineType(ELineTraceType _EMouseLineType);
	//通过线路类型获取老鼠的射线通道
	static ETraceTypeQuery GetMouseCollisonTraceTypeByLineType(ELineTraceType _EMouseLineType);
	//获取卡片射线碰撞通道
	static ETraceTypeQuery GetCardCollisionTraceType(ECardCollisionType _ECardCollisionType);
	//获取卡片碰撞盒通道
	static ECollisionChannel GetCardCollisionBoxType(ECardCollisionType _ECardCollisionType);
	//通道获取---------------------------------------结束

	//射线位置---------------------------------------结束

	//计算卡片攻击力
	static float CalculateCardATK(
		const float& BaseAtk, const float& AtkRate,
		const int32& CardGrade, const float& GradeUpRate
	)
	{
		return BaseAtk + (BaseAtk / 2.f) * AtkRate * (CardGrade * GradeUpRate);
	}

	//计算老鼠移动线路
	static FLine CalculateMouseMoveLine(int32 RowMax, FLine NewLine)
	{
		//计算移动线路
		int32 CurRow = NewLine.Row;
		if (CurRow == 0)
		{
			CurRow += 1;
		}
		else if (CurRow == RowMax - 1)
		{
			CurRow -= 1;
		}
		else {
			bool bResult = UGameSystemFunction::GetRange(50, 100);
			if (bResult)
			{
				CurRow++;
			}
			else {
				CurRow--;
			}
		}

		return FLine(CurRow, 0);
	}

	//计算射线坐标
	static void CalculateLineTracePosition(
		const FVector& _Location,
		const FLine& _Line,
		UUI_MapMeshe* _UI_Meshe,
		const  FLineTraceSetting& _LineTraceSetting,
		TArray<FLineTraceSetting>& _LineTraceSettingOut
	);
	//抛物线路径
	UFUNCTION(BlueprintCallable)
	static void GetCurveLineByTimeLineBlue(
		FVector CurLocation,
		FVector TargetLocation,
		float MaxHight,
		const float& TimeTick,
		const float& HightTick,
		FVector& OutLocation);
	static void GetCurveLineByTimeLine(
		const FVector& CurLocation,
		const FVector& TargetLocation,
		const float& MaxHight,
		const float& TimeTick,
		const float& HightTick,
		FVector& OutLocation
	);
	//射线位置---------------------------------------结束
public:
	//UI——------------------------------------------开始
	static UUI_MainFramework* GetMainFramework()
	{
		return UUI_MainFramework::M_UUI_MainFrameworkStatic;
	}
	//UI——------------------------------------------结束

	//时间——------------------------------------------开始
	//设置全局游戏场景时间
	UFUNCTION(BlueprintCallable)
	static void SetGlobalGameTime(const UObject* WorldContextObject, float _TimeRate);
	//设置鼠标捕获
	UFUNCTION(BlueprintCallable)
	static void SetCaptureLook(const UObject* WorldContextObject, EMouseLockMode Lock);
	//时间——------------------------------------------结束

public:

	//-----------------------------------------------角色数据开始----------------------------------------------------
	//发送补偿给玩家
	static void SendVersionCompensate();
	//发送邮件给玩家
	static bool SendMailToPlayerMailBox(const FString& _Name);
	//获取指定技能书的等级
	static int32 GetPlayerSkillBooksLv(const FString& _Name);

	//想背包发送指定的防御卡
	UFUNCTION(BlueprintCallable)
	static bool SendCardToPlayerBag(const FString& _CardName, int32 _Lv);
	//发送指定装备到背包( bSave = true 表示调用之后会自动保存一次(有消耗) )
	UFUNCTION(BlueprintCallable)
	static bool SendEquipmentToPlayerBag(const FString& _EquipmentName, int32 _Count, bool _bSave = true);
	//发送指定材料到背包( bSave = true 表示调用之后会自动保存一次(有消耗) )
	UFUNCTION(BlueprintCallable)
	static bool SendMaterialToPlayerBag(const FString& _MaterialName, int32 _Count, bool _bSave = true);

	//获取指向玩家存档卡片的数据(建立在加载存档之后)
	static void GetPlayerCardDatas(TArray<FItemCard*>& _datas, int32 _Count = 0);
	//获取玩家存档卡片数据的引用(建立在加载存档之后)
	static TArray<FItemCard>& GetPlayerCardDatasByRef();
	//装备武器ID查询->返回物品索引（[0]表示更具索引位置对半查找，[1]表示左查找，[2]表示右查找）
	static int32 SerachPlayerEquipmentIndexByID(const FString& _ItemName, int32 _EquipID, uint8 _SerarchType = 0);
	//获取(卡片小等级)的图片路径([1,16]否则返回"")
	static FString GetItemGradePath(int32 _Grade);

	//-----------------------------------------------角色数据结束---------------------------------------------------

	//-----------------------------------------------字符串处理开始---------------------------------------------------
	//追加字符串添加->如果添加的内容相同->则不会添加
	static void AddString(FString& _CurrentString, const FString& _Content);
	//检查字符串是否有效符合要求 [^a-zA-Z0-9]
	UFUNCTION(BlueprintCallable)
	static bool CheckStringIsValid(const FString str, const FString Reg);
	//-----------------------------------------------字符串处理结束---------------------------------------------------


public:
	//加载以及设置过的地图
	UFUNCTION(BlueprintCallable)
	static void LoadMap();
	//设置当前要加载的地图
	UFUNCTION(BlueprintCallable)
	static void SetMap(EGameVsMap _Map);
	//加载上一次的地图
	UFUNCTION(BlueprintCallable)
	static void LoadLastMap();
	//设置上一次的地图
	UFUNCTION(BlueprintCallable)
	static void SetLastMap(EGameVsMap _Map);
public:
	//获取当前玩家的等级图片路径
	UFUNCTION(BlueprintCallable)
	static FString GetPlayerGradeImagePath();
	//获取玩家进入的地图
	UFUNCTION(BlueprintCallable)
	static EGameVsMap GetPlayerInMap();
	//获取玩家当前进入的地图
	UFUNCTION(BlueprintCallable)
	static EGameVsMap GetPlayerCurrentMap();
	//获取玩家上次进入的地图
	UFUNCTION(BlueprintCallable)
	EGameVsMap GetPlayerLastInMap();
public:

	//设置数据表
	UFUNCTION(BlueprintCallable)
	static void ResetDataTable(class UDataTable* _datatable);

	//获取对象名称
	UFUNCTION()
	static FString GetObjectName(const UObject* Obj);

	//获取数据表内容
	template<class RowType>
	static void GetDataTableRows(const FString& _Path, TArray<RowType*>& _OutRows)
	{
		UDataTable* Table = LoadObject<UDataTable>(0, *_Path);
		Table->GetAllRows("List", _OutRows);
	}

	template<class RowType>
	static void GetDataTableRows(UDataTable* DataTableIns, TArray<RowType>& _OutRows)
	{
		if (IsValid(DataTableIns))
		{
			TArray<RowType*> L;
			DataTableIns->GetAllRows("List", L);
			_OutRows.Reserve(_OutRows.Num() + L.Num());
			for (auto Content : L)
			{
				_OutRows.Add(*Content);
			}
		}
	}

	template<class RowType>
	inline void GetTableRow(const FString& _Path, TArray<RowType*>& _OutRows)
	{
		UGameSystemFunction::GetDataTableRows<RowType>(_Path, _OutRows);
	}

	template<class RowType>
	static void GetDataTableRows(const FString& _Path, TArray<RowType>& _OutRows)
	{
		UDataTable* Table = Cast<UDataTable>(
			UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(_Path))
		);
		if (IsValid(Table))
		{
			TArray<RowType*> L;
			Table->GetAllRows("List", L);
			for (auto Content : L)
			{
				_OutRows.Emplace(*Content);
			}
		}
	}

	//删除玩家背包中有待删除的道具(任何物品)【会改变数组的内存地址】
	template<class ItemType>
	static void ClearWaitingItems(TArray<ItemType>& _Arrays)
	{
		TArray<ItemType> _Buff;

		for (ItemType& _Items : _Arrays)
		{
			//将未标记的物品添加到缓冲区
			if (!_Items.bWaitRemove)
				_Buff.Emplace(_Items);
		}
		//清空
		_Arrays.Empty();
		//重新复制新值
		_Arrays = _Buff;
	}

	//删除玩家背包中有待删除的道具->争对装备道具【会改变数组的内存地址】自动调用RemoveEquipmentFromPlayerBag
	static void ClearWaitingItemsForEquip(TArray<FEquipmentBase>& _Arrays);
public:
	//静态

	//通过Name直接获取UI具体的实例【使用前请确保{对象}有效】
	UFUNCTION(BlueprintPure)
	static class UWidgetBase* GetUserInterWidgetByClass(
		TSoftClassPtr<class UAssetCategoryName> ObjectType,
		FName Name
	);
	//通过Name直接获取UI具体的实例【使用前请确保Name有效】
	static class UWidgetBase* GetUserInterWidgetByName(
		const FName& CategoryName,
		const FName& Name
	);
	//通过Class获取UI实例【使用前请确保Class有效】
	UFUNCTION(BlueprintPure)
	static UGameUserInterfaceInstance* GetUserInterInstanceByClass(
		TSoftClassPtr<class UAssetCategoryName> ObjectType
	);
	//通过Name获取UI实例【使用前请确保Name有效】
	static UGameUserInterfaceInstance* GetUserInterInstanceByName(
		const FName& Name
	);
	//新增临时UI实例,游戏结束释放，不会被保存
	UFUNCTION(BlueprintCallable)
	static bool AddTempUserInterInstanceByName(FName Name);

	//获取分类名称类获取名称
	UFUNCTION(BlueprintPure)
	static FName GetAssetCategoryName(TSoftClassPtr<class UAssetCategoryName> ObjectType);
};

//分时加载器
template <class ClassType, class DataTableElement>
struct FTimerLoad {
private:
	//分时段
	FTimeClip M_FTimeClip;
	//模板类实例
	ClassType* M_Class = nullptr;
	//模板类代理函数
	void (ClassType::* Func)(ClassType* _Class, TArray<DataTableElement>& _Elements, int32 _Index) = nullptr;
	//模板类代理函数(当全部加载完毕之后结束运行函数)
	void (ClassType::* FuncEnd)(ClassType* _Class) = nullptr;
	//数据表
	TArray<DataTableElement> M_DataList;
	//最大长度
	int32 M_DataSizeMax = 0;
	//是否调用了stop
	bool M_bStop = false;
	//是否调用了run
	bool M_bRun = false;
	//是否完成了
public:
	//构造初始化
	FTimerLoad() :M_Class(0), Func(0) {}
	//构造初始化(类)
	FTimerLoad(
		ClassType* _Class,
		void (ClassType::* _Func)(ClassType* _Class, TArray<DataTableElement>& _Elements, int32 _Index) = nullptr
	) :M_Class(_Class), Func(_Func) {}
	//构造初始化(数据表)
	FTimerLoad(TArray<DataTableElement> Datas) :M_DataSizeMax(Datas.Num()) { this->SetDataList(Datas); }
	//构造初始化(数据表+类)
	FTimerLoad(
		TArray<DataTableElement> Datas,
		ClassType* _Class = nullptr,
		void (ClassType::* _Func)(ClassType* _Class, TArray<DataTableElement>& _Elements, int32 _Index) = nullptr
	) :M_Class(_Class), Func(_Func)
	{
		this->SetDataList(Datas);
	}
public:
	//析构
	~FTimerLoad() { this->Stop(); }
public:
	//初始化类
	inline void InitClass(ClassType* _Class)
	{
		this->M_Class = _Class;
	}

	//设置数据表(数组)
	inline void SetDataList(TArray<DataTableElement>& Datas) {
		this->M_DataList.Empty();
		this->M_DataList.Append(Datas);
		this->M_DataSizeMax = Datas.Num();
		this->M_bRun = false;
		this->M_bStop = false;
	}
	//设置类
	inline void SetClass(ClassType* _Class = nullptr, void (ClassType::* _Func)(ClassType* _Func_Class, TArray<DataTableElement>& _Func_Elements, int32 _Func_Index) = nullptr)
	{
		this->M_Class = _Class;
		this->Func = _Func;
	}
	//设置结束函数
	inline void SetEndFunction(void (ClassType::* _Func)(ClassType* _Class))
	{
		this->FuncEnd = _Func;
	}
public:
	//结束
	void Stop()
	{
		if (this->M_bStop)
			return;

		//清除定时器
		if (IsValid(this->M_Class))
			UGameSystemFunction::ClearTimeClip(this->M_Class->GetWorld()->GetTimerManager(), this->M_FTimeClip);

		//调用结束函数
		if (IsValid(this->M_Class) && this->FuncEnd)
			(this->M_Class->*(this->FuncEnd))(this->M_Class);

		this->M_bStop = true;
	}

	//运行
	void Run(float _loopDalay = 0.1f) {

		if (this->M_bRun)
			return;

		if (!IsValid(this->M_Class))
			return;

		if (!IsValid(this->M_Class->GetWorld()))
			return;

		//不管如何先停止
		//清除定时器
		if (this->M_Class)
			UGameSystemFunction::ClearTimeClip(this->M_Class->GetWorld()->GetTimerManager(), this->M_FTimeClip);


		this->M_Class->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip.M_FTimerHandle, [&]() {

			if (this->M_FTimeClip.M_Count < this->M_DataSizeMax)
			{
				//调用函数
				(this->M_Class->*(this->Func))(this->M_Class, this->M_DataList, this->M_FTimeClip.M_Count);

				this->M_FTimeClip.Update();
			}
			else {
				this->Stop();
			}
			}, _loopDalay, true);

		this->M_bRun = true;

	};
};

//函数代理调用
struct FFunctionDelegates {
private:
	TArray<FScriptDelegate> M_Delegates;
public:
	//绑定
	void Bind(UObject* _Class, const FString& _FuncName)
	{
		FScriptDelegate TempDele;
		TempDele.BindUFunction(_Class, *_FuncName);
		this->M_Delegates.Emplace(TempDele);
	}
	//调用
	void Call() {
		for (auto& Func : this->M_Delegates)
		{
			if (Func.IsBound())
			{
				Func.ProcessDelegate<UObject>(Func.GetUObject());
			}
		}
	}
	//是否已经绑定
	bool IsBind()
	{
		return this->M_Delegates.Num() > 0 ? true : false;
	}
	//移除绑定
	void RemoveBind()
	{
		for (auto& Func : this->M_Delegates)
		{
			Func.Clear();
		}

		this->M_Delegates.Empty();
	}
};


//券商城加载管理器
template<class TypeClass, class ShopGridClass>
struct TicketShopLoadManager
{
private:
	//加载个数
	int32 M_LoadCount = 0;
	//当前已经加载到界面的道具个数
	int32 M_LoadCurrentCountToSpace = 0;
	//当前已经加载的个数
	int32 M_LoadCurrentCount = 0;
	//当前隐藏个数
	int32 M_LoadHiddenCount = 0;
	//一行多少列
	int32 M_Col = 0;
	//加载延迟
	float M_LoadDelay = 0.02f;
	//时间分段
	FTimeClip* M_FTimeClip = nullptr;
	//类
	TypeClass* M_Class = nullptr;
	//数据表数据
	TArray<FItem_TicketPrice_Data> M_ShopItemDatas;
	//UI显示面板
	UUniformGridPanel* M_UFormPanel = nullptr;
	//代理函数
	ShopGridClass* (TypeClass::* M_Class_Delegate_Func)() = nullptr;
public:
	TicketShopLoadManager() {}
	TicketShopLoadManager(
		FTimeClip* _FTimeClip,
		int32 _LoadCount,
		int32 _LoadCol,
		float _LoadTickDelay
	) :M_FTimeClip(_FTimeClip), M_LoadCount(_LoadCount), M_Col(_LoadCol), M_LoadDelay(_LoadTickDelay) {}
public:
	//设置一些属性(加载个数,一行加载多少列,多少毫秒加载一次)
	void SetProperty(FTimeClip* _FTimeClip, int32 _LoadCount, int32 _LoadCol, float _LoadTickDelay)
	{
		this->M_FTimeClip = _FTimeClip;
		this->M_LoadCount = _LoadCount;
		this->M_Col = _LoadCol;
		this->M_LoadDelay = _LoadTickDelay;
		this->M_LoadCurrentCountToSpace = 0;
	}
	//设置目标界面和加载的数据表
	void SetTargetUIFormPanelAndDataTable(UUniformGridPanel* _UFormPanel, TArray<FItem_TicketPrice_Data>& _ShopItemDatas)
	{
		this->M_UFormPanel = _UFormPanel;
		this->M_ShopItemDatas = _ShopItemDatas;
	}
	//绑定类和函数
	void BindClassAndFunc(TypeClass* _Class, ShopGridClass* (TypeClass::* _Class_Delegate_Func)()) {
		this->M_Class = _Class;
		this->M_Class_Delegate_Func = _Class_Delegate_Func;
	}
	//运行
	void Run() {
		//销毁时间对象
		UGameSystemFunction::ClearTimeClip(this->M_Class->GetWorld()->GetTimerManager(), *this->M_FTimeClip);

		//先全部进行隐藏界面
		for (auto Panel : this->M_UFormPanel->GetAllChildren())
		{
			Panel->SetVisibility(ESlateVisibility::Collapsed);
		}

		//设置当前已经加载到空间的道具个数
		this->M_LoadCurrentCountToSpace = this->M_UFormPanel->GetAllChildren().Num();
		//重置加载个数
		this->M_LoadCurrentCount = 0;
		//重置隐藏个数
		this->M_LoadHiddenCount = 0;

		//开始循环
		this->M_Class->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip->M_FTimerHandle, [&]() {
			if (this->M_LoadCurrentCount < M_LoadCount)
			{
				//获取当前格子的价格数据
				FItemTicketPrice _Price = (this->M_ShopItemDatas[this->M_LoadCurrentCount]).M_FItemPrice;

				//如果表示显示则会显示在内容里面
				if (_Price.M_IsShow)
				{
					UUI_TicketShopGrid* _Grid = nullptr;

					//如果当前索引小于已经加载到空间的总个数
					if ((this->M_LoadCurrentCount - this->M_LoadHiddenCount) < this->M_LoadCurrentCountToSpace)
					{
						//设置界面内容显示
						this->M_UFormPanel->GetAllChildren()[this->M_LoadCurrentCount - this->M_LoadHiddenCount]->SetVisibility(ESlateVisibility::Visible);
						//Cast<UUI_TicketShopGrid>(this->M_UFormPanel->GetAllChildren()[this->M_LoadCurrentCount - this->M_LoadHiddenCount])->PlayAnimation_1();

						_Grid = Cast<UUI_TicketShopGrid>(this->M_UFormPanel->GetAllChildren()[this->M_LoadCurrentCount - this->M_LoadHiddenCount]);

					}
					else {
						//获取对应的格子
						_Grid = (this->M_Class->*M_Class_Delegate_Func)();

						//将格子添加到界面并且显示
						this->M_UFormPanel->AddChildToUniformGrid(_Grid, (this->M_LoadCurrentCount - this->M_LoadHiddenCount) / this->M_Col, (this->M_LoadCurrentCount - this->M_LoadHiddenCount) - this->M_Col * ((this->M_LoadCurrentCount - M_LoadHiddenCount) / this->M_Col));
					}

					if (_Grid && IsValid(_Grid))
					{
						//设置券的图标
						_Grid->SetTicketHead(_Price.M_TicketHeadTexture2DPath.ToString());
						//设置物品的头像
						_Grid->SetItemHead(_Price.M_ItemHeadTexture2DPath.ToString());
						//设置数据
						_Grid->SetItemData(_Price);
						//设置初始化
						_Grid->InitBase();
						//播放动画
						_Grid->Anim();
					}
				}
				else {
					this->M_LoadHiddenCount++;
				}
				//更新索引
				this->M_LoadCurrentCount++;
			}
			else {
				//销毁时间对象
				UGameSystemFunction::ClearTimeClip(this->M_Class->GetWorld()->GetTimerManager(), *this->M_FTimeClip);
			}
			}, this->M_LoadDelay, true);
	}
};

//时间线类
UCLASS()
class FVM_API UTimeLineClass : public UObject
{
	GENERATED_BODY()
public:
	static UTimeLineClass* MakeTimeLineClass();
public:
	//新增曲线
	void AddCurve(
		TSoftObjectPtr<UCurveFloat> NewCurve,
		UObject* NewObj,
		TFunction<void(UTimeLineClass*, UObject*, float)> NewUpdateFunc,
		TFunction<void(UTimeLineClass*, UObject*)> NewFinishFunc
	);

	void Tick(const float& DeltaTime);
	void Play();
	void Reverse();
	void PlayFromStart();
	void ReverseFromEnd();
	void Stop();
	void End();
	bool GetStart();
protected:
	UFUNCTION()
	void TimeUpdate();
	UFUNCTION()
	void TimeFinish();
private:
	UPROPERTY()
	bool bStart = true;
private:
	//时间线
	UPROPERTY()
	FTimeline Timeline;
	//曲线
	UPROPERTY()
	TArray<UCurveFloat*> Curves;
	//更新函数
	TArray<TFunction<void(UTimeLineClass*, UObject*, float time)>> CurveFuncsUpdate;
	//完成函数
	TArray<TFunction<void(UTimeLineClass*, UObject*)>> CurveFuncsFinish;
	//对象
	UPROPERTY()
	TArray<UObject*> AllObjs;
private:
	FOnTimelineFloatStatic OnTimelineFloatEvent;
	FOnTimelineEventStatic OnTimelineFinishedEvent;
};

