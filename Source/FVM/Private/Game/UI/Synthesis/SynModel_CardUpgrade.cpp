// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_CardUpgrade.h"
#include "GameSystem/TaskSubsystem.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"


#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void USynModel_CardUpgrade::InitializeBySynthesis(UUI_PlayerSynthesis* Class)
{
	this->PlayerSynthesis = Class;

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
		this->SecondCard1_GradeImg, TEXT("Image_15"),
		this->SecondCard1_GradeText, TEXT("TextBlock_108")
	);
	this->SecondCard1_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond1);

	//初始化副卡2
	this->InitWidgets(
		this->SecondCard2_But, TEXT("Button_1"),
		this->SecondCard2_GradeImg, TEXT("Image_109"),
		this->SecondCard2_GradeText, TEXT("TextBlock")
	);
	this->SecondCard2_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond2);

	//初始化副卡3
	this->InitWidgets(
		this->SecondCard3_But, TEXT("Button_2"),
		this->SecondCard3_GradeImg, TEXT("Image_16"),
		this->SecondCard3_GradeText, TEXT("TextBlock_1")
	);
	this->SecondCard3_But->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CancelSelectCardSecond3);

	this->M_UpGradeCard_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, TEXT("Upgrade"));
	this->M_UpGradeCard_Butt->OnClicked.AddDynamic(this, &USynModel_CardUpgrade::CardUpgrade);


	this->UpgradePriceText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, TEXT("Coin_Make_1"));
	this->UpgradeRateText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, TEXT("Rate"));
}

void USynModel_CardUpgrade::WidgetReset()
{
	//绑定加载器函数
	this->PlayerSynthesis->OnCardLoad.Unbind();
	this->PlayerSynthesis->OnCardLoad.BindUObject(this, &USynModel_CardUpgrade::OnLoadCardFunc);

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
	this->PlayerSynthesis->LoadMaterials(
		{
		{ FMaterialsSerachTypeBind(EMaterialType::E_Clover,{"AddUpGradeCardCloverSlot"}) }
		});
}

void USynModel_CardUpgrade::SetSelectClover(const FString& CloverName, int32 ItemIndex, float CloverRate)
{
	this->SelectCloverName = CloverName;
	this->SelectCloverIndex = ItemIndex;
	this->SelectCloverRate = CloverRate;

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(CloverName + " " +
				FString::FromInt(ItemIndex) + " " +
				FString::SanitizeFloat(CloverRate)
			));
	}
}

void USynModel_CardUpgrade::SetSelectCloverUI(class UUI_PlayerBagMaterialGrid* UIClover)
{
	this->UICloverGrid = UIClover;

	//取消四叶草按钮的所有绑定
	this->PlayerSynthesis->GetCloverButton()->OnClicked.Clear();
	this->PlayerSynthesis->GetCloverButton()->OnClicked.AddDynamic(
		this, &USynModel_CardUpgrade::CancelSelectClover);
	this->PlayerSynthesis->GetCloverButton()->OnClicked.AddDynamic(
		this->PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
}

void USynModel_CardUpgrade::CancelSelectClover()
{
	//取消四叶草按钮的所有绑定
	this->PlayerSynthesis->GetCloverButton()->OnClicked.Clear();
	this->PlayerSynthesis->GetCloverButton()->OnClicked.AddDynamic(
		this->PlayerSynthesis, &UWidgetBase::PlayOperateAudioDef);
	//取消四叶草的选择
	this->SelectCloverIndex = -1;
	//还原四叶草的显示UI
	UWidgetBase::SetButtonStyle(
		this->PlayerSynthesis->GetCloverButton(),
		"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_25.T_PS_25'"
	);
	//更新概率
	this->GetUpGradeRate();
	//加载四叶草
	this->PlayerSynthesis->LoadMaterials(
		{
		{ FMaterialsSerachTypeBind(EMaterialType::E_Clover,{"AddUpGradeCardCloverSlot"}) }
		});
}

int32 USynModel_CardUpgrade::GetSelectClover(FString& OutCloverName)
{
	OutCloverName = this->SelectCloverName;
	return this->SelectCloverIndex;
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
		if (CurData.PlayerBagIndex == Index)
		{
			CurData.CardGrid = Grid;
			CurData.CardData = *((FItemCard*)(Data->GetValue()));
			CurData.CardGrid->GetButton()->SetIsEnabled(false);
		}
	}
}

