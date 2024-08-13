// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_GoldCardUp.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>


void FGoldCardUpCardType::InitClass(UButton* ButtIns, USynModel_GoldCardUp* CurClass)
{
	if (IsValid(ButtIns))
	{
		this->CardButt = ButtIns;
	}

	this->Class = CurClass;
}

void FGoldCardUpCardType::ShowButton(const FString& HeadPath)
{
	CardButt->SetVisibility(ESlateVisibility::Visible);
	UWidgetBase::SetButtonStyle(CardButt
		, HeadPath);
}

bool USynModel_GoldCardUp::AddCardData(const FGoldCardUpCardType& NewData)
{
	int32 Count = 0;
	bool bOk = false;
	for (FGoldCardUpCardType& Data : this->CardsData)
	{
		if (Data.PlayerBagIndex != -1)
		{
			Count++;
			continue;
		}
		else {
			if (bOk)
			{
				Count++;
				continue;
			}

			Data.CardGrade = NewData.CardGrade;
			Data.CardHeadPath = NewData.CardHeadPath;
			Data.CardName = NewData.CardName;
			Data.Grid = NewData.Grid;
			Data.PlayerBagIndex = NewData.PlayerBagIndex;
			Data.UIGridIndex = NewData.UIGridIndex;
			Data.ShowButton(NewData.CardHeadPath);
			Count++;
			bOk = true;
		}
	}

	if (Count == 2)
	{
		//查询材料数据
		this->SearchMaterials();
	}

	return bOk;
}

void USynModel_GoldCardUp::UpdateListData()
{
	this->CardUp_Butt->SetIsEnabled(true);
	this->CardUpMaterial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->TargetCard_Img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool bEnableButt = true;
	for (FGoldCardUpCardType& Data : this->CardsData)
	{
		if (Data.PlayerBagIndex == -1)
		{
			bEnableButt = false;
			break;
		}
	}

	if (!bEnableButt)
	{
		this->CardUp_Butt->SetIsEnabled(false);
		this->CardUpMaterial->SetVisibility(ESlateVisibility::Collapsed);
		this->TargetCard_Img->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	//如果数量材料不足
	if (this->CardUpMaterialPlayerBagIndex == -1)
	{
		this->CardUp_Butt->SetIsEnabled(false);
	}

	if (this->bSearchOk)
	{
		UWidgetBase::SetImageBrush(
			this->CardUpMaterial, this->CardUpMaterialHeadPath,
			this->CardUpMaterialPlayerBagIndex == -1 ? (COLOR_DARK) : (COLOR_WHITE)
		);

		UWidgetBase::SetImageBrush(
			this->TargetCard_Img, this->TargetCardHeadPath,
			this->CardUpMaterialPlayerBagIndex == -1 ? (COLOR_DARK) : (COLOR_WHITE)
		);
	}
	else {
		this->CardUpMaterial->SetVisibility(ESlateVisibility::Collapsed);
		this->TargetCard_Img->SetVisibility(ESlateVisibility::Collapsed);
	}


}

void USynModel_GoldCardUp::SearchMaterials()
{
	this->CardUpMaterialPlayerBagIndex = -1;

	//卡片名称合并查询->进化凭证
	TArray<FMaterial_GoldCardUp_Data> Datas;
	UGameSystemFunction::GetDataTableRows(
		TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/GoldCardUpData.GoldCardUpData'"),
		Datas
	);

	//最后从背包搜寻的金卡转职凭证
	FString LResultItemName = TEXT("");
	//对应的卡片名称
	FString LResultCardName = TEXT("");

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前金卡进化查询进化凭证："));
	}

	for (auto Data = Datas.CreateConstIterator(); Data; ++Data)
	{
		bool TLResult = true;

		FString StateResult = TEXT("");

		//组合卡片名称
		for (auto& State : this->CardsData)
		{
			StateResult += State.CardName;
		}

		if (UFVMGameInstance::GetDebug())
		{
			FString Content;
			Content += TEXT("当前凭证名称:");
			Content += (*Data).M_GoldCardItemName;
			Content += TEXT(" 当前合成名字结果:");
			Content += StateResult;
			Content += TEXT(" 目标合成名称结果:");
			Content += (*Data).M_AddCardName;
			UGameSystemFunction::FVMLog(__FUNCTION__, Content);
		}

		//查询匹配最终结果
		if ((*Data).M_AddCardName.Equals(StateResult))
		{
			LResultItemName = (*Data).M_GoldCardItemName;
			LResultCardName = (*Data).M_GoldCardName;
			break;
		}
	}

	this->bSearchOk = true;
	//没有任何结果，返回
	if (LResultItemName.Equals(""))
	{
		this->bSearchOk = false;
		return;
	}

	//查询材料数据
	FCardChangeJobMaterial _Material;
	if (!UMaterialBaseStruct::GetMaterialSourceData<FCardChangeJobMaterial>(
		LResultItemName,
		_Material,
		EMaterialType::E_CardChangeJobMaterial
		))
	{
		//CurData.Visible = ESlateVisibility::Collapsed;

		if (UFVMGameInstance::GetDebug())
		{
			UWidgetBase::CreateTipWidget(FString(TEXT("[") + LResultItemName + TEXT("]查询失败!")));
		}

		return;
	}
	else
	{
		//赋值材料的数据
		this->CardUpMaterialHeadPath = _Material.ItemTexturePath.ToString();
		this->CardUpMaterialName = _Material.ItemName.ToString();
	}

	//材料在背包中的索引
	this->CardUpMaterialPlayerBagIndex = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(this->CardUpMaterialName);

	//新卡片数据
	FItemCard TargetData;
	UCardBaseStruct::SearchCardFromDataTable(LResultCardName, TargetData);
	this->TargetCardName = LResultCardName;
	this->TargetCardHeadPath = TargetData.ItemTexturePath.ToString();
}

