// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GameConfigSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/FVMGameInstance.h"
#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>

UGameConfigSubsystem* UGameConfigSubsystem::GetGameConfigSubsystemStatic()
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetSubsystem<UGameConfigSubsystem>();
	}

	return nullptr;
}

UGameConfigManager* UGameConfigSubsystem::GetGameConfigInstance()
{
	return this->CurrentGameConfigManager;
}

void UGameConfigSubsystem::SaveLocalGameConfigInstance()
{
	UGameplayStatics::SaveGameToSlot(this->CurrentGameConfigManager, TEXT("GameDataConfig_GCG"), 0);
}

void UGameConfigSubsystem::PlayGameBGM(const FSoftObjectPath& MusicName, bool bLoop /*= true*/)
{
	//如果组件存在那么先停止播放
	if (this->CurrentAudioBGMComponent)
	{
		this->CurrentAudioBGMComponent->Stop();
	}

	if (MusicName.ToString().Equals(""))
	{
		return;
	}

	USoundBase* Sound = LoadObject<USoundBase>(0, *MusicName.ToString());

	if (!Sound)
	{
		return;
	}

	this->CurrentPlayAudioBGMPath = MusicName;

	Cast<USoundWave>(Sound)->bLooping = bLoop;

	this->CurrentAudioBGMComponent = UGameplayStatics::SpawnSound2D(
		this, Sound, this->CurrentGameConfigManager->M_FMusic.M_BGM_Value, 1.f, 0.0f, 0, false, false
	);

	this->UpdateGameBGMVolume();
}

void UGameConfigSubsystem::SetGameBGMVolume(float Value)
{
	if (this->CurrentAudioBGMComponent)
	{
		CurrentAudioBGMComponent->SetVolumeMultiplier(
			this->CurrentGameConfigManager->M_FMusic.M_BGM_Value * Value
		);
	}
}

FSoftObjectPath UGameConfigSubsystem::GetCurrentPlayGameBgmPath()
{
	return this->CurrentPlayAudioBGMPath;
}

void UGameConfigSubsystem::UpdateGameBGMVolume()
{
	//将BMG本身的Volume * 指定Volume大小   [  0 - 1 * BGM(size)  ]
	if (this->CurrentAudioBGMComponent && this->CurrentGameConfigManager)
	{
		this->CurrentAudioBGMComponent->SetVolumeMultiplier(this->CurrentGameConfigManager->M_FMusic.M_BGM_Value);
	}
}

void UGameConfigSubsystem::PlayGameSoundEffects(const FString& SoundEffectName, const FString& SoundEffectPathName /*= FString(TEXT("ItemAudio") )*/)
{
	if (!this->CurrentGameConfigManager)
	{
		return;
	}

	if (SoundEffectName.Equals(""))
	{
		return;
	}

	const FString& LSoundPath =
		FString(TEXT("SoundWave'/Game/Resource/BP/Audio/")) +
		SoundEffectPathName + ("/") +
		SoundEffectName +
		FString(TEXT(".")) +
		SoundEffectName + FString(TEXT("'"));

	USoundBase* Sound = LoadObject<USoundBase>(0, *LSoundPath);

	if (!Sound)
		return;

	Cast<USoundWave>(Sound)->bLooping = false;

	UGameplayStatics::PlaySound2D(this, Sound, this->CurrentGameConfigManager->M_FMusic.M_BGM_S_Value);
}

float UGameConfigSubsystem::GetGameSoundEffectsVolume()
{
	return CurrentGameConfigManager->M_FMusic.M_BGM_S_Value;
}

void UGameConfigSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//判断配置文件是否存在
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GameDataConfig_GCG"), 0))
	{
		//加载并且返回
		this->CurrentGameConfigManager =
			Cast<UGameConfigManager>(
				UGameplayStatics::LoadGameFromSlot(TEXT("GameDataConfig_GCG"), 0)
				);

		return;
	}

	//文件不存在创建SaveGame实例
	this->CurrentGameConfigManager =
		Cast<UGameConfigManager>(
			UGameplayStatics::CreateSaveGameObject(
				LoadClass<UGameConfigManager>(0, TEXT("Class'/Script/FVM.GameConfigManager'"))
			));

	//设置游戏FPS配置
	if (UGameUserSettings* LUserSetting = GEngine->GetGameUserSettings())
	{
		LUserSetting->SetFrameRateLimit(this->CurrentGameConfigManager->M_GameFPS.M_FPS_Max);
		LUserSetting->ApplySettings(true);
	}
	//保存
	UGameplayStatics::SaveGameToSlot(this->CurrentGameConfigManager, TEXT("GameDataConfig_GCG"), 0);
}

void UGameConfigSubsystem::Deinitialize()
{
	Super::Deinitialize();

	//保存配置
	UGameplayStatics::SaveGameToSlot(this->CurrentGameConfigManager, TEXT("GameDataConfig_GCG"), 0);
}
