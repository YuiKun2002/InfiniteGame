// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_CardChange.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"

#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void FMainCardChangeType::ShowStyle(class USynModel_CardChange* Class)
{
	Class->GetChangeCardButt()->SetIsEnabled(true);

	UWidgetBase::SetImageBrush(
		this->FCardChangeMaterials[0].MaterialImg,
		TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_22.T_PS_22'")
	);
	UWidgetBase::SetImageBrush(
		this->FCardChangeMaterials[1].MaterialImg,
		TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_23.T_PS_23'")
	);
	UWidgetBase::SetImageBrush(
		this->FCardChangeMaterials[2].MaterialImg,
		TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_24.T_PS_24'")
	);

	Class->GetChangeMaterial3P()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (this->PlayerBagIndex == -1)
	{
		Class->GetMainChangeCardButt()->OnClicked.Clear();
		Class->GetMainChangeCardButt()->OnClicked.AddDynamic(
			Class->GetPlayerSynthesis(),
			&UWidgetBase::PlayOperateAudioDef
		);

		UWidgetBase::SetButtonStyle(
			Class->GetMainChangeCardButt(),
			TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'")
		);

		for (FCardChangeMaterialType& CurData : this->FCardChangeMaterials)
		{
			CurData.PlayerBagIndex = -1;
		}

		Class->GetChangeCardButt()->SetIsEnabled(false);

		Class->SetChangeCardRateText(TEXT("0%"));

		//设置保险金金额
		Class->GetPlayerSynthesis()->SetInsuranceText(TEXT("0"));

		return;
	}
	else {
		Class->GetMainChangeCardButt()->OnClicked.Clear();
		Class->GetMainChangeCardButt()->OnClicked.AddDynamic(
			Class,
			&USynModel_CardChange::CancelMainCard
		);
		Class->GetMainChangeCardButt()->OnClicked.AddDynamic(
			Class->GetPlayerSynthesis(),
			&UWidgetBase::PlayOperateAudioDef
		);

		UWidgetBase::SetButtonStyle(
			Class->GetMainChangeCardButt(),
			this->ChangeCardHeadPath
		);
	}

	//设置成功率
	Class->SetChangeCardRateText(TEXT("20%"));
	this->Rate = 20;


	bool bFinish = true;
	for (const FCardChangeMaterialType& CurData : this->FCardChangeMaterials)
	{

		if (CurData.Visible == ESlateVisibility::Collapsed)
		{
			Class->GetChangeMaterial3P()->SetVisibility(ESlateVisibility::Collapsed);

			if (bFinish)
			{
				Class->SetChangeCardRateText(TEXT("30%"));
				this->Rate = 30;
			}

			continue;
		}

		if (CurData.PlayerBagIndex == -1)
		{
			UWidgetBase::SetImageBrush(
				CurData.MaterialImg,
				CurData.ChangeMaterialHeadPath, COLOR_DARK
			);

			Class->GetChangeCardButt()->SetIsEnabled(false);

			bFinish = false;

			Class->SetChangeCardRateText(TEXT("0%"));
			this->Rate = 0;
		}
		else {
			UWidgetBase::SetImageBrush(
				CurData.MaterialImg,
				CurData.ChangeMaterialHeadPath
			);
		}
	}

	//设置保险金金额
	Class->GetPlayerSynthesis()->SetInsuranceText(
		FString::FromInt(this->CardChangeJobInsurance[this->CardGrade])
	);
}


float FMainCardChangeType::GetCardChangeJobInsurancePrice(const int32& Grade)
{
	return this->CardChangeJobInsurance[Grade];
}

void USynModel_CardChange::InitializeBySynthesis(class UUI_PlayerSynthesis* Class)
{
	this->PlayerSynthesis = Class;

	this->MainCardChangeTypeData.FCardChangeMaterials[0].MaterialImg =
		this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, "Image_118");

	this->MainCardChangeTypeData.FCardChangeMaterials[1].MaterialImg =
		this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, "Image_11");

	this->MainCardChangeTypeData.FCardChangeMaterials[2].MaterialImg =
		this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, "Image_12");


	this->ChangeMaterial3P = this->PlayerSynthesis->GetWidgetComponent<UCanvasPanel>(this->PlayerSynthesis, "CanvasPanel_4");

	this->MainChangeCard_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, "Button_5");
	this->ChangeCard_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, "ChangeCardType_Butt");
	this->ChangeCardRate_Text = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, "Change_Rate");
	//绑定转职功能
	this->ChangeCard_Butt->OnClicked.AddDynamic(this, &USynModel_CardChange::ChangeCard);

	this->MainCardChangeTypeData.ShowStyle(this);
}

void USynModel_CardChange::WidgetReset()
{
	//绑定加载
	this->PlayerSynthesis->OnCardLoad.Unbind();
	this->PlayerSynthesis->OnCardLoad.BindUObject(this, &USynModel_CardChange::OnCardLoad);
	//取消卡片的转职选择
	this->CancelMainCard();
}

void USynModel_CardChange::WidgetResetLoadData()
{
	//加载卡片
	this->LoadCards();
	//加载材料
	this->LoadMaterials();
}

void USynModel_CardChange::LoadCards()
{
	//背包容量
	int32 BagCount = UFVMGameInstance::GetFVMGameInstance()->
		GetPlayerStructManager()->
		M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, BagCount);

	//排序
	TArray<FItemCard*> CanChangeCards;
	TArray<FItemCard*> NotChangeCards;

	//获取可以进行转职的防御卡
	for (FItemCard* CurCard : Datas)
	{
		if (CurCard && !CurCard->bWaitRemove)
		{
			if (CurCard->M_FCardChangeJobs.M_IsChange && CurCard->M_CardGrade >= CurCard->M_FCardChangeJobs.M_ChangeGrade)
			{
				CanChangeCards.Emplace(CurCard);
			}
			else {
				NotChangeCards.Emplace(CurCard);
			}
		}
	}

	//合并数组
	CanChangeCards.Append(NotChangeCards);

	this->PlayerSynthesis->LoadCards(CanChangeCards,BagCount);
}

