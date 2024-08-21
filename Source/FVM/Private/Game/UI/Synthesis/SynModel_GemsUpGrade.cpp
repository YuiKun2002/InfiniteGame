// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_GemsUpGrade.h"

#include "Game/UI/Synthesis/SynModel_WepaonGems.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include "GameSystem/TaskSubsystem.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>


void USynModel_GemsUpGrade::InitializeBySynthesis(UUI_PlayerSynthesis* _Parent_UI_Class)
{
	this->M_SynUi = _Parent_UI_Class;

	if (IsValid(this->M_SynUi))
	{
		UE_LOG(LogTemp, Warning, TEXT("来自：USynModel_GemsUpGrade.cpp InitializeBySynthesis 有四个UI组件不用初始化，为空"));
		//初始化
		USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_Parent_UI_Class), this->Slot_0, { TEXT("Image_38"),TEXT("Image_39"),TEXT("Image_40"),TEXT(""),TEXT("Button_13") });
		USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_Parent_UI_Class), this->Slot_1, { TEXT("Image_48"),TEXT("Image_49"),TEXT("Image_50"),TEXT(""),TEXT("Button_18") });
		USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_Parent_UI_Class), this->Slot_2, { TEXT("Image_42"),TEXT("Image_43"),TEXT("Image_44"),TEXT(""),TEXT("Button_16") });
		USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_Parent_UI_Class), this->Slot_3, { TEXT("Image_45"),TEXT("Image_46"),TEXT("Image_47"),TEXT(""),TEXT("Button_17") });

		USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_0, { TEXT(""),TEXT("Gem_0") });
		USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_1, { TEXT(""),TEXT("Gem_1") });
		USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_2, { TEXT(""),TEXT("Gem_2") });
		USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_3, { TEXT(""),TEXT("Gem_3") });

		this->M_GemUp_WepaonHead = this->M_SynUi->GetWidgetComponent<UImage>(this->M_SynUi, TEXT("Image_56"));

		this->M_GemUp_GemHead = this->M_SynUi->GetWidgetComponent<UImage>(this->M_SynUi, TEXT("Image_57"));
		this->M_GemUp_GemCountText = this->M_SynUi->GetWidgetComponent<UTextBlock>(this->M_SynUi, TEXT("TextBlock_63"));

		this->M_GemUp_Butt = this->M_SynUi->GetWidgetComponent<UButton>(this->M_SynUi, TEXT("GemUp_Butt"));
		this->M_GemUp_Butt->OnClicked.AddDynamic(this, &USynModel_GemsUpGrade::GemUpGrade);

		this->M_GemUp_RateText = this->M_SynUi->GetWidgetComponent<UTextBlock>(this->M_SynUi, TEXT("GemUp_Rate"));

		this->M_GemUp_SelectWepaonText = this->M_SynUi->GetWidgetComponent<UTextBlock>(this->M_SynUi, TEXT("GU_WepaonName"));
		this->M_GemUp_SelectGemText = this->M_SynUi->GetWidgetComponent<UTextBlock>(this->M_SynUi, TEXT("GU_GemName"));

		//初始化普通水晶数据
		if (!IsValid(this->M_GU_NormalCrystal))
		{
			this->M_GU_NormalCrystal = NewObject<UItemDataTable>(this, TEXT("Syn_GU_NormalCrystal_Data"));
			this->M_GU_NormalCrystal->SetValue(nullptr);
		}

		//初始化高级水晶数据
		if (!IsValid(this->M_GU_AdvancedCrystal))
		{
			this->M_GU_AdvancedCrystal = NewObject<UItemDataTable>(this, TEXT("Syn_GU_AdvancedCrystal_Data"));
			this->M_GU_NormalCrystal->SetValue(nullptr);
		}

		//初始化四叶草数据
		if (!IsValid(this->M_GU_CurrentSelectClover))
		{
			this->M_GU_CurrentSelectClover = NewObject<UItemDataTable>(this, TEXT("Syn_GU_CurrentSelectClover_Data"));
			this->M_GU_CurrentSelectClover->SetValue(nullptr);
		}

	}
}

