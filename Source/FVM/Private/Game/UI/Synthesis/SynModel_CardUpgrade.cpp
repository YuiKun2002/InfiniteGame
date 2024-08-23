// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>

#include "GameSystem/TaskSubsystem.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"


#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void USynModel_CardUpgrade::InitializeBySynthesis(UUI_PlayerSynthesis* Class)
{
	this->PlayerSynthesis = Class;

	//初始化四叶草按钮
	this->M_Clover_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Button_3"));

	//四叶草界面
	this->ScrollBox_Clover = this->PlayerSynthesis->GetWidgetComponent<UScrollBox>(
		this->PlayerSynthesis, TEXT("ScrollBox_UniformFloverMater")
	);
	this->UniformGridPanel_Clover = this->PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(
		this->PlayerSynthesis, TEXT("Items_Spice_UniformFloverMater")
	);

	//卡片界面
	this->ScrollBox_Card_Uniform = this->PlayerSynthesis->GetWidgetComponent<UScrollBox>(
		this->PlayerSynthesis, TEXT("ScrollBox_Cards")
	);
	this->Uniform_CardGrid = this->PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(
		this->PlayerSynthesis, TEXT("Items_Cards")
	);

	//初始化卡槽
	for (int32 i = 0; i < 4; i++)
	{
		this->CardDatas.Emplace(FSynModelCardUpgradeData());
	}

	//初始化主卡
	this->InitWidgets(
		this->MainCard_But, TEXT("Button_98"),
		this->MainCard_GradeImg, TEXT("Image_14"),
		this->MainCard_GradeText, TEXT("TextBlock_2")
	);
	this->MainCard_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardMain);

	//初始化副卡1
	this->InitWidgets(
		this->SecondCard1_But, TEXT("Button"),
		this->SecondCard1_GradeImg, TEXT("Image_4"),
		this->SecondCard1_GradeText, TEXT("TextBlock")
	);
	this->SecondCard1_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond1);

	//初始化副卡2
	this->InitWidgets(
		this->SecondCard2_But, TEXT("Button_1"),
		this->SecondCard2_GradeImg, TEXT("Image_5"),
		this->SecondCard2_GradeText, TEXT("TextBlock_1")
	);
	this->SecondCard2_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond2);

	//初始化副卡3
	this->InitWidgets(
		this->SecondCard3_But, TEXT("Button_2"),
		this->SecondCard3_GradeImg, TEXT("Image_6"),
		this->SecondCard3_GradeText, TEXT("TextBlock_6")
	);
	this->SecondCard3_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond3);

	this->M_UpGradeCard_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Upgrade"));
	this->M_UpGradeCard_Butt->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CardUpgrade);


	this->UpgradePriceText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, TEXT("Coin_Upgrade"));
	this->UpgradeRateText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, TEXT("Rate"));
}

void USynModel_CardUpgrade::WidgetReset()
{
	//绑定加载器函数
	this->OnCardLoad.Unbind();
	this->OnCardLoad.BindUObject(this, &USynModel_CardUpgrade::OnLoadCardFunc);

	//取消四叶草的选择
	this->CancelSelectClover();
	//禁止刷新卡片
	this->bRefreshCards = false;
	//取消卡片选择
	this->CancelSelectCardMain();
	this->CancelSelectCardSecond1();
	this->CancelSelectCardSecond2();
	this->CancelSelectCardSecond3();
	//开启刷新卡片
	this->bRefreshCards = true;
	//初始化成功率
	this->UpdateUpRateText(TEXT("0.00%"));
}

void USynModel_CardUpgrade::WidgetResetLoadData()
{
	//加载卡片
	this->LoadCards();

	//加载四叶草
	this->LoadCloversToMakeCard(
		{
		{ FMaterialsSerachTypeBind(EMaterialType::E_Clover,{"AddUpGradeCardCloverSlot"}) }
		}, FMaterialsSerachKeyWordsIgnore());
}

void USynModel_CardUpgrade::SetSelectClover(FMaterialBase CopyData, float CloverRate)
{
	this->SelectCloverName = CopyData.ItemName.ToString();
	this->SelectCloverIndex = UFVMGameInstance::GetPlayerStructManager_Static()->GetMaterialByBagID(CopyData.BagID);
	this->SelectCloverRate = CloverRate;
	this->CloverData = CopyData;

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(this->SelectCloverName + " " +
				FString::FromInt(this->SelectCloverIndex) + " " +
				FString::SanitizeFloat(this->SelectCloverRate)
			));
	}

	//如果四叶草无效，取消选择
	if (this->SelectCloverIndex == -1)
	{
		this->CancelSelectClover();
	}
}

