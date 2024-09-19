// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/GamePlayerBase.h"
#include "GameStart/Flipbook/Components/FlipbookBaseComponent.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameSystem/FVMGameInstance.h"

void AGamePlayerBase::LoadAnimation(const FPlayerSuitItem& _Value, bool _bUseFirst, UFlipbookBaseComponent* _Position, UPaperFlipbook*& _Def, UPaperFlipbook*& _Attack, EPlayerEquipmentSlotPosition _Type)
{
	if (_Value.M_bUse)
	{
		_Def = LoadObject<UPaperFlipbook>(
			0, _bUseFirst ? *_Value.M_SuitFirstDefAnimPath.ToString() : *_Value.M_SuitSecondDefAnim2Path.ToString()
			);

		_Attack = LoadObject<UPaperFlipbook>(
			0, _bUseFirst ? *_Value.M_SuitFirstAttackAnimPath.ToString() : *_Value.M_SuitSecondAttackAnim2Path.ToString()
			);

		if (!_Def || !_Attack)
		{
			_Position->SetHiddenInGame(true);
		}
		else {
			_Position->SetPlayAnimation(_Def);
			_Position->SetRelativeLocation(
				_Position->GetRelativeLocation() + FVector(_Value.M_PlayerEq_Offset.X, 0.f, _Value.M_PlayerEq_Offset.Y)
			);
		}

	}
	else {

		UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

		//使用默认服装
		switch (_Type)
		{
		case EPlayerEquipmentSlotPosition::E_Player_Hair:
		{
			if (PlayerData->M_PlayerSex == 0)
			{
				_Def = LoadObject<UPaperFlipbook>(0, _bUseFirst ? TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenHair_Front_Def.FB_PlayerMenHair_Front_Def'") : TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenHair_Back_Def.FB_PlayerMenHair_Back_Def'"));
				_Attack = LoadObject<UPaperFlipbook>(0, _bUseFirst ? TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenHair_Front_Attack.FB_PlayerMenHair_Front_Attack'") : TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenHair_Back_Attack.FB_PlayerMenHair_Back_Attack'"));
			}
			else {
				_Def = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_ManBaseHair_Def.FB_ManBaseHair_Def'"));
				_Attack = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_ManBaseHair_Attack.FB_ManBaseHair_Attack'"));
			}
		}
		break;
		case EPlayerEquipmentSlotPosition::E_Player_Body:
		{
			_Def = LoadObject<UPaperFlipbook>(0,
				TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_BaseBody_Def_Anim.FB_BaseBody_Def_Anim'"));
			_Attack = LoadObject<UPaperFlipbook>(0,
				TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_BaseBody_Attack_Anim.FB_BaseBody_Attack_Anim'"));
		}
		break;
		case EPlayerEquipmentSlotPosition::E_Player_Head:
		{
			_Def = LoadObject<UPaperFlipbook>(0,
				TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerHead_Def.FB_PlayerHead_Def'"));
			_Attack = LoadObject<UPaperFlipbook>(0,
				TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerHead_Attack.FB_PlayerHead_Attack'"));
		}
		break;
		case EPlayerEquipmentSlotPosition::E_Player_Eye:
		{
			if (PlayerData->M_PlayerSex == 0)
			{
				_Def = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenEye_Def.FB_PlayerMenEye_Def'"));
				_Attack = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerMenEye_Attack.FB_PlayerMenEye_Attack'"));
			}
			else
			{
				_Def = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerManEye_Def.FB_PlayerManEye_Def'"));
				_Attack = LoadObject<UPaperFlipbook>(0,
					TEXT("PaperFlipbook'/Game/Resource/Texture/PlayerItems/Player_Mod/Base/Anim/FB_PlayerManEye_Attack.FB_PlayerManEye_Attack'"));
			}
		}
		break;
		default:
			_Position->SetFlipbook(nullptr);
			break;
		}

		if (_Def && _Attack)
		{
			_Position->SetFlipbook(_Def);
		}
	}
}

// Sets default values
AGamePlayerBase::AGamePlayerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePlayerBase::InitPlayerData()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();



}