void USynModel_CardUpgrade::CancelSelectCardMain()
{
	this->CancelSelectCard(0, this->MainCard_But, this->MainCard_GradeImg, this->MainCard_GradeText);
	this->PlayerSynthesis->SetInsuranceText(TEXT("0"));
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
		this->MainCard_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->MainCard_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
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
		this->SecondCard1_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard1_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
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
		this->SecondCard2_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard2_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
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
		this->SecondCard3_GradeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UWidgetBase::SetButtonStyle(this->SecondCard3_But,
			"Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");
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
	}

	//强化结果
	bool UpResult = UGameSystemFunction::GetRange(this->GetUpGradeRate(), 100);

	//标记卡片
	auto MarkCard = [&](TArray<FSynModelCardUpgradeData>& CurDatas, const int32& Index)->void {
		if (this->CardDatas[Index].PlayerBagIndex != -1)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
				CurDatas[Index].PlayerBagIndex
			].bWaitRemove = true;

			CurDatas[Index].PlayerBagIndex = -1;
		}
	};


	//卡片强化，提升 或者 下降
	for (int32 i = 1; i < 4; i++)
	{
		if (UpResult)
		{
			//强化成功
			//标记防御卡
			MarkCard(this->CardDatas, i);
		}
		else {
			//强化失败
			//没有选择保险就会被标记
			if (!this->PlayerSynthesis->GetSelectInsurance())
			{
				//标记防御卡
				MarkCard(this->CardDatas, i);
			}
		}
	}

	//更新保险金 金额[根据主卡计算]
	this->PlayerSynthesis->SetInsuranceText(
		FString::FromInt(this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade]));

	//更新概率
	this->GetUpGradeRate();

	if (UpResult)
	{
		//主卡等级提升
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
			this->CardDatas[0].PlayerBagIndex
		].M_CardGrade += 1;
		//覆盖数据
		this->CardDatas[0].CardData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
			this->CardDatas[0].PlayerBagIndex
		];


		//添加强化成功历史记录
		UPlayerRecord::AddCardUpGradeCount(this->CardDatas[0].CardData.M_CardGrade);

		//添加每日当前卡片强化历史记录
		UPlayerRecord::AddDayCardUpCount(
			this->CardDatas[0].CardData.M_CardGrade,
			this->CardDatas[0].CardData.ItemName
		);

		this->CurUpCardName = this->CardDatas[0].CardData.ItemName;

		//执行任务
		UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

		UWidgetBase::CreateTipWidget(TEXT("强化成功"), COLOR_Def2);
	}
	else {
		//没有选择保险
		if (!this->PlayerSynthesis->GetSelectInsurance())
		{
			int32 TempLevel = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
				this->CardDatas[0].PlayerBagIndex
			].M_CardGrade;

			if (TempLevel > 5)
			{
				//主卡等级下降
				UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
					this->CardDatas[0].PlayerBagIndex
				].M_CardGrade -= 1;
			}

			//覆盖数据
			this->CardDatas[0].CardData = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[
				this->CardDatas[0].PlayerBagIndex
			];
		}
		UWidgetBase::CreateTipWidget(TEXT("不够好运,强化失败"), COLOR_Def2);
	}

	//重新查询主卡【卡片数量发生变化，索引刷新】
	if (UpResult || !UpResult && !this->PlayerSynthesis->GetSelectInsurance())
	{
		//清空待删除的道具
		UGameSystemFunction::ClearWaitingItems<FItemCard>(
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card
			);

		//查询主卡索引
		TMap<int32, FItemCard> CurCards = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->FindCardByName(
			this->CardDatas[0].CardData.ItemName
		);

		bool bSearchResult = false;
		for (const auto& Card : CurCards)
		{
			if (this->CardDatas[0].CardData.M_CardGrade == Card.Value.M_CardGrade)
			{
				this->CardDatas[0].PlayerBagIndex = Card.Key;
				this->CardDatas[0].CardData = Card.Value;
				this->CardDatas[0].CardGrid = nullptr;
				bSearchResult = true;
				break;
			}
		}

		//查询失败！
		if (!bSearchResult)
		{
			//取消主卡的选择
			this->CardDatas[0].PlayerBagIndex = -1;
			//保险金重置
			this->PlayerSynthesis->SetInsuranceText(TEXT("0"));
		}
	}

	//如果选择了四叶草
	if (this->SelectCloverIndex != -1)
	{
		//消耗四叶草
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->UseMaterial(
			this->SelectCloverIndex, this->SelectCloverName, 1, true);

		//并且取消对四叶草的选择
		this->CancelSelectClover();
	}

	//刷新卡片列表外观
	this->UpdateCardListStyle();

	//检查强化
	this->CheckCardUpgrade();

	//更新货币显示
	this->PlayerSynthesis->Update_CoinText();

	//重新加载数据
	this->WidgetResetLoadData();

	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData();
}

void USynModel_CardUpgrade::InitWidgets(UButton*& CardButt, const FString& WidgetName1, UImage*& CardGradeImg, const FString& WidgetName2, UTextBlock*& CardPriceText, const FString& WidgetName3)
{
	CardButt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, WidgetName1);
	CardGradeImg = this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, WidgetName2);
	CardPriceText = this->PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->PlayerSynthesis, WidgetName3);
}

void USynModel_CardUpgrade::LoadCards()
{
	//背包容量
	int32 BagCount = UFVMGameInstance::GetFVMGameInstance()->
		GetPlayerStructManager()->
		M_PlayerBagGirdGroup.GetBagCount(1);

	//获取卡片数据
	TArray<FItemCard*> Datas;
	UGameSystemFunction::GetPlayerCardDatas(Datas, BagCount);

	this->PlayerSynthesis->LoadCards(Datas, BagCount);
}

void USynModel_CardUpgrade::UpdateUpRateText(const FString& Content)
{
	FText CurText = FText::FromString(Content);
	this->UpgradeRateText->SetText(CurText);
}

bool USynModel_CardUpgrade::AddCard(const int32 Index, FSynModelCardUpgradeData CopyData)
{
	if (this->CardDatas[Index].PlayerBagIndex != -1)
	{
		return false;
	}

	this->CardDatas[Index].CardData = CopyData.CardData;
	this->CardDatas[Index].CardGrid = CopyData.CardGrid;
	this->CardDatas[Index].PlayerBagIndex = CopyData.PlayerBagIndex;

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

	//更新保险金 金额
	if (this->CardDatas[0].PlayerBagIndex != -1)
	{
		this->PlayerSynthesis->SetInsuranceText(
			FString::FromInt(this->LInsuranceCoinNum[this->CardDatas[0].CardData.M_CardGrade]));
	}

	//更新概率
	this->GetUpGradeRate();

	return true;
}