void USynModel_CardUpgrade::SetSelectCloverUI(class UUI_PlayerBagMaterialGrid* UIClover)
{
	this->UICloverGrid = UIClover;

	//取消四叶草按钮的所有绑定
	this->M_Clover_Butt->OnClicked.Clear();
	this->M_Clover_Butt->OnClicked.AddDynamic(
		this, &USynModel_CardUpgrade::CancelSelectClover);
	this->M_Clover_Butt->OnClicked.AddDynamic(
		this->PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
}

void USynModel_CardUpgrade::CancelSelectClover()
{
	//取消四叶草按钮的所有绑定
	this->M_Clover_Butt->OnClicked.Clear();
	this->M_Clover_Butt->OnClicked.AddDynamic(
		this->PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
	//取消四叶草的选择
	this->SelectCloverIndex = -1;
	//还原四叶草的显示UI
	UWidgetBase::SetButtonStyle(
		this->M_Clover_Butt,
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_25.T_PS_25'"
	);
	this->M_Clover_Butt->SetVisibility(ESlateVisibility::Collapsed);
	//更新概率
	this->GetUpGradeRate();
	//加载四叶草
	this->LoadCloversToMakeCard(
		{
		{ FMaterialsSerachTypeBind(EMaterialType::E_Clover,{"AddUpGradeCardCloverSlot"}) }
		}, FMaterialsSerachKeyWordsIgnore());
}

int32 USynModel_CardUpgrade::GetSelectClover(FString& OutCloverName)
{
	OutCloverName = this->SelectCloverName;
	return this->SelectCloverIndex;
}

UButton* USynModel_CardUpgrade::GetCloverButton()
{
	return this->M_Clover_Butt;
}

class UUI_PlayerBagMaterialGrid* USynModel_CardUpgrade::GetSelectCloverUI()
{
	return this->UICloverGrid;
}

FString USynModel_CardUpgrade::GetCurrentCardName()
{
	return this->CurUpCardName;
}

//【卡片强化数据】获取卡片强化升级的概率
float USynModel_CardUpgrade::GetUpGradeRate()
{
	//如果主卡格子无效   概率为0
	if (this->CardDatas[0].PlayerBagIndex == -1)
	{
		//更新文字
		this->UpdateUpRateText(FString(TEXT("0.00%")));
		return 0.f;
	}

	//概率
	float Rate = 0.0f;
	//主卡等级
	int32 _MainCardGrade = 0;
	//索引
	int32 _Index_ = 0;
	//当前卡片概率（概率）
	TArray<float> _L_CardRate;
	for (; _Index_ < 4; _Index_++)
	{
		//获取主卡的概率 （第一次）
		if (_Index_ == 0)
		{
			//获取主卡等级
			_MainCardGrade = this->CardDatas[_Index_].CardData.M_CardGrade;

			if (_MainCardGrade == 16)
			{
				this->UpdateUpRateText(FString(TEXT("0.00%")));
				return 0.f;
			}

			continue;
		}

		//副卡
		if (_Index_ != 0 && this->CardDatas[_Index_].PlayerBagIndex != -1)
		{
			//当前概率
			float _L_Rate = 0.f;
			//当前卡槽副卡价格
			float _CardPrice = this->CardDatas[_Index_].CardData.M_CardPrice;
			//主卡减去副卡差
			int32 X = (_MainCardGrade - this->CardDatas[_Index_].CardData.M_CardGrade);

			//如果卡片特殊
			if (this->CardDatas[_Index_].CardData.M_SpecialCard_SynthesisUpGrade)
			{
				_L_Rate = GetCardPriceGradeRate(600.0f, _MainCardGrade, X);
			}
			else
			{
				_L_Rate = GetCardPriceGradeRate(_CardPrice, _MainCardGrade, X);
			}

			//将卡片概率添加到序列
			_L_CardRate.Emplace(_L_Rate);
		}
		else {
			//无效卡
			_L_CardRate.Emplace(0.f);
		}

	}

	//排序大小(概率)
	_L_CardRate.Sort([](const float& _A, const float& _B) {return _A > _B; });

	//设置概率
	uint8 _Index = 0u;

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("[UI_PlayerSynthesis.cpp UUI_PlayerSynthesis::GetUpGradeRate]"));
		UE_LOG(LogTemp, Error, TEXT("-------------合成屋概率展示(降序)-------------"));
	}

	for (auto Item : _L_CardRate) {
		if (_Index == 0u)
		{
			//第一个最大的概率不衰减
			Rate += Item;
			_Index += 1u;

			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("卡片强化概率：%.2f"), Item);
			}
		}
		else {
			//剩下的概率衰减原来的 0.3
			Rate += Item * 0.3f;

			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("卡片强化概率：%.2f"), Item * 0.3f);
			}
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------ \n\n"));
	}


	//是否有四叶草
	if (this->SelectCloverIndex != -1)
	{
		Rate *= this->SelectCloverRate;
	}

	//限制概率100 显示100
	if (Rate > 100.f)
	{
		Rate = 100.f;
	}

	//更新文字
	this->UpdateUpRateText(FString(
		FString::Printf(TEXT("%.2f"), Rate) + TEXT("%")
	));

	return Rate;
}