void USynModel_GemsUpGrade::WidgetReset()
{
	this->InitializeSlot(this->Slot_0);
	this->InitializeSlot(this->Slot_1);
	this->InitializeSlot(this->Slot_2);
	this->InitializeSlot(this->Slot_3);
	this->M_SynUi->SetButtonStyle(this->Slot_3.M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/344.344'"));

	this->SetCurrentSelectWeapon(nullptr, -1);
	this->SetCurrentSelectGem(nullptr, -1);

	this->M_GemUp_WepaonHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUp_GemHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUp_GemCountText->SetVisibility(ESlateVisibility::Collapsed);

	this->M_GemUp_Butt->SetIsEnabled(false);

	this->M_GemUp_SelectWepaonText->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUp_SelectGemText->SetVisibility(ESlateVisibility::Collapsed);

	//设置强化成功概率
	this->M_GemUp_RateText->SetText(FText::FromString((TEXT("0%"))));
	//设置保险金金额
	this->M_SynUi->SetInsuranceText(TEXT("0"));

	//重新绑定取消四叶草的选择
	this->M_SynUi->GetCloverButton()->OnClicked.Clear();
	this->M_SynUi->GetCloverButton()->OnClicked.AddDynamic(this->M_SynUi, &UWidgetBase::PlayOperateAudioDef);
	this->M_SynUi->GetCloverButton()->OnClicked.AddDynamic(this, &USynModel_GemsUpGrade::CancelClover);
}

void USynModel_GemsUpGrade::WidgetResetLoadData()
{
	this->LoadEquipment();
	this->CancelClover();
}

void USynModel_GemsUpGrade::LoadEquipment()
{
	this->M_SynUi->GetWepaonGemsFunction()->InitEquipmentLoadder();

	this->M_SynUi->GetWepaonGemsFunction()->SetEquipmentLoadRebind(this, TEXT("SetEquipmentData"));

	/*
		 //如果你镶嵌了宝石，当前宝石的ID会被注销，并且被删除，那么可能后面的道具ID为-1  此时可能需要注册ID并且可能有单独的存储库
		 //你需要去验证ID
	*/

	TArray<FEquipmentBase*> Arrays;
	UEquipmentBaseStruct::GetAllEquipmentsByPlayer(Arrays, true, { EEquipment::E_PlayerWeaponFirst,EEquipment::E_PlayerWeaponSecond,EEquipment::E_PlayerWeaponSuper ,EEquipment::E_WeaponGem });

	this->M_SynUi->GetWepaonGemsFunction()->LoadEquipmentRefresh(Arrays);
}

void USynModel_GemsUpGrade::LoadMaterials()
{
	//材料数据(获取指定的材料数据)		
	this->M_SynUi->LoadMaterials({ FMaterialsSerachTypeBind(EMaterialType::E_Crystal,{}) , FMaterialsSerachTypeBind(EMaterialType::E_Clover,{TEXT("GemUp_AddColoverRate")}) });
}

void USynModel_GemsUpGrade::SetCurrentSelectWeapon(UItemDataTable* _Value, const int32& _Index)
{
	if (!IsValid(this->M_GU_CurrentSelectWeapon))
		this->M_GU_CurrentSelectWeapon = NewObject<UItemDataTable>(this, TEXT("N_GU_CurrentSelectWeapon_Data_"));

	if (IsValid(_Value))
		this->M_GU_CurrentSelectWeapon->SetValue(_Value->GetValue());
	else
		this->M_GU_CurrentSelectWeapon->SetValue(nullptr);

	this->M_GU_CurrentSelectWeaponIndex = _Index;
}

void USynModel_GemsUpGrade::SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index)
{
	if (!IsValid(this->M_GU_CurrentSelectGem))
		this->M_GU_CurrentSelectGem = NewObject<UItemDataTable>(this, TEXT("N_GU_CurrentSelectGem_Data_"));

	if (IsValid(_Value))
		this->M_GU_CurrentSelectGem->SetValue(_Value->GetValue());
	else
		this->M_GU_CurrentSelectGem->SetValue(nullptr);

	this->M_GU_CurrentSelectGemIndex = _Index;
}

void USynModel_GemsUpGrade::SetCurrentSelectClover(UItemDataTable* _Value)
{
	this->M_GU_CurrentSelectClover->SetValue(_Value->GetValue());
}

