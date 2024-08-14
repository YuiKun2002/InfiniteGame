// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI/WidgetBase.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/UniformGridSlot.h>
#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>

#include <Kismet/KismetSystemLibrary.h>

void UItemLoadManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void UItemLoadManager::OnScrollRefresh(float _Value)
{

	this->M_Load_Table_List->SetScrollOffset(_Value);
}

void UItemLoadManager::OnUserScrolled(float _Value)
{

	//无惯性滑动
	this->M_Load_Table_List->EndInertialScrolling();

	//横向检测
	if (this->M_Ccroll_Condition_bHorizontal)
	{
		//往下翻页
		if (_Value >= UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->GetSize().X * this->M_Load_Table_List_Height_Rate)
		{
			//如果数据没有显示完毕
			if (!this->M_ConditionPage_OverData)
			{
				//翻页次数加一
				this->M_Page_CurrentNum++;
				//重新排列
				this->Run((int32)this->M_Page_LineWidth);
				//设置翻页的坐标位置
				//this->M_Load_Table_List->SetScrollOffset();
			}
		}
		else {
			if (_Value <= this->M_Page_LineWidth)
			{
				if (_Value < this->M_OnUserScrolled_Temp_UpOffset)
				{
					//往上翻页(当前页数必须大于0)
					if (this->M_Page_CurrentNum > 0)
					{
						//将数据显示完毕条件重置
						this->M_ConditionPage_OverData = false;
						//翻页次数减一
						this->M_Page_CurrentNum--;
						//重新排列
						this->Run((int32)UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->GetSize().X * this->M_Load_Table_List_Height_Rate);
						//设置翻页的坐标位置	
						//this->M_Load_Table_List->SetScrollOffset();
					}
				}
				else {
					this->M_OnUserScrolled_Temp_UpOffset = _Value;
				}
			}
		}
	}
	else {
		//往下翻页
		if (_Value >= UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->GetSize().Y * this->M_Load_Table_List_Height_Rate)
		{
			//如果数据没有显示完毕
			if (!this->M_ConditionPage_OverData)
			{
				//翻页次数加一
				this->M_Page_CurrentNum++;
				//重新排列
				this->Run((int32)this->M_Page_LineHight);
			}
		}
		else {
			if (_Value <= this->M_Page_LineHight)
			{
				if (_Value <= this->M_OnUserScrolled_Temp_UpOffset)
				{
					//往上翻页(当前页数必须大于0)
					if (this->M_Page_CurrentNum > 0)
					{
						//将数据显示完毕条件重置
						this->M_ConditionPage_OverData = false;
						//翻页次数减一
						this->M_Page_CurrentNum--;
						//重新排列
						this->Run((int32)UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->GetSize().Y * this->M_Load_Table_List_Height_Rate);
					}
				}
				else {
					this->M_OnUserScrolled_Temp_UpOffset = _Value;
				}
			}
		}
	}


}

int32 UItemLoadManager::GetPagetFirstIndex()
{
	return this->M_Page_CurrentNum * this->M_Page_BaseNum;
}

int32 UItemLoadManager::GetPagetLastIndex()
{
	if (this->M_Page_CurrentMaxCount + this->M_LoadIndex_First > this->M_Page_LoadMaxCount)
		return this->M_Page_LoadMaxCount;

	return this->M_Page_CurrentMaxCount + this->M_LoadIndex_First;
}

void UItemLoadManager::Run(float _Offset)
{
	//清除上一次存储的数据
	this->M_ShowDatas.Empty();

	//设置需要显示的数据起始索引
	this->M_LoadIndex_First = this->GetPagetFirstIndex();
	//设置需要显示的数据末尾索引(如果只显示一列->那么额外增加一个最大显示)
	this->M_LoadIndex_Last = this->GetPagetLastIndex();

	//如果末尾的索引大于大于最大数量->重置末尾索引
	if (this->M_LoadIndex_Last == this->M_Page_LoadMaxCount)
	{
		this->M_ConditionPage_OverData = true;
	}

	for (int i = this->M_LoadIndex_First; i < this->M_LoadIndex_Last; i++)
	{
		this->M_Item_CurrentFindData = this->M_SourceDatas.Find(i);

		if (this->M_Item_CurrentFindData)
		{
			this->M_ShowDatas.Emplace(*this->M_Item_CurrentFindData);

			this->M_Item_CurrentLoadCount = i + 1;
		}
		else {
			//设置显示的数据已经加载完毕
			this->M_ConditionPage_OverData = true;
			break;
		}
	}

	//更新
	this->UpdateData(_Offset);
}

