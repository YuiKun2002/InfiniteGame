// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TicketShop_0.h"
#include "Game/UI/UI_TicketShopGrid.h"

#include <Components/UniformGridPanel.h>
#include <Components/Button.h>
#include <Components/ScrollBox.h>

UWidget* UUI_TicketShop_0::WidgetCreate_Init(UItemDataTable* _Data, int32 _Index)
{
	UUI_TicketShopGrid* _Grid = CreateWidget<UUI_TicketShopGrid>(this, LoadClass<UUI_TicketShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TicketShopGrid.BPUI_TicketShopGrid_C'")));

	//设置券的图标
	_Grid->SetTicketHead(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice.M_TicketHeadTexture2DPath.ToString());
	//设置物品的头像
	_Grid->SetItemHead(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice.M_ItemHeadTexture2DPath.ToString());
	//设置数据
	_Grid->SetItemData(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice);
	//设置初始化
	_Grid->InitBase();
	//播放动画
	_Grid->Anim();

	return _Grid;
}

void UUI_TicketShop_0::WidgetRefresh_Update(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	//设置券的图标
	Cast<UUI_TicketShopGrid>(_UWidget)->SetTicketHead(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice.M_TicketHeadTexture2DPath.ToString());
	//设置物品的头像
	Cast<UUI_TicketShopGrid>(_UWidget)->SetItemHead(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice.M_ItemHeadTexture2DPath.ToString());
	//设置数据
	Cast<UUI_TicketShopGrid>(_UWidget)->SetItemData(((FItem_TicketPrice_Data*)(_Data->GetValue()))->M_FItemPrice);
	//设置初始化
	Cast<UUI_TicketShopGrid>(_UWidget)->InitBase();
	//播放动画
	Cast<UUI_TicketShopGrid>(_UWidget)->Anim();
}

void UUI_TicketShop_0::TransformData(const TArray<FItem_TicketPrice_Data>& _Datas, TArray<FItem_TicketPrice_Data>& _OutResult)
{
	for (auto& Data : _Datas)
	{
		if (Data.M_FItemPrice.M_IsShow)
		{
			_OutResult.Emplace(Data);
		}
	}
}

bool UUI_TicketShop_0::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_ShopItemPanel = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_56"));
	this->M_UScrollBox = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_19"));

	this->M_ButtonTables.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("T_Cards")));
	this->M_ButtonTables.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("T_CardSkills")));
	this->M_ButtonTables.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("T_CardMaterials")));
	this->M_ButtonTables.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("T_All")));

	return true;
}

