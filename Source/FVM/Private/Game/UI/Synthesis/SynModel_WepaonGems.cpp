// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除
#include "Game/UI/Synthesis/SynModel_WepaonGems.h"

#include "GameSystem/TaskSubsystem.h"

#include "Game/UI/UI_PlayerSynthesis.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>

void USynModel_WepaonGems::InitializeBySynthesis(UUI_PlayerSynthesis* _UI)
{
	this->M_UUI_PlayerSynthesis = _UI;

	USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_UI), this->Slot_0, { TEXT("Image_21"),TEXT("Image_26"),TEXT("Image_25"),TEXT("Open"),TEXT("Button_8") });
	USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_UI), this->Slot_1, { TEXT("Image_28"),TEXT("Image_29"),TEXT("Image_30"),TEXT("Open_1"),TEXT("Button_9") });
	USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_UI), this->Slot_2, { TEXT("Image_31"),TEXT("Image_32"),TEXT("Image_33"),TEXT("Open_2"),TEXT("Button_10") });
	USynModel_Structures::InitComponentBindGemSlot(Cast<UWidgetBase>(_UI), this->Slot_3, { TEXT("Image_34"),TEXT("Image_35"),TEXT("Image_36"),TEXT("Open_3"),TEXT("Button_11") });

	USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_0, { TEXT("OpenSlotOrUnInstall_0"),TEXT("RemoveGemToSlot") });
	USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_1, { TEXT("OpenSlotOrUnInstall_1"),TEXT("RemoveGemToSlot") });
	USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_2, { TEXT("OpenSlotOrUnInstall_2"),TEXT("RemoveGemToSlot") });
	USynModel_Structures::InitComponentBindGemSlotButton(this, this->Slot_3, { TEXT("OpenSlotOrUnInstall_3"),TEXT("RemoveGemToSlot") });

	this->M_WepaonHead = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UImage>(this->M_UUI_PlayerSynthesis, TEXT("Image_204"));

	this->M_WeaponNameText = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->M_UUI_PlayerSynthesis, TEXT("WepaonName"));
	this->M_GemNameText = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UTextBlock>(this->M_UUI_PlayerSynthesis, TEXT("GemName"));

	this->M_GemAttachmentButt = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UButton>(this->M_UUI_PlayerSynthesis, TEXT("WepaonGemUse_Butt"));
	this->M_GemAttachmentButt->OnClicked.AddDynamic(this, &USynModel_WepaonGems::GemAttach);

	this->M_Equipment_Scroll = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UScrollBox>(this->M_UUI_PlayerSynthesis, TEXT("ScrollBox_2"));
	this->M_Equipment_Gird = this->M_UUI_PlayerSynthesis->GetWidgetComponent<UUniformGridPanel>(this->M_UUI_PlayerSynthesis, TEXT("Cards_Box_2"));
}

void USynModel_WepaonGems::InitEquipmentLoadder()
{
	//初始化装备加载器
	if (!IsValid(this->M_Equipment_Loadder))
	{
		this->M_Equipment_Loadder = NewObject<UItemLoadManager>(this, TEXT("USynModelWepaonGems_Equipment"));
		this->M_Equipment_Loadder->InitWidget_First(this->M_Equipment_Gird, this->M_Equipment_Scroll);
		this->M_Equipment_Loadder->InitRange_Second(4, 7, 800, 100.f, 100.f);
		this->M_Equipment_Loadder->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitEquipment"));
		this->M_Equipment_Loadder->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateEquipment"));
	}
}

FSynModelEquipmentLoad& USynModel_WepaonGems::GetEquipmentLoadBind()
{
	return this->OnEquipmentLoad;
}

UItemLoadManager*& USynModel_WepaonGems::GetEquipmentLoadder()
{
	return this->M_Equipment_Loadder;
}

void USynModel_WepaonGems::WidgetReset()
{
	this->InitializeSlot(this->Slot_0);
	this->InitializeSlot(this->Slot_1);
	this->InitializeSlot(this->Slot_2);
	this->InitializeSlot(this->Slot_3);

	if (IsValid(this->M_GemAttachmentButt))
		this->M_GemAttachmentButt->SetIsEnabled(false);

	this->SetCurrentSelectWeapon(nullptr, -1);
	this->SetCurrentSelectGem(nullptr, -1);

	this->M_WepaonHead->SetRenderOpacity(0.f);
}

