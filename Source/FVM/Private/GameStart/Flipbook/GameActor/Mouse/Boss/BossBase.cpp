// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/UI/UI_MouseRound.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>


FLine UBossUniformState::GetLine()
{
	return this->Get()->GetMouseLine();
}

AMapMeshe* UBossUniformState::GetMeshe(int32 Row, int32 Col)
{
	return this->Get()->GetMesheComp()->GetMapMesh(Row, Col);
}

FVector UBossUniformState::GetMesheLocation(int32 Row, int32 Col)
{
	return this->Get()->GetMesheComp()->GetMapMeshLocation(Row, Col);
}

FLine UBossUniformState::GetRowAndCol()
{
	return this->Get()->GetMesheComp()->GetMapMeshRowAndCol();
}

int32 UBossUniformState::GetLevel()
{
	return this->Get()->GetMouseLevel();
}

void UBossUniformState::PlayOpacity(bool bStart)
{
	if (IsValid(this->TimeLine))
	{
		if (bStart)
		{
			this->TimeLine->PlayFromStart();
		}
		else {
			this->TimeLine->ReverseFromEnd();
		}
	}
}

void UBossUniformState::BossOutDef()
{
	this->PlayOpacity(true);

	//随机选择路线
	int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->GetRowAndCol().Row - 1);
	int32 ConstCol = this->GetRowAndCol().Col - 1;

	//切换路线
	this->Get()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->Get()),
		this->GetLine().Row, RanRow, ConstCol
	);

	//设置老鼠进入网格
	this->Get()->InMapMeshe(this->GetMeshe(RanRow, ConstCol)->GetLineType());
	this->Get()->CloseInWaterTimeLine();

	//设置老鼠目标位置
	this->Get()->SetActorLocation(this->GetMesheLocation(RanRow, ConstCol));
}

void UBossUniformState::BossExitDef()
{
	this->PlayOpacity(false);
}

void UBossUniformState::Init()
{
	this->TimeLine = UTimeLineClass::MakeTimeLineClass();
	this->TimeLine->AddCurve(
		this->Get()->BossOpacityCurve, this->Get(),
		[](UTimeLineClass* time, UObject* obj, float value) {
			//Cast<ABossBase>(obj)->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), value);
		},
		[](UTimeLineClass* time, UObject* obj) {

		}
	);
}

void UBossUniformState::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->TimeLine))
	{
		this->TimeLine->Tick(DeltaTime);
	}
}

void UBossUniformState::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (
		BuffTag == EGameBuffTag::Freeze
		||
		BuffTag == EGameBuffTag::Solidification
		||
		BuffTag == EGameBuffTag::SlowDown
		)
	{
		CurBuffTime = 0.f;
	}
}



void ABossBase::MouseTick(const float& DeltaTime)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	if (IsValid(this->Get()))
	{
		this->Get()->MouseTick(DeltaTime);
	}
}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();
	this->bInit = false;
}

void ABossBase::MouseInit()
{
	Super::MouseInit();

	if (IsValid(this->M_BossHP_Progress))
	{
		this->M_BossHP_Progress->UpdateHPText(this->GetCurrentHP(), this->GetTotalHP());
	}

	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ABossBase::BossAnimPlayEnd);

	//广播boss生成
	AGameMapInstance::GetGameMapInstance()->OnSpawnMouseBossDelegate.Broadcast();
}

void ABossBase::InitBoss(const FString& Name, const FString& BossCode)
{
	UE_LOG(LogTemp, Error, TEXT("ABossBase::MouseInit:%s"), *UKismetSystemLibrary::GetObjectName(this));

	//初始化管理器的管理对象
	this->MouseActor = this;

	//创建血条
	this->M_BossHP_Progress = CreateWidget<UUI_MouseBoss_Slider>(
		this->GetWorld(),
		LoadClass<UUI_MouseBoss_Slider>(nullptr,
			TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/MouseRound/UI_MouseBossSlider.UI_MouseBossSlider_C'")
		)
	);

	if (IsValid(this->M_BossHP_Progress))
	{
		if (IsValid(AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound))
		{
			AGameMapInstance::GetMouseManagerComponent_Static()->
				M_UUI_MouseRound->AddMouseBossSlider(
					UKismetSystemLibrary::GetObjectName(this),
					this->M_BossHP_Progress
				);
		}
		else {
			UWidgetBase::CreateTipWidget(TEXT("Boss进度条出错，请检查Boss的初始化实现逻辑！！！！！"));
			this->SetMouseDeath(true);
			this->SetActorTickEnabled(false);
			this->Destroy();
			return;
		}

		this->M_BossHP_Progress->UpdateHPText(
			this->GetCurrentHP(),
			this->GetTotalHP()
		);

		//播放(Boss)BGM(如果没有任何血条才会播放BGM)
		if (
			AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound
			&&
			AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->GetMouseBossSliderCount() == 1)
		{
			if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager())
			{
				UFVMGameInstance::PlayBGM_Static(AGameMapInstance::GetGameMapInstance()->
					GetGameMapStructManager()->LevelConfig.LevelBossMusic);
			}
		}

		this->M_BossHP_Progress->SetBossName(Name);
		this->M_BossHP_Progress->SetBossHead(BossCode);
		this->bInit = true;
	}
	else {
		this->M_BossHP_Progress = nullptr;
	}
}

