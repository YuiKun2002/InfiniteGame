// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/Instructions/PlayerInstructions.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/PlayerDataSubsystem.h"

bool UPlayerInstructions::ReadInstruction(const TArray<FString>& Instruction)
{
	if (Instruction.Num() == 0)
	{
		return false;
	}

	//设置指令集合描述
	this->SetInstructions(TEXT("DataTable'/Game/Resource/BP/Data/Instructions/Player_Instruction.Player_Instruction'"));

	//帮助指令
	if (this->GetSystem()->AddHelpInstructionAndPrint(Instruction[0],
		TEXT("DataTable'/Game/Resource/BP/Data/Instructions/Player_Instruction.Player_Instruction'")))
	{
		return false;
	}

	//发送帮助指令
	if (this->PrintHelpInstruction(TEXT("player"), Instruction))
	{
		return false;
	}

	//发送指令
	if (this->ExecuteInsHead(Instruction, TEXT("send"), std::bind(&UPlayerInstructions::SendIns, this, std::placeholders::_1)))
	{
		return true;
	}

	//删除指令
	if (this->ExecuteInsHead(Instruction, TEXT("clear"), std::bind(&UPlayerInstructions::ClearIns, this, std::placeholders::_1)))
	{
		return true;
	}

	//设置指令
	if (this->ExecuteInsHead(Instruction, TEXT("set"), std::bind(&UPlayerInstructions::SetIns, this, std::placeholders::_1)))
	{
		return true;
	}

	//回退指令
	if (this->ExecuteInsHead(Instruction, TEXT("return"), std::bind(&UPlayerInstructions::ReturnIns, this, std::placeholders::_1)))
	{
		return true;
	}

	return false;
}

bool UPlayerInstructions::SendIns(const TArray<FString>& Instruction)
{
	//执行装备指令
	if (this->ExecuteInsHead(Instruction, TEXT("equip"), std::bind(&UPlayerInstructions::SendEquip, this, std::placeholders::_1)))
	{
		return true;
	}

	//执行卡片指令
	if (this->ExecuteInsHead(Instruction, TEXT("card"), std::bind(&UPlayerInstructions::SendCard, this, std::placeholders::_1)))
	{
		return true;
	}

	//执行材料指令
	if (this->ExecuteInsHead(Instruction, TEXT("mater"), std::bind(&UPlayerInstructions::SendMaterial, this, std::placeholders::_1)))
	{
		return true;
	}

	return false;
}

bool UPlayerInstructions::SendEquip(const TArray<FString>& Instruction)
{
	if (Instruction.Num() == 0)
	{
		return false;
	}

	if (Instruction.Num() > 0)
	{
		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-list")))
		{
			TArray<FItemBase> CurEquip;
			UEquipmentBaseStruct::GetAllEquipmentName(CurEquip);
			FString CurStr = FALD(TEXT("装备道具列表：")) + TEXT("\r\n");
			for (auto& CurItem : CurEquip)
			{
				CurStr += FALD(TEXT("【")) + FAL(CurItem.ItemName.ToString(), TEXT("c")) + FALD(TEXT("】")) + TEXT("\r\n");
			}
			this->GetSystem()->AddCurrent(
				CurStr
			);
			return false;
		}

		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-all")))
		{
			TArray<FEquipmentBase> CurEquip;
			UEquipmentBaseStruct::GetAllEquipments(CurEquip);

			for (auto& CurItem : CurEquip)
			{
				if (UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(CurItem))
				{
					UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(CurItem);
					UGameLogSubsystem::AddPlayerGetEquipmentLog(CurItem.ItemName.ToString(), 1, 0);
				}
			}

			return true;
		}
	}


	int32 Count = 1;
	if (Instruction.Num() > 1)
	{

		auto SendItem = [&](const FString& Name, int32 Count, int32 Grade)->bool {

			if (Count <= 0)
			{
				Count = 1;
			}

			if (Grade <= 0)
			{
				Grade = 0;
			}
			else if (Grade >= 16)
			{
				Grade = 16;
			}

			FEquipmentBase CurE;
			if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(Name, CurE, true, EEquipment::E_WeaponGem
			))
			{
				for (int32 i = 0; i < Count; i++)
				{
					FEquipmentBase NewData = CurE;
					NewData.M_EquipmentGrade = Grade;
					if (UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(NewData))
					{
						UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment.Emplace(NewData);
						UGameLogSubsystem::AddPlayerGetEquipmentLog(NewData.ItemName.ToString(), 1, Grade);
					}
				}
				return true;
			}
			else {

				return false;
			}
			};

		//其他指令【-g】 设置宝石等级
		if (this->GetSystem()->InstructionTest(Instruction[1], TEXT("-g")))
		{
			return SendItem(Instruction[0], 1, FCString::Atoi(*Instruction[2]));
		}
		else if (Instruction.Num() > 3 && this->GetSystem()->InstructionTest(Instruction[2], TEXT("-g")))
		{
			return SendItem(Instruction[0], FCString::Atoi(*Instruction[1]), FCString::Atoi(*Instruction[3]));
		}
		else {
			//数字指令
			Count = FCString::Atoi(*Instruction[1]);
			if (Count == 0)
			{
				Count = 1;
			}
		}
	}

	//发送装备
	if (!UGameSystemFunction::SendEquipmentToPlayerBag(Instruction[0], Count))
	{
		this->GetSystem()->AddCurrent(FAL(FString(Instruction[0] + TEXT("不存在")), TEXT("g")));
		return false;
	}

	return true;
}

