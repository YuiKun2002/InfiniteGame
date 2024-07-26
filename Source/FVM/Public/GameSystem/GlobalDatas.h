// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/CardData/CardDataStruct.h"

#include "Data/EquipmentDataSturct.h"

#include "Data/MaterialDataStruct.h"

//数据表数据-技能书(由GameInstance自动检测更新)
#include "Data/CardSkillBookStruct.h"

#include "GlobalDatas.generated.h"


UCLASS()
class FVM_API UGlobalDatas : public UObject
{
	GENERATED_BODY()
public:
	//-------------------------------------------卡片数据预加载方法-----------------------------------------
    //卡片源数据->攻击
	static TArray<FCard_ATK_Data> Global_SourceData_CardATK;
	//卡片源数据->生产
	static TArray<FCard_Spawn_Data> Global_SourceData_Spawn;
	//卡片源数据->防御
	static TArray<FCard_Defence_Data> Global_SourceData_Defence;
	//卡片源数据->功能
	static TArray<FCard_Function_Data> Global_SourceData_Function;
	//卡片源数据->范围攻击
	static TArray<FCard_RangeATK_Data> Global_SourceData_RangeATK;
	//-------------------------------------------卡片数据预加载方法-----------------------------------------

	//-------------------------------------------材料数据预加载方法-----------------------------------------
	//材料-合成配方
	static TArray<FMaterial_CardSynthesisBlueprint_Data> Global_SourceMaterialData_Blueprint;
	//材料-合成配方材料
	static TArray<FMaterial_CardSynthesisMaterial_Data> Global_SourceMaterialData_BlueprintMaterial;
	//材料-转职材料
	static TArray<FMaterial_CardChangeJobMaterial_Data> Global_SourceMaterialData_ChangeJobMaterial;
	//材料-香料
	static TArray<FMaterial_SpicesMaterial_Data> Global_SourceMaterialData_Spices;
	//材料-四叶强化草
	static TArray<FMaterial_CloverMaterial_Data> Global_SourceMaterialData_Clover;
	//材料-防御卡技能书
	static TArray<FCardSkillBooks_Data> Global_SourceMaterialData_CardSkillBooks;
	//材料-兑换券
	static TArray<FMaterial_TicketMaterial_Data> Global_SourceMaterialData_Tickets;
	//材料-强化水晶
	static TArray<FMaterial_Crystal_Data> Global_SourceMaterialData_Crystals;
	//材料-武器开槽钻头
	static TArray<FMaterial_Bit_Data> Global_SourceMaterialData_Bits;
	//材料-关卡钥匙
	static TArray<FMaterial_LevelKey_Data> Global_SourceMaterialData_LevelKeys;
	//-------------------------------------------材料数据预加载方法-----------------------------------------

	//-------------------------------------------装备数据预加载方法-----------------------------------------
	//背包
	static TArray<FEquipment_Bag_Data> Global_SourceEquipmentData_Bag;
	//服装
	static TArray<FEquipment_PlayerEquipment_Data> Global_SourceEquipmentData_PlayerEquipment;
	//礼盒
	static TArray<FEquipment_GiftBox_Data> Global_SourceEquipmentData_GiftBox;
	//角色主武器
	static TArray<FEquipment_WeaponFirst_Data> Global_SourceEquipmentData_WeaponFirst;
	//角色副武器
	static TArray<FEquipment_WeaponSecond_Data> Global_SourceEquipmentData_WeaponSecond;
	//角色超级武器
	static TArray<FEquipment_WeaponSuper_Data> Global_SourceEquipmentData_WeaponSuper;

	//角色宝石{激光宝石类型}
	static TArray<FEquipment_GemLaser_Data> Global_SourceEquipmentData_WeaponGem_Frist_Laser;
	//角色宝石{攻击宝石类型}
	static TArray<FEquipment_GemAttack_Data> Global_SourceEquipmentData_WeaponGem_Second_Attack;
	//-------------------------------------------装备数据预加载方法-----------------------------------------


	//-------------------------------------------其他数据预加载方法-----------------------------------------
	//邮件数据
	static TArray<FEquipment_FMail_Data> Global_SourceMailData_Mail;

	//-------------------------------------------其他数据预加载方法-----------------------------------------
public:
	//卡片数据初始化加载器
	UFUNCTION()
		static void InitCardsDatasLoader();
	//卡片数据卸载器
	UFUNCTION()
		static void UnLoadCardsDatasLoader();
};