EMouseBaseType ABossBase::GetMouseBaseType()
{
	Super::GetMouseBaseType();

	return EMouseBaseType::Boss;
}

void ABossBase::MoveingUpdate(float DeltaTime)
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::MoveingUpdate(DeltaTime);

	if (IsValid(this->Get()))
	{
		this->Get()->MoveingUpdate(DeltaTime);
	}
}

void ABossBase::MoveingStop()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::MoveingStop();

}

void ABossBase::MoveingBegin()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::MoveingBegin();

}

void ABossBase::AttackedBegin()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::AttackedBegin();
}

void ABossBase::AttackedEnd()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::AttackedEnd();

}


bool ABossBase::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!this->bInit)
	{
		this->SetMouseDeath(true);
		return false;
	}

	if (this->GetCurrentHP() <= 0.f)
	{
		return false;
	}

	if (IsValid(this->Get()))
	{
		this->Get()->BeHit(CurHitMouseObj, _HurtValue, AttackType);
	}

	//更新血量
	this->UpdateHP(_HurtValue);

	//更新进度条
	if (IsValid(this->M_BossHP_Progress))
	{
		this->M_BossHP_Progress->SetSliderRate(this->GetCurrentHP() / this->GetTotalHP());
	}
	else {
		return false;
	}

	if (this->bInit)
	{
		if (IsValid(this->M_BossHP_Progress))
		{
			this->M_BossHP_Progress->PlayAnimation(
				this->M_BossHP_Progress->GetWidgetTreeOwningClass()->Animations[1]);

			this->M_BossHP_Progress->UpdateHPText(this->GetCurrentHP(), this->GetTotalHP());
		}
		else {
			return false;
		}
	}

	return true;
}

void ABossBase::BeAttakedBegin()
{
	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::BeAttakedBegin();
}


void ABossBase::MouseDeathed()
{
	//移除控件
	if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound)
	{
		AGameMapInstance::GetMouseManagerComponent_Static()->
			M_UUI_MouseRound->RemoveMouseBossSlider(UKismetSystemLibrary::GetObjectName(this));

		this->M_BossHP_Progress = nullptr;
	}

	//播放BGM
	if (AGameMapInstance::GetMouseManagerComponent_Static()->M_UUI_MouseRound->GetMouseBossSliderCount() <= 0)
	{
		if (AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager())
		{
			UFVMGameInstance::PlayBGM_Static(AGameMapInstance::GetGameMapInstance()->
				GetGameMapStructManager()->LevelConfig.LevelUpMusic);
		}
	}

	Super::MouseDeathed();
}

void ABossBase::InMapMeshe(ELineType CurLineType)
{

	if (this->bInWaterAnimState)
	{
		Super::InMapMeshe(CurLineType);

		if (IsValid(this->Get()))
		{
			this->Get()->InMapMeshe(CurLineType);
		}
	}
	else {
		if (IsValid(this->Get()))
		{
			this->Get()->InMapMeshe(CurLineType);
		}
	}

}

void ABossBase::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);

	if (
		BuffTag == EGameBuffTag::Freeze
		||
		BuffTag == EGameBuffTag::Solidification
		)
	{
		CurBuffTime = 0.f;
	}

	if (IsValid(this->Get()))
	{
		this->Get()->ExecuteBuff(BuffTag, CurBuffTime);
	}
}

void ABossBase::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);
}

void ABossBase::BossAnimPlayEnd()
{
	if (IsValid(this->Get()))
	{
		this->Get()->AnimPlayEnd();
	}
}

UMesheControllComponent* ABossBase::GetMesheComp()
{
	return AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
}

AMapLauncherBase* ABossBase::GetMapLauncher()
{
	if (this->GetMesheComp())
	{
		return this->GetMesheComp()->GetMapLauncher();
	}

	return nullptr;
}

int32 ABossBase::GetMouseLevel()
{
	if (
		IsValid(UFVMGameInstance::GetFVMGameInstance())
		&&
		IsValid(UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager())
		)
	{
		return UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
	}

	return 1;
}

void ABossBase::UpdateBossOpacity(const float& value)
{
	//float Time = UKismetMathLibrary::Lerp(0.f, 1.f, value);
	//this->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), Time);
	//this->InWaterAnim->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, Time));
}

void ABossBase::SpawnMouseByName(const FString& MouseName, int32 Row, int32 Col /*= -1*/)
{
	FVector Loca = FVector::ZeroVector;
	if (Col != -1 && Col < this->GetMesheComp()->GetMapMeshRowAndCol().Col)
	{
		Loca = this->GetMesheComp()->GetMapMeshLocation(Row, Col);
		Loca.Z += MOUSE_Z_OFFSET;
	}

	UMouseSpawnManager::MakeNewMouseByName(
		UGameSystemFunction::LoadRes(
			TSoftObjectPtr<UDataTable>(FString(TEXT("/Game/Resource/BP/Data/MouseData/MouseData.MouseData")))
		), MouseName, FLine(Row, Col), Loca
	);
}