//【卡片强化数据】获取卡片强化升级的概率
float USynModel_CardUpgrade::GetCardPriceGradeRate(float _Price, int32 _CardGrade, int32 MainCard_CurrentCard_Grade)
{
	float Rate = 0.f;

	//同星
	if (MainCard_CurrentCard_Grade == 0)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_0_Hei[_CardGrade];
		}
	}//差1星
	else if (MainCard_CurrentCard_Grade == 1)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_1_Hei[_CardGrade];
		}
	}//差2星
	else if (MainCard_CurrentCard_Grade == 2)
	{
		if (_Price < 100)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			Rate = M_UpGradeRate_0_Arrays_2_Hei[_CardGrade];
		}
	}
	else if (MainCard_CurrentCard_Grade > 3) // 差3直接0
	{
		return  0.f;
	}//比主高1
	else if (MainCard_CurrentCard_Grade == -1)
	{
		if (_Price < 100)
		{
			return M_UpGradeRate_0_Arrays_P1_Low[_CardGrade];
		}
		else if (_Price >= 100 && _Price < 200)
		{
			return M_UpGradeRate_0_Arrays_P1_Mid[_CardGrade];
		}
		else if (_Price >= 200)
		{
			return M_UpGradeRate_0_Arrays_P1_Hei[_CardGrade];
		}

	}//比主高2
	else if (MainCard_CurrentCard_Grade <= -2) {
		return 100.f;
	}

	return Rate;
}

void USynModel_CardUpgrade::AddCardDataMain(FSynModelCardUpgradeData CopyData)
{
	for (int32 i = 0; i < 4; i++)
	{
		if (this->AddCard(i, CopyData))
		{
			break;
		}
	}

	//更新概率
	this->GetUpGradeRate();
	//检查强化
	this->CheckCardUpgrade();
	//更新外观
	this->UpdateCardListStyle();
}

void USynModel_CardUpgrade::OnLoadCardFunc(class UUI_PlayerBagCardGrid* Grid, class UItemDataTable* Data, int32 Index)
{
	//绑定卡片选择事件
	Grid->GetButton()->OnClicked.AddDynamic(Grid, &UUI_PlayerBagCardGrid::AddUpGradeFunc);

	for (FSynModelCardUpgradeData& CurData : this->CardDatas)
	{
		FItemCard CardData = Data->GetTransValue<FItemCard>();
		if (CurData.PlayerBagIndex == Index)
		{
			CurData.CardGrid = Grid;
			CurData.CardData = Data->GetTransValue<FItemCard>();
			CurData.CardGrid->GetButton()->SetIsEnabled(false);
		}
	}
}

void USynModel_CardUpgrade::CancelSelectCardMain()
{
	this->CancelSelectCard(0, this->MainCard_But, this->MainCard_GradeImg, this->MainCard_GradeText);
	//this->PlayerSynthesis->SetInsuranceText(TEXT("0"));
}

void USynModel_CardUpgrade::CancelSelectCardSecond1()
{
	this->CancelSelectCard(1, this->SecondCard1_But, this->SecondCard1_GradeImg, this->SecondCard1_GradeText);
}

void USynModel_CardUpgrade::CancelSelectCardSecond2()
{
	this->CancelSelectCard(2, this->SecondCard2_But, this->SecondCard2_GradeImg, this->SecondCard2_GradeText);
}

void USynModel_CardUpgrade::CancelSelectCardSecond3()
{
	this->CancelSelectCard(3, this->SecondCard3_But, this->SecondCard3_GradeImg, this->SecondCard3_GradeText);
}

void USynModel_CardUpgrade::CancelSelectCard(const int32 Index, UButton* LMainCard_But, UImage* LMainCard_GradeImg, UTextBlock* LMainCard_GradeText)
{
	//取消选择
	this->CardDatas[Index].PlayerBagIndex = -1;

	//更新外观
	this->UpdateCardListStyle();

	this->GetUpGradeRate();
	this->CheckCardUpgrade();

	if (this->bRefreshCards)
	{
		this->LoadCards();
	}
}

