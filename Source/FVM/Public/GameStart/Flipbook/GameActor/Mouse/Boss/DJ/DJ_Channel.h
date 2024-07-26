// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Custom/MouseCustomBase.h"
#include "DJ_Channel.generated.h"

/**
 * 洞君的管道对象
 */

class ABoss_DongJun;
class AMapMeshe;

//洞军创建的的管道状态
UENUM()
enum class EDongJun_Channel_State : uint8 {
	None,
	Init,
	Finish,
	End
};

UCLASS()
class FVM_API ADJ_Channel : public AMouseCustomBase
{
	GENERATED_BODY()
private:
	//洞君
	UPROPERTY()
		ABoss_DongJun* M_Owner = nullptr;
	//被连接的列
	UPROPERTY()
		AMapMeshe* M_AMapMeshe = nullptr;
	//管道入口
	UPROPERTY()
		AGameActorFlipbookBase* M_Channel_In = nullptr;
	//管道创建的状态
	UPROPERTY()
		EDongJun_Channel_State M_DongJun_Channel_State = EDongJun_Channel_State::None;
private:
	//当前状态是否完毕
	bool M_StateFinish = false;
	//通道入口原始地形
	ELineType M_Channel_MesheType_In = ELineType::DisEnable;
	//通道出口原始地形
	ELineType M_Channel_MesheType_Out = ELineType::DisEnable;
public:
	//老鼠网格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_MouseTypeBox = nullptr;
	//老鼠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_MouseBodyBox = nullptr;
private:
	//当前管道位置（出口）
	UPROPERTY()
		FLine M_CurrentLine;
	//管道连接位置（入口）
	UPROPERTY()
		FLine M_TargetLine;
public:
	//获取管道入口位置
	FLine GetChannel_In();
	//获取管道出口位置
	FLine GetChannel_Out();
public:
	ADJ_Channel();
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//动画播放完毕
	void OnAnimationPlayEnd();
public:
	//设置父对象
	void SetMouseDongJun(ABoss_DongJun* _Owner);
	//通道连接
	bool LinkChannel(FLine _CurrentLine);
	//设置列连接
	void SetLinkCol(AMapMeshe* _AMapMeshe);
	//应用设置
	void Apply();
	//生命值恢复
	void HPReset();
	//还原地形
	void MesheReset();
public:
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj,float _HurtValue,EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
};
