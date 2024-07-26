// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Task/Inter/TaskInterface.h"

bool UTaskFunction::ExeTask(const FString& TaskName, UObject* TargetObject)
{
	return false;
}

UTaskFunction* UTaskFunction::MakeNewClass()
{
	return NewObject<UTaskFunction>();
}
