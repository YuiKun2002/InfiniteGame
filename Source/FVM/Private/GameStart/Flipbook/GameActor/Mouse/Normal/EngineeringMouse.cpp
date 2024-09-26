// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/EngineeringMouse.h"
#include <Components/BoxComponent.h>
#include <Components/Capsulecomponent.h>
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/MapMeshe.h"

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

			UGameSystemFunction::AddLineTrance(
				this->GetWorld(),
				this->GetActorLocation(), FVector(0.f, -10.f, 0.f), FVector(0.f, 10.f, 0.f),
				UGameSystemFunction::GetCardCollisionBoxType(ECardCollisionType::E_CardActor), this,
				[](UObject* OwnerActor, AActor* Card) {
					AEngineeringMouse* Mouse = Cast<AEngineeringMouse>(OwnerActor);
					ACardActor* CurCard = Cast<ACardActor>(Card);
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

							Mouse->SetbIsHurt(true);
							Mouse->BeHit(CurCard, Mouse->GetTotalHP(), EFlyItemAttackType::Def);
							Mouse->bCheck = false;
							return;
						}
					}
				});

			/*TArray<FHitResult> Res;
			if (UGameSystemFunction::AddSphereTraceMulti(
				this, Res, FVector(0.f, 0.f, 15.f), FVector(0.f, 0.f, 15.f), 20.f,
				UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)))
			{
				for (const FHitResult& CurHit : Res)
				{

				}
			}*/
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

	//初始化碰撞网格位置
	this->MesheComp->SetBoxExtent(this->BoxCompSize, true);
	this->MesheComp->AddLocalOffset(this->CollisionOffset);
	this->BodyComp->AddLocalOffset(this->CollisionOffset);

	this->Meshe = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

	//播放移动动画
	UTrackEntry* Track = this->SetAnimation(0, this->Anim.Move.GetDefaultObject()->GetCategoryName().ToString(), true);
	this->SetTrackEntry(Track);
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
			//移动停止
			this->MoveStop();
			//播放动画【等待动画】
			this->PlayIdleAnim();
			//开始移动完成
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
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();

	this->MesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->BodyComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->bCheck = false;
	this->bProjStart = false;

	Super::MouseDeathed();

	if (!this->GetPlayPlayBombEffAnim())
	{
		//播放死亡动画
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Death.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AMouseActor::AlienDeadAnimationCompelet);
		this->SetTrackEntry(Track);
	}
}

void AEngineeringMouse::AnimPlayEnd(UTrackEntry* TrackEntry)
{
	if (!this->bBeginMove)
	{
		return;
	}

	if (this->GetCurrentHP() <= 0.f)
	{
		return;
	}

	//动画结束允许移动
	this->bMove = true;

	//如果处于发射模式
	if (this->bShoot)
	{
		//查询一次网格
		for (int32 i = 0; i < this->Meshe->GetMapMeshRowAndCol().Col; i++)
		{
			AMapMeshe* CurMapMeshe = this->Meshe->GetMesh(this->GetMouseLine().Row, i);
			if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
			{
				if (CurMapMeshe->GetActorLocation().Y < this->GetActorLocation().Y)
				{
					if (CurMapMeshe->GetUIMapMeshe()->GetCardDatas().Num())
					{
						//检测有卡片，禁止移动			
						this->bMove = false;
						break;
					}
				}
			}
		}
	}

	//允许移动
	if (this->bMove)
	{
		this->MoveStart();
		this->PlayMoveAnim();
	}
	else {
		//禁止移动
		this->MoveStop();
		this->PlayIdleAnim();
	}

	//子弹有效，禁止移动，允许射击
	if (this->bValidBullet && !this->bMove && this->bShoot)
	{
		//发射时间小于0
		if (this->ProjectileBulletTime <= 0.f)
		{
			//进行发射，开启当前发射状态
			this->ProjectileBulletTime = this->ShootNewBulletTime;
			this->ReadyBulletTime = this->ReadyNewBulletTime;
			this->bCurShoot = true;
			this->bValidBullet = false;

			//播放发射动画
			this->PlayAttackAnim();

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
	Trans.SetLocation(this->GetLauncherPoint());

	UE_LOG(LogTemp, Error, TEXT("%f %f %f"),
		this->GetActorLocation().X,
		this->GetActorLocation().Y,
		this->GetActorLocation().Z
	);

	UE_LOG(LogTemp, Error, TEXT("%f %f %f"),
		this->GetLauncherPoint().X,
		this->GetLauncherPoint().Y,
		this->GetLauncherPoint().Z
	);

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
						CurProj->SetRenderLayer(9999);
						CurProj->CInit(CurMapMeshe->GetActorLocation(), this->GetLauncherPoint(), CurCard);
						return;
					}
				}
			}
		}

		CurProj->Destroy();
	}
}

FVector AEngineeringMouse::GetLauncherPoint()
{
	/*return FVector(
		this->GetActorLocation().X,
		this->GetActorLocation().Y + this->GetPointComponent()->GetRelativeLocation().X + this->LauncherBulletPointComp->GetRelativeLocation().X,
		this->GetActorLocation().Z + this->GetPointComponent()->GetRelativeLocation().Z + this->LauncherBulletPointComp->GetRelativeLocation().Z
	);
	*/

	return this->GetActorLocation() +
		this->GetPointComponent()->GetRelativeLocation() +
		this->LauncherBulletPointComp->GetRelativeLocation();
}

void AEngineeringMouse::PlayIdleAnim()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Idle.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
	else {
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Idle_Damage.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
}

void AEngineeringMouse::PlayMoveAnim()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Move.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
	else {
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Move_Damage.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
}

void AEngineeringMouse::PlayAttackAnim()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Attack.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
	else {
		UTrackEntry* Track = this->SetAnimation(0, this->Anim.Attack_Damage.GetDefaultObject()->GetCategoryName().ToString(), true);
		BINDANIMATION(Track, this, &AEngineeringMouse::AnimPlayEnd);
		this->SetTrackEntry(Track);
	}
}

AEngineeringPrjBullet::AEngineeringPrjBullet()
{
	this->bInit = false;
}

void AEngineeringPrjBullet::CInit(const FVector& TargetLocation, const FVector& CurLocation, const TArray<class ACardActor*>& TargetHitCard)
{
	this->Init(TargetLocation, CurLocation, TargetHitCard);
	this->bInit = true;
}

void AEngineeringPrjBullet::Tick(float DeltaTime)
{
	if (this->bInit)
	{
		this->Update(DeltaTime);
	}
}