void USynModel_WepaonGems::WidgetDataReload()
{
	this->LoadMaterials();
	this->LoadEquipment();
}

void USynModel_WepaonGems::LoadEquipment()
{
	this->InitEquipmentLoadder();

	this->SetEquipmentLoadRebind(this, TEXT("SetEquipmentData"));


	/*
		 //如果你镶嵌了宝石，当前宝石的ID会被注销，并且被删除，那么可能后面的道具ID为-1  此时可能需要注册ID并且可能有单独的存储库
		 //你需要去验证ID
	*/

	TArray<FEquipmentBase*> Arrays;
	UEquipmentBaseStruct::GetAllEquipmentsByPlayer(Arrays, true, { EEquipment::E_PlayerWeaponFirst,EEquipment::E_PlayerWeaponSecond,EEquipment::E_PlayerWeaponSuper ,EEquipment::E_WeaponGem });

	this->LoadEquipmentRefresh(Arrays);
}

void USynModel_WepaonGems::LoadEquipmentRefresh(TArray<FEquipmentBase*>& _datas)
{
	this->M_Equipment_Loadder->SetMainScrollTransfrom(FVector2D(0.f, -165.f));
	this->M_Equipment_Loadder->UpdateDatatable(_datas);
	this->M_Equipment_Loadder->SetLoadItemMaxCount(UFVMGameInstance::GetPlayerStructManager_Static()->GetBagMaxCount(0));
	this->M_Equipment_Loadder->ContinueRun();
}

UWidget* USynModel_WepaonGems::WidgetCreate_InitEquipment(UItemDataTable* _Data, int32 _Index)
{
	TSoftClassPtr<UUI_PlayerBagMaterialGrid> LocalUIClass = Cast <UClass>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftClassPath(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")))));
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this->M_UUI_PlayerSynthesis, LocalUIClass.Get());

	if (this->OnEquipmentLoad.IsBound())
		this->OnEquipmentLoad.Execute(_Data, _Index, Cast<UWidget>(Grid));

	return Grid;
}

void USynModel_WepaonGems::WidgetRefresh_UpdateEquipment(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	if (this->OnEquipmentLoad.IsBound())
		this->OnEquipmentLoad.Execute(_Data, _Index, _UWidget);
}

void USynModel_WepaonGems::LoadMaterials()
{
	//材料数据(获取指定的材料数据)		
	this->M_UUI_PlayerSynthesis->LoadMaterials({ FMaterialsSerachTypeBind(EMaterialType::E_Crystal,{}) , FMaterialsSerachTypeBind(EMaterialType::E_Bit,{}) });
}

