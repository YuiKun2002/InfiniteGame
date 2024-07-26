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

	this->M_SlotParent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//------------------------------------------------------------
	//角色套装插槽
	this->M_SlotParentChild_2 = CreateDefaultSubobject<USceneComponent>(TEXT("ParentChild_2_Suit"));
	//------------------------------------------------------------
	//角色基础服装插槽
	this->M_SlotParentChild_1 = CreateDefaultSubobject<USceneComponent>(TEXT("ParentChild_1_BaseSuit"));

	this->M_Hair = CreateDefaultSubobject<USceneComponent>(TEXT("Hair"));
	this->M_Glasses = CreateDefaultSubobject<USceneComponent>(TEXT("Glasses"));
	this->M_Face = CreateDefaultSubobject<USceneComponent>(TEXT("Face"));
	this->M_Eye = CreateDefaultSubobject<USceneComponent>(TEXT("Eye"));
	this->M_Head = CreateDefaultSubobject<USceneComponent>(TEXT("Head"));
	this->M_Body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	this->M_HairBack = CreateDefaultSubobject<USceneComponent>(TEXT("HairBack"));
	this->M_FlyItem = CreateDefaultSubobject<USceneComponent>(TEXT("FlyItem"));
	this->M_Cap = CreateDefaultSubobject<USceneComponent>(TEXT("Cap"));


	//依附
	this->SetRootComponent(this->M_SlotParent);
	//------------------------------------------------------------
	this->M_SlotParentChild_1->SetupAttachment(this->M_SlotParent);
	this->M_SlotParentChild_2->SetupAttachment(this->M_SlotParent);
	this->M_FlyItem->SetupAttachment(this->M_SlotParent);

	this->M_HairBack->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Body->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Head->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Eye->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Face->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Glasses->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Hair->SetupAttachment(this->M_SlotParentChild_1);
	this->M_Cap->SetupAttachment(this->M_SlotParentChild_1);


	//角色基础着装动画组件创建
	this->M_FlyItemAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("FlyItemAnim"));
	this->M_HairBackAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("HairBackAnim"));
	this->M_BodyAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("BodyAnim"));
	this->M_HeadAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("HeadAnim "));
	this->M_EyeAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("EyeAnim"));
	this->M_FaceAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("FaceAnim"));
	this->M_GlassesAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("GlassesAnim"));
	this->M_HairAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("HairAnim"));
	this->M_CapAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("CapAnim"));
	//角色服装动画组件创建
	this->M_SuitAnim = CreateDefaultSubobject<UFlipbookBaseComponent>(TEXT("SuitAnim"));

	//基础着装组件依附
	this->M_FlyItemAnim->SetupAttachment(this->M_FlyItem);
	this->M_HairBackAnim->SetupAttachment(this->M_HairBack);
	this->M_BodyAnim->SetupAttachment(this->M_Body);
	this->M_HeadAnim->SetupAttachment(this->M_Head);
	this->M_EyeAnim->SetupAttachment(this->M_Eye);
	this->M_FaceAnim->SetupAttachment(this->M_Face);
	this->M_GlassesAnim->SetupAttachment(this->M_Glasses);
	this->M_HairAnim->SetupAttachment(this->M_Hair);
	this->M_CapAnim->SetupAttachment(this->M_Cap);
	//套装组件依附
	this->M_SuitAnim->SetupAttachment(this->M_SlotParentChild_2);
}

// Called when the game starts or when spawned
void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();

	//默认取消所有动画
	this->M_FlyItemAnim->SetFlipbook(nullptr);
	this->M_HairBackAnim->SetFlipbook(nullptr);
	this->M_BodyAnim->SetFlipbook(nullptr);
	this->M_HeadAnim->SetFlipbook(nullptr);
	this->M_EyeAnim->SetFlipbook(nullptr);
	this->M_FaceAnim->SetFlipbook(nullptr);
	this->M_GlassesAnim->SetFlipbook(nullptr);
	this->M_HairAnim->SetFlipbook(nullptr);
	this->M_CapAnim->SetFlipbook(nullptr);
	this->M_SuitAnim->SetFlipbook(nullptr);
}

// Called every frame
void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePlayerBase::InitPlayerData()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	//区别套装和角色基础着装
	if (PlayerData->M_FPlayerSuit.M_bPlayerSuit && PlayerData->M_FPlayerSuit.M_ShowPlayerSuit)
	{
		//加载翅膀
		if (!PlayerData->M_FPlayerSuit.M_HiddenFlyItem)
		{
			this->LoadAnimation(
				PlayerData->M_FPlayerSuit.M_FlyItem,
				true,
				this->M_FlyItemAnim,
				this->M_Anim_Suit_FlyItemDef,
				this->M_Anim_Suit_FlyItemAttack,
				EPlayerEquipmentSlotPosition::E_Player_FlyItem
			);
		}
		//加载套装
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Suit,
			true,
			this->M_SuitAnim,
			this->M_Anim_Suit_Def,
			this->M_Anim_Suit_Attack,
			EPlayerEquipmentSlotPosition::E_Player_Suit
		);
	}
	else {
		//加载翅膀
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_FlyItem,
			true,
			this->M_FlyItemAnim,
			this->M_Anim_Suit_FlyItemDef,
			this->M_Anim_Suit_FlyItemAttack,
			EPlayerEquipmentSlotPosition::E_Player_FlyItem
		);

		//加载头发-后
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Hair,
			false,
			this->M_HairBackAnim,
			this->M_Anim_BaseSuit_HairBackDef,
			this->M_Anim_BaseSuit_HairBackAttack,
			EPlayerEquipmentSlotPosition::E_Player_Hair
		);

		//加载着装
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Body,
			true,
			this->M_BodyAnim,
			this->M_Anim_BaseSuit_BodyDef,
			this->M_Anim_BaseSuit_BodyAttack,
			EPlayerEquipmentSlotPosition::E_Player_Body
		);

		//加载头部->裸头
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Head,
			true,
			this->M_HeadAnim,
			this->M_Anim_BaseSuit_HeadDef,
			this->M_Anim_BaseSuit_HeadAttack,
			EPlayerEquipmentSlotPosition::E_Player_Head
		);

		//加载眼睛
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Eye,
			true,
			this->M_EyeAnim,
			this->M_Anim_BaseSuit_EyeDef,
			this->M_Anim_BaseSuit_EyeAttack,
			EPlayerEquipmentSlotPosition::E_Player_Eye
		);

		//加载脸饰
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Face,
			true,
			this->M_FaceAnim,
			this->M_Anim_BaseSuit_FaceDef,
			this->M_Anim_BaseSuit_FaceAttack,
			EPlayerEquipmentSlotPosition::E_Player_Face
		);

		//加载眼镜
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Glasses,
			true,
			this->M_GlassesAnim,
			this->M_Anim_BaseSuit_GlassesDef,
			this->M_Anim_BaseSuit_GlassesAttack,
			EPlayerEquipmentSlotPosition::E_Player_Glasses
		);

		//加载头发-前
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Hair,
			true,
			this->M_HairAnim,
			this->M_Anim_BaseSuit_HairDef,
			this->M_Anim_BaseSuit_HairAttack,
			EPlayerEquipmentSlotPosition::E_Player_Hair
		);

		//加载帽子
		this->LoadAnimation(
			PlayerData->M_FPlayerSuit.M_Cap,
			true,
			this->M_CapAnim,
			this->M_Anim_BaseSuit_CapDef,
			this->M_Anim_BaseSuit_CapAttack,
			EPlayerEquipmentSlotPosition::E_Player_Cap
		);
	}
}

