// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/MapLauncher/Launchers/MeshesLauncherComponent.h"
#include "GameStart/MapLauncher/Launchers/Meshe/MesheLauncherActor.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"

#include "GameStart/VS/GameMapInstance.h"

#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UMeshesLauncherComponent::UMeshesLauncherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeshesLauncherComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	this->M_Owner = Cast<AMapLauncherBase>(this->GetOwner());
	if (!this->M_Owner)
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("无效的地图发射器"));
	}

}


// Called every frame
void UMeshesLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeshesLauncherComponent::SetMeshs(const TArray<FLine>& _Lines)
{
	for (auto& LLine : _Lines)
	{
		if (LLine.Row < this->M_Owner->GetMapMeshLine().Row)
		{
			if (LLine.Col < this->M_Owner->GetMapMeshLine().Col)
			{
				this->M_Meshes.Emplace(this->M_Owner->GetMapMeshes(FLine(LLine.Row, LLine.Col)), nullptr);
			}
		}
	}
}

void UMeshesLauncherComponent::SetAnimationResPath_C(TSoftClassPtr<AGameActorFlipbookBase> _AnimRes)
{
	if (!_AnimRes.Get())
	{
		_AnimRes.LoadSynchronous();
	}
	this->M_AnimRes = _AnimRes.Get();
}

void UMeshesLauncherComponent::ApplySetting()
{
	if (!this->M_AnimRes)
		return;

	for (auto& Meshe : this->M_Meshes)
	{
		FTransform Trans;

		Trans = Meshe.Key->GetTransform();
		Trans.SetLocation(Trans.GetLocation() + FVector(-0.8f, 0.f, 10.f));

		//生成对象
		AMesheLauncherActor* CurActor = AMesheLauncherActor::MakeMesheLauncherActor(this->M_Owner->GetWorld(), this->M_AnimRes, Meshe.Key, Meshe.Key->GetLine(), Trans);
	}

	this->M_Meshes.Empty();

	this->M_IsbApply = true;
}
