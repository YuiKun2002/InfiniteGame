// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain.h"

#include "Data/TreviFountainStruct.h"

#include "Game/UI/UI_TreviFountianItemBox.h"

#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include <Components/RichTextBlock.h>
#include <Components/HorizontalBox.h>
#include <Components/HorizontalBoxSlot.h>
#include <Components/UniformGridPanel.h>
#include <Components/UniformGridSlot.h>
#include <Components/VerticalBox.h>
#include <Components/VerticalBoxSlot.h>

bool UUI_TreviFountain::Initialize()
{
	if (!Super::Initialize())
		return false;

	//-----------------------------------------------------------------------------------------------------
	// 奖池选项卡-按钮绑定
	this->M_TF_TabButton.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_82")));
	this->M_TF_TabButton.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_2")));
	this->M_TF_TabButton.Emplace(this->GetWidgetComponent<UButton>(this, TEXT("Button_3")));
	this->M_TF_TabDef = { FString(TEXT("180")),FString(TEXT("185")),FString(TEXT("190")) };
	this->M_TF_TabClick = { FString(TEXT("177")),FString(TEXT("183")),FString(TEXT("188")) };
	//界面绑定
	this->M_TF_PoolBgPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("Panel_1")));
	this->M_TF_PoolBgPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("Panel_2")));
	this->M_TF_PoolBgPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("Panel_3")));
	//对应界面的抽奖按钮绑定
	this->M_TF_PoolGetButtonsPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("GetPanel_1")));
	this->M_TF_PoolGetButtonsPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("GetPanel_2")));
	this->M_TF_PoolGetButtonsPanel.Emplace(this->GetWidgetComponent<UWidget>(this, TEXT("GetPanel_3")));
	//按钮事件绑定
	this->M_TF_TabButton[0]->OnClicked.AddDynamic(this, &UUI_TreviFountain::TF_Tab_0);
	this->M_TF_TabButton[1]->OnClicked.AddDynamic(this, &UUI_TreviFountain::TF_Tab_1);
	this->M_TF_TabButton[2]->OnClicked.AddDynamic(this, &UUI_TreviFountain::TF_Tab_2);


	//-----------------------------------------------------------------------------------------------------
	//奖池的按钮图片绑定
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_603")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_1")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_2")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_3")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_4")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_5")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_6")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_7")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_8")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_9")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_10")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_11")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_12")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_13")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_23")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_22")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_21")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_20")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_19")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_18")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_17")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_16")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_15")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_14")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_28")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_27")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_26")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_25")));
	this->M_ItemsPool.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("CanvasPanel_24")));

	//-----------------------------------------------------------------------------------------------------

	//富文本
	this->M_GetContentText = this->GetWidgetComponent<URichTextBlock>(this, TEXT("RichTextBlock_24"));
	//单抽展示盒
	this->M_GetOnceBox = this->GetWidgetComponent<UVerticalBox>(this, TEXT("VerticalBox_51"));
	//五连抽展示盒
	this->M_GetFifthBox = this->GetWidgetComponent<UHorizontalBox>(this, TEXT("HorizontalBox_83"));
	//十连抽展示盒
	this->M_GetTenthBox = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_171"));
	//-----------------------------------------------------------------------------------------------------


	return true;
}

void UUI_TreviFountain::UpdateCoin()
{

	this->M_PlayerCoinType_0 = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0);
	this->M_PlayerCoinType_2 = FString::FromInt(UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2);
}

void UUI_TreviFountain::LoadItemsToPanel(const FString& _DataTableName)
{
	//数据清空
	this->M_ItemsPoolData.Empty();

	//所有界面子界面清空
	for (auto& _Panel : this->M_ItemsPool)
	{
		_Panel->ClearChildren();
	}

	UTreviFountainStruct::LoadTreviFountainDatas(_DataTableName, this->M_ItemsPoolData);

	//分时加载
	this->ItemsLoadManager.SetClass(this, &UUI_TreviFountain::LoadItem);
	this->ItemsLoadManager.SetDataList(this->M_ItemsPoolData);
	this->ItemsLoadManager.Run(0.01f);
}

