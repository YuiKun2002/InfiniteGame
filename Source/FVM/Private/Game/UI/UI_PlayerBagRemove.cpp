// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagRemove.h"

#include "Game/UI/MyUI_BagRemove_Card.h"
#include "Game/UI/UI_BagRemove_Equipment.h"
#include "Game/UI/UI_PlayerBag.h"

#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/PlayerDataSubsystem.h"

#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>

#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>


void UUI_PlayerBagRemove::AddCurrentSelect(int32 _Key, const FString& _ItemName)
{
	this->M_CurrentSelect.Emplace(_Key, _ItemName);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, _ItemName + TEXT("[已经被加入待删除列表] key:") + FString::FromInt(_Key));
	}
}

void UUI_PlayerBagRemove::RemoveCurrentSelect(int32 _Key)
{
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, (*M_CurrentSelect.Find(_Key)) + TEXT("[已经被加入待删除列表]"));
	}

	this->M_CurrentSelect.Remove(_Key);
}

bool UUI_PlayerBagRemove::FindCurrentSelect(int32 _Key)
{
	return this->M_CurrentSelect.Contains(_Key);
}

UWidget* UUI_PlayerBagRemove::WidgetCreate_InitEquipment(UItemDataTable* _Data, int32 L_Index)
{
	//创建显示的UI
	UUI_BagRemove_Equipment* Widget = CreateWidget<UUI_BagRemove_Equipment>(this, LoadClass<UUI_BagRemove_Equipment>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagItemsEquipmentView.BP_PlayerBagItemsEquipmentView_C'")));

	if (this->M_PanelNumber == 0)
	{
		const FEquipmentBase& Obj = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment[((FItemData_Index*)(_Data->GetValue()))->M_Index];
		//设置道具名称
		Widget->SetItemName(Obj.ItemName);
		Widget->SetEquipment(Obj.ItemTexturePath.ToString(), Obj.M_IsOverlap, Obj.M_Count);
		//设置界面序号和界面的索引
		Widget->SetTagItems(this->M_FUI_PlayerBagRemove_Item_Index[L_Index].M_Index, this->M_PanelNumber);
	}
	else
	{
		Widget->SetEquipment(((FMaterialBase*)(_Data->GetValue()))->ItemTexturePath.ToString(), true, ((FMaterialBase*)(_Data->GetValue()))->M_Count);
		//设置界面序号和界面的索引
		Widget->SetTagItems(L_Index, this->M_PanelNumber);
		//设置道具名称
		Widget->SetItemName(((FEquipmentBase*)(_Data->GetValue()))->ItemName);
	}

	//设置删除界面对象
	Widget->SetUIPlayerBagRemove(this);
	//默认不选中
	Widget->SelectItems(false);

	Widget->BindSelectFunc();

	return Widget;
}

void UUI_PlayerBagRemove::WidgetRefresh_UpdateEquipment(UItemDataTable* _Data, int32 L_Index, UWidget* _UWidget)
{
	//开启指定搜索
	if (this->M_bUseSerach)
	{
		if (this->M_PanelNumber == 0)
		{
			const FEquipmentBase& Obj = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment[((FItemData_Index*)(_Data->GetValue()))->M_Index];
			//设置道具名称
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetItemName(Obj.ItemName);
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetEquipment(Obj.ItemTexturePath.ToString(), Obj.M_IsOverlap, Obj.M_Count);
			//设置道具的对应索引，和界面索引
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetTagItems(this->M_FUI_PlayerBagRemove_Item_Index[L_Index].M_Index, this->M_PanelNumber);
		}
		else {
			const FMaterialBase& Obj = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material[((FItemData_Index*)(_Data->GetValue()))->M_Index];
			//设置道具名称
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetItemName(Obj.ItemName);
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetEquipment(Obj.ItemTexturePath.ToString(), true, Obj.M_Count);
			//设置道具的对应索引，和界面索引
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetTagItems(L_Index, this->M_PanelNumber);
		}
	}
	else {
		//刷新UI	
		if (this->M_PanelNumber == 0)
		{
			const FEquipmentBase& Obj = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment[((FItemData_Index*)(_Data->GetValue()))->M_Index];
			//设置道具名称
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetItemName(Obj.ItemName);
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetEquipment(Obj.ItemTexturePath.ToString(), Obj.M_IsOverlap, Obj.M_Count);
			//设置道具的对应索引，和界面索引
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetTagItems(this->M_FUI_PlayerBagRemove_Item_Index[L_Index].M_Index, this->M_PanelNumber);
		}
		else
		{
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetEquipment(((FMaterialBase*)(_Data->GetValue()))->ItemTexturePath.ToString(), true, ((FMaterialBase*)(_Data->GetValue()))->M_Count);
			//设置道具的对应索引，和界面索引
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetTagItems(L_Index, this->M_PanelNumber);
			//设置道具名称
			Cast<UUI_BagRemove_Equipment>(_UWidget)->SetItemName(((FMaterialBase*)(_Data->GetValue()))->ItemName);
		}
	}

	//设置选择状态
	Cast<UUI_BagRemove_Equipment>(_UWidget)->Update();
}

UWidget* UUI_PlayerBagRemove::WidgetCreate_InitCards(UItemDataTable* _Data, int32 L_Index)
{
	UMyUI_BagRemove_Card* Widget = CreateWidget<UMyUI_BagRemove_Card>(this, LoadClass<UMyUI_BagRemove_Card>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagItemsCardView.BP_PlayerBagItemsCardView_C'")));
	Widget->SetUIPlayerBagRemove(this);
	Widget->SetCard(((FItemCard*)(_Data->GetValue()))->ItemTexturePath.ToString(), ((FItemCard*)(_Data->GetValue()))->M_CardGrade);
	Widget->SetItemName(((FItemCard*)(_Data->GetValue()))->ItemName);
	Widget->SetTagItems(L_Index, this->M_PanelNumber);
	Widget->SelectItems(false);
	Widget->BindSelectFunc();
	return Widget;
}

void UUI_PlayerBagRemove::WidgetRefresh_UpdateCards(UItemDataTable* _Data, int32 L_Index, UWidget* _UWidget)
{
	if (this->M_bUseSerach)
	{
		//获取数据
		const FItemCard& Obj = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Card[((FItemData_Index*)(_Data->GetValue()))->M_Index];
		//更新数据
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetCard(Obj.ItemTexturePath.ToString(), Obj.M_CardGrade);
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetItemName(Obj.ItemName);
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetTagItems(((FItemData_Index*)(_Data->GetValue()))->M_Index, this->M_PanelNumber);
	}
	else {
		//更新数据
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetCard(((FItemCard*)(_Data->GetValue()))->ItemTexturePath.ToString(), ((FItemCard*)(_Data->GetValue()))->M_CardGrade);
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetItemName(((FItemCard*)(_Data->GetValue()))->ItemName);
		Cast<UMyUI_BagRemove_Card>(_UWidget)->SetTagItems(L_Index, this->M_PanelNumber);
	}



	Cast<UMyUI_BagRemove_Card>(_UWidget)->Update();
}

void UUI_PlayerBagRemove::InitLoader(uint8 _PanelNumber)
{
	switch (_PanelNumber)
	{
	case 1u: {
		this->M_UItemLoadManager_Cards = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Cards_Manager_SearchT"));
		this->M_UItemLoadManager_Cards->InitWidget_First(this->M_UUniformGridPanel, this->M_UScrollBox, 0.01f);
		this->M_UItemLoadManager_Cards->InitRange_Second(9, 1, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(_PanelNumber), 700.f, 90.f);
		this->M_UItemLoadManager_Cards->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitCards"));
		this->M_UItemLoadManager_Cards->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateCards"));
	}break;
	case 0u:
	case 2u: {
		this->M_UItemLoadManager_Equipment = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Equipment_Manager_SearchT"));
		this->M_UItemLoadManager_Equipment->InitWidget_First(this->M_UUniformGridPanel, this->M_UScrollBox, 0.01f);
		this->M_UItemLoadManager_Equipment->InitRange_Second(9, 1, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(_PanelNumber), 700.f, 90.f);
		this->M_UItemLoadManager_Equipment->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_InitEquipment"));
		this->M_UItemLoadManager_Equipment->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_UpdateEquipment"));
	}break;
	}

	//绑定关闭界面
	this->M_ClosePanelButton->OnClicked.AddDynamic(this, &UUI_PlayerBagRemove::CloseCurrentPanel);
	this->M_RemoveButton->OnClicked.AddDynamic(this, &UUI_PlayerBagRemove::Remove);
}

bool UUI_PlayerBagRemove::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_InputItemNameBox = this->GetWidgetComponent<UEditableTextBox>(this, "Input_SearchItemBox");
	this->M_SearchButton = this->GetWidgetComponent<UButton>(this, "Serach_Butt");
	this->M_RemoveButton = this->GetWidgetComponent<UButton>(this, "Remove_Item");
	this->M_ClosePanelButton = this->GetWidgetComponent<UButton>(this, "CloseCurrentPanel");

	this->M_UUniformGridPanel = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_55");
	this->M_UScrollBox = this->GetWidgetComponent<UScrollBox>(this, "ScrollBox_20");

	return true;
}

void UUI_PlayerBagRemove::SearchBag_Card()
{
	this->M_bSelect_Tag = true;
	//使用搜索模式
	this->M_bUseSerach = true;

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(1); return;
	}

	//清空索引
	this->M_FUI_PlayerBagRemove_Item_Index.Empty();

	int32 _SearchMaxCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

	int32 L_Index = 0;
	for (const FItemCard& _Item : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card)
	{
		//限定搜索个数
		if (L_Index >= _SearchMaxCount)
			break;

		//搜索到之后存储索引
		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()))
		{
			FItemData_Index LLItem;
			LLItem.M_Index = L_Index;
			this->M_FUI_PlayerBagRemove_Item_Index.Emplace(LLItem);
		}

		L_Index++;
	}

	this->M_UItemLoadManager_Cards->UpdateDatatable(this->M_FUI_PlayerBagRemove_Item_Index);
	this->M_UItemLoadManager_Cards->SetLoadItemMaxCount(_SearchMaxCount);
	this->M_UItemLoadManager_Cards->ContinueRun();
}