//卡片区域
UUI_TicketShopGrid* UUI_TicketShop_0::CardsRange()
{
	return CreateWidget<UUI_TicketShopGrid>(this, LoadClass<UUI_TicketShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TicketShopGrid.BPUI_TicketShopGrid_C'")));
}

void UUI_TicketShop_0::UpdateCoin()
{
	this->M_CoinText = TEXT("0");

	for (const auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
	{
		if (Data.ItemName.ToString().Equals(TEXT("生肖兑换券")))
		{
			this->M_CoinText = FString::FromInt(Data.M_Count);
		}
	}
}

void UUI_TicketShop_0::Init()
{
	//加载数据表-卡片
	TArray<FItem_TicketPrice_Data> DataBuffer;
	UGameSystemFunction::GetDataTableRows<FItem_TicketPrice_Data>(TEXT("DataTable'/Game/Resource/BP/Data/TicketShop/Ticket_0_Card_0.Ticket_0_Card_0'"), DataBuffer);

	//数据转换
	this->TransformData(DataBuffer, this->M_Cards);
	DataBuffer.Empty();

	UGameSystemFunction::GetDataTableRows<FItem_TicketPrice_Data>(TEXT("DataTable'/Game/Resource/BP/Data/TicketShop/Ticket_0_CardSkillbooks_0.Ticket_0_CardSkillbooks_0'"), DataBuffer);

	//数据转换
	this->TransformData(DataBuffer, this->M_CardSkillbooks);
	DataBuffer.Empty();

	UGameSystemFunction::GetDataTableRows<FItem_TicketPrice_Data>(TEXT("DataTable'/Game/Resource/BP/Data/TicketShop/Ticket_0_CardChangeJob_0.Ticket_0_CardChangeJob_0'"), DataBuffer);

	//数据转换
	this->TransformData(DataBuffer, this->M_CardChangedJobs);
	DataBuffer.Empty();

	this->M_All.Append(this->M_Cards);
	this->M_All.Append(this->M_CardSkillbooks);
	this->M_All.Append(this->M_CardChangedJobs);

	this->M_Ticket_UItemLoadManager = NewObject<UItemLoadManager>(this, TEXT("Ticket_UItemLoadManager"));
	this->M_Ticket_UItemLoadManager->InitWidget_First(this->M_ShopItemPanel, this->M_UScrollBox, 0.01f);
	this->M_Ticket_UItemLoadManager->InitRange_Second(5, 4, 9999, 400.f, 170.f);
	this->M_Ticket_UItemLoadManager->SetItemMargin(FVector2D(80.f, 0.f));
	this->M_Ticket_UItemLoadManager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Init"));
	this->M_Ticket_UItemLoadManager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_Update"));
}

void UUI_TicketShop_0::Cards()
{
	this->UpdateCoin();

	UGameSystemFunction::TabSelect(this->M_ButtonTables, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TicketShop/"), this->M_ButtTab_Def_, this->M_ButtTab_Click_, 0);


	this->M_Ticket_UItemLoadManager->UpdateDatatable(this->M_Cards);
	this->M_Ticket_UItemLoadManager->SetLoadItemMaxCount(this->M_Cards.Num());
	this->M_Ticket_UItemLoadManager->SetCurrentPage(0);
	this->M_Ticket_UItemLoadManager->SetResetScrollBoxOffset();
	this->M_Ticket_UItemLoadManager->ContinueRun();
}

void UUI_TicketShop_0::CardSkillbooks()
{
	this->UpdateCoin();

	UGameSystemFunction::TabSelect(this->M_ButtonTables, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TicketShop/"), this->M_ButtTab_Def_, this->M_ButtTab_Click_, 1);

	this->M_Ticket_UItemLoadManager->UpdateDatatable(this->M_CardSkillbooks);
	this->M_Ticket_UItemLoadManager->SetLoadItemMaxCount(this->M_CardSkillbooks.Num());
	this->M_Ticket_UItemLoadManager->SetCurrentPage(0);
	this->M_Ticket_UItemLoadManager->SetResetScrollBoxOffset();
	this->M_Ticket_UItemLoadManager->ContinueRun();
}

void UUI_TicketShop_0::CardChangedJobs()
{
	this->UpdateCoin();

	UGameSystemFunction::TabSelect(this->M_ButtonTables, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TicketShop/"), this->M_ButtTab_Def_, this->M_ButtTab_Click_, 2);

	this->M_Ticket_UItemLoadManager->UpdateDatatable(this->M_CardChangedJobs);
	this->M_Ticket_UItemLoadManager->SetLoadItemMaxCount(this->M_CardChangedJobs.Num());
	this->M_Ticket_UItemLoadManager->SetCurrentPage(0);
	this->M_Ticket_UItemLoadManager->SetResetScrollBoxOffset();
	this->M_Ticket_UItemLoadManager->ContinueRun();
}

void UUI_TicketShop_0::TicketShop_0_All()
{
	this->UpdateCoin();

	UGameSystemFunction::TabSelect(this->M_ButtonTables, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TicketShop/"), this->M_ButtTab_Def_, this->M_ButtTab_Click_, 3);

	this->M_Ticket_UItemLoadManager->UpdateDatatable(this->M_All);
	this->M_Ticket_UItemLoadManager->SetLoadItemMaxCount(this->M_All.Num());
	this->M_Ticket_UItemLoadManager->SetCurrentPage(0);
	this->M_Ticket_UItemLoadManager->SetResetScrollBoxOffset();
	this->M_Ticket_UItemLoadManager->ContinueRun();
}
