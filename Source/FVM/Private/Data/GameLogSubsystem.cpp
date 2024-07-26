// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/GameLogSubsystem.h"
#include "UI/WidgetBase.h"
#include "Data/Instructions/PlayerInstructions.h"
#include "Data/Instructions/GameInstructions.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

void UGameLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	this->AddPrivate(FALD(TEXT("游戏测试版本信息面板：")));
}

void UGameLogSubsystem::Deinitialize()
{
}

void UGameLogSubsystem::AddCurrent(const FString& Content)
{
	if (this->CurrentListConetnt.Num() > 100)
	{
		TArray<FString> Temp;
		for (int32 i = this->CurrentListConetnt.Num() - 6; i < this->CurrentListConetnt.Num(); i++)
		{
			Temp.Emplace(this->CurrentListConetnt[i]);
		}
		this->CurrentListConetnt.Empty();
		this->CurrentListConetnt = Temp;
	}

	this->CurrentListConetnt.Emplace(Content);
}

void UGameLogSubsystem::GetCurrent(TArray<FString>& Contents)
{
	Contents = this->CurrentListConetnt;
}

int32 UGameLogSubsystem::GetCurrentCount()
{
	return this->CurrentListConetnt.Num();
}

void UGameLogSubsystem::AddPrivate(const FString& Content)
{
	if (this->PrivateListConetnt.Num() > 6000)
	{
		this->PrivateListConetnt.Empty();
	}

	this->PrivateListConetnt.Emplace(Content);
}

void UGameLogSubsystem::GetPrivate(TArray<FString>& Contents)
{
	Contents = this->PrivateListConetnt;
}

int32 UGameLogSubsystem::GetPrivateCount()
{
	return this->PrivateListConetnt.Num();
}

bool UGameLogSubsystem::ExeInstructions(const FString& Content, bool bTip)
{
	FString CurCo = Content;
	if (!CurCo.Left(1).Equals(TEXT("/")))
	{
		return false;
	}

	//移除 /
	FString CurText = Content;
	CurText.RemoveFromStart(TEXT("/"));

	//拆分指令
	TArray<FString> Ins;
	CurText.ParseIntoArray(Ins, TEXT(" "), false);

	//如果没有信息则退出
	if (Ins.Num() == 0)
	{
		this->AddCurrent(
			FAL(TEXT("请输入 -h 或者 -help 来查看所有指令"), TEXT("c"))
		);

		return false;
	}

	//执行帮助命令
	if (UGameLogSubsystem::AddHelpInstructionAndPrint(
		Ins[0],
		TEXT("DataTable'/Game/Resource/BP/Data/Instructions/All_Instruction.All_Instruction'")
	))
	{
		return false;
	}

	if (Ins[0].ToLower().Equals(TEXT("clear")))
	{
		this->CurrentListConetnt.Empty();
		this->PrivateListConetnt.Empty();
		return false;
	}

	//指令对象
	UObject* CurInstructions = nullptr;

	//FString Head = 邮件
	GenInstruction<UMailInstructions>(Ins[0], TEXT("mail"), CurInstructions);
	GenInstruction<UPlayerInstructions>(Ins[0], TEXT("player"), CurInstructions);
	GenInstruction<UGameInstructions>(Ins[0], TEXT("game"), CurInstructions);


	//执行命令
	if (IsValid(CurInstructions))
	{
		//移除头部信息
		Ins.RemoveAt(0);
		if (Cast<ILogInstructionsInterface>(CurInstructions)->ReadInstruction(Ins))
		{

			if (bTip)
			{
				UWidgetBase::CreateTipWidget(TEXT("指令执行成功"));
			}

			return true;
		}
	}

	return false;
}

FString UGameLogSubsystem::GetInstructions(const FString& DataTablePath)
{
	TArray<FInstructionsDataTable*> AllList;
	UGameSystemFunction::GetDataTableRows(DataTablePath, AllList);
	FString CurText = TEXT("<t>指令列表：</>\r\n");
	for (const FInstructionsDataTable* const CurRow : AllList)
	{
		FString CurDes = CurRow->Des.Replace(TEXT(";"), TEXT("</>\r\n<t>"));

		FString A = CurDes.Replace(TEXT("【"), TEXT("</><g>【</><t>"));
		FString B = A.Replace(TEXT("】"), TEXT("</><g>】</><t>"));
		FString C = B.Replace(TEXT("param"), TEXT("</><c>param</><t>"));

		CurText += FAL(TEXT("---【"), TEXT("g")) + FAL(CurRow->InstructionName, TEXT("g")) + FAL(TEXT("】---"), TEXT("g")) + FString(TEXT("\r\n")) +
			FALD(C) + FString(TEXT("\r\n"));
	}

	return CurText;
}