void USynModel_GemsUpGrade::SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	//界面
	UUI_PlayerBagMaterialGrid* const LocalGrid = Cast<UUI_PlayerBagMaterialGrid>(_UWidget);
	//数据
	FEquipmentBase const LocalData = (_Data->GetTransValue<FEquipmentBase>());
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
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper:
	{
		switch (LocalData.M_EquipmentType)
		{
		case EEquipment::E_PlayerWeaponFirst:LocalGrid->UpdateMaterialsShowCount(TEXT("武器")); break;
		case EEquipment::E_PlayerWeaponSecond:LocalGrid->UpdateMaterialsShowCount(TEXT("武器")); break;
		case EEquipment::E_PlayerWeaponSuper:LocalGrid->UpdateMaterialsShowCount(TEXT("武器")); break;
		}

		if (this->M_GU_CurrentSelectWeaponIndex != -1)
			if (_Index == this->M_GU_CurrentSelectWeaponIndex)
			{
				this->M_GU_CurrentSelectWeapon->SetValue(_Data->GetValue());
				LocalButton->SetIsEnabled(false);
			}
	}break;
	case EEquipment::E_WeaponGem:
	{
		LocalGrid->UpdateMaterialsShowCount(TEXT("宝石"));
		LocalGrid->SetItemGrade(UGameSystemFunction::GetItemGradePath(LocalData.M_EquipmentGrade));

		if (this->M_GU_CurrentSelectGemIndex != -1)
			if (_Index == this->M_GU_CurrentSelectGemIndex)
			{
				this->M_GU_CurrentSelectGem->SetValue(_Data->GetValue());
				LocalButton->SetIsEnabled(false);
			}
	}break;
	}


	//移除绑定
	LocalGrid->GetButton()->OnClicked.Clear();

	switch (LocalData.M_EquipmentType)
	{
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper:
	{
		FScriptDelegate LFunc;
		LFunc.BindUFunction(LocalGrid, TEXT("GemUp_AddWepaonToSlot"));
		LocalGrid->GetButton()->OnClicked.Add(LFunc);
	}break;
	case EEquipment::E_WeaponGem:
	{
		FScriptDelegate LFunc;
		LFunc.BindUFunction(LocalGrid, TEXT("GemUp_AddGemToSlot"));
		LocalGrid->GetButton()->OnClicked.Add(LFunc);
	}break;
	}

	FScriptDelegate LFuncDef;
	LFuncDef.BindUFunction(LocalGrid, TEXT("PlayOperateAudioDef"));
	LocalGrid->GetButton()->OnClicked.Add(LFuncDef);
}

