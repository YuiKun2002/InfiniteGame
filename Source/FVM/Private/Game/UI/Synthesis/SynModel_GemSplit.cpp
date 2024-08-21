// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_GemSplit.h"
//武器宝石
#include "Game/UI/Synthesis/SynModel_WepaonGems.h"
//宝石强化
#include "Game/UI/Synthesis/SynModel_GemsUpGrade.h"

#include "GameSystem/TaskSubsystem.h"

#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerSynthesis.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>

void USynModel_GemSplit::InitializeBySynthesis(UUI_PlayerSynthesis* _Parent_UI_Class)
{
	this->M_SynUi = _Parent_UI_Class;

	if (IsValid(this->M_SynUi))
	{
		this->M_Head = this->M_SynUi->GetWidgetComponent<UImage>(this->M_SynUi, TEXT("Image_52"));
		this->M_Grade = this->M_SynUi->GetWidgetComponent<UImage>(this->M_SynUi, TEXT("Image_53"));
		this->M_GemHead = this->M_SynUi->GetWidgetComponent<UImage>(this->M_SynUi, TEXT("Image_135"));
		this->M_GemCountText = this->M_SynUi->GetWidgetComponent<UTextBlock>(this->M_SynUi, TEXT("TextBlock_3"));


		this->M_GemSplit = this->M_SynUi->GetWidgetComponent<UButton>(this->M_SynUi, TEXT("SplitGem_Butt"));
		this->M_GemSplit->OnClicked.AddDynamic(this, &USynModel_GemSplit::GemSplit);
	}
}

void USynModel_GemSplit::SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index)
{
	if (!IsValid(this->M_GS_CurrentSelectGem))
		this->M_GS_CurrentSelectGem = NewObject<UItemDataTable>(this, TEXT("Syn_GS_CurrentSelectGem_Data"));

	if (IsValid(_Value))
		this->M_GS_CurrentSelectGem->SetValue(_Value->GetValue());
	else
		this->M_GS_CurrentSelectGem->SetValue(nullptr);

	this->M_GS_CurrentSelectGemIndex = _Index;
}

void USynModel_GemSplit::WidgetReset()
{
	this->SetCurrentSelectGem(nullptr, -1);

	this->M_Head->SetVisibility(ESlateVisibility::Collapsed);
	this->M_Grade->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemCountText->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemSplit->SetIsEnabled(false);
}

void USynModel_GemSplit::WidgetResetLoadData()
{
	this->LoadEquipment();
	this->LoadMaterials();
}

void USynModel_GemSplit::LoadEquipment()
{
	this->M_SynUi->GetWepaonGemsFunction()->InitEquipmentLoadder();

	this->M_SynUi->GetWepaonGemsFunction()->SetEquipmentLoadRebind(this, TEXT("SetEquipmentData"));

	/*
		 //如果你镶嵌了宝石，当前宝石的ID会被注销，并且被删除，那么可能后面的道具ID为-1  此时可能需要注册ID并且可能有单独的存储库
		 //你需要去验证ID
	*/

	TArray<FEquipmentBase*> Arrays;
	UEquipmentBaseStruct::GetAllEquipmentsByPlayer(Arrays, true, { EEquipment::E_WeaponGem });

	this->M_SynUi->GetWepaonGemsFunction()->LoadEquipmentRefresh(Arrays);
}

void USynModel_GemSplit::LoadMaterials()
{
	//材料数据(获取指定的材料数据)		
	this->M_SynUi->LoadMaterials({ FMaterialsSerachTypeBind(EMaterialType::E_Crystal,{}) });
}