void USynModel_WepaonGems::InitializeSlot(FSynModel_GemSlot& _Slot)
{
	if (_Slot.M_ItemAttachmentButton)
	{
		_Slot.M_ItemAttachmentButton->SetIsEnabled(false);
		_Slot.M_ItemAttachmentButton->SetVisibility(ESlateVisibility::Visible);
		this->M_UUI_PlayerSynthesis->SetButtonStyle(_Slot.M_ItemAttachmentButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/329.329'"));

	}

	if (_Slot.M_ItemClickButton)
	{
		_Slot.M_ItemClickButton->SetIsEnabled(false);
		_Slot.M_ItemClickButton->SetVisibility(ESlateVisibility::Visible);
		this->M_UUI_PlayerSynthesis->SetButtonStyle(_Slot.M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/347.347'"));
	}

	if (_Slot.M_ItemBorderStyle)
		_Slot.M_ItemBorderStyle->SetRenderOpacity(0.f);

	if (_Slot.M_ItemGrade)
		_Slot.M_ItemGrade->SetRenderOpacity(0.f);

	if (_Slot.M_ItemHead)
		_Slot.M_ItemHead->SetRenderOpacity(0.f);
}

void USynModel_WepaonGems::SetEquipmentData(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	//界面
	UUI_PlayerBagMaterialGrid* const LocalGrid = Cast<UUI_PlayerBagMaterialGrid>(_UWidget);
	//数据
	FEquipmentBase* const LocalData = ((FEquipmentBase*)(_Data->GetValue()));
	//按钮
	UButton* const LocalButton = LocalGrid->GetButton();

	//设计基本属性
	LocalGrid->SetUI_PlayerSynthesis(this->M_UUI_PlayerSynthesis);
	LocalGrid->SetEquipmentBase(LocalData);
	LocalGrid->SetIndex(_Index);
	LocalGrid->SetItemGrade(TEXT(""));


	LocalButton->SetIsEnabled(true);
	this->M_UUI_PlayerSynthesis->SetButtonStyle(LocalButton, LocalData->ItemTexturePath.ToString());

	switch (LocalData->M_EquipmentType)
	{
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper:
	{
		switch (LocalData->M_EquipmentType)
		{
		case EEquipment::E_PlayerWeaponFirst:LocalGrid->UpdateMaterialsShowCount(TEXT("主武器")); break;
		case EEquipment::E_PlayerWeaponSecond:LocalGrid->UpdateMaterialsShowCount(TEXT("副武器")); break;
		case EEquipment::E_PlayerWeaponSuper:LocalGrid->UpdateMaterialsShowCount(TEXT("超级武器")); break;
		}

		if (this->M_CurrentSelectWeaponIndex != -1)
			if (_Index == this->M_CurrentSelectWeaponIndex)
			{
				this->M_CurrentSelectWeapon->SetValue(_Data->GetValue());
				LocalButton->SetIsEnabled(false);
			}
	}break;
	case EEquipment::E_WeaponGem:
	{
		LocalGrid->UpdateMaterialsShowCount(TEXT("宝石"));
		LocalGrid->SetItemGrade(UGameSystemFunction::GetItemGradePath(LocalData->M_EquipmentGrade));

		if (this->M_CurrentSelectGemIndex != -1)
			if (_Index == this->M_CurrentSelectGemIndex)
			{
				this->M_CurrentSelectGem->SetValue(_Data->GetValue());
				LocalButton->SetIsEnabled(false);
			}
	}break;
	}


	//移除绑定
	LocalGrid->GetButton()->OnClicked.Clear();

	switch (LocalData->M_EquipmentType)
	{
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper:
	{
		FScriptDelegate LFunc;
		LFunc.BindUFunction(LocalGrid, TEXT("AddWepaonToSlot"));
		LocalGrid->GetButton()->OnClicked.Add(LFunc);
	}break;
	case EEquipment::E_WeaponGem:
	{
		FScriptDelegate LFunc;
		LFunc.BindUFunction(LocalGrid, TEXT("AddGemToSlot"));
		LocalGrid->GetButton()->OnClicked.Add(LFunc);
	}break;
	}

	FScriptDelegate LFuncDef;
	LFuncDef.BindUFunction(LocalGrid, TEXT("PlayOperateAudioDef"));
	LocalGrid->GetButton()->OnClicked.Add(LFuncDef);
}

void USynModel_WepaonGems::SetCurrentSelectWeapon(UItemDataTable* _Value, const int32& _Index)
{
	if (!IsValid(this->M_CurrentSelectWeapon))
		this->M_CurrentSelectWeapon = NewObject<UItemDataTable>(this, TEXT("N_CurrentSelectWeapon_Data_"));

	if (IsValid(_Value))
		this->M_CurrentSelectWeapon->SetValue(_Value->GetValue());
	else
		this->M_CurrentSelectWeapon->SetValue(nullptr);

	if (this->M_CurrentSelectWeapon->GetValue() && UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
	{
		//设置名称
		this->M_WeaponNameText->SetText(FText::FromString(((FEquipmentBase*)(_Value->GetValue()))->ItemName));

		this->M_WeaponNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->M_WeaponNameText->SetVisibility(ESlateVisibility::Collapsed);
	}

	this->M_CurrentSelectWeaponIndex = _Index;
}

void USynModel_WepaonGems::SetCurrentSelectGem(UItemDataTable* _Value, const int32& _Index)
{
	if (!IsValid(this->M_CurrentSelectGem))
		this->M_CurrentSelectGem = NewObject<UItemDataTable>(this, TEXT("N_CurrentSelectGem_Data_"));

	if (IsValid(_Value))
		this->M_CurrentSelectGem->SetValue(_Value->GetValue());
	else
		this->M_CurrentSelectGem->SetValue(nullptr);

	if (this->M_CurrentSelectGem->GetValue() && UGameplayStatics::GetPlatformName().Equals(TEXT("Android")))
	{
		//设置名称
		this->M_GemNameText->SetText(FText::FromString(((FEquipmentBase*)(_Value->GetValue()))->ItemName));

		this->M_GemNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->M_GemNameText->SetVisibility(ESlateVisibility::Collapsed);
	}

	this->M_CurrentSelectGemIndex = _Index;
}

void USynModel_WepaonGems::EnableWepaonToSlot()
{
	//显示头像
	this->M_WepaonHead->SetRenderOpacity(1.f);
	this->M_UUI_PlayerSynthesis->SetImageBrush(this->M_WepaonHead, ((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->ItemTexturePath.ToString());

	//从玩家背包获取当前武器数据
	int32 LID = -1;
	if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
	{
		const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

		FPlayerWeaponBase* const CurrentData = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID];

		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前武器支持最大槽数量：") + FString::FromInt(CurrentData->M_WeaponGemCountMax));

		//判断槽个数
		if (CurrentData->M_WeaponGem.Num() == 0)
		{
			//创建槽位置
			for (int32 i = 0; i < CurrentData->M_WeaponGemCountMax; i++)
				CurrentData->M_WeaponGem.Emplace();
		}

		//解析槽位置
		for (int32 i = 0; i < CurrentData->M_WeaponGem.Num(); i++)
		{
			FPlayerWeaponGemSlot* const Slot = &CurrentData->M_WeaponGem[i];

			//如果当前宝石未解锁
			if (!Slot->M_CurrentWeaponGemEnable)
			{
				Locals[i]->M_ItemAttachmentButton->SetIsEnabled(true);
				this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemAttachmentButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/329.329'"));

				//关闭背景点击按钮
				Locals[i]->M_ItemClickButton->SetIsEnabled(false);

				/*遇到未解锁的槽位，开启解锁按钮并且结束整个函数*/
				return;
			}
			else {
				//启用背景点击按钮
				Locals[i]->M_ItemClickButton->SetIsEnabled(true);

				//如果【没有】宝石名称（隐藏按钮）
				if (Slot->M_WepaonGemName.Equals(""))
				{
					//隐藏开槽按钮
					Locals[i]->M_ItemAttachmentButton->SetVisibility(ESlateVisibility::Hidden);

					//显示背景按钮
					if (i == 3)
						this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/344.344'"));
					else
						this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_1.T_PS_1'"));

					//设置边框显示
					Locals[i]->M_ItemBorderStyle->SetRenderOpacity(1.f);
					//设置边框缩放
					Locals[i]->M_ItemBorderStyle->SetRenderScale(FVector2D(1.f));

					//隐藏头像和等级
					Locals[i]->M_ItemHead->SetRenderOpacity(0.f);
					Locals[i]->M_ItemGrade->SetRenderOpacity(0.f);
				}
				else {

					// Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_1.T_PS_1'

					//宝石解锁->并且已经嵌入宝石
					// Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/675_1.675_1'

					//显示（并开启）卸载按钮
					Locals[i]->M_ItemAttachmentButton->SetVisibility(ESlateVisibility::Visible);
					Locals[i]->M_ItemAttachmentButton->SetIsEnabled(true);
					this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemAttachmentButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/334.334'"));

					//显示背景按钮
					if (i == 3)
						this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/344.344'"));
					else
						this->M_UUI_PlayerSynthesis->SetButtonStyle(Locals[i]->M_ItemClickButton, TEXT("Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_1.T_PS_1'"));

					//显示边框，设置边框
					Locals[i]->M_ItemBorderStyle->SetRenderOpacity(1.f);
					//设置边框缩放
					Locals[i]->M_ItemBorderStyle->SetRenderScale(FVector2D(0.8f));

					//显示头像
					Locals[i]->M_ItemHead->SetRenderOpacity(1.f);
					this->M_UUI_PlayerSynthesis->SetImageBrush(Locals[i]->M_ItemHead, CurrentData->M_WeaponGem[i].M_WepaonGemHeadPath);

					//显示等级
					if (CurrentData->M_WeaponGem[i].M_WeaponGemGrade > 0)
					{
						Locals[i]->M_ItemGrade->SetRenderOpacity(1.f);
						this->M_UUI_PlayerSynthesis->SetImageBrush(Locals[i]->M_ItemGrade, UGameSystemFunction::GetItemGradePath(CurrentData->M_WeaponGem[i].M_WeaponGemGrade));
					}
				}

			}
		}


	}
}

bool USynModel_WepaonGems::EnableGemToSlot()
{
	//判断玩家是否放置了武器
	if (this->M_CurrentSelectWeapon->GetValue() && this->M_CurrentSelectWeaponIndex != -1)
	{
		//获取槽位数量，并且查找是否有空槽位

		//从玩家背包获取当前武器数据
		int32 LID = -1;
		if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
		{
			FPlayerWeaponBase* const CurrentData = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID];

			//初始化槽索引
			this->M_CurrentGemSlotIndex = -1;

			//宝石名称查重
			for (int32 i = 0; i < CurrentData->M_WeaponGem.Num(); i++)
			{
				//判断当前槽位的宝石名称是否匹配当前选择的宝石名称
				if (CurrentData->M_WeaponGem[i].M_WepaonGemName.Equals(((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->ItemName))
				{
					//名称匹配->无法加入当前宝石

					this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前武器已经嵌入了【") + CurrentData->M_WeaponGem[i].M_WepaonGemName + TEXT("】"));

					return false;
				}
			}

			//宝石解析嵌入
			for (int32 i = 0; i < CurrentData->M_WeaponGem.Num(); i++)
			{
				//查询到空槽位
				if (CurrentData->M_WeaponGem[i].M_CurrentWeaponGemEnable && CurrentData->M_WeaponGem[i].M_WepaonGemName.Equals(TEXT("")))
				{
					const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

					Locals[i]->M_ItemHead->SetRenderOpacity(0.f);
					Locals[i]->M_ItemGrade->SetRenderOpacity(0.f);

					//宝石解析
					return this->GemEnableFunc(*Locals[i], i, (*CurrentData));
				}
			}

			this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前武器需要开槽-否则无法嵌入宝石"));
		}
		else {
			this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前武器数据无效-无法嵌入宝石"));
		}



		return false;
	}

	this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("请先选择【武器】"));

	return false;
}

void USynModel_WepaonGems::OpenSlotOrUnInstall_0()
{
	UE_LOG(LogTemp, Error, TEXT("A"));

	this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);

	this->GemsSlot(0);
}

void USynModel_WepaonGems::OpenSlotOrUnInstall_1()
{
	UE_LOG(LogTemp, Error, TEXT("B"));
	this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);

	this->GemsSlot(1);
}

void USynModel_WepaonGems::OpenSlotOrUnInstall_2()
{
	UE_LOG(LogTemp, Error, TEXT("C"));
	this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);

	this->GemsSlot(2);
}

void USynModel_WepaonGems::OpenSlotOrUnInstall_3()
{
	UE_LOG(LogTemp, Error, TEXT("D"));
	this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);

	this->GemsSlot(3);
}