void USynModel_CardUpgrade::UpdateCardListStyle()
{
	auto SetImage = [&](UImage* _img, const int32& Index)->void {
		if (this->CardDatas[Index].PlayerBagIndex != -1)
		{
			_img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UWidgetBase::SetImageBrush(_img, UGameSystemFunction::GetCardGradeImagePath(this->CardDatas[Index].CardData.M_CardGrade));
		}
		};

	if (this->CardDatas[0].PlayerBagIndex != -1)
	{
		UWidgetBase::SetButtonStyle(
			this->MainCard_But,
			this->CardDatas[0].CardData.ItemTexturePath.ToString()
		);
		SetImage(this->MainCard_GradeImg, 0);
		this->MainCard_GradeText->SetText(FText::FromString(
			FString::FromInt(this->CardDatas[0].CardData.M_CardPrice)
		));
		this->MainCard_But->SetVisibility(ESlateVisibility::Visible);
		this->MainCard_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->MainCard_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
		this->MainCard_But->SetVisibility(ESlateVisibility::Collapsed);
		this->MainCard_GradeImg->SetVisibility(ESlateVisibility::Hidden);
		this->MainCard_GradeText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (this->CardDatas[1].PlayerBagIndex != -1)
	{
		UWidgetBase::SetButtonStyle(
			this->SecondCard1_But,
			this->CardDatas[1].CardData.ItemTexturePath.ToString()
		);
		SetImage(this->SecondCard1_GradeImg, 1);
		this->SecondCard1_GradeText->SetText(FText::FromString(
			FString::FromInt(this->CardDatas[1].CardData.M_CardPrice)
		));
		this->SecondCard1_But->SetVisibility(ESlateVisibility::Visible);
		this->SecondCard1_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard1_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
		this->SecondCard1_But->SetVisibility(ESlateVisibility::Collapsed);
		this->SecondCard1_GradeImg->SetVisibility(ESlateVisibility::Hidden);
		this->SecondCard1_GradeText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (this->CardDatas[2].PlayerBagIndex != -1)
	{
		UWidgetBase::SetButtonStyle(
			this->SecondCard2_But,
			this->CardDatas[2].CardData.ItemTexturePath.ToString()
		);
		SetImage(this->SecondCard2_GradeImg, 2);
		this->SecondCard2_GradeText->SetText(FText::FromString(
			FString::FromInt(this->CardDatas[2].CardData.M_CardPrice)
		));
		this->SecondCard2_But->SetVisibility(ESlateVisibility::Visible);
		this->SecondCard2_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard2_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
		this->SecondCard2_But->SetVisibility(ESlateVisibility::Collapsed);
		this->SecondCard2_GradeImg->SetVisibility(ESlateVisibility::Hidden);
		this->SecondCard2_GradeText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (this->CardDatas[3].PlayerBagIndex != -1)
	{
		UWidgetBase::SetButtonStyle(
			this->SecondCard3_But,
			this->CardDatas[3].CardData.ItemTexturePath.ToString()
		);
		SetImage(this->SecondCard3_GradeImg, 3);
		this->SecondCard3_GradeText->SetText(FText::FromString(
			FString::FromInt(this->CardDatas[3].CardData.M_CardPrice)
		));
		this->SecondCard3_But->SetVisibility(ESlateVisibility::Visible);
		this->SecondCard3_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard3_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
		this->SecondCard3_But->SetVisibility(ESlateVisibility::Collapsed);
		this->SecondCard3_GradeImg->SetVisibility(ESlateVisibility::Hidden);
		this->SecondCard3_GradeText->SetVisibility(ESlateVisibility::Hidden);
	}

}

void USynModel_CardUpgrade::CheckCardUpgrade()
{
	this->M_UpGradeCard_Butt->SetIsEnabled(true);
	if (this->CardDatas[0].PlayerBagIndex == -1)
	{
		this->M_UpGradeCard_Butt->SetIsEnabled(false);
	}
	else {
		int32 Count = 0;
		for (int32 i = 1; i < 4; i++)
		{
			if (this->CardDatas[i].PlayerBagIndex == -1)
			{
				Count++;
			}
		}

		if (Count == 3)
		{
			this->M_UpGradeCard_Butt->SetIsEnabled(false);
		}
	}
}

void USynModel_CardUpgrade::CardUpgrade()
{
	//判断主卡等级
	if (this->CardDatas[0].CardData.M_CardGrade == 16)
	{
		//提示玩家主卡等级已经到最高级
		UWidgetBase::CreateTipWidget(TEXT("目前最高等级的卡片不能参与强化"), COLOR_RED);

		return;
	}


	FString Card0, Card1, Card2, Card3, Stone;
	if (this->CardDatas[0].PlayerBagIndex != -1)
	{
		Card0 = this->CardDatas[0].CardData.BagID;
	}
	else {

		UWidgetBase::CreateTipWidget(TEXT("没有主卡！"), COLOR_RED);
		return;
	}

	if (this->CardDatas[1].PlayerBagIndex != -1)
	{
		Card1 = this->CardDatas[1].CardData.BagID;
	}
	else {
		Card1 = TEXT("-1");
	}

	if (this->CardDatas[2].PlayerBagIndex != -1)
	{
		Card2 = this->CardDatas[2].CardData.BagID;
	}
	else {
		Card2 = TEXT("-1");
	}

	if (this->CardDatas[3].PlayerBagIndex != -1)
	{
		Card3 = this->CardDatas[3].CardData.BagID;
	}
	else {
		Card3 = TEXT("-1");
	}

	if (this->SelectCloverIndex != -1)
	{
		Stone = this->CloverData.BagID;
	}
	else {
		Stone = TEXT("-1");
	}

	int32 Count = 0;
	for (int32 i = 1; i < 4; i++)
	{
		if (this->CardDatas[i].PlayerBagIndex != -1)
		{
			Count++;
		}
	}

	if (Count == 0)
	{
		UWidgetBase::CreateTipWidget(TEXT("必须拥有副卡！"), COLOR_RED);
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("主卡ID %s"), *FString(Card0));
	UE_LOG(LogTemp, Error, TEXT("副卡[A]ID %s"), *FString(Card1));
	UE_LOG(LogTemp, Error, TEXT("副卡[B]ID %s"), *FString(Card2));
	UE_LOG(LogTemp, Error, TEXT("副卡[C]ID %s"), *FString(Card3));
	UE_LOG(LogTemp, Error, TEXT("矿石ID %s"), *FString(Stone));

	//强化卡片
	this->PlayerSynthesis->OnCardUpgradeRequest(
		this,
		Card0,
		Card1,
		Card2,
		Card3,
		Stone
	);


	//更新货币显示
//this->PlayerSynthesis->Update_CoinText();

	////如果选择了四叶草
//if (this->SelectCloverIndex != -1)
//{
//	//消耗四叶草
//	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->UseMaterial(
//		this->SelectCloverIndex, this->SelectCloverName, 1, true);

//	//并且取消对四叶草的选择
//	this->CancelSelectClover();
//}

	//保存存档
	//UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("卡片强化操作")));

	/*
	//选择保险金
	if (this->PlayerSynthesis->GetSelectInsurance())
	{
		//保险金点券金额(主卡等级决定)
		if (
			UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2
			>=
			this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade]
			)
		{
			if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(
					this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade], 2
				);
				UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
			}
			else {
				UWidgetBase::CreateTipWidget(TEXT("金币不足"));
				return;
			}
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("点券不足"));
			return;
		}
	}
	else {
		if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("金币不足"));
			return;
		}
	}*/

	/*if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 >= 200)
	{
		UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);
	}
	else {
		UWidgetBase::CreateTipWidget(TEXT("金币不足"));
		return;
	}*/

	//强化结果
	//bool UpResult = UGameSystemFunction::GetRange(this->GetUpGradeRate(), 100);

	////标记卡片
	//auto MarkCard = [&](TArray<FSynModelCardUpgradeData>& CurDatas, const int32& Index)->void {
	//	if (this->CardDatas[Index].PlayerBagIndex != -1)
	//	{
	//		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//			CurDatas[Index].PlayerBagIndex
	//		].bWaitRemove = true;

	//		CurDatas[Index].PlayerBagIndex = -1;
	//	}
	//	};


	//卡片强化，提升 或者 下降
	//for (int32 i = 1; i < 4; i++)
	//{
	//	if (UpResult)
	//	{
	//		//强化成功
	//		//标记防御卡
	//		MarkCard(this->CardDatas, i);
	//	}
	//	else {
	//		/*
	//		//强化失败
	//		//没有选择保险就会被标记
	//		if (!this->PlayerSynthesis->GetSelectInsurance())
	//		{
	//			//标记防御卡
	//			MarkCard(this->CardDatas, i);
	//		}

	//		*/

	//		//标记防御卡
	//		MarkCard(this->CardDatas, i);
	//	}
	//}

	//更新保险金 金额[根据主卡计算]
//	this->PlayerSynthesis->SetInsuranceText(
//		FString::FromInt(this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade]));



	//if (UpResult)
	//{
	//	//主卡等级提升
	//	UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//		this->CardDatas[0].PlayerBagIndex
	//	].M_CardGrade += 1;
	//	//覆盖数据
	//	this->CardDatas[0].CardData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//		this->CardDatas[0].PlayerBagIndex
	//	];


	//	//添加强化成功历史记录
	//	UPlayerRecord::AddCardUpGradeCount(this->CardDatas[0].CardData.M_CardGrade);

	//	//添加每日当前卡片强化历史记录
	//	UPlayerRecord::AddDayCardUpCount(
	//		this->CardDatas[0].CardData.M_CardGrade,
	//		this->CardDatas[0].CardData.ItemName.ToString()
	//	);

	//	this->CurUpCardName = this->CardDatas[0].CardData.ItemName.ToString();

	//	//执行任务
	//	UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

	//	UWidgetBase::CreateTipWidget(TEXT("强化成功"), COLOR_Def2);
	//}
	//else {
	//	/*
	//	//没有选择保险
	//	if (!this->PlayerSynthesis->GetSelectInsurance())
	//	{
	//		int32 TempLevel = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//			this->CardDatas[0].PlayerBagIndex
	//		].M_CardGrade;

	//		if (TempLevel > 5)
	//		{
	//			//主卡等级下降
	//			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//				this->CardDatas[0].PlayerBagIndex
	//			].M_CardGrade -= 1;
	//		}

	//		//覆盖数据
	//		this->CardDatas[0].CardData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//			this->CardDatas[0].PlayerBagIndex
	//		];
	//	}
	//	*/

	//	int32 TempLevel = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//		this->CardDatas[0].PlayerBagIndex
	//	].M_CardGrade;

	//	if (TempLevel > 5)
	//	{
	//		//主卡等级下降
	//		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//			this->CardDatas[0].PlayerBagIndex
	//		].M_CardGrade -= 1;
	//	}

	//	//覆盖数据
	//	this->CardDatas[0].CardData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
	//		this->CardDatas[0].PlayerBagIndex
	//	];
	//	UWidgetBase::CreateTipWidget(TEXT("不够好运,强化失败"), COLOR_Def2);
	//}

	//重新查询主卡【卡片数量发生变化，索引刷新】
	//if (UpResult || !UpResult) //&& !this->PlayerSynthesis->GetSelectInsurance())
	//{
	//	//清空待删除的道具
	//	UGameSystemFunction::ClearWaitingItems<FItemCard>(
	//		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card
	//	);

		////查询主卡索引
		//TMap<int32, FItemCard> CurCards =
		//	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->FindCardByName(
		//		this->CardDatas[0].CardData.ItemName.ToString()
		//	);



		//bool bSearchResult = false;
		//for (const auto& Card : CurCards)
		//{
		//	if (this->CardDatas[0].CardData.M_CardGrade == Card.Value.M_CardGrade)
		//	{
		//		this->CardDatas[0].PlayerBagIndex = Card.Key;
		//		this->CardDatas[0].CardData = Card.Value;
		//		this->CardDatas[0].CardGrid = nullptr;
		//		bSearchResult = true;
		//		break;
		//	}
		//}

		////查询失败！
		//if (!bSearchResult)
		//{
		//	//取消主卡的选择
		//	this->CardDatas[0].PlayerBagIndex = -1;
		//	//保险金重置
		//	//this->PlayerSynthesis->SetInsuranceText(TEXT("0"));
		//}
	//}
}

void USynModel_CardUpgrade::ReLoadCardUpgrade(bool Result)
{
	int32 SerachLevel = this->CardDatas[0].CardData.M_CardGrade;
	if (Result)
	{
		SerachLevel++;
	}
	else {
		if (SerachLevel > 5)
		{
			SerachLevel--;
		}
	}

	//强化后卡片的结果
	int32 Index = UFVMGameInstance::GetPlayerStructManager_Static()->GetCardByGrade(SerachLevel);

	//更新主卡索引
	this->CardDatas[0].PlayerBagIndex = Index;
	this->CardDatas[0].CardData.M_CardGrade = SerachLevel;
	/*
		UFVMGameInstance::GetPlayerStructManager_Static()->GetCardByBagID(this->CardDatas[0].CardData.BagID);
	//提供UI索引进行二次查找
	FItemCard& SecondData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[this->CardDatas[0].PlayerUIID];
	//进行ID匹配
	if (SecondData.BagID.Equals(SecondData.BagID))
	{
		//赋值UI的索引
		this->CardDatas[0].PlayerBagIndex = this->CardDatas[0].PlayerUIID;
	}
	*/
	//清理副卡
	this->CardDatas[1].PlayerBagIndex = -1;
	this->CardDatas[2].PlayerBagIndex = -1;
	this->CardDatas[3].PlayerBagIndex = -1;

	//重新选择四叶草
	this->SetSelectClover(this->CloverData, this->SelectCloverRate);

	//更新概率
	this->GetUpGradeRate();

	//刷新卡片列表外观
	this->UpdateCardListStyle();

	//检查强化
	this->CheckCardUpgrade();

	//重新加载数据
	this->WidgetResetLoadData();
}

void USynModel_CardUpgrade::InitWidgets(UButton*& CardButt, const FString& WidgetName1, UImage*& CardGradeImg, const FString& WidgetName2, UTextBlock*& CardPriceText, const FString& WidgetName3)
{
	CardButt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, WidgetName1);
	CardGradeImg = this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, WidgetName2);
	CardPriceText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, WidgetName3);
}