bool UPlayerInstructions::SendCard(const TArray<FString>& Instruction)
{
	if (Instruction.Num() == 0)
	{
		return false;
	}

	if (Instruction.Num() > 0)
	{
		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-list")))
		{
			TArray<FItemBase> CurEquip;
			UCardBaseStruct::GetAllCardName(CurEquip);
			FString CurStr = FALD(TEXT("卡片列表：")) + TEXT("\r\n");
			for (auto& CurItem : CurEquip)
			{
				CurStr += FALD(TEXT("【")) + FAL(CurItem.ItemName.ToString(), TEXT("c")) + FALD(TEXT("】")) + TEXT("\r\n");
			}
			this->GetSystem()->AddCurrent(
				CurStr
			);
			return false;
		}

		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-all")))
		{
			int32 LGrade = FCString::Atoi(*Instruction[1]);

			if (LGrade <= 0)
			{
				LGrade = 0;
			}
			else if (LGrade >= 16)
			{
				LGrade = 16;
			}

			TArray<FItemBase> CurEquip;
			UCardBaseStruct::GetAllCardName(CurEquip);
			for (auto& CurItem : CurEquip)
			{
				UGameSystemFunction::SendCardToPlayerBag(CurItem.ItemName.ToString(), LGrade);
			}
		}
	}

	//追加了多少指令
	bool bRe = false;
	int32 Count = Instruction.Num() / 3;
	for (int32 i = 0; i < Count; i++)
	{
		bRe = true;

		FString CurName = Instruction[0 + 3 * i];
		int32 Num = FCString::Atoi(*Instruction[1 + 3 * i]);
		int32 Grade = FCString::Atoi(*Instruction[2 + 3 * i]);

		if (Num <= 0)
		{
			Num = 1;
		}

		if (Grade <= 0)
		{
			Grade = 0;
		}
		else if (Grade >= 16)
		{
			Grade = 16;
		}

		for (int32 CI = 0; CI < Num; CI++)
		{
			UGameSystemFunction::SendCardToPlayerBag(CurName, Grade);
		}
	}

	if (!bRe)
	{
		if (Instruction.Num() > 1)
		{
			int32 LCount = FCString::Atoi(*Instruction[1]);
			if (LCount <= 0)
			{
				LCount = 1;
			}

			for (int32 CI = 0; CI < LCount; CI++)
			{
				//并没有指令追加
				UGameSystemFunction::SendCardToPlayerBag(Instruction[0], 0);
			}
		}
		else if (Instruction.Num() > 0)
		{
			UGameSystemFunction::SendCardToPlayerBag(Instruction[0], 0);
		}
		else {
			return false;
		}
	}

	return true;
}