void USynModel_WepaonGems::GemEnable()
{
	bool LResult = false;
	//搜索材料是否存在->不存在则不用执行
	for (auto& MData : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
	{
		if (MData.ItemName.Equals(this->M_NeedMaterialName))
		{
			//获得槽位
			const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };
			//获取具体数据
			FPlayerWeaponBase* const Local = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[this->M_WepaonID];
			//获取槽
			FSynModel_GemSlot* const LocalGemSlot = Locals[this->M_GemSlotIndex];

			//减去道具
			MData.M_Count--;
			if (MData.M_Count <= 0)
				MData.bWaitRemove = true;

			//将宝石槽位设置为开启
			Local->M_WeaponGem[this->M_GemSlotIndex].M_CurrentWeaponGemEnable = true;
			//移除开槽卸载按钮
			LocalGemSlot->M_ItemAttachmentButton->SetVisibility(ESlateVisibility::Hidden);

			LResult = true;

			break;
		}
	}

	if (LResult)
	{
		UGameSystemFunction::ClearWaitingItems(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material);
		UGameSystemFunction::SaveCurrentPlayerData();
		this->WidgetDataReload();

		//刷新槽
		this->EnableWepaonToSlot();
	}
	else {
		//提升 你并没有获得 铁钻道具
		this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("开启失败，【") + this->M_NeedMaterialName + TEXT("】不够"));
	}

}