bool UGameLogSubsystem::AddHelpInstruction(const FString& Instruction)
{
	if (Instruction.ToLower().Equals(TEXT("-help")) || Instruction.ToLower().Equals(TEXT("-h")))
	{
		return true;
	}

	return false;
}

UGameLogSubsystem* UGameLogSubsystem::GetGameLogSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UGameLogSubsystem>();
	}

	return nullptr;
}

bool UGameLogSubsystem::AddHelpInstructionAndPrint(const FString& Instruction, const FString& DataTablePath)
{
	if (UGameLogSubsystem::GetGameLogSubsystemStatic()->AddHelpInstruction(Instruction))
	{
		UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
			UGameLogSubsystem::GetGameLogSubsystemStatic()->GetInstructions(DataTablePath)
		);

		return true;
	}

	return false;
}

bool UGameLogSubsystem::InstructionTest(const FString& Instruction, const FString& TestInstruction)
{
	return Instruction.ToLower().Equals(TestInstruction);
}

void UGameLogSubsystem::AddPlayerGetEquipmentLog(const FString& ItemName, int32 Count, int32 Grade)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		if (Grade == 0)
		{
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
				FALD(TEXT("【公告】说：玩家【")) + FAL(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName,
					TEXT("player")) + FALD(TEXT("】获得道具【")) + FAL(ItemName, TEXT("c")) + FALD(TEXT("】")) +
				FAL(FString(TEXT("*") + FString::FromInt(Count)), TEXT("g"))
			);
		}
		else {
			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
				FALD(TEXT("【公告】说：玩家【")) + FAL(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName,
					TEXT("player")) + FALD(TEXT("】获得")) +
				FAL(FString(FString::FromInt(Grade) + TEXT("级")), TEXT("g")) +
				FALD(TEXT("道具【")) + FAL(ItemName, TEXT("c")) + FALD(TEXT("】")) +
				FAL(FString(TEXT("*") + FString::FromInt(Count)), TEXT("g"))
			);
		}
	}
}

void UGameLogSubsystem::AddPlayerGetCardLog(const FString& ItemName, int32 Grade /*= 0*/)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
			FALD(TEXT("【公告】说：玩家【")) + FAL(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName,
				TEXT("player")) + FALD(TEXT("】获得一张")) +
			FAL(FString(FString::FromInt(Grade) + TEXT("星")), TEXT("g")) +
			FALD(TEXT("[")) + FAL(ItemName, TEXT("c")) + FALD(TEXT("]卡片"))
		);
	}
}

void UGameLogSubsystem::AddPlayerGetMaterialLog(const FString& ItemName, int32 Count /*= 0*/)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(
			FALD(TEXT("【公告】说：玩家【")) + FAL(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName,
				TEXT("player")) + FALD(TEXT("】获得材料【")) + FAL(ItemName, TEXT("c")) + FALD(TEXT("】")) +
			FAL(FString(TEXT("*") + FString::FromInt(Count)), TEXT("g"))
		);
	}
}