void UItemLoadManager::ContinueRun()
{
	//如果数据没有加载完毕->加载数据
	if (!this->M_ConditionPage_OverData)
	{
		this->Run(this->M_Load_Table_List->GetScrollOffset());
	}
	else {
		if (this->M_Condition_RefreshEndPage)
		{
			this->M_Condition_RefreshEndPage = false;

			this->Run(this->M_Load_Table_List->GetScrollOffset());
		}
		else {
			this->UpdateData(this->M_Load_Table_List->GetScrollOffset());
		}
	}
}

void UItemLoadManager::UpdateData(float _Offset)
{
	//判断 子UI缓冲对象数量  是否 适用于 现在的数据
	//如果不适合->添加新的子UI缓冲区(再一次调用Create函数)
	if (this->M_Load_Uniform_Grid->GetAllChildren().Num() < this->M_ShowDatas.Num())
	{
		//如果有对象->那么额外创建UI->之后刷新
		if (this->M_Load_Uniform_Grid->HasAnyChildren())
		{
			this->CreateItem();
			this->RefreshItem(_Offset);
		}
		else {
			//如果没有对象->只需要创建
			this->CreateItem();
		}
	}
	else {
		this->RefreshItem(_Offset);
	}
}

void UItemLoadManager::CreateItem()
{
	if (!this->OnCreateItem.IsBound())
	{
		return;
	}

	for (int32 i = this->M_Load_Uniform_Grid->GetAllChildren().Num(); i < this->M_ShowDatas.Num(); i++)
	{
		//调用创建函数->返回界面
		UItemDataTable* LData = NewObject<UItemDataTable>();
		LData->SetValue(this->M_ShowDatas[i]);

		UWidget* LWidget = this->OnCreateItem.Execute(LData, i);

		if (LWidget)
		{
			//加入到统一格子
			if (this->M_Ccroll_Condition_bHorizontal)
			{
				this->M_GridSlotBuffers.Emplace(
					this->M_Load_Uniform_Grid->AddChildToUniformGrid(
						LWidget, i % this->M_Page_CurrentColCount, i / this->M_Page_CurrentColCount
					));
			}
			else
			{
				this->M_GridSlotBuffers.Emplace(
					this->M_Load_Uniform_Grid->AddChildToUniformGrid(
						LWidget, i / this->M_Page_CurrentColCount, i % this->M_Page_CurrentColCount
					));
			}
		}
	}

	//加载结束
	if (this->OnLoadEndItem.IsBound())
	{
		this->OnLoadEndItem.Execute();
	}
}

void UItemLoadManager::RefreshItem(float _Offset)
{
	if (!this->OnRefreshItem.IsBound())
	{
		return;
	}

	if (this->OnBeginRefreshItem.IsBound())
	{
		//调用预刷新
		this->OnBeginRefreshItem.Execute();
	}

	//将需要加载的界面透明度设置为0
	for (int32 i = 0; i < this->M_ShowDatas.Num(); i++)
	{
		//调用函数
		//执行代理(设置数据表数据)
		UItemDataTable* LData = NewObject<UItemDataTable>();
		LData->SetValue(this->M_ShowDatas[i]);
		//执行代理函数->刷新
		this->OnRefreshItem.Execute(
			LData,
			this->M_LoadIndex_First + i,
			this->M_Load_Uniform_Grid->GetChildAt(i)
		);
		//显示界面
		this->M_Load_Uniform_Grid->GetChildAt(i)->SetRenderOpacity(1);
	}

	//将其他的界面隐藏
	if (this->M_ConditionPage_OverData)
	{
		for (int32 i = this->M_ShowDatas.Num(); i < this->M_Load_Uniform_Grid->GetAllChildren().Num(); i++)
		{
			//this->M_Load_Uniform_Grid->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);

			this->M_Load_Uniform_Grid->GetChildAt(i)->SetRenderOpacity(0);
		}
	}

	//执行加载结束
	if (this->OnLoadEndItem.IsBound())
	{
		this->OnLoadEndItem.Execute();
	}

	//刷新滑动区
	if (this->OnBeginScrollRefresh.IsBound())
	{
		this->OnBeginScrollRefresh.Execute(_Offset);
	}
}

