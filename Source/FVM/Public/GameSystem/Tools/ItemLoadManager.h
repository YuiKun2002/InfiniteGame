// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Tools/ItemDataTable.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "ItemLoadManager.generated.h"

/**
 * 物品加载管理器
 *
 * 你需要绑定->创建与刷新  代理函数 OnCreateItem OnRefreshItem
 * 同时你也必须将本类的OnUserScrolled绑定
 * 最后你也必须要绑定数据表更新函数 OnUserScrolledFunction
 */

class UWidget;
class UScrollBox;
class UUniformGridSlot;
class UUniformGridPanel;


//更新下滑列表
DECLARE_DELEGATE_OneParam(FRefreshScrollFunctionDelegate, float);

//创建
DECLARE_DELEGATE_RetVal_TwoParams(UWidget*, FCreateFunctionDelegate, UItemDataTable*, int32);
//刷新
DECLARE_DELEGATE_ThreeParams(FRefreshFunctionDelegate, UItemDataTable*, int32, UWidget*);

//预刷新
DECLARE_DELEGATE(FBeginRefreshFunctionDelegate);
//加载结束
DECLARE_DELEGATE(FLoadEndFunctionDelegate);



//出现翻页的时候
//DECLARE_DELEGATE(FUserScrolledFunctionDelegate);


//数据表转换初始化
template<typename TableType>
void TableTransformInit(TArray<FTableRowBase*>& _SourceTableDatas, TArray<TableType>& _OutTable)
{
	for (auto Item : _SourceTableDatas)
	{
		_OutTable.Emplace(*((TableType*)(Item)));
	}
}

/*
	//创建材界面
	UFUNCTION()
		UWidget* WidgetCreate_Init(UItemDataTable* _Data, int32 _Index);
	//刷新界面
	UFUNCTION()
		void WidgetRefresh_Update(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);


	->OnCreateItem.BindUFunction(this, TEXT(""));
	->OnRefreshItem.BindUFunction(this, TEXT(""));
*/
UCLASS()
class FVM_API UItemLoadManager : public UObject
{
	GENERATED_BODY()
public:
	//析构销毁
	virtual void BeginDestroy() override;
private:
	//加载列表的容器
	UPROPERTY()
	UScrollBox* M_Load_Table_List = nullptr;
	//加载列表的格子
	UPROPERTY()
	UUniformGridPanel* M_Load_Uniform_Grid = nullptr;
private:
	//指向源数据的数组指针头
	TMap<int32, FTableRowBase*> M_SourceDatas;

	//当前展示数据缓冲区
	TArray<FTableRowBase*> M_ShowDatas;
private:
	//界面网格缓冲区
	UPROPERTY()
	TArray<UUniformGridSlot*> M_GridSlotBuffers;
private:
	//滑动区的位移
	FVector2D M_ScrollOffset = FVector2D(0.f);
	//物品之间的间隔
	FVector2D M_MarginOffset = FVector2D(0.f, 0.f);

	//当前加载隐藏的道具数量
	int32 M_Hidden_Count = 0;
	//当前UI创建的数量->索引
	int32 M_CurrentCreateUINum = 0;

	//需要显示的数据起始索引
	int32 M_LoadIndex_First = 0;
	//需要显示的数据末尾索引
	int32 M_LoadIndex_Last = 0;

	//当前的页数
	int32 M_Page_CurrentNum = 0;
	//页数的基数
	int32 M_Page_BaseNum = 0;

	//当前页数最大的物品数量
	int32 M_Page_CurrentMaxCount = 0;
	//当前一行中有多少个物品(n列)
	int32 M_Page_CurrentColCount = 0;
	//最大显示多少行(n行)
	int32 M_Page_CurrentRowCount = 0;
	//能加载是最大数据->控制显示长度
	int32 M_Page_LoadMaxCount = 100;
	//当前被查找到的数据
	FTableRowBase** M_Item_CurrentFindData = nullptr;
	//当前已经加载的数量
	int32 M_Item_CurrentLoadCount = 0;

	//UI的加载索引(更新列表的时候自动更新UI的当前索引 与 物品索引无关)
	//int32 M_CurrentUI_Index = 0;

	//一行的行高
	float M_Page_LineHight = 0.f;
	//一个物品的宽度
	float M_Page_LineWidth = 0.f;
	//列表高度倍数值（翻滚多少才算一页）
	int32 M_Load_Table_List_Height_Rate = 1;

	//是否翻页到了最后
	bool M_ConditionPage_OverData = false;

