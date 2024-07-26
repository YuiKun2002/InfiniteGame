// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/Task/Inter/TaskInterface.h"


const FString& UTaskStructBase::GetTaskName() const
{
	return this->Data.TaskName;
}

const int32& UTaskStructBase::GetTaskID() const
{
	return this->Data.TaskID;
}

const EGameTaskType& UTaskStructBase::GetTaskType() const
{
	return this->TaskType;
}

const FTaskBase& UTaskStructBase::GetData() const
{
	return this->Data;
}

void UTaskFuncObjectExe::OnInit()
{
	this->OnExe();
}