UWidget* USynModel_CardUpgrade::WidgetCreate_InitCards(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this->PlayerSynthesis,
		UGameSystemFunction::GetUserInterClassByName(UI_LABSUINAME, TEXT("Card")));

	this->SetCardData(Grid, _Data, _Index);

	return Grid;
}

void USynModel_CardUpgrade::WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetCardData(Cast<UUI_PlayerBagCardGrid>(_UWidget), _Data, _Index);
}

void USynModel_CardUpgrade::SetCardData(UUI_PlayerBagCardGrid* _Grid, UItemDataTable* _Data, int32 _Index)
{
	//设置数据
	_Grid->SetFItemCardData(_Data->GetTransValue<FItemCard>());
	//设置纹理
	_Grid->M_CardTexturePath = _Grid->GetFItemCardData()->ItemTexturePath.ToString();
	//设置指向
	_Grid->SetUI_PlayerSynthesis(this->PlayerSynthesis);
	//启动按钮
	_Grid->GetButton()->SetIsEnabled(true);

	//设置索引
	_Grid->SetUIIndex(_Index);
	_Grid->SetCardIndex(_Index);

	//更新文本
	_Grid->UpdateButtonTexture(FString::FromInt(_Grid->GetFItemCardData()->M_CardPrice));

	//清理绑定
	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	this->OnCardLoad.ExecuteIfBound(_Grid, _Data, _Index);

	//绑定音效
	FScriptDelegate AddFunc_;
	AddFunc_.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc_);
}