UButton* USynModel_GoldCardUp::GetCardFristButt()
{
	return this->CardFrist_Butt;
}

UButton* USynModel_GoldCardUp::GetCardSecondButt()
{
	return this->CardSecond_Butt;
}

UButton* USynModel_GoldCardUp::GetCardUpButt()
{
	return this->CardUp_Butt;
}

UImage* USynModel_GoldCardUp::GetCardUpMaterialImg()
{
	return this->CardUpMaterial;
}

UImage* USynModel_GoldCardUp::GetTargetCardImg()
{
	return this->TargetCard_Img;
}

void USynModel_GoldCardUp::InitializeBySynthesis(class UUI_PlayerSynthesis* UI_PlayerSynthesis)
{
	this->PlayerSynthesis = UI_PlayerSynthesis;

	this->CardFrist_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, "Button_143");
	this->CardFrist_Butt->OnClicked.AddDynamic(this, &USynModel_GoldCardUp::CancelCard1);

	this->CardSecond_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, "Button_7");
	this->CardSecond_Butt->OnClicked.AddDynamic(this, &USynModel_GoldCardUp::CancelCard2);

	this->CardUp_Butt = this->PlayerSynthesis->GetWidgetComponent<UButton>(this->PlayerSynthesis, "GoldCardUp_Butt");
	this->CardUp_Butt->OnClicked.AddDynamic(this, &USynModel_GoldCardUp::CardUp);

	this->CardUpMaterial = this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, "GoldCardUpHead");
	this->TargetCard_Img = this->PlayerSynthesis->GetWidgetComponent<UImage>(this->PlayerSynthesis, "Image_164");

	//初始化卡片组件
	this->CardsData[0].InitClass(this->CardFrist_Butt, this);
	this->CardsData[1].InitClass(this->CardSecond_Butt, this);
}

void USynModel_GoldCardUp::WidgetReset()
{
	//绑定卡片加载
	this->PlayerSynthesis->OnCardLoad.Unbind();

	this->PlayerSynthesis->OnCardLoad.BindUObject(this, &USynModel_GoldCardUp::OnCardLoad);

	this->CancelCard1();
	this->CancelCard2();
}

