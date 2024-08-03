// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain2.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "Game/UI/UI_TreviFountain2Panel.h"

#include "Components/UniformGridPanel.h"
#include "Game/UI/UI_TreviFountain2Box.h"
#include "Game/UI/UI_TreviFountain2BoxSelect.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"


bool UUI_TreviFountain2::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化
	this->M_UUniformGridPanel = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_47"));
	this->M_UUniformGrid_CustomSelect_Panel = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_31"));


	this->M_UButton_Get = this->GetWidgetComponent<UButton>(this, TEXT("Item_Get"));
	this->M_UTextBlock = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_41"));
	this->M_GetItemsCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_39"));

	this->M_UI_TreviFountain2Panel = this->GetWidgetComponent<UUI_TreviFountain2Panel>(this, TEXT("BPUI_TreviFountain2Panel"));

	return true;
}

void UUI_TreviFountain2::Reset()
{
	if (this->M_UButton_Get->OnClicked.IsBound())
		this->M_UButton_Get->OnClicked.Clear();

	UWidgetBase::ButtonDelegateBind(this->M_UButton_Get, this, TEXT("GetItems_8"));
	this->PlayOperateAudioDef();

	this->M_UTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//  TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_2/2.2'")
	UWidgetBase::SetButtonStyle(this->M_UButton_Get, this->M_8GetDefTexture2DPath);

	for (auto LItem : this->M_Boxs)
	{
		LItem->Reset();
	}

	this->M_bHidd = false;
}

void UUI_TreviFountain2::Hidden()
{
	if (!this->M_bHidd)
		this->M_bHidd = true;

	if (this->M_UButton_Get->OnClicked.IsBound())
		this->M_UButton_Get->OnClicked.Clear();


	UWidgetBase::ButtonDelegateBind(this->M_UButton_Get, this, TEXT("Reset"));

	this->M_UTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	// TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_2/3.3'")
	UWidgetBase::SetButtonStyle(this->M_UButton_Get, this->M_8GetOpenTexture2DPath);
}