void UUI_PlayerBagRemove::SearchBag_Equipment()
{

	this->M_bSelect_Tag = true;
	//使用搜索模式
	this->M_bUseSerach = true;

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(0); return;
	}

	this->M_FUI_PlayerBagRemove_Item_Index.Empty();

	int32 _SearchMaxCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0);

	int32 L_Index = 0;

	for (const FEquipmentBase& _Item : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
	{
		//限制搜索个数
		if (L_Index >= _SearchMaxCount)
			break;

		//搜到之后只能搜索到未使用的装备对象
		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()) && !_Item.M_Used)
		{
			FItemData_Index LLItem;
			LLItem.M_Index = L_Index;
			this->M_FUI_PlayerBagRemove_Item_Index.Emplace(LLItem);
		}

		L_Index++;
	}

	this->M_UItemLoadManager_Equipment->UpdateDatatable(this->M_FUI_PlayerBagRemove_Item_Index);
	this->M_UItemLoadManager_Equipment->SetLoadItemMaxCount(_SearchMaxCount);
	this->M_UItemLoadManager_Equipment->ContinueRun();
}

void UUI_PlayerBagRemove::SearchBag_Materials()
{
	//标记
	this->M_bSelect_Tag = true;
	//使用搜索模式
	this->M_bUseSerach = true;

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(2); return;
	}

	this->M_FUI_PlayerBagRemove_Item_Index.Empty();

	int32 L_Index = 0;
	for (const FMaterialBase& _Item : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material)
	{
		//搜到之后只能搜索到未使用的装备对象
		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()))
		{
			FItemData_Index LLItem;
			LLItem.M_Index = L_Index;
			this->M_FUI_PlayerBagRemove_Item_Index.Emplace(LLItem);
		}
		L_Index++;
	}

	this->M_UItemLoadManager_Equipment->UpdateDatatable(this->M_FUI_PlayerBagRemove_Item_Index);
	this->M_UItemLoadManager_Equipment->SetLoadItemMaxCount(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(2));
	this->M_UItemLoadManager_Equipment->ContinueRun();
}

