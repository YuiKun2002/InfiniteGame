// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameLevelSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameStart/VS/GameMapInstance.h"
UGameLevelSubsystem* UGameLevelSubsystem::GetGameLevelSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		if (IsValid(UFVMGameInstance::GetFVMGameInstance()->GetWorld()))
		{
			return UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetSubsystem<
				UGameLevelSubsystem
			>();
		}
	}

	return nullptr;
}

void UGameLevelSubsystem::SetGameMapInstance(AGameMapInstance* NewGameMapInstance)
{
	this->GameMapInstance = NewGameMapInstance;
}

AGameMapInstance* UGameLevelSubsystem::GetGameMapInstance()
{
	return this->GameMapInstance;
}