void UUI_TreviFountain::LoadItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index)
{
	//不能超过索引29  道具个数不会超过30个
	if (_Index > 29)
		return;

	//创建物品展示盒子
	UUI_TreviFountianItemBox* Box = CreateWidget<UUI_TreviFountianItemBox>(_Class->GetWorld(), LoadClass<UUI_TreviFountianItemBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountian_ItemBox.BPUI_TreviFountian_ItemBox_C'")));
	//设置数据
	Box->SetTreviFountainItemData(_Datas[_Index]);
	//添加到画布（并且使用自动大小）
	_Class->M_ItemsPool[_Index]->AddChildToCanvas(Box)->SetAutoSize(true);
}


//---------------------------------------------------------------------------------------------------------

//选项卡按钮(切换)
void UUI_TreviFountain::TF_Tab_0()
{
	//更新金币数量
	this->UpdateCoin();

	this->M_GetFifthBox->ClearChildren();
	this->M_GetTenthBox->ClearChildren();
	this->M_GetOnceBox->ClearChildren();

	//初始化【加载的类】
	this->ShowItemsLoadManager.InitClass(this);
	this->ShowItemsLoadManager.SetEndFunction(&UUI_TreviFountain::ShowItemText);

	UGameSystemFunction::TabSelect(this->M_TF_TabButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain/"), this->M_TF_TabDef, this->M_TF_TabClick, 0);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolGetButtonsPanel, ESlateVisibility::SelfHitTestInvisible, 0);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolBgPanel, ESlateVisibility::SelfHitTestInvisible, 0);
	UWidgetBase::PlayOperateAudio();

	this->LoadItemsToPanel(TEXT("TreviFount_Pool_0"));
	//设置奖池属性
	this->M_TreviFountainRandomCategoyStruct.SetRandomItems(this->M_ItemsPoolData);
	this->M_TreviFountainRandomCategoyStruct.SetRadom(0.05f, 30, 5.f, 10);
	//设置抽奖界面
	this->M_GetCoinType = 0;
}

void UUI_TreviFountain::TF_Tab_1()
{
	this->M_GetFifthBox->ClearChildren();
	this->M_GetTenthBox->ClearChildren();
	this->M_GetOnceBox->ClearChildren();


	UGameSystemFunction::TabSelect(this->M_TF_TabButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain/"), this->M_TF_TabDef, this->M_TF_TabClick, 1);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolBgPanel, ESlateVisibility::SelfHitTestInvisible, 1);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolGetButtonsPanel, ESlateVisibility::SelfHitTestInvisible, 1);
	UWidgetBase::PlayOperateAudio();


	this->LoadItemsToPanel(TEXT("TreviFount_Pool_1"));
	//设置奖池属性
	this->M_TreviFountainRandomCategoyStruct.SetRandomItems(this->M_ItemsPoolData);
	this->M_TreviFountainRandomCategoyStruct.SetRadom(0.05f, 80, 3.f, 10);
	//设置抽奖界面
	this->M_GetCoinType = 1;
}

void UUI_TreviFountain::TF_Tab_2()
{
	this->M_GetFifthBox->ClearChildren();
	this->M_GetTenthBox->ClearChildren();
	this->M_GetOnceBox->ClearChildren();

	UGameSystemFunction::TabSelect(this->M_TF_TabButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain/"), this->M_TF_TabDef, this->M_TF_TabClick, 2);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolBgPanel, ESlateVisibility::SelfHitTestInvisible, 2);
	UGameSystemFunction::WidgetSelect(this->M_TF_PoolGetButtonsPanel, ESlateVisibility::SelfHitTestInvisible, 2);
	UWidgetBase::PlayOperateAudio();

	this->LoadItemsToPanel(TEXT("TreviFount_Pool_2"));
	//设置奖池属性
	this->M_TreviFountainRandomCategoyStruct.SetRandomItems(this->M_ItemsPoolData);
	this->M_TreviFountainRandomCategoyStruct.SetRadom(0.05f, 90, 3.f, 10);
	//设置抽奖界面
	this->M_GetCoinType = 2;
}


void UUI_TreviFountain::BGetRandomItems(int32 _Count)
{
	if (_Count == 1)
	{
		M_GetType = 0;
		//单
		if (!this->SwitchPay(this->M_GetCoinType, 300, 200, 500))
			return;
	}

	if (_Count == 5)
	{
		M_GetType = 1;
		//5
		if (!this->SwitchPay(this->M_GetCoinType, 1500, 850, 2400))
			return;
	}

	if (_Count == 10)
	{
		M_GetType = 2;

		//10
		if (!this->SwitchPay(this->M_GetCoinType, 2900, 1600, 4500))
			return;
	}

	this->ShowItemsLoadManager.Stop();
	this->M_CurrentGetItems.Empty();
	this->M_GetFifthBox->ClearChildren();
	this->M_GetTenthBox->ClearChildren();
	this->M_GetOnceBox->ClearChildren();

	switch (UTreviFountainStruct::GetRandomItems(this->M_TreviFountainRandomCategoyStruct, _Count, 100, this->M_CurrentGetItems))
	{
	case ETreviFountainItemQualityType::TQT_Normal:
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[1]);
		break;
	case ETreviFountainItemQualityType::TQT_Excellent:
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[2]);
		break;
	case ETreviFountainItemQualityType::TQT_Epic:
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[3]);
		break;
	}

	//发送到背包
	for (auto& Item : this->M_CurrentGetItems)
		UTreviFountainStruct::SendItemToPlayerBag(Item);
}
void UUI_TreviFountain::ShowGet()
{
	switch (this->M_GetType)
	{
	case 1:this->ShowGetFifthItem(); break;
	case 2:this->ShowGetTenthItem(); break;
	default:this->ShowGetFirstItem(); break;
	}
}
bool UUI_TreviFountain::SwitchPay(int32 _Index, int32 _FirstCoin, int32 _SecondCoin, int32 _ThirdCoin)
{
	//第M_GetCoinType界面的x连抽
	switch (_Index)
	{
	case 1: {
		//支付_SecondCoin点券抽奖一次
		if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(_SecondCoin, 2))
		{
			//点券不足提示
			UWidgetBase::CreateTipWidget(TEXT("点券不足"));
			return false;
		}
	} break;
	case 2: {
		//支付_ThirdCoin点券抽奖一次
		if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(_ThirdCoin, 2))
		{
			//点券不足提示
			UWidgetBase::CreateTipWidget(TEXT("点券不足"));
			return false;
		}
	}break;
	default:
	{
		//支付_FirstCoin金币抽奖一次
		if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(_FirstCoin, 0))
		{
			//金币不足提示
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return false;
		}
	}
	break;
	}

	//更新金币数量
	this->UpdateCoin();

	//保存
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("许愿池支付操作")));

	return true;
}
void UUI_TreviFountain::ShowGetFirstItem()
{
	this->ShowItemsLoadManager.SetClass(this, &UUI_TreviFountain::LoadGetFirstItem);
	this->ShowItemsLoadManager.SetDataList(this->M_CurrentGetItems);
	this->ShowItemsLoadManager.Run(0.001f);
}
void UUI_TreviFountain::ShowGetFifthItem()
{
	this->ShowItemsLoadManager.SetClass(this, &UUI_TreviFountain::LoadGetFifthItem);
	this->ShowItemsLoadManager.SetDataList(this->M_CurrentGetItems);
	this->ShowItemsLoadManager.Run(0.001f);
}

