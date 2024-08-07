// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMouse.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicChildMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/MapMeshe.h"

AMagicMouse::AMagicMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AMagicMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicMouse::MouseTick(const float& DeltaTime)
{
	if (this->bCheck)
	{
		if (this->AtkDelay > 0.f)
		{
			this->AtkDelay -= DeltaTime;
		}
		else {
			this->AtkDelay = 0.2f;

			TArray<FHitResult> Res;
			if (UGameSystemFunction::AddSphereTraceMulti(
				this, Res, FVector(0.f, 0.f, 15.f), FVector(0.f, 0.f, 15.f), 20.f,
				UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)))
			{
				for (const FHitResult& CurHit : Res)
				{
					ACardActor* CurCard = Cast<ACardActor>(CurHit.GetActor());
					if (IsValid(CurCard) && CurCard->GetCurrentHP() > 0.f)
					{
						if (CurCard->GetCardData().M_ECardCollisionType == ECardCollisionType::E_CardActor2)
						{
							if (CurCard->UpdateCardState(CurCard->GetCardData().M_CardHP, 0.f))
							{
								CurCard->KillCard();
								UResourceManagerComponent::ResourceAddBadCard();
							}
						}
						else if (CurCard->GetCardData().M_ECardCollisionType == ECardCollisionType::E_CardActor3)
						{
							if (CurCard->UpdateCardState(1, 0.f))
							{
								CurCard->KillCard();
								UResourceManagerComponent::ResourceAddBadCard();
							}

							this->SetbIsHurt(true);
							this->BeHit(CurCard,this->GetTotalHP(), EFlyItemAttackType::Def);
							this->bCheck = false;
							return;
						}
					}
				}
			}
		}
	}

	if (!this->bShoot)
	{
		return;
	}

	if (this->bMouseChild && this->bBeginMove)
	{
		if (this->ShootMouseTime > 0.f)
		{
			this->ShootMouseTime -= DeltaTime;
		}
	}
	else {
		if (this->bMove || !this->bBeginMove)
		{
			return;
		}

		if (this->MouseTime > 0.f)
		{
			this->MouseTime -= DeltaTime;
		}
		else {
			this->bMouseChild = true;
			if (this->GetCurrentHP() > 0.f)
			{
				//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Create), true);

				this->SetAnimation(0, TEXT("SpineTag"), true);
			}
		}
	}
}

void AMagicMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AMagicMouse::AnimPlayEnd);
	this->Meshe = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMagicMouse::MouseInit()
{
	Super::MouseInit();

	this->MouseTime = 0;//this->ReadyNewMouseTime;
	this->ShootMouseTime = this->ShootNewMouseTime;
}

void AMagicMouse::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	//开局移动距离
	if (!this->bBeginMove)
	{
		//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

		this->UpdateMove(DeltaTime);

		if (this->GetActorLocation().Y < 425.f - (55.f * this->BeginMoveGrid))
		{
			this->MoveStop();
			this->bBeginMove = true;
			return;
		}

		return;
	}

	if (this->bMove)
	{
		//this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));

		this->UpdateMove(DeltaTime);

		if (this->bShoot)
		{
			if (this->GetActorLocation().Y < 425.f - (55.f * this->MoveShootGrid))
			{
				this->bShoot = false;
				this->bMove = true;
			}
		}
	}
}

void AMagicMouse::MouseDeathed()
{
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->bCheck = false;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		//播放死亡动画
		//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Death), true);

		this->SetAnimation(0, TEXT("SpineTag"), true);

	}
}

void AMagicMouse::AnimPlayEnd()
{
	if (!this->bBeginMove)
	{
		return;
	}

	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	this->bMove = true;

	if (this->bShoot)
	{
		for (int32 i = 0; i < this->Meshe->GetMapMeshRowAndCol().Col; i++)
		{
			AMapMeshe* CurMapMeshe = this->Meshe->GetMesh(this->GetMouseLine().Row, i);
			if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
			{
				if (CurMapMeshe->GetUIMapMeshe()->GetCardDatas().Num())
				{
					//检测是否允许移动				
					this->bMove = false;
					break;
				}
			}
		}
	}

	if (this->bMove)
	{
		this->MoveStart();
	}
	else {
		this->MoveStop();
	}

	if (this->bMouseChild && !this->bMove && this->bShoot)
	{
		if (this->ShootMouseTime <= 0.f)
		{
			this->ShootMouseTime = this->ShootNewMouseTime;
			this->MouseTime = this->ReadyNewMouseTime;
			this->bCurShoot = true;
			this->bMouseChild = false;

			//发射新老鼠
			//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Shoot), true);
			this->SetAnimation(0, TEXT("SpineTag"), true);

			if (this->bCurShoot)
			{
				this->bCurShoot = false;

				//根据血量决定射击多少行
				if (this->GetTotalHP() <= 45.f)
				{
					//射击本行
					this->ShootMouse(15, this->GetMouseLine());
				}
				else if (this->GetTotalHP() > 45.f && this->GetTotalHP() < 90.f)
				{
					this->ShootMouse(25, this->GetMouseLine());

					if (this->GetMouseLine().Row - 1 >= 0)
					{
						this->ShootMouse(25, FLine(this->GetMouseLine().Row - 1, 0));
					}

					if (this->GetMouseLine().Row + 1 < this->Meshe->GetMapMeshRowAndCol().Row)
					{
						this->ShootMouse(25, FLine(this->GetMouseLine().Row + 1, 0));
					}
				}
				else {
					for (int32 i = 0; i < this->Meshe->GetMapMeshRowAndCol().Row; i++)
					{
						this->ShootMouse(40, FLine(i, 0));
					}
				}

			}

			return;
		}
	}

	if (this->bMouseChild)
	{
		//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Def1), true);

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else {
		//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Def2), true);

		this->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void AMagicMouse::ShootMouse(const float& Hp, const FLine& CurLine)
{
	//创建小弟
	AMouseActor* CurMouse = UMouseSpawnManager::MakeNewMouseByClass(
		this->GetMouseManager(),
		this->TargetCreateMouse,
		this->GetActorLocation() + FVector(0.f, -30.f, 60.f),
		Hp,
		1.f,
		20.f,
		CurLine,
		ELineType::OnGround
	);
	if (IsValid(CurMouse))
	{
		//设置层级
		//CurMouse->SetTranslucentSortPriority(this->Meshe->GetMapMesh(CurLine.Row, 3)->GetTranslucency() + 20);
		//初始化
		Cast<AMagicChildMouse>(CurMouse)->OnInitBlue();
	}
}