void ABossBase::DefSpawnMouseTemp(int32 Level, const TArray<FString>& MouseNames)
{
	if (MouseNames.Num() == 0)
	{
		return;
	}

	if (Level == 1)
	{
		int32 LRow = UGameSystemFunction::GetRandomRange(0, this->GetMesheComp()->GetMapMeshRowAndCol().Row - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], LRow, -1);
	}
	else if (Level == 2)
	{
		TArray<int32> Rows;
		for (int32 i = 0; i < this->GetMesheComp()->GetMapMeshRowAndCol().Row; i++)
		{
			Rows.Emplace(i);
		}

		int32 Ran1 = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], Ran1, -1);
		Rows.RemoveAt(Ran1);

		int32 Ran2 = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], Ran2, -1);
		Rows.RemoveAt(Ran2);
	}
	else if (Level == 3)
	{
		TArray<int32> Rows;
		for (int32 i = 0; i < this->GetMesheComp()->GetMapMeshRowAndCol().Row; i++)
		{
			Rows.Emplace(i);
		}

		int32 Ran1 = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], Ran1, -1);
		Rows.RemoveAt(Ran1);

		int32 Ran2 = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], Ran2, -1);
		Rows.RemoveAt(Ran2);

		int32 Ran3 = UGameSystemFunction::GetRandomRange(0, Rows.Num() - 1);
		this->SpawnMouseByName(MouseNames[
			UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
		], Ran3, -1);
		Rows.RemoveAt(Ran3);
	}
	else {
		for (int32 i = 0; i < this->GetMesheComp()->GetMapMeshRowAndCol().Row; i++)
		{
			this->SpawnMouseByName(MouseNames[
				UGameSystemFunction::GetRandomRange(0, MouseNames.Num() - 1)
			], i, -1);
		}
	}
}

void ABossBase::DefSpawnMouse(int32 Level)
{
	this->DefSpawnMouseTemp(Level, this->SpawnDefMouseNames);
}

void ABossBase::SetBossName(const FString& Name)
{
	if (IsValid(this->M_BossHP_Progress))
	{
		this->M_BossHP_Progress->SetBossName(Name);
	}
}

void ABossStateManager::InitBoss(const FString& Name, const FString& BossCode)
{
}

void ABossStateManager::ChangeState(class UBossStateBase* NewState)
{
	if (IsValid(NewState))
	{
		//设置新的状态
		this->State = NewState;
		this->State->InitState(this);
		this->State->Init();
	}
	else {
		this->State = nullptr;
	}
}

UBossStateBase* ABossStateManager::ABossStateManager::Get()
{
	return this->State;
}

void UBossStateBase::Init() {}
void UBossStateBase::MouseTick(const float& DeltaTime) {}
void UBossStateBase::MoveingUpdate(const float& DeltaTime) {}
void UBossStateBase::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) {}
void UBossStateBase::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) {}
void UBossStateBase::InMapMeshe(ELineType CurLineType) {}

void UBossStateBase::AnimPlayEnd() {}

void UBossStateBase::InitState(ABossStateManager* NewMan)
{
	if (IsValid(NewMan))
	{
		this->CurMan = NewMan;
	}
}

class ABossBase* UBossStateBase::Get()
{
	if (IsValid(this->CurMan))
	{
		return this->CurMan->MouseActor;
	}
	return nullptr;
}

void UBossStateBase::PlayAnim(UPaperFlipbook* Anim1, UPaperFlipbook* Anim2Low)
{
	if (IsValid(this->Get()))
	{
		if (this->Get()->GetCurrentHP() > this->Get()->GetTotalHP() * 0.5f)
		{
			//this->Get()->GetRenderComponent()->SetPlayAnimation(Anim1);

			this->Get()->SetAnimation(0, TEXT("SpawnTag"), true);

		}
		else {
			//this->Get()->GetRenderComponent()->SetPlayAnimation(Anim2Low);

			this->Get()->SetAnimation(0, TEXT("SpawnTag"), true);
		}
	}
}

ABossBoxBase::ABossBoxBase()
{
	this->MMeshe = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckMesheComp"));
	this->MBody = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollsionComp"));

	this->MMeshe->SetupAttachment(this->GetRootComponent());
	this->MBody->SetupAttachment(this->MMeshe);
}

ABossSphereBase::ABossSphereBase()
{
	this->MMeshe = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckMesheComp"));
	this->MBody = CreateDefaultSubobject<USphereComponent>(TEXT("BodyCollsionComp"));

	this->MMeshe->SetupAttachment(this->GetRootComponent());
	this->MBody->SetupAttachment(this->MMeshe);
}