bool UPlayerInstructions::SendMaterial(const TArray<FString>& Instruction)
{
	if (Instruction.Num() == 0)
	{
		return false;
	}

	if (Instruction.Num() > 0)
	{
		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-list")))
		{
			TArray<FItemBase> CurEquip;
			UMaterialBaseStruct::GetAllMaterial(CurEquip);
			FString CurStr = FALD(TEXT("材料列表：")) + TEXT("\r\n");
			for (auto& CurItem : CurEquip)
			{
				CurStr += FALD(TEXT("【")) + FAL(CurItem.ItemName.ToString(), TEXT("c")) + FALD(TEXT("】")) + TEXT("\r\n");
			}
			this->GetSystem()->AddCurrent(
				CurStr
			);
			return false;
		}

		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-all")))
		{
			TArray<FMaterialBase> CurEquip;

			if (Instruction.Num() > 1)
			{
				int32 LLCount = FCString::Atoi(*Instruction[1]);
				if (LLCount <= 0)
				{
					LLCount = 1;
				}
				else if (LLCount >= 1000000000)
				{
					LLCount = 1000000000;
				}

				UMaterialBaseStruct::GetAllMaterial(CurEquip, LLCount);
			}
			else {
				UMaterialBaseStruct::GetAllMaterial(CurEquip, 1);
			}

			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material.Empty();
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Material = CurEquip;

			return true;
		}
	}

	//追加了多少指令
	bool bRe = false;
	int32 Count = Instruction.Num() / 2;
	for (int32 i = 0; i < Count; i++)
	{
		bRe = true;

		FString CurName = Instruction[0 + 2 * i];
		int32 Num = FCString::Atoi(*Instruction[1 + 2 * i]);

		UGameSystemFunction::SendMaterialToPlayerBag(CurName, Num, true);
	}

	if (!bRe)
	{
		if (Instruction.Num() > 1)
		{
			UGameSystemFunction::SendMaterialToPlayerBag(Instruction[0], FCString::Atoi(*Instruction[1]), true);
		}
		else if (Instruction.Num() > 0)
		{
			UGameSystemFunction::SendMaterialToPlayerBag(Instruction[0], 1, true);
		}
		else {
			return false;
		}
	}

	return true;
}

bool UPlayerInstructions::ClearIns(const TArray<FString>& Instruction)
{
	if (Instruction.Num() == 0)
	{
		return false;
	}

	if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("equip")))
	{
		//标记能够删除的道具
		for (auto& CurEquip : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
		{
			if (!CurEquip.M_Used)
			{
				UFVMGameInstance::GetPlayerStructManager_Static()->RemoveEquipmentFromPlayerBag(CurEquip);
			}
		}

		UFVMGameInstance::GetPlayerStructManager_Static()->Remove_Item(
			UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment
		);

		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("装备全部销毁操作")));

		return true;
	}

	if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("card")))
	{

		if (UPlayerDataSubsystem::GetPlayerDataSubsystemStatic())
		{
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance()->M_PlayerItems_Card.Empty();

			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("卡片全部销毁操作")));
		}

		return true;
	}

	if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("mater")))
	{

		if (UPlayerDataSubsystem::GetPlayerDataSubsystemStatic())
		{
			UPlayerDataSubsystem::GetPlayerDataSubsystemStatic()->GetPlayerDataInstance()->M_PlayerItems_Material.Empty();

			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("材料全部销毁操作")));
		}

		return true;
	}


	return false;
}

bool UPlayerInstructions::SetIns(const TArray<FString>& Instruction)
{
	//执行设置技能书指令
	if (this->ExecuteInsHead(Instruction, TEXT("sk"), std::bind(&UPlayerInstructions::SetSKIns, this, std::placeholders::_1)))
	{
		return true;
	}

	//执行设置角色等级指令
	if (this->ExecuteInsHead(Instruction, TEXT("pg"), std::bind(&UPlayerInstructions::SetPGIns, this, std::placeholders::_1)))
	{
		return true;
	}

	//执行设置货币数量指令
	if (this->ExecuteInsHead(Instruction, TEXT("coin"), std::bind(&UPlayerInstructions::SetCoinIns, this, std::placeholders::_1)))
	{
		return true;
	}

	return false;
}