void USynModel_CardChange::LoadMaterials()
{
	this->PlayerSynthesis->LoadMaterials(
		{
		FMaterialsSerachTypeBind(EMaterialType::E_CardChangeJobMaterial,{TEXT("")})
		});
}

UButton* USynModel_CardChange::GetMainChangeCardButt()
{
	return this->MainChangeCard_Butt;
}

UButton* USynModel_CardChange::GetChangeCardButt()
{
	return this->ChangeCard_Butt;
}

void USynModel_CardChange::SetChangeCardRateText(const FString& Text)
{
	this->ChangeCardRate_Text->SetText(FText::FromString(Text));
}

UUI_PlayerSynthesis* USynModel_CardChange::GetPlayerSynthesis()
{
	return this->PlayerSynthesis;
}

UCanvasPanel* USynModel_CardChange::GetChangeMaterial3P()
{
	return this->ChangeMaterial3P;
}

FMainCardChangeType& USynModel_CardChange::GetFMainCardChangeData()
{
	return this->MainCardChangeTypeData;
}

void USynModel_CardChange::CancelMainCard()
{
	this->MainCardChangeTypeData.PlayerBagIndex = -1;
	this->MainCardChangeTypeData.UIGrideIndex = -1;
	this->MainCardChangeTypeData.ShowStyle(this);
	this->WidgetResetLoadData();
}

void USynModel_CardChange::OnCardLoad(UUI_PlayerBagCardGrid* Grid, UItemDataTable* Data, int32 Index)
{
	FItemCard* CurData = ((FItemCard*)(Data->GetValue()));
	if (!CurData->M_FCardChangeJobs.M_IsChange || CurData->M_FCardChangeJobs.M_IsChange && CurData->M_CardGrade < CurData->M_FCardChangeJobs.M_ChangeGrade)
	{
		Grid->GetButton()->SetIsEnabled(false);
	}
	else {
		Grid->GetButton()->OnClicked.AddDynamic(Grid, &UUI_PlayerBagCardGrid::AddSynthesisChangeJobs);

		//有卡片选择
		if (this->MainCardChangeTypeData.PlayerBagIndex != -1)
		{
			if (this->MainCardChangeTypeData.UIGrideIndex == Index)
			{
				Grid->GetButton()->SetIsEnabled(false);
				this->MainCardChangeTypeData.Grid = Grid;
			}
		}
	}
}

void USynModel_CardChange::ChangeCard()
{

	// GetSelectInsurance
	// Update_CoinText



	//（是否开启保险金）
	if (this->PlayerSynthesis->GetSelectInsurance())
	{
		//先确认金币是否充足
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			if (
				UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2 >=
				this->MainCardChangeTypeData.GetCardChangeJobInsurancePrice(this->MainCardChangeTypeData.CardGrade)
				)
			{
				//支付点券
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(
					this->MainCardChangeTypeData.GetCardChangeJobInsurancePrice(this->MainCardChangeTypeData.CardGrade)
					, 2);
				//支付金币
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);

			}
			else {
				UWidgetBase::CreateTipWidget(TEXT("点券不足"));
				return;
			}
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}
	else {
		//先确认金币是否充足
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			//支付金币
			UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}

	//匹配结果
	bool bResult = UGameSystemFunction::GetRange(this->MainCardChangeTypeData.Rate, 100);

	//是否要消耗材料
	bool bFinish = true;

	if (!bResult && this->PlayerSynthesis->GetSelectInsurance())
	{
		bFinish = false;
	}

	//更新货币
	this->PlayerSynthesis->Update_CoinText();

	//打印提升
	if (bResult)
	{
		UWidgetBase::CreateTipWidget(TEXT("转职成功"));
	}
	else
	{
		UWidgetBase::CreateTipWidget(TEXT("不够好运，转职失败"));
	}

	//不消耗材料
	if (!bFinish)
	{
		return;
	}

	//转职成功
	if (bResult)
	{
		FItemCard NewCardData;
		if (UCardBaseStruct::SearchCardFromDataTable(
			this->MainCardChangeTypeData.TargetChangeCardName,
			NewCardData,
			true,
			this->MainCardChangeTypeData.TargetType
		))
		{
			//获取目标卡片数据
			FItemCard& CurData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
				this->MainCardChangeTypeData.PlayerBagIndex
			];
			//存储等级
			int32 CurDataGrade = CurData.M_CardGrade;
			//赋值新卡片数据
			CurData = NewCardData;
			//赋值等级
			CurData.M_CardGrade = CurDataGrade;
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("卡片转职失败，目标卡片不存在!!"));
			return;
		}
	}

	//消耗材料
	for (const FCardChangeMaterialType& CurMaterialData : this->MainCardChangeTypeData.FCardChangeMaterials)
	{
		if (CurMaterialData.PlayerBagIndex != -1)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->UseMaterial(
				CurMaterialData.PlayerBagIndex,
				CurMaterialData.ChangeMaterialName,
				1,
				false
			);
		}
	}

	//清除材料
	UFVMGameInstance::GetPlayerStructManager_Static()->Remove_Item(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material
	);

	//保存游戏
	UGameSystemFunction::SaveCurrentPlayerData();

	//取消选择
	this->CancelMainCard();

	//重新加载数据
	this->WidgetResetLoadData();
}

