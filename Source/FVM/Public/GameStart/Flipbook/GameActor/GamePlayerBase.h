// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSystem/PlayerStructManager.h"
#include "GamePlayerBase.generated.h"

class UPaperFlipbook;
class USceneComponent;
class UFlipbookBaseComponent;
class AGameActorFlipbookBase;

UCLASS()
class FVM_API AGamePlayerBase : public AActor
{
	GENERATED_BODY()
public:
	//根部
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_SlotParent = nullptr;
	//-------------------------------------------------------------------------
		//子根部1->基础着装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_SlotParentChild_1 = nullptr;
	//插槽-翅膀
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_FlyItem = nullptr;
	//插槽-头发后部分
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_HairBack = nullptr;
	//插槽-着装(身体部分)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Body = nullptr;
	//插槽-头部->裸头
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Head = nullptr;
	//插槽-眼睛
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Eye = nullptr;
	//插槽-脸饰
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Face = nullptr;
	//插槽-眼镜
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Glasses = nullptr;
	//插槽-头发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Hair = nullptr;
	//插槽-帽子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_Cap = nullptr;
	//-------------------------------------------------------------------------
		//子根部2->套装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* M_SlotParentChild_2 = nullptr;
private:

public:
	//角色基础着装动画组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_FlyItemAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_HairBackAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_BodyAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_HeadAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_EyeAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_FaceAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_GlassesAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_HairAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_CapAnim = nullptr;

	//角色套装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFlipbookBaseComponent* M_SuitAnim = nullptr;
	//武器动画
public:
	UPROPERTY(EditAnywhere)//基础服装-翅膀默认动画
		UPaperFlipbook* M_Anim_Suit_FlyItemDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-翅膀攻击动画
		UPaperFlipbook* M_Anim_Suit_FlyItemAttack = nullptr;

	UPROPERTY(EditAnywhere)//套装-默认动画
		UPaperFlipbook* M_Anim_Suit_Def = nullptr;
	UPROPERTY(EditAnywhere)//套装-攻击动画
		UPaperFlipbook* M_Anim_Suit_Attack = nullptr;

	//--------------------------------------------------------------

	UPROPERTY(EditAnywhere)//基础服装-【头发(后)】默认动画
		UPaperFlipbook* M_Anim_BaseSuit_HairBackDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-【头发(后)】攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_HairBackAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-着装(身体)默认动画
		UPaperFlipbook* M_Anim_BaseSuit_BodyDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-着装(身体)攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_BodyAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-头(裸头)默认动画
		UPaperFlipbook* M_Anim_BaseSuit_HeadDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-头(裸头)攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_HeadAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-眼睛默认动画
		UPaperFlipbook* M_Anim_BaseSuit_EyeDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-眼睛攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_EyeAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-脸饰默认动画
		UPaperFlipbook* M_Anim_BaseSuit_FaceDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-脸饰攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_FaceAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-眼镜默认动画
		UPaperFlipbook* M_Anim_BaseSuit_GlassesDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-脸眼镜攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_GlassesAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-【头发(前)】默认动画
		UPaperFlipbook* M_Anim_BaseSuit_HairDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-【头发(前)】攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_HairAttack = nullptr;

	UPROPERTY(EditAnywhere)//基础服装-【头发(前)】默认动画
		UPaperFlipbook* M_Anim_BaseSuit_CapDef = nullptr;
	UPROPERTY(EditAnywhere)//基础服装-【头发(前)】攻击动画
		UPaperFlipbook* M_Anim_BaseSuit_CapAttack = nullptr;
public:
	UFUNCTION()
		void LoadAnimation(
			const FPlayerSuitItem& _Value, //角色服装数据
			bool _bUseFirst,	//服装位置【前，后】
			UFlipbookBaseComponent* _Position,//服装动画组件
			UPaperFlipbook*& _Def,//默认动画
			UPaperFlipbook*& _Attack,//攻击动画
			EPlayerEquipmentSlotPosition _Type//服装类型
		);
public:
	// Sets default values for this actor's properties
	AGamePlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//初始化角色基本数据
	void InitPlayerData();

};