	//是否已经初始化
	bool M_Condition_Init = false;
	//刷新最后一页
	bool M_Condition_RefreshEndPage = false;
	//是否隐藏当前界面
	bool M_Condition_Widget_Hidden = false;
	//设置横向检测
	bool M_Ccroll_Condition_bHorizontal = false;
	//预刷新检测
	bool M_Ccroll_Condition_bHorizontalBeginRefresh = false;

private:
	//记录玩家翻页时，是否是往上翻页
	float M_OnUserScrolled_Temp_UpOffset = 0.f;
private:
	//当需要触发滑动区更新时
	FRefreshScrollFunctionDelegate OnBeginScrollRefresh;
private:
	UFUNCTION()
	void OnScrollRefresh(float _Value);
public:
	//物品创建
	FCreateFunctionDelegate OnCreateItem;
	//物品刷新(异步刷新->分时间刷新)
	FRefreshFunctionDelegate OnRefreshItem;
	//预刷新
	FBeginRefreshFunctionDelegate OnBeginRefreshItem;
	//加载结束触发
	FLoadEndFunctionDelegate OnLoadEndItem;
public:
	//当用户滑动时
	UFUNCTION()
	void OnUserScrolled(float _Value);
private:
	//获取页面首索引
	int32 GetPagetFirstIndex();
	//获取页面尾索引
	int32 GetPagetLastIndex();
public:
	//继续加载（可以重复调用）
	void ContinueRun();
private:
	//加载组合数据
	void Run(float _Offset);
	//更新数据
	void UpdateData(float _Offset);
	//刷新物品
	void RefreshItem(float _Offset);
	//创建物品
	void CreateItem();
public:
	/*  尽量保证调用一次  */
	//初始化界面(格子，滑动界面，刷新物品加载时间)
	void InitWidget_First(
		UUniformGridPanel* _M_Load_Uniform_Grid,
		UScrollBox* _M_Load_Table_List,
		float _LoadTime = 0.01f
	);
	//初始化加载项(行，列，最大加载数量，加载物品-宽，加载物品-高，滑动方向：false纵向  true横向)
	bool InitRange_Second(
		int32 _Row,
		int32 _Col,
		int32 _LoadMax,
		float _TargetItem_Width,
		float _TargetItem_Height,
		bool _bMode_Horizontal = false
	);
public:
	//重新设置加载的行列【在加载前使用】
	void SetLoadRowAndCol(int32 _Row, int32 _Col);
	//设置道具最大加载数量（可以重复调用）
	void SetLoadItemMaxCount(int32 _NewCount);
	//设置当前页数
	void SetCurrentPage(int32 _NewPage);
	//IsPage 0 to offset to 0
	//重置ScrollBox的位移->前提是页数为0
	void SetResetScrollBoxOffset();
	//设置滑动区的UI平面位移
	void SetMainScrollTransfrom(FVector2D _NewOffset, FVector2D _NewScale = FVector2D(1.f), float _Angle = 0.f);
	//设置物品之间的间距
	void SetItemMargin(FVector2D _NewMargin);
public:
	//获取总数量
	int32 GetTotal();
	//获取当前已加载的数量
	int32 GetCount();
	//获取当前页数
	int32 GetCurrentPage();
	//获取当前位移位置
	FVector2D GetMainScrollTransfrom();
private:
	template<typename FTableRowType>
	void DataTableCondition(const TArray<FTableRowType>& _SourceItems)
	{
		bool LResult = false;
		for (int32 i = this->M_SourceDatas.Num() - 1; i > _SourceItems.Num() - 1; i--)
		{
			this->M_SourceDatas.Remove(i);
			LResult = true;
		}

		//如果当前总数组小于已存在的数组->删除
		if (LResult && this->M_ConditionPage_OverData)
		{
			this->M_Condition_RefreshEndPage = true;
		}
		else if (!LResult && this->M_ConditionPage_OverData)
		{
			this->M_Condition_RefreshEndPage = true;
			this->M_ConditionPage_OverData = false;
		}
	}
public:
	//更新数据表(模板)（可以重复调用）
	template<typename FTableRowType>
	void UpdateDatatable(const TArray<FTableRowType>& _SourceItems)
	{

		this->DataTableCondition(_SourceItems);

		int32 i = 0;
		int32 LDataNum = this->M_SourceDatas.Num();
		FTableRowBase** LData = nullptr;
		for (auto& Item : _SourceItems)
		{
			if (i < LDataNum)
			{
				LData = this->M_SourceDatas.Find(i);
				if (LData)
				{
					(*LData) = ((FTableRowBase*)(&Item));
				}
			}
			else {
				this->M_SourceDatas.Emplace(i, ((FTableRowBase*)(&Item)));
			}

			i++;
		}
	}

	//更新数据表(模板)（可以重复调用）
	template<typename FTableRowType>
	void UpdateDatatable(const TArray<FTableRowType*>& _SourceItems)
	{
		this->DataTableCondition(_SourceItems);

		int32 i = 0;
		int32 LDataNum = this->M_SourceDatas.Num();
		FTableRowBase** LData = nullptr;
		for (auto& Item : _SourceItems)
		{
			if (i < LDataNum)
			{
				LData = this->M_SourceDatas.Find(i);
				if (LData)
				{
					(*LData) = ((FTableRowBase*)(Item));
				}
			}
			else {
				this->M_SourceDatas.Emplace(i, ((FTableRowBase*)(Item)));
			}
			i++;
		}
	}
};
