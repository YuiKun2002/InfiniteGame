// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain2Box.h"
#include "Game/UI/UI_TreviFountain2.h"


#include <Components/CanvasPanelSlot.h>

#include <Components/Button.h>
#include <Components/Image.h>

bool UUI_TreviFountain2Box::Initialize()
{
	if (!Super::Initialize())
		return false;

	// Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_2/80.80'
	// Texture2D'/Game/Resource/Texture/UI/Game/TreviFountain_2/83.83'
	// DataTable'/Game/Resource/BP/Data/TreviFountainData/TreviFount2_Pool_0.TreviFount2_Pool_0'

	this->M_UButton = this->GetWidgetComponent<UButton>(this, TEXT("Button_40"));
	this->M_UImage = this->GetWidgetComponent<UImage>(this, TEXT("Image_70"));
	this->M_UImageEpicTip = this->GetWidgetComponent<UImage>(this, TEXT("Image_48"));


	this->M_UButton->OnClicked.AddDynamic(this, &UUI_TreviFountain2Box::Get);
	this->M_UImage->SetVisibility(ESlateVisibility::Collapsed);
	this->M_UImageEpicTip->SetVisibility(ESlateVisibility::Collapsed);

	return true;
}

void UUI_TreviFountain2Box::SetUI_TreviFountain2(UUI_TreviFountain2* _UI_TreviFountain2)
{
	this->M_UUI_TreviFountain2 = _UI_TreviFountain2;
}

void UUI_TreviFountain2Box::Reset()
{
	this->M_bGet = false;

	this->SetBoxStyle(TEXT(""), this->M_bGet);
}

void UUI_TreviFountain2Box::SetFinish()
{
	this->M_bGet = true;
}

void UUI_TreviFountain2Box::SetBoxStyle(const FString& _ItemHeadPath, bool _bOpened)
{
	if (_bOpened)
		this->M_UImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		this->M_UImage->SetVisibility(ESlateVisibility::Collapsed);

	if (_bOpened)
	{
		UWidgetBase::SetImageBrush(this->M_UImage, _ItemHeadPath, FVector(1.f), 1.f, _bOpened, FVector(1.f, 1.f, 0.f), true);
	}


	if (UCanvasPanelSlot* C = UWidgetBase::WidgetToCanvasSlot(this->M_UButton))
	{
		if (_bOpened)
		{
			// 317 240 38
			C->SetSize(FVector2D(this->OffsetScaleAndPosition.X, this->OffsetScaleAndPosition.Y));
			C->SetPosition(FVector2D(this->OffsetScaleAndPosition.Z, 0.f));
			UWidgetBase::SetButtonStyle(this->M_UButton, this->M_BoxOpenTexture2DPath);
		}
		else {

			// 240
			C->SetSize(FVector2D(this->DefScaleAndPosition.X, this->DefScaleAndPosition.Y));
			C->SetPosition(FVector2D(this->DefScaleAndPosition.Z, 0.f));
			UWidgetBase::SetButtonStyle(this->M_UButton, this->M_BoxDefTexture2DPath);
			//抽奖极品道具显示
			this->M_UImageEpicTip->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUI_TreviFountain2Box::Get()
{
	if (this->M_bGet)
	{
		//非Windows平台  才可以显示
		if (!UGameplayStatics::GetPlatformName().Equals(TEXT("Windows")))
			UWidgetBase::CreateTipWidget(this->M_Name);
		return;
	}

	//获取玩家货币
	if (UPlayerStructManager* Player = UFVMGameInstance::GetPlayerStructManager_Static())
	{
		if (Player->M_FPlayerCoin.M_Coin_2 < this->M_GetPrice)
		{
			UWidgetBase::CreateTipWidget(FString(TEXT("点券不足,") + FString::FromInt(this->M_GetPrice) + TEXT("点券一次")));
			return;
		}
		else {
			Player->ReduceCoin(this->M_GetPrice, 2);
		}
	}



	this->M_bGet = true;

	this->M_UUI_TreviFountain2->Hidden();

	TArray<FTreviFountainItemStruct> OutResults;
	ETreviFountainItemQualityType Type = this->M_UUI_TreviFountain2->GetItems_1(OutResults);

	//自选道具是否触发保底机制
	bool L_CustomItem = false;

	//如果是史诗级别则显示提示
	if (Type == ETreviFountainItemQualityType::TQT_Epic)
	{
		//抽奖极品道具显示
		this->M_UImageEpicTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//更新次数
		L_CustomItem = this->M_UUI_TreviFountain2->UpdateCustomSelectText(OutResults[0].M_ItemName);
	}

	//触发了保底
	if (L_CustomItem)
	{
		//获取保底道具名称
		this->M_Name = this->M_UUI_TreviFountain2->GetCustomSelectItemName();

		//直接获取最终道具(发送到背包)
		this->M_CurrentItemHeadPath = this->M_UUI_TreviFountain2->GetCustomSelectItem();

		if (this->M_CurrentItemHeadPath.Equals(""))
		{
			this->M_CurrentItemHeadPath = OutResults[0].M_ItemHeadPath.ToString();
		}
	}
	else
	{
		this->M_CurrentItemHeadPath = OutResults[0].M_ItemHeadPath.ToString();

		//获取保底道具名称
		this->M_Name = OutResults[0].M_ItemName;

		this->SendItem(OutResults[0]);
	}

	this->SetBoxStyle(this->M_CurrentItemHeadPath, this->M_bGet);



	//播放动画
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);


	UGameSystemFunction::SendMaterialToPlayerBag(TEXT("三级四叶草"), 1);
}

FString UUI_TreviFountain2Box::ToString_Implementation()
{
	return this->M_Name;
}

void UUI_TreviFountain2Box::SendItem(const FTreviFountainItemStruct& _Item)
{
	//发送的结果
	bool LSendResult = false;

	switch (_Item.M_ItemType)
	{
	case ETreviFountainItemType::TT_Equipment:LSendResult = UGameSystemFunction::SendEquipmentToPlayerBag(_Item.M_ItemName, _Item.M_ItemCount, false); break;
	case ETreviFountainItemType::TT_Card:LSendResult = UGameSystemFunction::SendCardToPlayerBag(_Item.M_ItemName, 0); break;
	case ETreviFountainItemType::TT_Material:LSendResult = UGameSystemFunction::SendMaterialToPlayerBag(_Item.M_ItemName, _Item.M_ItemCount, false); break;
	}

	if (!LSendResult)
	{
		//如果道具发送失败则发送货币资源
		TArray<FString> Items = { FString(TEXT("金币")),FString(TEXT("礼券")),FString(TEXT("点券")),FString(TEXT("威望")) };
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (_Item.M_ItemName.Equals(Items[i]))
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(_Item.M_ItemCount, i);
			}
		}
	}
}

void UUI_TreviFountain2Box::InitBoxImgSource(const FString& _DefPath, const FString& _OpenPath, FVector _DefScaleAndPosition, FVector _OffsetScaleAndPosition, int32 GetItemCoin)
{
	this->M_BoxDefTexture2DPath = _DefPath;
	this->M_BoxOpenTexture2DPath = _OpenPath;
	this->DefScaleAndPosition = _DefScaleAndPosition;
	this->OffsetScaleAndPosition = _OffsetScaleAndPosition;
	this->M_GetPrice = GetItemCoin;

	this->Reset();
}
