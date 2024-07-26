// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/EngineeringMouse.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/MapMeshe.h"

AEngineeringMouse::AEngineeringMouse()
{
	this->MesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheComp"));
	this->CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//设置依附
	this->MesheComp->SetupAttachment(this->GetRootComponent());
	this->CollisionComp->SetupAttachment(this->MesheComp);
}

void AEngineeringMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEngineeringMouse::MouseTick(const float& DeltaTime)
{
	if (this->bProjStart)
	{
		if (this->ProjectileDelay > 0.f)
		{
			this->ProjectileDelay -= DeltaTime;
		}
		else {

			this->bProjStart = false;

			//根据血量决定射击多少行
			if (this->GetTotalHP() <= 45.f)
			{
				//射击本行
				this->ProjectileBullet(this->GetMouseLine());
			}
			else if (this->GetTotalHP() > 45.f && this->GetTotalHP() < 90.f)
			{
				this->ProjectileBullet(this->GetMouseLine());

				if (this->GetMouseLine().Row - 1 >= 0)
				{
					this->ProjectileBullet(FLine(this->GetMouseLine().Row - 1, 0));
				}

				if (this->GetMouseLine().Row + 1 < this->Meshe->GetMapMeshRowAndCol().Row)
				{
					this->ProjectileBullet(FLine(this->GetMouseLine().Row + 1, 0));
				}
			}
			else {
				for (int32 i = 0; i < this->Meshe->GetMapMeshRowAndCol().Row; i++)
				{
					this->ProjectileBullet(FLine(i, 0));
				}
			}
		}
	}

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

	if (this->bValidBullet && this->bBeginMove)
	{
		if (this->ProjectileBulletTime > 0.f)
		{
			this->ProjectileBulletTime -= DeltaTime;
		}
	}
	else {
		if (this->bMove || !this->bBeginMove)
		{
			return;
		}

		if (this->ReadyBulletTime > 0.f)
		{
			this->ReadyBulletTime -= DeltaTime;
		}
		else {
			this->bValidBullet = true;
		}
	}
}

void AEngineeringMouse::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->MesheComp, this->CollisionComp);

	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &AEngineeringMouse::AnimPlayEnd);
	this->Meshe = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEngineeringMouse::MouseInit()
{
	Super::MouseInit();

	this->bProjStart = false;
	if (this->ProjectileConstDelayTime < 0.f || ProjectileConstDelayTime > 1.5f)
	{
		this->ProjectileConstDelayTime = 0.4f;
	}

	this->ReadyBulletTime = 0;//this->ReadyNewMouseTime;
	this->ProjectileBulletTime = this->ShootNewBulletTime;
}

void AEngineeringMouse::MoveingUpdate(float DeltaTime)
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

void AEngineeringMouse::MouseDeathed()
{
	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->bCheck = false;
	this->bProjStart = false;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		//播放死亡动画
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Death), true);
	}
}

void AEngineeringMouse::AnimPlayEnd()
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
				if (CurMapMeshe->GetActorLocation().Y < this->GetActorLocation().Y)
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
	}

	if (this->bMove)
	{
		this->MoveStart();
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Def1), true);
	}
	else {
		this->MoveStop();
		this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Anim.Wait));
	}

	if (this->bValidBullet && !this->bMove && this->bShoot)
	{
		if (this->ProjectileBulletTime <= 0.f)
		{
			this->ProjectileBulletTime = this->ShootNewBulletTime;
			this->ReadyBulletTime = this->ReadyNewBulletTime;
			this->bCurShoot = true;
			this->bValidBullet = false;

			//发射新老鼠
			this->SetPlayAnimationOnce(
				UGameSystemFunction::LoadRes(this->Anim.Shoot),
				UGameSystemFunction::LoadRes(this->Anim.Wait)
			);

			if (this->bCurShoot)
			{
				this->bCurShoot = false;
				this->bProjStart = true;
				this->ProjectileDelay = this->ProjectileConstDelayTime;
			}
			return;
		}
	}
}

void AEngineeringMouse::ProjectileBullet(const FLine& CurLine)
{

	if (!this->TargetCreateBullet.Get())
	{
		UGameSystemFunction::LoadRes(this->TargetCreateBullet);
	}

	//生成投射物体
	FTransform Trans;
	Trans.SetLocation(this->GetActorLocation());

	//生成子弹对象
	AEngineeringPrjBullet* CurProj = this->GetWorld()->SpawnActor<AEngineeringPrjBullet>(
		this->TargetCreateBullet.Get(),
		Trans
		);

	if (IsValid(CurProj))
	{
		/*
		计算投掷目标
		*/
		for (int32 i = 0; i < this->Meshe->GetMapMeshRowAndCol().Col; i++)
		{
			AMapMeshe* CurMapMeshe = this->Meshe->GetMesh(CurLine.Row, i);
			if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
			{
				//主卡
				ACardActor* TargetCard0 = CurMapMeshe->GetUIMapMeshe()->GetCardByID(0);
				//保护卡
				ACardActor* TargetCard1 = CurMapMeshe->GetUIMapMeshe()->GetCardByID(1);
				//存在其中一张
				if ((IsValid(TargetCard0) && TargetCard0->GetCurrentHP() > 0)
					||
					(IsValid(TargetCard1) && TargetCard1->GetCurrentHP() > 0)
					)
				{
					//如果目标板块的坐标在当前老鼠的前方，才会进行投掷
					if (CurMapMeshe->GetActorLocation().Y < this->GetActorLocation().Y)
					{
						TArray<ACardActor*> CurCard;
						CurCard.Emplace(TargetCard0);
						CurCard.Emplace(TargetCard1);

						//CurProj->SetFlipbookPitchRotation(90.f);
						CurProj->SetTranslucentSortPriority(9999);
						CurProj->CInit(CurMapMeshe->GetActorLocation(), CurCard);
						return;
					}
				}
			}
		}

		CurProj->Destroy();
	}
}

AEngineeringPrjBullet::AEngineeringPrjBullet()
{
	this->bInit = false;
}

void AEngineeringPrjBullet::CInit(const FVector& TargetLocation, const TArray<class ACardActor*>& TargetHitCard)
{
	this->Init(TargetLocation, TargetHitCard);
	this->bInit = true;
}

void AEngineeringPrjBullet::Tick(float DeltaTime)
{
	if (this->bInit)
	{
		this->Update(DeltaTime);
	}
}