void USynModel_CardUpgrade::LoadCards()
{
	////背包容量
	//int32 BagCount = UFVMGameInstance::GetFVMGameInstance()->
	//	GetPlayerStructManager()->
	//	M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	const TArray<FItemCard>& Datas = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card;
	/*UGameSystemFunction::GetPlayerCardDatas(
		Datas,
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card.Num()
	);*/

	this->LoadCards(Datas, Datas.Num());
}

void USynModel_CardUpgrade::LoadCards(const TArray<FItemCard>& CardsData, const int32& LoadCount)
{
	if (!IsValid(this->ItemLoadManager_Card))
	{
		//卡片加载器
		this->ItemLoadManager_Card = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Card_Upgrade"));
		this->ItemLoadManager_Card->InitWidget_First(this->Uniform_CardGrid, this->ScrollBox_Card_Uniform);
		this->ItemLoadManager_Card->InitRange_Second(4, 5, 800, 120.f, 150.f);
		this->ItemLoadManager_Card->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitCards"));
		this->ItemLoadManager_Card->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateCards"));
	}

	this->ItemLoadManager_Card->UpdateDatatable(CardsData);
	this->ItemLoadManager_Card->SetLoadItemMaxCount(LoadCount);
	this->ItemLoadManager_Card->ContinueRun();
}