bool UPlayerInstructions::SetSKIns(const TArray<FString>& Instruction)
{

	if (Instruction.Num() > 0)
	{
		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-list")))
		{
			FString CurStr = FALD(TEXT("技能书列表：")) + TEXT("\r\n");
			for (auto& CurSkill : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
			{
				CurStr += FALD(TEXT("【")) + FAL(CurSkill.M_Name, TEXT("c")) + FALD(TEXT("】")) + TEXT("\r\n");
			}

			this->GetSystem()->AddCurrent(
				CurStr
			);

			return false;
		}
	}


	//必须是两个指令
	if (Instruction.Num() > 1)
	{
		if (this->GetSystem()->InstructionTest(Instruction[0], TEXT("-all")))
		{
			int32 UniGrade = FCString::Atoi(*Instruction[1]);

			for (auto& CurSkill : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
			{
				CurSkill.M_LV = UniGrade;
				if (CurSkill.M_LV >= 9)
				{
					CurSkill.M_LV = 9;
				}
				else if (CurSkill.M_LV <= 0)
				{
					CurSkill.M_LV = 0;
				}
			}

			UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("设置技能书等级操作")));

			return true;
		}

		FString CurName = Instruction[0];
		for (auto& CurSkill : UFVMGameInstance::GetPlayerStructManager_Static()->M_SkillBookDatas)
		{
			if (CurSkill.M_Name.Equals(CurName))
			{
				int32 UniGrade = FCString::Atoi(*Instruction[1]);
				CurSkill.M_LV = UniGrade;
				if (CurSkill.M_LV >= 9)
				{
					CurSkill.M_LV = 9;
				}
				else if (CurSkill.M_LV <= 0)
				{
					CurSkill.M_LV = 0;
				}

				return true;
			}
		}
	}

	return false;
}

bool UPlayerInstructions::SetPGIns(const TArray<FString>& Instruction)
{
	if (Instruction.Num() > 0)
	{
		int32 Grade = FCString::Atoi(*Instruction[0]);
		if (Grade <= 0)
		{
			Grade = 1;
		}
		else if (Grade >= 60)
		{
			Grade = 60;
		}

		UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerEx.M_PlayerGrade = Grade;

		return true;
	}

	return false;
}

bool UPlayerInstructions::SetCoinIns(const TArray<FString>& Instruction)
{
	if (Instruction.Num() > 1)
	{
		int32 Num = FCString::Atoi(*Instruction[1]);

		UFVMGameInstance::GetPlayerStructManager_Static()->SetCoin(Instruction[0], Num);

		return true;
	}

	return false;
}

bool UPlayerInstructions::ReturnIns(const TArray<FString>& Instruction)
{
	//执行设置技能书指令
	if (this->ExecuteInsHead(Instruction, TEXT("dia"), std::bind(&UPlayerInstructions::ReturnDialogue, this, std::placeholders::_1)))
	{
		return true;
	}

	return false;
}

bool UPlayerInstructions::ReturnDialogue(const TArray<FString>& Instruction)
{
	if (IsValid(UFVMGameInstance::GetPlayerStructManager_Static()))
	{
		if (UFVMGameInstance::GetPlayerStructManager_Static()->PlayerLastDialogueID > 0)
		{
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerLastDialogueID -= 1;
			UFVMGameInstance::GetPlayerStructManager_Static()->PlayerCurrentDialogueID =
				UFVMGameInstance::GetPlayerStructManager_Static()->PlayerLastDialogueID;

			UGameSystemFunction::AddGameLog(UFVMGameInstance::GetFVMGameInstance(), FALD(TEXT("剧情已回退！请前往查看进度！")));

			UFVMGameInstance::GetPlayerStructManager_Static()->Save(__FUNCTION__ + FString(TEXT("剧情回退操作")));
		}
	}

	return true;
}