void UUI_TreviFountain2::ContentInit(const FString& _BoxPath, const FString& _ItemsDataTablePath)
{
	if (this->M_UButton_Get->OnClicked.IsBound())
		this->M_UButton_Get->OnClicked.Clear();

	FScriptDelegate LAudio;
	LAudio.BindUFunction(this, TEXT("GetItems_8"));
	this->M_UButton_Get->OnClicked.Add(LAudio);

	this->UpdateCustomSelectText("");

	//加载8格子
	for (int32 i = 0; i < 8; i++)
	{
		//加载界面  // TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountain2Box.BPUI_TreviFountain2Box_C'")
		// WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountain3Box.BPUI_TreviFountain3Box_C'
		UUI_TreviFountain2Box* Box = CreateWidget<UUI_TreviFountain2Box>(this, LoadClass<UUI_TreviFountain2Box>(0, *_BoxPath));
		Box->SetUI_TreviFountain2(this);
		Box->Reset();
		this->M_Boxs.Emplace(Box);
		this->M_UUniformGridPanel->AddChildToUniformGrid(Box, (i) / 4, (i)-4 * ((i) / 4));
	}

	// DataTable'/Game/Resource/BP/Data/TreviFountainData/TreviFount2_Pool_0.TreviFount2_Pool_0'

	//初始化奖池
	TArray<FTreviFountainStructData> Datas;
	UGameSystemFunction::GetDataTableRows<FTreviFountainStructData>(*_ItemsDataTablePath, Datas);

	for (auto Item : Datas)
		this->M_FTreviFountainItemStruct.Emplace(Item.M_FTreviFountainStruct);

	//初始化奖池
	this->M_FTreviFountainRandomCategoyStruct.SetRandomItems(this->M_FTreviFountainItemStruct);
	//设置概率(史诗(0.03),90次保底,优秀(0.4),10次保底)
	this->M_FTreviFountainRandomCategoyStruct.SetRadom(0.03f, 90, 0.4f, 10);

	//加载自选盒子
	int32 Count = this->M_FTreviFountainRandomCategoyStruct.GetEpicPool().Num();
	for (int32 i = 0; i < Count; i++)
	{
		UUI_TreviFountain2BoxSelect* SelectBox = CreateWidget<UUI_TreviFountain2BoxSelect>(this, LoadClass<UUI_TreviFountain2BoxSelect>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountain2BoxSelect.BPUI_TreviFountain2BoxSelect_C'")));
		SelectBox->SetUI_TreviFountain2(this);
		SelectBox->SetHead(this->M_FTreviFountainRandomCategoyStruct.GetEpicPool()[i].M_ItemHeadPath.ToString());
		SelectBox->SetItemName(this->M_FTreviFountainRandomCategoyStruct.GetEpicPool()[i].M_ItemName);
		this->M_CustonSelectBoxs.Emplace(SelectBox);
		this->M_UUniformGrid_CustomSelect_Panel->AddChildToUniformGrid(SelectBox, (i) / 4, (i)-4 * ((i) / 4));
	}
}

void UUI_TreviFountain2::Init8GetButtonStyle(const FString& _DefPath, const FString& _OpenPath)
{
	this->M_8GetDefTexture2DPath = _DefPath;
	this->M_8GetOpenTexture2DPath = _OpenPath;
}

void UUI_TreviFountain2::SetCustonSelectItemName(const FString& _Name)
{
	this->M_CustonSelectItemName = _Name;
}

void UUI_TreviFountain2::SetCustonSelectItemCount(int32 _Count)
{
	this->M_CustonSelectItemCount = _Count;
}

void UUI_TreviFountain2::SetCustonSelectItemEnable(bool _bActive)
{
	this->M_bCustonSelectItemEnable = _bActive;
}

const TArray<UUI_TreviFountain2BoxSelect*>& UUI_TreviFountain2::GetCustomBoxSelectItems()
{
	return this->M_CustonSelectBoxs;
}

FString UUI_TreviFountain2::GetCustomSelectItem()
{
	if (this->M_bCustonSelectItemEnable)
	{
		for (int32 i = 0; i < this->M_FTreviFountainRandomCategoyStruct.GetEpicPool().Num(); i++)
		{
			if (this->M_FTreviFountainRandomCategoyStruct.GetEpicPool()[i].M_ItemName.Equals(this->M_CustonSelectItemName))
			{

				this->M_Boxs[0]->SendItem(this->M_FTreviFountainRandomCategoyStruct.GetEpicPool()[i]);

				return this->M_FTreviFountainRandomCategoyStruct.GetEpicPool()[i].M_ItemHeadPath.ToString();
			}
		}
	}

	return FString();
}

FString UUI_TreviFountain2::GetCustomSelectItemName()
{
	return this->M_CustonSelectItemName;
}

bool UUI_TreviFountain2::UpdateCustomSelectText(const FString& _CurrentItemName)
{
	//更新文字(如果你没有开启自选)
	if (!this->M_bCustonSelectItemEnable)
	{
		this->M_CustomSelect = TEXT("未开启自选道具");
		this->M_CustonSelectItemCount = 0;
	}
	else {

		if (_CurrentItemName.Equals(this->M_CustonSelectItemName) && !_CurrentItemName.Equals(""))
		{
			this->M_CustonSelectItemCount = 0;

			this->M_CustomSelect = FString(TEXT("保底次数:") + FString::FromInt(this->M_CustonSelectItemCount) + TEXT("/ 2"));

			//获取自选道具
			return true;
		}
		else {
			if (this->M_CustonSelectItemCount >= 2)
			{
				this->M_CustonSelectItemCount = 0;

				this->M_CustomSelect = FString(TEXT("保底次数:") + FString::FromInt(this->M_CustonSelectItemCount) + TEXT("/ 2"));

				//触发大保底
				return true;
			}
			else {
				if (!_CurrentItemName.Equals(""))
					this->M_CustonSelectItemCount++;
			}
		}

		this->M_CustomSelect = FString(TEXT("保底次数:") + FString::FromInt(this->M_CustonSelectItemCount) + TEXT("/ 2"));
	}

	return false;
}

ETreviFountainItemQualityType UUI_TreviFountain2::GetItems_1(TArray<FTreviFountainItemStruct>& _OutResults)
{
	ETreviFountainItemQualityType LResult = UTreviFountainStruct::GetRandomItems(this->M_FTreviFountainRandomCategoyStruct, 1, 100, _OutResults);

	this->M_GetItemsCountText->SetText(FText::FromString(TEXT("当前许愿次数：") + FString::FromInt(this->M_FTreviFountainRandomCategoyStruct.GetRandomCount())));

	if (LResult == ETreviFountainItemQualityType::TQT_Epic)
		//如果抽到史诗重置抽奖次数
		this->M_FTreviFountainRandomCategoyStruct.SetRandomCount(0);

	//抽奖
	return LResult;
}

void UUI_TreviFountain2::GetItems_8()
{

	this->PlayOperateAudioDef();

	//8连抽

	//先判断点券是否充足
	if (UPlayerStructManager* PlayData = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		if (PlayData->M_FPlayerCoin.M_Coin_2 < 1600)
		{
			UWidgetBase::CreateTipWidget(TEXT("点券不够了"));
			return;
		}
	}


	this->M_GetItemsCountText->SetText(FText::FromString(TEXT("当前许愿次数：") + FString::FromInt(this->M_FTreviFountainRandomCategoyStruct.GetRandomCount())));

	//头像集合
	TArray<FString> HeadImages;

	//开启奖励
	for (int32 i = 0; i < 8; i++)
	{
		this->M_Boxs[i]->Get();

		HeadImages.Emplace(this->M_Boxs[i]->M_CurrentItemHeadPath);
	}

	//将抽奖界面显示
	//this->M_UI_TreviFountain2Panel->SetVisibility(ESlateVisibility::Visible);
	//播放动画
	this->M_UI_TreviFountain2Panel->TPlayAnimation();
	//设置图片
	this->M_UI_TreviFountain2Panel->SetItemHeads(HeadImages);

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("进行8次许愿操作")));
}