void UItemLoadManager::InitWidget_First(UUniformGridPanel* _M_Load_Uniform_Grid, UScrollBox* _M_Load_Table_List, float _LoadTime)
{
	//每一个物品的集合->统一格子
	this->M_Load_Uniform_Grid = _M_Load_Uniform_Grid;
	//总容器列表
	this->M_Load_Table_List = _M_Load_Table_List;

	//绑定刷新函数
	if (!this->OnBeginScrollRefresh.IsBound())
		this->OnBeginScrollRefresh.BindUFunction(this, TEXT("OnScrollRefresh"));

	//绑定事件
	if (this->M_Load_Table_List->OnUserScrolled.IsBound())
		this->M_Load_Table_List->OnUserScrolled.Clear();
	this->M_Load_Table_List->OnUserScrolled.AddDynamic(this, &UItemLoadManager::OnUserScrolled);
}

bool UItemLoadManager::InitRange_Second(int32 _Row, int32 _Col, int32 _LoadMax, float _TargetItem_Width, float _TargetItem_Height, bool _bMode_Horizontal)
{
	if (this->M_Condition_Init)
		return false;

	//设置容器的大小
	UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->SetSize(FVector2D(_Col * _TargetItem_Width, _Row * _TargetItem_Height));

	//如果设置了 横向模式
	if (_bMode_Horizontal)
	{
		int32 TempL = _Row;
		_Row = _Col;
		_Col = TempL;

		this->M_Load_Table_List->SetOrientation(EOrientation::Orient_Horizontal);
	}
	else {
		this->M_Load_Table_List->SetOrientation(EOrientation::Orient_Vertical);
	}

	//设置初始化
	this->M_Condition_Init = true;
	//设置加载最大个数
	this->M_Page_LoadMaxCount = _LoadMax;
	//设置多少列
	this->M_Page_CurrentColCount = _Col;
	//设置多少行
	this->M_Page_CurrentRowCount = _Row;
	//设置翻页基数
	this->M_Page_BaseNum = _Row * _Col - _Col * ((_Row == 1) ? 0 : 1);
	//设置默认页数
	this->M_Page_CurrentNum = 0;
	//设置一页最大显示个数
	this->M_Page_CurrentMaxCount = _Row * _Col * 2 + _Col;
	//设置行高
	this->M_Page_LineHight = _TargetItem_Height;
	//设置行宽
	this->M_Page_LineWidth = _TargetItem_Width;
	//设置下拉模式
	this->M_Ccroll_Condition_bHorizontal = _bMode_Horizontal;

	//设置下滑区域高度倍率
	this->M_Load_Table_List_Height_Rate = ((_Row == 1) ? 2 : 1);

	return true;
}