void USynModel_GemSplit::AddGem()
{
	if (FEquipmentBase* const Gem = (FEquipmentBase*)this->M_GS_CurrentSelectGem->GetValue())
	{
		this->M_Head->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_Grade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_GemHead->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_GemCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_GemSplit->SetIsEnabled(true);

		this->M_SynUi->SetImageBrush(this->M_Head, Gem->ItemTexturePath.ToString());
		this->M_SynUi->SetImageBrush(this->M_Grade, UGameSystemFunction::GetItemGradePath(Gem->M_EquipmentGrade));

		//分解的宝石数量
		const int32& CrystalNum = this->M_SynUi->GetGemsUpGradeFunction()->GetGemsNeedCrystalNums()[Gem->M_EquipmentGrade];

		//设置文字显示
		this->M_GemCountText->SetText(FText::FromString(TEXT("x") + FString::FromInt(CrystalNum)));

		//当宝石等级小于等级10
		if (Gem->M_EquipmentGrade <= 10)
		{
			//设置头像
			this->M_SynUi->SetImageBrush(this->M_GemHead, TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/PlayerWeapon/Gem/0x22700020.0x22700020'"));
		}
		else {
			this->M_SynUi->SetImageBrush(this->M_GemHead, TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/Player_Mod/PlayerWeapon/Gem/0x22700110.0x22700110'"));
		}
	}
}

void USynModel_GemSplit::GemSplit()
{
	if (FEquipmentBase* const Gem = (FEquipmentBase*)this->M_GS_CurrentSelectGem->GetValue())
	{
		Gem->bWaitRemove = true;

		//分解的宝石数量
		const int32& CrystalNum = this->M_SynUi->GetGemsUpGradeFunction()->GetGemsNeedCrystalNums()[Gem->M_EquipmentGrade];

		//当宝石等级小于等级10
		if (Gem->M_EquipmentGrade <= 10)
		{
			UGameSystemFunction::SendMaterialToPlayerBag(TEXT("强化水晶"), CrystalNum, false);
		}
		else
		{
			UGameSystemFunction::SendMaterialToPlayerBag(TEXT("高级强化水晶"), CrystalNum, false);
		}

		UGameSystemFunction::ClearWaitingItemsForEquip(
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment
		);

		//添加历史记录
		UPlayerRecord::AddDayDayNewGemAttachName(Gem->ItemName.ToString());
		UPlayerRecord::AddGemSplitCount();

		//执行任务
		UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

		this->WidgetReset();
		this->WidgetResetLoadData();

		this->M_SynUi->CreateTipWidget(TEXT("分解成功"));

		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("宝石分解操作")));
	}
}

void USynModel_GemSplit::SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	//界面
	UUI_PlayerBagMaterialGrid* const LocalGrid = Cast<UUI_PlayerBagMaterialGrid>(_UWidget);
	//数据
	FEquipmentBase const LocalData = _Data->GetTransValue<FEquipmentBase>();
	//按钮
	UButton* const LocalButton = LocalGrid->GetButton();

	//设计基本属性
	LocalGrid->SetUI_PlayerSynthesis(this->M_SynUi);
	LocalGrid->SetEquipmentBase(LocalData);
	LocalGrid->SetIndex(_Index);
	LocalGrid->SetItemGrade(TEXT(""));

	LocalButton->SetIsEnabled(true);
	this->M_SynUi->SetButtonStyle(LocalButton, LocalData.ItemTexturePath.ToString());

	switch (LocalData.M_EquipmentType)
	{
	case EEquipment::E_WeaponGem:
	{
		LocalGrid->UpdateMaterialsShowCount(TEXT("宝石"));
		LocalGrid->SetItemGrade(UGameSystemFunction::GetItemGradePath(LocalData.M_EquipmentGrade));

		if (this->M_GS_CurrentSelectGemIndex != -1)
			if (_Index == this->M_GS_CurrentSelectGemIndex)
			{
				this->M_GS_CurrentSelectGem->SetValue(_Data->GetValue());
				LocalButton->SetIsEnabled(false);
			}
	}break;
	}


	//移除绑定
	LocalGrid->GetButton()->OnClicked.Clear();

	switch (LocalData.M_EquipmentType)
	{
	case EEquipment::E_WeaponGem:
	{
		FScriptDelegate LFunc;
		LFunc.BindUFunction(LocalGrid, TEXT("GemSlip_AddGemToSlot"));
		LocalGrid->GetButton()->OnClicked.Add(LFunc);
	}break;
	}

	FScriptDelegate LFuncDef;
	LFuncDef.BindUFunction(LocalGrid, TEXT("PlayOperateAudioDef"));
	LocalGrid->GetButton()->OnClicked.Add(LFuncDef);
}