void UUI_TreviFountain::ShowGetTenthItem()
{
	this->ShowItemsLoadManager.SetClass(this, &UUI_TreviFountain::LoadGetTenthItem);
	this->ShowItemsLoadManager.SetDataList(this->M_CurrentGetItems);
	this->ShowItemsLoadManager.Run(0.001f);
}

void UUI_TreviFountain::LoadGetFirstItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index)
{
	UUI_TreviFountianItemBox* Box = CreateWidget<UUI_TreviFountianItemBox>(_Class->GetWorld(), LoadClass<UUI_TreviFountianItemBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountian_ItemBox.BPUI_TreviFountian_ItemBox_C'")));
	Box->SetTreviFountainItemData(_Datas[_Index]);
	Box->SetRenderScale(FVector2D(1.6f, 1.6f));
	this->M_GetOnceBox->AddChildToVerticalBox(Box);
}

void UUI_TreviFountain::LoadGetFifthItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index)
{
	UUI_TreviFountianItemBox* Box = CreateWidget<UUI_TreviFountianItemBox>(_Class->GetWorld(), LoadClass<UUI_TreviFountianItemBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountian_ItemBox.BPUI_TreviFountian_ItemBox_C'")));
	Box->SetTreviFountainItemData(_Datas[_Index]);
	Box->SetRenderScale(FVector2D(1.6f, 1.6f));
	UHorizontalBoxSlot* LSlot = this->M_GetFifthBox->AddChildToHorizontalBox(Box);
	FSlateChildSize Size;
	Size.SizeRule = ESlateSizeRule::Fill;
	Size.Value = 1.f;
	LSlot->SetSize(Size);
	LSlot->SetVerticalAlignment(VAlign_Fill);
}

void UUI_TreviFountain::LoadGetTenthItem(UUI_TreviFountain* _Class, TArray<FTreviFountainItemStruct>& _Datas, int32 _Index)
{
	UUI_TreviFountianItemBox* Box = CreateWidget<UUI_TreviFountianItemBox>(_Class->GetWorld(), LoadClass<UUI_TreviFountianItemBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountian_ItemBox.BPUI_TreviFountian_ItemBox_C'")));
	Box->SetTreviFountainItemData(_Datas[_Index]);
	Box->SetRenderScale(FVector2D(1.6f, 1.6f));
	UUniformGridSlot* LSlot = this->M_GetTenthBox->AddChildToUniformGrid(Box, _Index / 5, _Index - 5 * (_Index / 5));
	LSlot->SetHorizontalAlignment(HAlign_Fill);
	LSlot->SetVerticalAlignment(VAlign_Fill);
}

void UUI_TreviFountain::ShowItemText(UUI_TreviFountain* _Class)
{
	_Class->M_GetContentText->SetText(FText());

	for (int32 i = 0; i < _Class->M_CurrentGetItems.Num(); i++)
	{
		FString Content(TEXT("<t_24_title>[获得] </>"));

		switch (_Class->M_CurrentGetItems[i].M_ItemQuality)
		{
		case ETreviFountainItemQualityType::TQT_Normal:
			Content += FString(TEXT("<t_20_blue>") + _Class->M_CurrentGetItems[i].M_ItemName.ToString() + TEXT("</>"));
			break;
		case ETreviFountainItemQualityType::TQT_Excellent:
			Content += FString(TEXT("<t_20_purple>") + _Class->M_CurrentGetItems[i].M_ItemName.ToString() + TEXT("</>"));
			break;
		case ETreviFountainItemQualityType::TQT_Epic:
			Content += FString(TEXT("<t_20_gold>") + _Class->M_CurrentGetItems[i].M_ItemName.ToString() + TEXT("</>"));
			break;
		}

		UWidgetBase::AddTextToRichText(Content, _Class->M_GetContentText);
	}


}
void UUI_TreviFountain::ClosePanel()
{
	//停止加载
	this->ItemsLoadManager.Stop();
	this->ShowItemsLoadManager.Stop();

	this->RemoveFromParent();
}
//---------------------------------------------------------------------------------------------------------