void USynModel_CardUpgrade::UpdateUpRateText(const FString& Content)
{
	FText CurText = FText::FromString(Content);
	this->UpgradeRateText->SetText(CurText);
}

bool USynModel_CardUpgrade::AddCard(const int32 Index, FSynModelCardUpgradeData CopyData)
{
	//背包索引不等于-1
	if (this->CardDatas[Index].PlayerBagIndex != -1)
	{
		//获取背包道具总数量
		int32 BagCount = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card.Num();
		//判断背包索引是否有效
		if (this->CardDatas[Index].PlayerBagIndex < BagCount)
		{
			//将查找的数据，与新传入的数据进行匹配
			FItemCard& CurData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
				this->CardDatas[Index].PlayerBagIndex
			];

			//如果可以匹配上，表示已经存在
			if (this->CardDatas[Index].CardData.BagID.Equals(CurData.BagID))
			{
				return false;
			}
		}
	}

	//查询当前卡片是在背包是否存在【新查询的Index】
	int32 BagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->GetCardByBagID(CopyData.CardData.BagID);
	//二次校验
	//提供UI索引进行二次查找
	FItemCard& SecondData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[CopyData.PlayerUIID];
	//进行ID匹配
	if (SecondData.BagID.Equals(CopyData.CardData.BagID))
	{
		//赋值UI的索引
		BagIndex = CopyData.PlayerUIID;
	}
	this->CardDatas[Index].CardData = CopyData.CardData;
	this->CardDatas[Index].CardGrid = CopyData.CardGrid;
	this->CardDatas[Index].PlayerBagIndex = BagIndex;

	//如果网格有效
	if (IsValid(this->CardDatas[Index].CardGrid))
	{
		//禁用当前按钮
		this->CardDatas[Index].CardGrid->GetButton()->SetIsEnabled(false);
	}
	else {
		this->CardDatas[Index].PlayerBagIndex = -1;

		return false;
	}

	//更新外观
	auto SetImage = [&](UImage* _img)->void {
		_img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UWidgetBase::SetImageBrush(_img,
			UGameSystemFunction::GetCardGradeImagePath(this->CardDatas[Index].CardData.M_CardGrade)
		);
		};

	/*
//更新保险金 金额
if (this->CardDatas[0].PlayerBagIndex != -1)
{
	this->PlayerSynthesis->SetInsuranceText(
		FString::FromInt(this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade]));
}
*/
//更新概率
	this->GetUpGradeRate();

	return true;
}

UWidget* USynModel_CardUpgrade::WidgetCreate_InitMaterial(UItemDataTable* _Data, int32 _Index)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this->PlayerSynthesis,
		UGameSystemFunction::GetUserInterClassByName(UI_LABSUINAME, TEXT("Material"))
	);

	this->SetMaterialsData(Grid, _Data, _Index, this->M_BindFunctionName_Materials);

	return Grid;
}

