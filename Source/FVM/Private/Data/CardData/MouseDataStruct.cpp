// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/MouseStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"



bool UMouseDataStruct::SetMouseResDatas(TMap<FString, FString>& _MouseMapRes)
{
	//获取老鼠数据表
	TArray<FMouse_Data*> Datas;
	UGameSystemFunction::GetDataTableRows<FMouse_Data>(
		TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"),
		Datas);

	//默认是老鼠资源
	FString DefResPath = FString();

	//设置默认老鼠资源
	if (Datas.Num() > 0)
	{
		DefResPath = Datas[0]->M_Mouse.ItemTarget_ActorFilePath.ToString();
	}
	else
	{
		return false;
	}


	for (auto MouseResPP = _MouseMapRes.CreateIterator(); MouseResPP; ++MouseResPP)
	{
		//赋值老鼠资源
		for (auto Mouse = Datas.CreateConstIterator(); Mouse; ++Mouse)
		{
			//如果匹配赋值人很好跳出一次
			if (MouseResPP.Key().Equals((*Mouse)->M_Mouse.M_MouseName))
			{
				MouseResPP.Value() = (*Mouse)->M_Mouse.ItemTarget_ActorFilePath.ToString();
				break;
			}
		}
	}

	//最后寻找一次(如果有空value值则使用默认的老鼠资源)
	for (auto MouseResPP = _MouseMapRes.CreateIterator(); MouseResPP; ++MouseResPP)
	{
		if (MouseResPP.Key().Equals(TEXT("")))
		{
			MouseResPP.Value() = DefResPath;
		}
	}

	return true;
}