void UItemLoadManager::SetLoadRowAndCol(int32 _Row, int32 _Col)
{
	this->M_Condition_Init = false;

	this->InitRange_Second(_Row, _Col, this->M_Page_LoadMaxCount, this->M_Page_LineWidth, this->M_Page_LineHight, this->M_Ccroll_Condition_bHorizontal);

	//重置格子排列
	for (int32 i = 0; i < this->M_GridSlotBuffers.Num(); i++)
	{
		if (this->M_GridSlotBuffers[i] != nullptr)
		{
			if (this->M_Ccroll_Condition_bHorizontal)
			{
				this->M_GridSlotBuffers[i]->SetRow(i % this->M_Page_CurrentColCount);
				this->M_GridSlotBuffers[i]->SetColumn(i / this->M_Page_CurrentColCount);
			}
			else {
				this->M_GridSlotBuffers[i]->SetRow(i / this->M_Page_CurrentColCount);
				this->M_GridSlotBuffers[i]->SetColumn(i % this->M_Page_CurrentColCount);
			}

		}
	}
}

void UItemLoadManager::SetLoadItemMaxCount(int32 _NewCount)
{
	if (this->M_Page_LoadMaxCount == _NewCount)
		return;

	this->M_Page_LoadMaxCount = _NewCount;
}

void UItemLoadManager::SetCurrentPage(int32 _NewPage)
{
	//得到最后一页
	int32 LLocalEndPage = ((int32)(this->M_Page_LoadMaxCount / (this->M_Page_CurrentColCount * this->M_Page_CurrentRowCount))) - 1;

	//设置页数
	if (LLocalEndPage < 0)
		LLocalEndPage = 0;
	else
		if (_NewPage < LLocalEndPage)
			LLocalEndPage = _NewPage;

	//设置页数
	this->M_Page_CurrentNum = LLocalEndPage;
}

int32 UItemLoadManager::GetTotal()
{
	return this->M_SourceDatas.Num();
}

int32 UItemLoadManager::GetCount()
{
	return this->M_Item_CurrentLoadCount;
}

int32 UItemLoadManager::GetCurrentPage()
{
	return this->M_Page_CurrentNum;
}

FVector2D UItemLoadManager::GetMainScrollTransfrom()
{
	return this->M_ScrollOffset;
}

void UItemLoadManager::SetResetScrollBoxOffset()
{
	//如果当前页数为起始页->下拉位移归0
	if (this->M_Page_CurrentNum == 0)
	{
		this->M_Load_Table_List->SetScrollOffset(0.f);
	}
}

void UItemLoadManager::SetMainScrollTransfrom(FVector2D _NewOffset, FVector2D _NewScale, float _Angle)
{
	this->M_ScrollOffset = _NewOffset;

	FWidgetTransform Trans;
	Trans.Scale = _NewScale;
	Trans.Angle = _Angle;
	Trans.Translation = _NewOffset;
	Trans.Shear = FVector2D(0.f);

	this->M_Load_Table_List->SetRenderTransform(Trans);

}

void UItemLoadManager::SetItemMargin(FVector2D _NewMargin)
{
	//还原偏移
	//设置行高
	this->M_Page_LineHight -= this->M_MarginOffset.Y;
	//设置行宽
	this->M_Page_LineWidth -= this->M_MarginOffset.X;

	this->M_MarginOffset = _NewMargin;

	//重新设置偏移
	//设置行高
	this->M_Page_LineHight += _NewMargin.Y;
	//设置行宽
	this->M_Page_LineWidth += _NewMargin.X;


	if (this->M_Ccroll_Condition_bHorizontal)
	{
		FMargin LMargin = 0.f;
		LMargin.Right = _NewMargin.X;
		this->M_Load_Uniform_Grid->SetSlotPadding(LMargin);

		//设置容器的大小
		UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->SetSize(FVector2D(this->M_Page_CurrentRowCount * this->M_Page_LineWidth, this->M_Page_CurrentColCount * this->M_Page_LineHight));
	}
	else {
		FMargin LMargin = 0.f;
		LMargin.Bottom = _NewMargin.Y;
		this->M_Load_Uniform_Grid->SetSlotPadding(LMargin);

		//设置容器的大小
		UWidgetBase::WidgetToCanvasSlot(this->M_Load_Table_List)->SetSize(FVector2D(this->M_Page_CurrentColCount * this->M_Page_LineWidth, this->M_Page_CurrentRowCount * this->M_Page_LineHight));
	}
}