bool USynModel_WepaonGems::GemEnableFunc(FSynModel_GemSlot& _Slot, const int32& _Index, const FPlayerWeaponBase& _WepaonData)
{
	if (this->M_CurrentSelectGem->GetValue() && this->M_CurrentSelectGemIndex != -1)
	{
		//查询宝石数据
		int32 LID = UPlayerStructManager::SerachEquipDataByID(((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->M_ItemID, UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas);
		if (LID != -1)
		{
			FWeaponGem* const LGem = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas[LID];

			//判断类型是否匹配

			/*

			1.判断宝石是否有对应武器名称
			2.判断宝石是否对应武器类型
			3.

			*/

			//无需专一武器->可以镶嵌任何武器
			if (LGem->M_TargetAttachWepaonName.Equals(TEXT("")))
			{
				//判断武器对应类型
				if (LGem->M_EPlayerWeaponType == _WepaonData.M_EPlayerWeaponType)
				{
					//武器类型匹配 -> 

				}
				else
				{
					// 武器类型不匹配 -> 当前宝石类型不能镶嵌到此武器上->只能镶嵌对应武器类型上					
					switch (LGem->M_EPlayerWeaponType)
					{
					case EPlayerWeaponType::E_First:this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石只能镶嵌在【主武器】")); return false;
					case EPlayerWeaponType::E_Second:this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石只能镶嵌在【副武器】")); return false;
					case EPlayerWeaponType::E_Super:this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石只能镶嵌在【超级武器】")); return false;
					}
					return false;
				}
			}
			else {
				if (LGem->M_TargetAttachWepaonName.Equals(_WepaonData.ItemName))
				{
					//武器名称匹配 -> 
				}
				else {
					// 武器名称不匹配 -> 当前宝石不能镶嵌到此武器上->只能镶嵌对应武器名称

					this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石只能镶嵌在【") + LGem->M_TargetAttachWepaonName + TEXT("】武器上"));

					return false;
				}
			}

			//得到(宝石)名称
			this->M_WaitAttachGemName = ((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->ItemName;
			this->M_CurrentGemSlotIndex = _Index;

			//设置图片
			this->M_UUI_PlayerSynthesis->SetImageBrush(_Slot.M_ItemHead, LGem->ItemTexturePath.ToString());
			this->M_UUI_PlayerSynthesis->SetImageBrush(_Slot.M_ItemGrade, UGameSystemFunction::GetItemGradePath(((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->M_EquipmentGrade));

			//显示图形
			_Slot.M_ItemHead->SetRenderOpacity(1.f);
			if (((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->M_EquipmentGrade > 0)
				_Slot.M_ItemGrade->SetRenderOpacity(1.f);

			//启动镶嵌按钮
			this->M_GemAttachmentButt->SetIsEnabled(true);

			return true;
		}
	}

	this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石数据无效-选择失败"));

	return false;
}

void USynModel_WepaonGems::RemoveGemToSlot()
{
	UE_LOG(LogTemp, Error, TEXT("E"));
	this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);


	if (this->M_CurrentGemSlotIndex != -1 && this->M_CurrentSelectGemIndex != -1)
	{
		const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

		this->SetCurrentSelectGem(nullptr, -1);

		Locals[this->M_CurrentGemSlotIndex]->M_ItemHead->SetRenderOpacity(0.f);
		Locals[this->M_CurrentGemSlotIndex]->M_ItemGrade->SetRenderOpacity(0.f);

		this->M_GemAttachmentButt->SetIsEnabled(false);

		this->LoadEquipment();
	}
}

void USynModel_WepaonGems::DirectRemoveGemDestory()
{
	FPlayerWeaponBase* WeaponData = (FPlayerWeaponBase*)(this->M_CurrentGemUninstallWepaonData->GetValue());

	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_CurrentWeaponGemEnable = true;
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WepaonGemName.Empty();
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WepaonGemHeadPath.Empty();
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WeaponGemGrade = 0;

	UGameSystemFunction::SaveCurrentPlayerData();

	//重新加载
	this->WidgetDataReload();
	//重新选择当前武器
	this->EnableWepaonToSlot();


	this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("宝石摧毁成功"));
}

void USynModel_WepaonGems::DirectRemoveGemReturnToBag()
{
	FPlayerWeaponBase* WeaponData = (FPlayerWeaponBase*)(this->M_CurrentGemUninstallWepaonData->GetValue());

	//创建道具到背包
	FEquipmentBase LGemData;
	if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WepaonGemName, LGemData, true, EEquipment::E_WeaponGem))
	{
		//复制等级
		LGemData.M_EquipmentGrade = WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WeaponGemGrade;
		//注册并且添加到背包
		UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(LGemData);
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(LGemData);
	}

	//当前武器数据卸载
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_CurrentWeaponGemEnable = true;
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WepaonGemName.Empty();
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WepaonGemHeadPath.Empty();
	WeaponData->M_WeaponGem[this->M_CurrentUninstallGemSlotIndex].M_WeaponGemGrade = 0;

	//消耗拆卸改锥
	((FMaterialBase*)(this->M_CurrentGemUninstallBit->GetValue()))->M_Count--;
	if (((FMaterialBase*)(this->M_CurrentGemUninstallBit->GetValue()))->M_Count <= 0)
	{
		((FMaterialBase*)(this->M_CurrentGemUninstallBit->GetValue()))->bWaitRemove = true;

		UGameSystemFunction::ClearWaitingItems<FMaterialBase>(UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material);
	}

	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData();
	//重新加载数据
	this->WidgetDataReload();
	//重新选择当前武器
	this->EnableWepaonToSlot();

	this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("宝石拆卸成功，已返还背包"));
}

void USynModel_WepaonGems::GemsSlot(const int32& _Index)
{
	int32 LID = -1;
	if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
	{
		//获得槽位
		const TArray<FSynModel_GemSlot*>& Locals = { &this->Slot_0 ,&this->Slot_1,&this->Slot_2,&this->Slot_3 };

		//获取具体数据
		FPlayerWeaponBase* const Local = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID];

		if (Local->M_WeaponGem[_Index].M_CurrentWeaponGemEnable)
		{
			//如果【有】宝石名称
			if (!Local->M_WeaponGem[_Index].M_WepaonGemName.Equals(""))
			{
				this->GemSlotUninstall(_Index);
			}
		}
		else {
			this->GemSlotOpen(_Index);
		}
	}
}

void USynModel_WepaonGems::GemSlotOpen(const int32& _Index)
{
	//获取武器的数据
	int32 LID = -1;
	if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
	{
		FString LText = TEXT("是否使用：【");

		switch (_Index)
		{
		case 0: {LText += TEXT("铁钻"); this->M_NeedMaterialName = TEXT("铁钻"); }break;
		case 1: {LText += TEXT("钢钻"); this->M_NeedMaterialName = TEXT("钢钻"); }break;
		case 2: {LText += TEXT("金刚钻"); this->M_NeedMaterialName = TEXT("金刚钻"); }break;
		case 3: {LText += TEXT("月石钻"); this->M_NeedMaterialName = TEXT("月石钻"); }break;
		}

		LText += TEXT("】开启当前槽位?");

		//需要钻头才能开启
		this->M_UUI_PlayerSynthesis->CreateSelectTipWidget(LText)->OnClicked.AddDynamic(this, &USynModel_WepaonGems::GemEnable);

		//设置宝石槽索引
		this->M_GemSlotIndex = _Index;
		//设置武器ID号
		this->M_WepaonID = LID;
	}

}

void USynModel_WepaonGems::GemSlotUninstall(const int32& _Index)
{
	UE_LOG(LogTemp, Error, TEXT("D：%d"), _Index);

	/*
	1.判断玩家是否拥有-拆卸改锥->开启以下提示
	2.如果有将使用拆卸改锥进行宝石卸载
	3.如果玩家没有拆卸改锥-开始以下提示
	4.如果没有拆卸改锥->宝石卸载后会被摧毁
	*/

	//初始化拆卸改锥数据
	if (!IsValid(this->M_CurrentGemUninstallBit))
		this->M_CurrentGemUninstallBit = NewObject<UItemDataTable>(this, TEXT("SynModel_GemUninstallBit_"));

	//初始化武器数据
	if (!IsValid(this->M_CurrentGemUninstallWepaonData))
		this->M_CurrentGemUninstallWepaonData = NewObject<UItemDataTable>(this, TEXT("SynModel_GemUninstallWepaonData_"));


	bool LResult = false;

	//查询玩家背包
	for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material)
	{
		if (Data.ItemName.Equals(TEXT("拆卸改锥")))
		{
			LResult = true;

			this->M_CurrentGemUninstallBit->SetValue((FTableRowBase*)(&Data));

			break;
		}
	}

	//得到武器数据
	int32 LID = -1;
	if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue()))->M_ItemID, LID) != -1)
	{
		//设置武器数据
		this->M_CurrentGemUninstallWepaonData->SetValue((FTableRowBase*)(&UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponDatas[LID]));
		//设置拆卸索引
		this->M_CurrentUninstallGemSlotIndex = _Index;

		if (LResult)
		{
			//有拆卸改锥
			this->M_UUI_PlayerSynthesis->CreateSelectTipWidget(TEXT("准备拆卸：【") + ((FPlayerWeaponBase*)(this->M_CurrentGemUninstallWepaonData->GetValue()))->M_WeaponGem[_Index].M_WepaonGemName + TEXT("】，检测到你拥有【拆卸改锥】道具，当你拆卸宝石时，宝石将回到你的背包"))->OnClicked.AddDynamic(this, &USynModel_WepaonGems::DirectRemoveGemReturnToBag);
		}
		else {
			//无拆卸改锥
			this->M_UUI_PlayerSynthesis->CreateSelectTipWidget(TEXT("准备拆卸：【") + ((FPlayerWeaponBase*)(this->M_CurrentGemUninstallWepaonData->GetValue()))->M_WeaponGem[_Index].M_WepaonGemName + TEXT("】，【!!注意!!】：当前你未拥有【拆卸改锥】道具!!!! 当你拆卸宝石时，宝石将直接被摧毁-永远消失!"))->OnClicked.AddDynamic(this, &USynModel_WepaonGems::DirectRemoveGemDestory);
		}
	}
	else {
		this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("宝石拆卸失败-武器数据有误"));
	}
}

