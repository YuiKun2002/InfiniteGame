// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameConfigSubsystem.generated.h"

class UGameConfigManager;
class UAudioComponent;

/**
 * 游戏配置子系统
 */
UCLASS()
class FVM_API UGameConfigSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//获取游戏配置子系统
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统")
		static UGameConfigSubsystem* GetGameConfigSubsystemStatic();

	//获取游戏配置
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 配置")
		UGameConfigManager* GetGameConfigInstance();
	//保存游戏配置到本地
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 配置")
		void SaveLocalGameConfigInstance();

	//播放背景音乐
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		void PlayGameBGM(const FSoftObjectPath& MusicName, bool bLoop = true);
	//更新背景音乐音量
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		void UpdateGameBGMVolume();
	//设置背景音乐的音量大小
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		void SetGameBGMVolume(float Value);
	//获取当前正在播放的背景音乐路径
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		FSoftObjectPath GetCurrentPlayGameBgmPath();

	//播放音效
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		void PlayGameSoundEffects(
			const FString& SoundEffectName,
			const FString& SoundEffectPathName = FString(TEXT("ItemAudio")
			));
	//获取音效的音量
	UFUNCTION(BlueprintCallable, Category = "游戏配置子系统 | 音频")
		float GetGameSoundEffectsVolume();

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
private:
	//当前的游戏配置
	UPROPERTY()
		UGameConfigManager* CurrentGameConfigManager = nullptr;
	//音频管理背景音乐
	UPROPERTY()
		UAudioComponent* CurrentAudioBGMComponent = nullptr;
	//当前正在播放的背景音乐路径
	UPROPERTY()
		FSoftObjectPath CurrentPlayAudioBGMPath;
	//音频管理背景音效
	UPROPERTY()
		UAudioComponent* CurrentAudioSoundEffectsComponent = nullptr;
};