void UUI_PlayerBagRemove::CloseCurrentPanel()
{
	//将选择的标记全部取消
	if (this->M_PanelNumber == 0)
		for (FEquipmentBase& _Equipment : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
			_Equipment.bWaitRemove = false;

	if (this->M_PanelNumber == 1)
		for (FItemCard& _Card : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card)
			_Card.bWaitRemove = false;

	if (this->M_PanelNumber == 2)
		for (FMaterialBase& _Materials : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material)
			_Materials.bWaitRemove = false;

	//销毁列表
	this->M_CurrentSelect.Empty();
	this->M_FUI_PlayerBagRemove_Item_Index.Empty();

	this->RemoveFromParent();
}

void UUI_PlayerBagRemove::SearchBag(uint8 _PanelNumber)
{
	//不使用搜索
	this->M_bUseSerach = false;
	//索引
	int32 L_Index = 0;
	//界面索引
	this->M_PanelNumber = _PanelNumber;
	//不标记
	this->M_bSelect_Tag = false;

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 0u)
	{
		this->M_FUI_PlayerBagRemove_Item_Index.Empty();

		int32 _SearchMaxCount_0 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0);

		for (const FEquipmentBase& _Item : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
		{
			if (L_Index >= _SearchMaxCount_0)
				break;

			//搜到之后只能搜索到未使用的装备对象
			if (!_Item.M_Used)
			{
				FItemData_Index TLocalIndex;
				TLocalIndex.M_Index = L_Index;
				this->M_FUI_PlayerBagRemove_Item_Index.Emplace(TLocalIndex);
			}

			L_Index++;
		}

		this->M_UItemLoadManager_Equipment->UpdateDatatable(this->M_FUI_PlayerBagRemove_Item_Index);
		this->M_UItemLoadManager_Equipment->SetLoadItemMaxCount(_SearchMaxCount_0);
		this->M_UItemLoadManager_Equipment->SetResetScrollBoxOffset();
		this->M_UItemLoadManager_Equipment->ContinueRun();
	}

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 1u)
	{
		int32 _SearchMaxCount_1 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

		this->M_UItemLoadManager_Cards->UpdateDatatable(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card);
		this->M_UItemLoadManager_Cards->SetLoadItemMaxCount(_SearchMaxCount_1);
		this->M_UItemLoadManager_Cards->SetResetScrollBoxOffset();
		this->M_UItemLoadManager_Cards->ContinueRun();
	}

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 2u)
	{
		int32 _SearchMaxCount_2 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(2);
		this->M_UItemLoadManager_Equipment->UpdateDatatable(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);
		this->M_UItemLoadManager_Equipment->SetLoadItemMaxCount(_SearchMaxCount_2);
		this->M_UItemLoadManager_Equipment->SetResetScrollBoxOffset();
		this->M_UItemLoadManager_Equipment->ContinueRun();
	}

}

UButton* UUI_PlayerBagRemove::GetSearchButton()
{
	return this->M_SearchButton;
}

void UUI_PlayerBagRemove::Remove()
{
	if (this->M_PanelNumber == 0)
	{
		this->M_UIBag->Clear();

		UGameSystemFunction::ClearWaitingItemsForEquip(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);

		//重新加载背包物品
		this->M_UIBag->LoadItemsData();
	}

	if (this->M_PanelNumber == 1)
	{
		this->M_UIBag->Clear();

		UPlayerStructManager::Remove_Item<FItemCard>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card);

		//加载数据
		this->M_UIBag->LoadCardData();
	}

	if (this->M_PanelNumber == 2)
	{
		this->M_UIBag->Clear();

		UPlayerStructManager::Remove_Item<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);

		this->M_UIBag->LoadMaterialData();
	}

	//销毁列表
	this->M_CurrentSelect.Empty();

	//移出UI
	this->RemoveFromParent();

	//保存数据
	int32 RemoveNum = this->M_PanelNumber;
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("进行背包道具移除操作，移除界面号：")) + FString::FromInt(RemoveNum));
}