void USynModel_GemsUpGrade::SetCrystalUIFunc(const int32& _GemGrade, const UItemDataTable* const _Crystal)
{
	//获得水晶数据
	const FMaterialBase* const Crystal = (const FMaterialBase* const)(const_cast<UItemDataTable* const>(_Crystal)->GetValue());

	//设置UI[水晶]
	this->M_SynUi->SetImageBrush(this->M_GemUp_GemHead, Crystal->ItemTexturePath.ToString());
	this->M_GemUp_GemHead->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//设置UI[文字]
	this->M_GemUp_GemCountText->SetText(FText::FromString(FString::FromInt(Crystal->M_Count) + TEXT(" / ") + FString::FromInt(this->GetGemsNeedCrystalNums()[_GemGrade])));
	this->M_GemUp_GemCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//如果材料充足，开启强化
	if (Crystal->M_Count >= this->GetGemsNeedCrystalNums()[_GemGrade])
	{
		this->M_GemUp_GemCountText->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		M_GemUp_Butt->SetIsEnabled(true);
	}
	else {
		this->M_GemUp_GemCountText->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USynModel_GemsUpGrade::SetUpGradeGemData(const int32& _Index)
{
	//设置宝石数据
	if (_Index == -1)
		this->M_GU_CurrentGem = this->M_GU_CurrentSelectGem;
	else
		this->M_GU_CurrentGem = this->M_GU_WepaonGemsData[_Index];
}

const TArray<int32>& USynModel_GemsUpGrade::GetGemsValue() const
{
	return this->GemsValue;
}

const TArray<int32>& USynModel_GemsUpGrade::GetGemsNeedCrystalNums() const
{
	return this->NeedCrystalNums;
}

const TArray<int32>& USynModel_GemsUpGrade::GetGemsNeedInsurance() const
{
	return this->NeedInsuranceNums;
}

UItemDataTable* const USynModel_GemsUpGrade::GetCrystalData(const int32& _Grade)
{
	if (_Grade >= 10)
		return this->M_GU_AdvancedCrystal;

	return this->M_GU_NormalCrystal;
}

int32 USynModel_GemsUpGrade::GetCurrentGemGrade()
{
	return this->CurUpGemGrade;
}

void USynModel_GemsUpGrade::AddWeapon()
{
	this->M_GemUp_Butt->SetIsEnabled(false);
	this->M_GemUp_GemHead->SetVisibility(ESlateVisibility::Collapsed);
	this->M_GemUp_GemCountText->SetVisibility(ESlateVisibility::Collapsed);

	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
	{
		this->M_GemUp_SelectWepaonText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->M_GemUp_SelectGemText->SetVisibility(ESlateVisibility::Collapsed);
	}

	//初始化宝石数据
	if (this->M_GU_WepaonGemsData.Num() == 0)
		for (int32 i = 0; i < 4; i++)
			this->M_GU_WepaonGemsData.Emplace(NewObject<UItemDataTable>(this, *FString(TEXT("GU_WepaonGemsData_") + FString::FromInt(i))));

	//设置头像
	this->M_GemUp_WepaonHead->SetVisibility(ESlateVisibility::HitTestInvisible);
	this->M_SynUi->SetImageBrush(this->M_GemUp_WepaonHead, ((FEquipmentBase*)(this->M_GU_CurrentSelectWeapon->GetValue()))->ItemTexturePath.ToString());

	//从玩家背包获取当前武器数据
	int32 LID = -1;
	if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_GU_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
	{
		const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

		//武器数据
		FPlayerWeaponBase* const CurrentData = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID];

		if (UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
		{
			//设置武器名称文字显示
			this->M_GemUp_SelectWepaonText->SetText(CurrentData->ItemName);
		}

		//清除数据
		for (const auto& Data : this->M_GU_WepaonGemsData)
			Data->SetValue(nullptr);

		//设置宝石数据
		for (int32 i = 0; i < CurrentData->M_WeaponGem.Num(); i++)
			if (CurrentData->M_WeaponGem[i].M_CurrentWeaponGemEnable && !CurrentData->M_WeaponGem[i].M_WepaonGemName.Equals(TEXT("")))
				this->M_GU_WepaonGemsData[i]->SetValue(((FTableRowBase*)(&CurrentData->M_WeaponGem[i])));
			else
				this->M_GU_WepaonGemsData[i]->SetValue(nullptr);

		//设置宝石头像和等级
		for (int32 i = 0; i < this->M_GU_WepaonGemsData.Num(); i++)
			if (this->M_GU_WepaonGemsData[i]->GetValue())
			{
				const FPlayerWeaponGemSlot* const Slot = (FPlayerWeaponGemSlot*)(this->M_GU_WepaonGemsData[i]->GetValue());

				//设置头像
				Locals[i]->M_ItemHead->SetRenderOpacity(1.f);
				this->M_SynUi->SetImageBrush(Locals[i]->M_ItemHead, Slot->M_WepaonGemHeadPath);

				//设置等级
				if (Slot->M_WeaponGemGrade > 0)
				{
					Locals[i]->M_ItemGrade->SetRenderOpacity(1.f);
					this->M_SynUi->SetImageBrush(Locals[i]->M_ItemGrade, UGameSystemFunction::GetItemGradePath(Slot->M_WeaponGemGrade));
				}
				else {
					Locals[i]->M_ItemGrade->SetRenderOpacity(0.f);
				}

				//设置背景点击启用
				Locals[i]->M_ItemClickButton->SetIsEnabled(true);

				//设置背景样式
				if (i == 3)
					this->M_SynUi->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/344.344'"));
				else
					this->M_SynUi->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_1.T_PS_1'"));

				//显示边框
				Locals[i]->M_ItemBorderStyle->SetRenderOpacity(1.f);
				//设置边框收缩
				Locals[i]->M_ItemBorderStyle->SetRenderScale(FVector2D(0.8f));
				//设置边框背景的颜色
				Locals[i]->M_ItemBorderStyle->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f));
			}
			else {
				//隐藏头像
				Locals[i]->M_ItemHead->SetRenderOpacity(0.f);
				//隐藏等级
				Locals[i]->M_ItemGrade->SetRenderOpacity(0.f);
				//隐藏边框
				Locals[i]->M_ItemBorderStyle->SetRenderOpacity(0.f);
				//设置背景点击禁用
				Locals[i]->M_ItemClickButton->SetIsEnabled(false);
				//设置背景样式
				this->M_SynUi->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/347.347'"));
			}
	}
}