bool UMailInstructions::ReadInstruction(const TArray<FString>& Instruction)
{
	if (Instruction.Num() > 0)
	{
		if (UGameLogSubsystem::AddHelpInstructionAndPrint(
			Instruction[0],
			TEXT("DataTable'/Game/Resource/BP/Data/Instructions/Mail_Instruction.Mail_Instruction'")
		))
		{
			return false;
		}

		if (!IsValid(UFVMGameInstance::GetFVMGameInstance()))
		{
			return false;
		}

		if (!IsValid(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()))
		{
			return false;
		}

		//获取全部邮箱内容
		if (UGameLogSubsystem::InstructionTest(Instruction[0], TEXT("-get")))
		{
			for (const FMail& CurM : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails)
			{
				//背包寻找数据
				FEquipmentBase* EquipmentData = UItemBaseStruct::GetItemFromBag(
					UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment,
					CurM.M_MailSendItemName
				);

				FEquipmentBase Data;
				//从数据表提前数据
				if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(
					CurM.M_MailSendItemName, Data, true, EEquipment::E_Gift)
					)
				{
					//发送给背包
					if (EquipmentData)
					{
						EquipmentData->M_Count += CurM.M_MailItemCount;
					}
					else {
						Data.M_Count = CurM.M_MailItemCount;
						//申请ID号
						Data.M_ItemID = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetEquipID();
						UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment.Emplace(
							Data
						);
					}

					UGameLogSubsystem::AddPlayerGetEquipmentLog(CurM.M_MailSendItemName, CurM.M_MailItemCount);
				}
			}

			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails.Empty();

			UGameSystemFunction::SaveCurrentPlayerData();

			return true;
		}

		//获取全部邮箱内容
		if (UGameLogSubsystem::InstructionTest(Instruction[0], TEXT("-list")))
		{
			//初始化指令大全
			TArray<FMail> Mail_;
			UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_FMail_Data, FMail>(
				UGlobalDatas::Global_SourceMailData_Mail, Mail_
				);

			FString LContent;
			LContent += FALD(TEXT("邮件所有的道具名称：")) + TEXT("\r\n");
			for (const auto& _Data_Name : Mail_)
			{
				LContent += FAL(_Data_Name.M_MailTitle, TEXT("c")) + TEXT("\r\n");
			}

			UGameLogSubsystem::GetGameLogSubsystemStatic()->AddCurrent(LContent);

			return false;
		}
	}


	bool b = false;
	for (const FString& Cur : Instruction)
	{
		FMail _MailData;

		TArray<FMail> Mail_;

		UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_FMail_Data, FMail>(
			UGlobalDatas::Global_SourceMailData_Mail, Mail_
			);

		if (UEquipmentBaseStruct::GetMailArraysData(Cur, Mail_, _MailData))
		{
			b = true;
			UGameSystemFunction::SendMailToCurrentPlayer(_MailData);
		}
	}

	if (b)
	{
		//保存数据
		UGameSystemFunction::SaveCurrentPlayerData();
		return true;
	}

	return false;
}

bool ULogInstructionSystemBase::ReadInstruction(const TArray<FString>& Instruction)
{
	return false;
}

UGameLogSubsystem* ULogInstructionSystemBase::GetSystem()
{
	return UGameLogSubsystem::GetGameLogSubsystemStatic();
}

void ULogInstructionSystemBase::SetInstructions(const FString& InstructionsPath)
{
	this->CurData.Empty();
	UGameSystemFunction::GetDataTableRows(InstructionsPath, this->CurData);
}

FString ULogInstructionSystemBase::GetInstructionDes(const FString& Instruction)
{
	for (const FInstructionsDataTable& Cur : this->CurData)
	{
		if (Cur.InstructionName.Equals(Instruction))
		{
			return Cur.Des;
		}
	}

	return TEXT("");
}

bool ULogInstructionSystemBase::PrintHelpInstruction(const FString& HeadIns, const TArray<FString>& Instruction)
{
	int32 CurIndex = Instruction.Find(TEXT("-h"));
	if (CurIndex >= 0)
	{
		FString CurIns = HeadIns + TEXT(" ");
		for (int32 i = 0; i <= CurIndex; i++)
		{
			if (i == CurIndex)
			{
				CurIns += Instruction[i];
			}
			else {
				CurIns += Instruction[i] + TEXT(" ");
			}
		}

		FString TargetIns = this->GetInstructionDes(CurIns);
		if (TargetIns.Equals(""))
		{
			return false;
		}
		else {

			FString Cur = TargetIns.Replace(TEXT(";"), TEXT("</>\r\n<t>"));
			FString A = Cur.Replace(TEXT("【"), TEXT("</><g>【</><t>"));
			FString B = A.Replace(TEXT("】"), TEXT("</><g>】</><t>"));
			FString C = B.Replace(TEXT("param"), TEXT("</><c>param</><t>"));
			this->GetSystem()->AddCurrent(FALD(C));

			return true;
		}
	}

	return false;
}

bool ULogInstructionSystemBase::ExecuteInsHead(const TArray<FString>& Instruction, const FString& TargetIns, TFunction<bool(const TArray<FString>&)> ExeFunc)
{
	if (Instruction.Num() > 0)
	{
		//发送指令
		if (this->GetSystem()->InstructionTest(Instruction[0], TargetIns))
		{
			TArray<FString> CurIns = Instruction;
			CurIns.RemoveAt(0);
			return ExeFunc(CurIns);
		}
	}

	return false;
}