void USynModel_CardUpgrade::WidgetRefresh_UpdateMaterial(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	this->SetMaterialsData(Cast<UUI_PlayerBagMaterialGrid>(_UWidget), _Data, _Index, this->M_BindFunctionName_Materials);
}

void USynModel_CardUpgrade::SetMaterialsData(UUI_PlayerBagMaterialGrid* _Grid, UItemDataTable* _CardData, int32 _Index, const TArray<FMaterialsSerachTypeBind>& _BindFuncName)
{
	//设置材料索引
	_Grid->SetIndex(_Index);
	//设置材料按钮表示可以选择
	_Grid->GetButton()->SetIsEnabled(true);
	//设置材料数据
	_Grid->SetMaterialData(_CardData->GetTransValue<FMaterialBase>());
	//设置材料的数量
	int32 CurCount = _Grid->GetMaterialData()->M_Count;
	//设置显示上限
	_Grid->UpdateMaterialsShowCount("x" + FString::FromInt(CurCount > 9999 ? 9999 : CurCount));
	//设置材料的外观
	UWidgetBase::SetButtonStyle(_Grid->GetButton(), _Grid->GetMaterialData()->ItemTexturePath.ToString());

	if (_Grid->GetButton()->OnClicked.IsBound())
	{
		_Grid->GetButton()->OnClicked.Clear();
	}

	//绑定函数
	for (auto& LBind : _BindFuncName)
	{
		if (((FMaterialBase*)(_CardData->GetValue()))->M_MaterialType == LBind.M_Type)
		{
			//设置指向合成屋的UI
			_Grid->SetUI_PlayerSynthesis(this->PlayerSynthesis);
			for (const auto& FnName : LBind.M_BindFnName)
			{
				//如果是空的则不绑定
				if (FnName.IsEqual(""))
				{
					continue;
				}

				//绑定
				FScriptDelegate AddFunc;
				AddFunc.BindUFunction(_Grid, FnName);
				_Grid->GetButton()->OnClicked.Add(AddFunc);
			}

			break;
		}
	}

	//绑定音效
	FScriptDelegate AddFunc;
	AddFunc.BindUFunction(_Grid, "PlayOperateAudioDef");
	_Grid->GetButton()->OnClicked.Add(AddFunc);
}

void USynModel_CardUpgrade::LoadCloversToMakeCard(const TArray<FMaterialsSerachTypeBind>& _BindFuncName, const FMaterialsSerachKeyWordsIgnore& IgnoreKeyWords)
{
	if (!IsValid(this->ItemLoadManager_Clover))
	{
		this->ItemLoadManager_Clover = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Clover_Upgrade"));
		this->ItemLoadManager_Clover->InitWidget_First(this->UniformGridPanel_Clover, this->ScrollBox_Clover);
		this->ItemLoadManager_Clover->InitRange_Second(1, 4, 800, 142.f, 132.f, true);
		this->ItemLoadManager_Clover->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitMaterial"));
		this->ItemLoadManager_Clover->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateMaterial"));
	}

	this->M_BindFunctionName_Materials.Empty();

	//赋值绑定数据
	this->M_BindFunctionName_Materials.Append(_BindFuncName);

	//材料数据(获取指定的材料数据)		
	TArray<FMaterialBase*> TempArrays;
	//材料数据(获取指定的材料数据)		
	TArray<FMaterialBase*> TargetArrays;

	//获取类型
	TArray<EMaterialType> Types;
	//按照顺序添加类型
	for (const auto& _Type : _BindFuncName)
	{
		Types.Emplace(_Type.M_Type);
	}

	//添加材料
	UGameSystemFunction::GetMaterialsArrayByType(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material,
		Types,
		TempArrays
	);

	//排除数据
	if (IgnoreKeyWords.GetKeyWords().Num())
	{
		for (FMaterialBase* CurData : TempArrays)
		{
			for (const FString& CurIgnoreKeyWords : IgnoreKeyWords.GetKeyWords())
			{
				if (CurData->ItemName.ToString().Contains(CurIgnoreKeyWords))
				{
					continue;
				}
				else
				{
					TargetArrays.Emplace(CurData);
				}
			}
		}

		this->ItemLoadManager_Clover->UpdateDatatable(TargetArrays);
		this->ItemLoadManager_Clover->SetLoadItemMaxCount(TargetArrays.Num());
	}
	else {
		this->ItemLoadManager_Clover->UpdateDatatable(TempArrays);
		this->ItemLoadManager_Clover->SetLoadItemMaxCount(TempArrays.Num());
	}

	this->ItemLoadManager_Clover->SetCurrentPage(0);
	this->ItemLoadManager_Clover->SetResetScrollBoxOffset();
	this->ItemLoadManager_Clover->ContinueRun();
}
