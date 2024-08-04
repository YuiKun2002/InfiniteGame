// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameStart/Flipbook/SpriteActor.h"
#include "Kismet/KismetMathLibrary.h"

#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

//游戏实例
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/Tools/GameSystemFunction.h"

ANormalBase::ANormalBase()
{

}


void ANormalBase::SetEscalatorMove(int32 NextTranslucenty, bool bEnbale)
{
	//如果已经开始上下移动，则无需走楼梯
	if (this->GetMouseMoveOtherLine())
	{
		return;
	}

	if (bEnbale)
	{
		if (!this->bEnableEscalatorMove)
		{
			this->bEnableEscalatorMove = true;
			this->bForceCloseEscalator = false;
			this->bTimePlay = true;
			this->fCurZValue = this->GetActorLocation().Z;
			this->fConstZValue = this->GetActorLocation().Z;
			this->fSpeedBaseValue = 30;
			this->iNextTranslucenty = NextTranslucenty;
		}
	}
	else {
		this->bEnableEscalatorMove = false;
		this->bForceCloseEscalator = true;
		this->fMoveTime = 0.f;
		this->fCurZValue = this->GetActorLocation().Z;
		this->fConstMoveTime = 0.4f;
	}

}

void ANormalBase::SetMouseMoveOtherLineFunc(FLine NewLine)
{
	//如果启动楼梯移动，则无需进行上下移动
	if (this->bEnableEscalatorMove)
	{
		return;
	}

	Super::SetMouseMoveOtherLineFunc(NewLine);
}

void ANormalBase::UpdateMoveFunc(const float& DeltaTime)
{
	if (this->bEnableEscalatorMove)
	{
		this->fMoveTime += DeltaTime;
		//动态速度计算
		this->fConstMoveTime = this->fSpeedBaseValue / this->GetCurrentMoveSpeed();
		//当前高度
		float TargetLocationZ = 0.f;

		//运动结束
		if (this->fMoveTime / this->fConstMoveTime > 1.f)
		{
			if (this->bTimePlay)
			{
				//重新计时
				this->fMoveTime = 0.f;
				this->bTimePlay = false;
				this->fCurZValue = this->fCurZValue + 25.f;
				TargetLocationZ = this->fCurZValue;
				//修改移动基数
				this->fSpeedBaseValue = this->fSpeedBaseValue / 3.2f;
				//设置渲染层
				this->SetRenderLayer(this->iNextTranslucenty);
			}
			else {
				//设置目标位置
				FVector CurLoca = this->GetActorLocation();
				//是否进入水中
				float InWaterZ = 0.f;
				if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
				{
					InWaterZ = this->fMouseInWaterZ;
				}
				this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, this->fConstZValue - InWaterZ));
				//
				this->bForceCloseEscalator = false;
				//高度重置为0
				this->fCurZValue = 0.f;
				//结束移动
				this->bEnableEscalatorMove = false;
				return;
			}
		}
		else {
			if (this->bTimePlay)
			{
				TargetLocationZ = UKismetMathLibrary::Lerp(
					this->fCurZValue, this->fCurZValue + 25.f, this->fMoveTime / this->fConstMoveTime
				);
			}
			else
			{
				//是否进入水中
				float InWaterZ = 0.f;
				if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
				{
					InWaterZ = this->fMouseInWaterZ;
				}

				TargetLocationZ = UKismetMathLibrary::Lerp(
					this->fCurZValue, this->fConstZValue - InWaterZ, this->fMoveTime / this->fConstMoveTime
				);
			}

		}

		//移动
		this->AddActorLocalOffset(FVector(0.f, (DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed) * this->GetMouseBaseMoveDirection(), 0.f));
		//设置目标位置
		FVector CurLoca = this->GetActorLocation();
		this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, TargetLocationZ));
	}
	else {
		//强行打断扶梯运动模式
		if (this->bForceCloseEscalator)
		{
			this->fMoveTime += DeltaTime;

			//结束运动
			if (this->fMoveTime / this->fConstMoveTime > 1.f)
			{
				//设置目标位置
				FVector CurLoca = this->GetActorLocation();
				//是否进入水中
				float InWaterZ = 0.f;
				if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
				{
					InWaterZ = this->fMouseInWaterZ;
				}
				this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, this->fConstZValue - InWaterZ));
				this->bForceCloseEscalator = false;
				return;
			}

			//是否进入水中
			float InWaterZ = 0.f;
			if (this->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
			{
				InWaterZ = this->fMouseInWaterZ;
			}
			float TargetLocationZ = UKismetMathLibrary::Lerp(
				this->fCurZValue, this->fConstZValue - InWaterZ, this->fMoveTime / this->fConstMoveTime
			);

			//移动
			this->AddActorLocalOffset(
				FVector(0.f,
					(DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed) * this->GetMouseBaseMoveDirection(),
					0.f));
			//设置目标位置
			FVector CurLoca = this->GetActorLocation();
			this->SetActorLocation(FVector(CurLoca.X, CurLoca.Y, TargetLocationZ));
		}
		else
		{
			//允许老鼠默认移动和上下换行
			Super::UpdateMoveFunc(DeltaTime);
		}
	}
}