void USynModel_GemsUpGrade::AddGem()
{
	if (UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
	{
		this->M_GemUp_SelectWepaonText->SetVisibility(ESlateVisibility::Collapsed);
		this->M_GemUp_SelectGemText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (FEquipmentBase* const Gem = (FEquipmentBase*)this->M_GU_CurrentSelectGem->GetValue())
	{
		if (!Gem)
		{
			if (UFVMGameInstance::GetFVMGameInstance())
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前宝石数据无效"));

			return;
		}

		const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

		for (const auto& Data : Locals)
			Data->M_ItemClickButton->SetIsEnabled(false);

		this->Slot_1.M_ItemClickButton->SetIsEnabled(true);


		if (Gem->M_EquipmentGrade > 0)
		{
			this->Slot_1.M_ItemGrade->SetRenderOpacity(1.f);
			this->M_SynUi->SetImageBrush(this->Slot_1.M_ItemGrade, UGameSystemFunction::GetItemGradePath(Gem->M_EquipmentGrade));
		}
		else {
			this->Slot_1.M_ItemGrade->SetRenderOpacity(0.f);
		}

		this->Slot_1.M_ItemHead->SetRenderOpacity(1.f);
		this->M_SynUi->SetImageBrush(this->Slot_1.M_ItemHead, Gem->ItemTexturePath.ToString());

		//设置宝石名称文字显示
		if (UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
			this->M_GemUp_SelectGemText->SetText(Gem->ItemName);

		this->SetUpGradeGemData(-1);

		//选择第二个槽位
		this->SelectGem(1);

		//搜索材料[]
		this->SearchCrystalData();

		//添加水晶
		this->AddCrystal(Gem->ItemName.ToString(), Gem->M_EquipmentGrade);
	}
}

void USynModel_GemsUpGrade::AddCrystal(const FString& _GemName, const int32& _GemGrade)
{
	//查询宝石相关数据
	FEquipmentBase* LBaseData = nullptr;
	if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(_GemName, LBaseData, true, EEquipment::E_WeaponGem))
	{
		//转换数据得到宝石数据
		FWeaponGem* const GemData = (FWeaponGem*)LBaseData;

		//赋值当前宝石最大等级
		this->M_GU_CurrentGemGradeMax = GemData->M_GemGradeMax;
		//赋值当前宝石等级
		this->M_GU_CurrentGemGrade = _GemGrade;

		//允许强化->装备添加水晶
		if (_GemGrade < GemData->M_GemGradeMax)
		{
			this->AddUpGradeCrystal(_GemGrade);
		}
		else {
			this->M_GemUp_GemHead->SetVisibility(ESlateVisibility::Collapsed);
			this->M_GemUp_GemCountText->SetVisibility(ESlateVisibility::Collapsed);

			this->M_GemUp_Butt->SetIsEnabled(false);
		}

	}
}

void USynModel_GemsUpGrade::AddUpGradeCrystal(const int32& _GemGrade)
{
	M_GemUp_Butt->SetIsEnabled(false);

	if (_GemGrade < 10 && this->M_GU_NormalCrystal->GetValue())
	{
		//设置UI
		this->SetCrystalUIFunc(_GemGrade, this->M_GU_NormalCrystal);
		//指向数据
		this->M_GU_CurrentCrystal = this->M_GU_NormalCrystal;
	}
	else if (_GemGrade >= 10 && this->M_GU_AdvancedCrystal->GetValue()) {
		//使用的不是普通水晶
		this->M_GU_UseGemNormalCrytal = false;
		//设置UI
		this->SetCrystalUIFunc(_GemGrade, this->M_GU_AdvancedCrystal);
		//指向数据
		this->M_GU_CurrentCrystal = this->M_GU_AdvancedCrystal;
	}
	else {
		this->M_GemUp_GemHead->SetVisibility(ESlateVisibility::Collapsed);
		this->M_GemUp_GemCountText->SetVisibility(ESlateVisibility::Collapsed);
	}

	//更新概率
	this->UpdateRate();
}

void USynModel_GemsUpGrade::CancelClover()
{
	if (IsValid(this->M_GU_CurrentSelectClover))
		this->M_GU_CurrentSelectClover->SetValue(nullptr);

	this->M_SynUi->SetButtonStyle(this->M_SynUi->GetCloverButton(), TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_25.T_PS_25'"));

	this->UpdateRate();

	this->LoadMaterials();
}

void USynModel_GemsUpGrade::SelectGem(const int32& _Index)
{
	const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };
	for (int32 i = 0; i < 4; i++)
		Locals[i]->M_ItemBorderStyle->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f));
	Locals[_Index]->M_ItemBorderStyle->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void USynModel_GemsUpGrade::SelectAndCancel(const int32& _Index)
{
	//宝石被选择
	if (this->M_GU_CurrentSelectGem->GetValue())
	{
		this->WidgetReset();
		this->WidgetResetLoadData();
	}
	else if (this->M_GU_CurrentSelectWeapon->GetValue()) {
		//设置当前宝石选择索引
		this->M_GU_CurrentSelectGemSlotIndex = _Index;
		//武器被选择
		this->SearchCrystalData();
		//选择宝石数据
		this->SetUpGradeGemData(_Index);
		//添加水晶
		this->AddCrystal(((FPlayerWeaponGemSlot*)(this->M_GU_WepaonGemsData[_Index]->GetValue()))->M_WepaonGemName, ((FPlayerWeaponGemSlot*)(this->M_GU_WepaonGemsData[_Index]->GetValue()))->M_WeaponGemGrade);

		//在手机上显示
		if (UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
		{
			//设置宝石显示的名字
			this->M_GemUp_SelectGemText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			this->M_GemUp_SelectGemText->SetText(FText::FromString(((FPlayerWeaponGemSlot*)(this->M_GU_WepaonGemsData[_Index]->GetValue()))->M_WepaonGemName));
		}
	}
}

void USynModel_GemsUpGrade::SearchCrystalData()
{
	bool LResult1 = false;
	bool LResult2 = false;

	for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
	{
		if (Data.M_MaterialType == EMaterialType::E_Crystal && Data.ItemName.ToString().Equals(TEXT("强化水晶")))
		{
			this->M_GU_NormalCrystal->SetValue((FTableRowBase*)(&Data));
			LResult1 = true;
		}
		else if (Data.M_MaterialType == EMaterialType::E_Crystal && Data.ItemName.ToString().Equals(TEXT("高级强化水晶")))
		{
			this->M_GU_AdvancedCrystal->SetValue((FTableRowBase*)(&Data));
			LResult2 = true;
		}

		if (LResult1 && LResult2)
			break;
	}
}

const float USynModel_GemsUpGrade::UpdateRate() const
{
	//基础概率
	int32 BaseRate = 0;
	//四叶草概率
	int32 ColoverRateAdd = 0;

	//如果选择了武器或者宝石->计算基础概率
	if (this->M_GU_CurrentSelectWeapon->GetValue() || this->M_GU_CurrentSelectGem->GetValue())
	{
		BaseRate = this->GetGemsValue()[this->M_GU_CurrentGemGrade];
	}

	//如果选择了四叶草
	if (this->M_GU_CurrentSelectClover->GetValue())
	{
		FCloverMaterial LClover;
		if (UMaterialBaseStruct::GetMaterialSourceData(
			((FMaterialBase*)this->M_GU_CurrentSelectClover->GetValue())->ItemName.ToString(), LClover, EMaterialType::E_Clover)
			)
		{
			ColoverRateAdd = ((LClover.M_UpGrateRate - 1.f) * BaseRate);
		}
	}

	//设置强化成功概率
	this->M_GemUp_RateText->SetText(FText::FromString(FString::FromInt(BaseRate + ColoverRateAdd > 100 ? 100 : BaseRate + ColoverRateAdd) + TEXT("%")));
	//设置保险金金额
	this->M_SynUi->SetInsuranceText(FString::FromInt(this->GetGemsNeedInsurance()[this->M_GU_CurrentGemGrade]));

	return BaseRate + ColoverRateAdd > 100 ? 100 : BaseRate + ColoverRateAdd;
}

void USynModel_GemsUpGrade::InitializeSlot(FSynModel_GemSlot& _Slot)
{
	if (_Slot.M_ItemAttachmentButton)
	{
		_Slot.M_ItemAttachmentButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_Slot.M_ItemClickButton)
	{
		_Slot.M_ItemClickButton->SetIsEnabled(true);
		//_Slot.M_ItemClickButton->SetVisibility(ESlateVisibility::Visible);
		this->M_SynUi->SetButtonStyle(_Slot.M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_1.T_PS_1'"));
	}

	if (_Slot.M_ItemBorderStyle)
	{
		//设置边框收缩
		_Slot.M_ItemBorderStyle->SetRenderScale(FVector2D(1.f));
		_Slot.M_ItemBorderStyle->SetRenderOpacity(1.f);
		this->M_SynUi->SetImageBrush(_Slot.M_ItemBorderStyle, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/675_2.675_2'"));
	}

	if (_Slot.M_ItemGrade)
		_Slot.M_ItemGrade->SetRenderOpacity(0.f);

	if (_Slot.M_ItemHead)
		_Slot.M_ItemHead->SetRenderOpacity(0.f);
}

void USynModel_GemsUpGrade::Gem_0() {
	UE_LOG(LogTemp, Error, TEXT("A"));
	this->M_SynUi->PlayOperateAudio(false);

	this->SelectGem(0);
	this->SelectAndCancel(0);
}
void USynModel_GemsUpGrade::Gem_1() {
	UE_LOG(LogTemp, Error, TEXT("B"));
	this->M_SynUi->PlayOperateAudio(false);

	this->SelectGem(1);
	this->SelectAndCancel(1);
}
void USynModel_GemsUpGrade::Gem_2() {
	UE_LOG(LogTemp, Error, TEXT("C"));
	this->M_SynUi->PlayOperateAudio(false);

	this->SelectGem(2);
	this->SelectAndCancel(2);
}
void USynModel_GemsUpGrade::Gem_3() {
	UE_LOG(LogTemp, Error, TEXT("D"));
	this->M_SynUi->PlayOperateAudio(false);

	this->SelectGem(3);
	this->SelectAndCancel(3);
}

void USynModel_GemsUpGrade::GemUpGrade()
{
	this->M_SynUi->PlayOperateAudio(false);

	// M_GU_CurrentGemGradeMax   当前最大
	// M_GU_CurrentGemGrade  当前等级

	//判断是否勾选保险金,并且是否点券不足
	if (this->M_SynUi->GetSelectInsurance() && this->GetGemsNeedInsurance()[this->M_GU_CurrentGemGrade] > UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_2)
	{
		this->M_SynUi->CreateTipWidget(TEXT("点券不足"));

		return;
	}

	//判断金币是否充足
	if (UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerCoin.M_Coin_0 < 200)
	{
		this->M_SynUi->CreateTipWidget(TEXT("金币不足"));

		return;
	}

	//武器中镶嵌的宝石
	if (this->M_GU_CurrentSelectWeapon->GetValue() && this->M_GU_CurrentGem)
	{
		FPlayerWeaponGemSlot* const LSlot = (FPlayerWeaponGemSlot*)this->M_GU_CurrentGem->GetValue();

		if (UGameSystemFunction::GetRange(this->UpdateRate(), 100))
		{
			//强化成功
			this->M_SynUi->CreateTipWidget(TEXT("强化成功"));

			if (LSlot->M_WeaponGemGrade < this->M_GU_CurrentGemGradeMax)
			{
				LSlot->M_WeaponGemGrade++;
			}

			//添加历史记录
			UPlayerRecord::AddDayDayNewGemAttachName(LSlot->M_WepaonGemName);
			//添加宝石强化历史记录
			UPlayerRecord::AddGemUpGrade(LSlot->M_WepaonGemName, LSlot->M_WeaponGemGrade);

			//设置强化的等级
			this->CurUpGemGrade = LSlot->M_WeaponGemGrade;

			//调用任务
			UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);
		}
		else {
			this->M_SynUi->CreateTipWidget(TEXT("不够好运，强化失败"));

			//没有开保险
			if (!this->M_SynUi->GetSelectInsurance())
			{
				if (LSlot->M_WeaponGemGrade >= 6)
				{
					LSlot->M_WeaponGemGrade--;
				}
			}
		}
	}
	else if (this->M_GU_CurrentSelectGem->GetValue() && this->M_GU_CurrentGem)//单独的宝石
	{
		FEquipmentBase* const LSlot = (FEquipmentBase*)this->M_GU_CurrentGem->GetValue();

		if (UGameSystemFunction::GetRange(this->UpdateRate(), 100))
		{
			//强化成功
			this->M_SynUi->CreateTipWidget(TEXT("强化成功"));

			if (LSlot->M_EquipmentGrade < this->M_GU_CurrentGemGradeMax)
			{
				LSlot->M_EquipmentGrade++;
			}

			//添加历史记录
			UPlayerRecord::AddDayDayNewGemAttachName(LSlot->ItemName.ToString());
			//添加宝石强化历史记录
			UPlayerRecord::AddGemUpGrade(LSlot->ItemName.ToString(), LSlot->M_EquipmentGrade);

			//设置强化的等级
			this->CurUpGemGrade = LSlot->M_EquipmentGrade;

			//调用任务
			UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);
		}
		else {
			this->M_SynUi->CreateTipWidget(TEXT("不够好运，强化失败"));

			//没有开保险
			if (!this->M_SynUi->GetSelectInsurance())
			{
				if (LSlot->M_EquipmentGrade >= 6)
				{
					LSlot->M_EquipmentGrade--;
				}
			}
		}
	}
	else {

		this->M_SynUi->CreateTipWidget(TEXT("当前宝石数据出错![SynModel_GemsUpGrade.cpp (USynModel_GemsUpGrade::GemUpGrade)]"));

		return;
	}

	//扣除资源

	//保险金
	if (this->M_SynUi->GetSelectInsurance())
		UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(this->GetGemsNeedInsurance()[this->M_GU_CurrentGemGrade], 2u);

	//金币
	UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(200, 0);

	//更新金币文字
	this->M_SynUi->Update_CoinText();

	//是否删除材料
	bool LRemoveItemResult = false;

	//扣除四叶草
	if (this->M_GU_CurrentSelectClover->GetValue())
	{
		((FMaterialBase*)this->M_GU_CurrentSelectClover->GetValue())->M_Count--;
		if (((FMaterialBase*)this->M_GU_CurrentSelectClover->GetValue())->M_Count <= 0)
		{
			((FMaterialBase*)this->M_GU_CurrentSelectClover->GetValue())->bWaitRemove = true;
			LRemoveItemResult = true;
		}
	}

	/*------------------------------------------------------------------*/
	//水晶扣除
	FMaterialBase* LCrystal = nullptr;
	if (this->M_GU_CurrentGemGrade >= 10)
		//高级水晶
		LCrystal = (FMaterialBase*)this->M_GU_AdvancedCrystal->GetValue();
	else
		//普通水晶
		LCrystal = (FMaterialBase*)this->M_GU_NormalCrystal->GetValue();
	LCrystal->M_Count -= this->GetGemsNeedCrystalNums()[this->M_GU_CurrentGemGrade];
	if (LCrystal->M_Count <= 0)
	{
		LCrystal->bWaitRemove = true;
		LRemoveItemResult = true;
	}
	/*------------------------------------------------------------------*/

	//清除材料
	if (LRemoveItemResult)
	{
		UGameSystemFunction::ClearWaitingItems(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material);
	}

	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("宝石强化操作")));

	//重新加载数据
	this->LoadEquipment();

	//取消对四叶草的选择【自动加载材料区】
	this->CancelClover();

	//重新选择和刷新数据
	if (this->M_GU_CurrentSelectWeapon->GetValue())
	{
		this->AddWeapon();
		this->SelectGem(this->M_GU_CurrentSelectGemSlotIndex);
		this->SelectAndCancel(this->M_GU_CurrentSelectGemSlotIndex);
	}
	else if (this->M_GU_CurrentSelectGem->GetValue()) {
		this->AddGem();
	}
}
