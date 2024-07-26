// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/PlayerUpdate/PlayerSkillBooksUpdate.h"

void UPlayerSkillBooksUpdate::Update_Implementation(class UPlayerStructManager* PlayerDataIns)
{
	//获取存档
	UPlayerStructManager* PlayerData = PlayerDataIns;

	//加载技能书数据表(缺少多少项添加多少项)
	TArray<FSkillBookData_Data*> Datas;
	UGameSystemFunction::GetDataTableRows<FSkillBookData_Data>(
		TEXT("DataTable'/Game/Resource/BP/Data/CardSkillBookData/CardSkillBookData.CardSkillBookData'"),
		Datas);

	//判断背包中的技能书个数是否匹配数据表个数
	if (PlayerData->M_SkillBookDatas.Num() != Datas.Num())
	{
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Warning, TEXT("技能书数量不匹配->更新技能书"));
		}

		int32 Items = Datas.Num();
		for (int32 i = PlayerData->M_SkillBookDatas.Num(); i < Items; i++)
		{
			PlayerData->M_SkillBookDatas.Emplace(Datas[i]->M_FSkillBookData);
		}

		//更新数据
		int32 LNums = PlayerData->M_SkillBookDatas.Num();
		for (int32 i = 0; i < LNums; i++)
		{
			//保存等级和当前经验值
			int32 Level = PlayerData->M_SkillBookDatas[i].M_LV;
			int32 CurrentEx = PlayerData->M_SkillBookDatas[i].M_CurrentEx;
			int32 CurrentTotalEx = PlayerData->M_SkillBookDatas[i].M_CurrentTotalEx;
			//重新更新数据
			PlayerData->M_SkillBookDatas[i] = Datas[i]->M_FSkillBookData;
			//重新赋予数据值
			PlayerData->M_SkillBookDatas[i].M_LV = Level;
			PlayerData->M_SkillBookDatas[i].M_CurrentEx = CurrentEx;
			PlayerData->M_SkillBookDatas[i].M_CurrentTotalEx = CurrentTotalEx;
		}

		if (UFVMGameInstance::GetDebug() && PlayerData->M_SkillBookDatas.Num() == Datas.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("技能书更新完成"));
		}
	}
}