//老鼠目前的行为

//移动(每0.02ms自动调用(允许移动的情况下))
void ANormalBase::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);
}
//停止移动(当设置停止移动时调用一次)
void ANormalBase::MoveingStop()
{
	Super::MoveingStop();

}
//开始移动(当设置开始移动时调用一次)
void ANormalBase::MoveingBegin()
{
	Super::MoveingBegin();

}

//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
void ANormalBase::AttackedBegin()
{
	Super::AttackedBegin();

	this->MoveStop();
}
//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
void ANormalBase::AttackedEnd()
{
	Super::AttackedEnd();

	//如果老鼠没有死亡则可以移动
	if (!this->GetMouseIsDeath())
	{
		this->MoveStart();
	}
}

//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
bool ANormalBase::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	//如果血量已经为0，则直接退出
	if (this->GetCurrentHP() <= 0.f)
	{
		return false;
	}

	if (this->GetCurrentHP() - _HurtValue <= 0)
	{
		//判断是否被炸死
		if (AttackType == EFlyItemAttackType::Bomb)
		{
			this->M_MouseDeathByBomb = true;
		}
	}

	//更新血量
	return !this->UpdateHP(_HurtValue);
}

//老鼠受到攻击((当设置isHurt为true时)调用一次)
void ANormalBase::BeAttakedBegin()
{
	Super::BeAttakedBegin();
}

//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
void ANormalBase::MouseDeathed()
{
	Super::MouseDeathed();

	//直接移除冰块
	if (IsValid(this->IceBlock))
	{
		this->IceBlock->Destroy();
	}
}

void ANormalBase::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);

	if (BuffTag == EGameBuffTag::Freeze && CurBuffTime > 0.f)
	{
		if (this->GetCurrentHP() > 0.f && this->M_Proper_Condition.M_CurrentInType != ELineType::OnWater)
		{
			//创建冰块
			this->IceBlock = UGameSystemFunction::CreateIceBlock(this, FVector(0.f, 0.f, -3.5f),
				this->GetTranslucentSortPriority() + 4, CurBuffTime);
		}
	}
}

void ANormalBase::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);

	if (BuffTag == EGameBuffTag::Freeze)
	{
		if (this->GetCurrentHP() > 0.f)
		{
			if (IsValid(this->IceBlock))
			{
				//播放冻结结束的音效
				UFVMGameInstance::PlayBGM_S_Static(TEXT("Freeze_Over"), TEXT("ItemAudio"));
			}
		}

		if (IsValid(this->IceBlock))
		{
			this->IceBlock->Destroy();
		}
	}
}

void ANormalBase::InMapMeshe(ELineType CurLineType)
{
	//如果进入扶梯移动模式
	if (this->bEnableEscalatorMove)
	{
		this->ForceSetWaterAnimShow(CurLineType);
	}
	else {
		Super::InMapMeshe(CurLineType);
	}
}

//Tick 更新
void ANormalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalBase::MouseTick(const float& DeltaTime)
{
	Super::MouseTick(DeltaTime);
}

//初始化
void ANormalBase::BeginPlay()
{
	Super::BeginPlay();

	this->bEnableEscalatorMove = false;
}

void ANormalBase::PlayBombEffAnim()
{
	this->SetPlayAnimation(
		LoadObject<UPaperFlipbook>(0,
			TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Other/FB_MouseBombEff.FB_MouseBombEff'"))
	);

}

bool ANormalBase::GetMouseDeathByBomb() const
{
	return this->M_MouseDeathByBomb;
}

bool ANormalBase::GetPlayPlayBombEffAnim()
{
	if (this->GetMouseDeathByBomb())
	{
		this->PlayBombEffAnim();
		return true;
	}

	return false;
}

//老鼠初始化(Base BeginPlay自动调用一次)
void ANormalBase::MouseInit()
{
	Super::MouseInit();

	this->bEnableEscalatorMove = false;
}

ANormalSphereBase::ANormalSphereBase()
{
	this->MMeshe = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckNormalMesheComp"));
	this->MBody = CreateDefaultSubobject<USphereComponent>(TEXT("BodyNormalSphereComp"));

	this->MMeshe->SetupAttachment(this->GetRootComponent());
	this->MBody->SetupAttachment(this->MMeshe);
}