void USynModel_GoldCardUp::WidgetResetLoadData()
{
	//获取卡片数组
	TArray<FItemCard>& CurPlayerCards = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card;

	TArray<FItemCard*> NewCards;
	for (FItemCard& CardData : CurPlayerCards)
	{
		if (!CardData.M_FCardChangeJobs.M_IsChange)
		{
			NewCards.Emplace(&CardData);
		}
	}

	//加载卡片数据
	this->PlayerSynthesis->LoadCards(NewCards, UFVMGameInstance::GetPlayerStructManager_Static()->GetBagMaxCount(1));
	//加载材料
	this->PlayerSynthesis->LoadMaterials(
		{
			FMaterialsSerachTypeBind(EMaterialType::E_CardChangeJobMaterial,{})
		}, { TEXT("转") });
}

void USynModel_GoldCardUp::OnCardLoad(class UUI_PlayerBagCardGrid* Grid, class UItemDataTable* Data, int32 Index)
{

	//绑定事件
	Grid->GetButton()->OnClicked.AddDynamic(Grid, &UUI_PlayerBagCardGrid::AddSynthesisGoldCardUp);

	for (FGoldCardUpCardType& CurData : this->CardsData)
	{
		if (CurData.PlayerBagIndex == -1)
		{
			continue;
		}
		else {
			if (CurData.UIGridIndex == Index)
			{
				Grid->GetButton()->SetIsEnabled(false);
				CurData.Grid = Grid;
				break;
			}
		}
	}
}

void USynModel_GoldCardUp::CancelCard1()
{
	this->CardFrist_Butt->SetVisibility(ESlateVisibility::Collapsed);
	this->CardsData[0].PlayerBagIndex = -1;
	this->CardsData[0].UIGridIndex = -1;
	if (IsValid(this->CardsData[0].Grid))
	{
		this->CardsData[0].Grid->GetButton()->SetIsEnabled(true);
	}

	this->UpdateListData();
}

void USynModel_GoldCardUp::CancelCard2()
{
	this->CardSecond_Butt->SetVisibility(ESlateVisibility::Collapsed);

	this->CardsData[1].PlayerBagIndex = -1;
	this->CardsData[1].UIGridIndex = -1;
	if (IsValid(this->CardsData[1].Grid))
	{
		this->CardsData[1].Grid->GetButton()->SetIsEnabled(true);
	}

	this->UpdateListData();
}

void USynModel_GoldCardUp::CardUp()
{
	if (!this->bSearchOk)
	{
		return;
	}


	FItemCard TargetData;
	UCardBaseStruct::SearchCardFromDataTable(this->TargetCardName, TargetData);
	//最大等级
	int32 GradeMax = this->CardsData[0].CardGrade > this->CardsData[1].CardGrade ?
		this->CardsData[0].CardGrade : this->CardsData[1].CardGrade;
	TargetData.M_CardGrade = GradeMax;

	//清理数据
	UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[this->CardsData[0].PlayerBagIndex].bWaitRemove = true;
	UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[this->CardsData[1].PlayerBagIndex].bWaitRemove = true;

	//发送背包
	UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card.EmplaceAt(
		this->CardsData[0].PlayerBagIndex
		, TargetData
	);

	//消耗素材
	UFVMGameInstance::GetPlayerStructManager_Static()->UseMaterial(
		this->CardUpMaterialPlayerBagIndex,
		this->CardUpMaterialName,
		1, false
	);

	//清理背包
	UFVMGameInstance::GetPlayerStructManager_Static()->Remove_Item(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card
	);
	UFVMGameInstance::GetPlayerStructManager_Static()->Remove_Item(
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material
	);

	//保存
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("金卡进化操作")));

	UWidgetBase::CreateTipWidget(
		FString(TEXT("进化成功【") + TargetData.ItemName.ToString() + TEXT("】"))
	);

	//重新加载数据
	this->WidgetReset();
	this->WidgetResetLoadData();
}