void USynModel_WepaonGems::GemAttach()
{
	UE_LOG(LogTemp, Error, TEXT("F"));
	//this->M_UUI_PlayerSynthesis->PlayOperateAudio(false);


	/*

	1.判断武器是否有效
	2.判断宝石是否有效
	3.镶嵌宝石填写数据
	4.从背包消除宝石数据

	*/

	if (this->M_CurrentSelectWeapon->GetValue())
	{
		if (this->M_CurrentSelectGem->GetValue())
		{
			//得到武器数据
			int32 LID = -1;
			if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(
				((FEquipmentBase*)(this->M_CurrentSelectWeapon->GetValue())
					)->M_ItemID, LID) != -1)
			{
				//获取具体数据（武器）
				FPlayerWeaponBase* const Local =
					&UFVMGameInstance::GetPlayerStructManager_Static()->
					M_FPlayerWeaponDatas[LID];

				//获取宝石数据（宝石）
				int32 LGemID = UPlayerStructManager::SerachEquipDataByID(
					((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->M_ItemID,
					UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas);
				if (LGemID != -1)
				{
					FWeaponGem* const LGem = &UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas[LGemID];

					this->M_GemAttachmentButt->SetIsEnabled(false);

					//赋值数据
					for (auto& Data : Local->M_WeaponGem)
					{
						if (Data.M_CurrentWeaponGemEnable && Data.M_WepaonGemName.Equals(TEXT("")))
						{
							Data.M_WeaponGemGrade = (
								(FEquipmentBase*)(this->M_CurrentSelectGem->GetValue())
								)->M_EquipmentGrade;
							Data.M_WepaonGemName = LGem->ItemName;
							Data.M_WepaonGemHeadPath = LGem->ItemTexturePath.ToString();

							if (UFVMGameInstance::GetDebug())
							{
								UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("宝石镶嵌成功：") +
									LGem->ItemName);
							}

							//添加历史记录
							UPlayerRecord::AddGemAttachCount();
							UPlayerRecord::AddDayDayNewGemAttachName(LGem->ItemName);

							//调用任务
							UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);
							UGameSystemFunction::SaveCurrentPlayerData();

							break;
						}
					}

					//刷新当前选择的武器ID[当前武器UI ID大于宝石UI ID  则武器UI ID索引减一]
					if (this->M_CurrentSelectWeaponIndex > this->M_CurrentSelectGemIndex)
					{
						this->M_CurrentSelectWeaponIndex--;
					}

					//注销当前宝石数据
					((FEquipmentBase*)(this->M_CurrentSelectGem->GetValue()))->bWaitRemove = true;
					UGameSystemFunction::ClearWaitingItemsForEquip(
						UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment
					);

					this->SetCurrentSelectGem(nullptr, -1);

					//保存数据
					UGameSystemFunction::SaveCurrentPlayerData();

					//重新加载数据
					this->WidgetDataReload();
					//重新解析武器
					this->EnableWepaonToSlot();

					this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("宝石镶嵌成功"));
				}
			}
		}
		else {
			this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前宝石数据无效-镶嵌失败"));
		}
	}
	else {
		this->M_UUI_PlayerSynthesis->CreateTipWidget(TEXT("当前武器数据无效-镶嵌失败"));
	}
}
