// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/GlobalDatas.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/FVMGameInstance.h"

TArray<FCard_ATK_Data> UGlobalDatas::Global_SourceData_CardATK;
//卡片源数据->生产
TArray<FCard_Spawn_Data> UGlobalDatas::Global_SourceData_Spawn;
//卡片源数据->防御
TArray<FCard_Defence_Data> UGlobalDatas::Global_SourceData_Defence;
//卡片源数据->功能
TArray<FCard_Function_Data> UGlobalDatas::Global_SourceData_Function;
//卡片源数据->范围攻击
TArray<FCard_RangeATK_Data> UGlobalDatas::Global_SourceData_RangeATK;
//-------------------------------------------卡片数据预加载方法-----------------------------------------

//-------------------------------------------材料数据预加载方法-----------------------------------------
//材料-合成配方
TArray<FMaterial_CardSynthesisBlueprint_Data> UGlobalDatas::Global_SourceMaterialData_Blueprint;
//材料-合成配方材料
TArray<FMaterial_CardSynthesisMaterial_Data> UGlobalDatas::Global_SourceMaterialData_BlueprintMaterial;
//材料-转职材料
TArray<FMaterial_CardChangeJobMaterial_Data> UGlobalDatas::Global_SourceMaterialData_ChangeJobMaterial;
//材料-香料
TArray<FMaterial_SpicesMaterial_Data> UGlobalDatas::Global_SourceMaterialData_Spices;
//材料-四叶强化草
TArray<FMaterial_CloverMaterial_Data> UGlobalDatas::Global_SourceMaterialData_Clover;
//材料-防御卡技能书
TArray<FCardSkillBooks_Data> UGlobalDatas::Global_SourceMaterialData_CardSkillBooks;
//材料-兑换券
TArray<FMaterial_TicketMaterial_Data> UGlobalDatas::Global_SourceMaterialData_Tickets;
//材料-强化水晶
TArray<FMaterial_Crystal_Data> UGlobalDatas::Global_SourceMaterialData_Crystals;
//材料-武器开槽钻头
TArray<FMaterial_Bit_Data> UGlobalDatas::Global_SourceMaterialData_Bits;
//材料-关卡钥匙
TArray<FMaterial_LevelKey_Data> UGlobalDatas::Global_SourceMaterialData_LevelKeys;
//-------------------------------------------材料数据预加载方法-----------------------------------------

//-------------------------------------------装备数据预加载方法-----------------------------------------
//背包
TArray<FEquipment_Bag_Data> UGlobalDatas::Global_SourceEquipmentData_Bag;
//服装
TArray<FEquipment_PlayerEquipment_Data> UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment;
//礼盒
TArray<FEquipment_GiftBox_Data> UGlobalDatas::Global_SourceEquipmentData_GiftBox;
//角色主武器
TArray<FEquipment_WeaponFirst_Data> UGlobalDatas::Global_SourceEquipmentData_WeaponFirst;
//角色副武器
TArray<FEquipment_WeaponSecond_Data> UGlobalDatas::Global_SourceEquipmentData_WeaponSecond;
//角色超级武器
TArray<FEquipment_WeaponSuper_Data> UGlobalDatas::Global_SourceEquipmentData_WeaponSuper;

//角色宝石{主动-激光宝石类型}
TArray<FEquipment_GemLaser_Data> UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Frist_Laser;
//角色宝石{被动-攻击宝石类型}
TArray<FEquipment_GemAttack_Data> UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Second_Attack;
//-------------------------------------------装备数据预加载方法-----------------------------------------


//-------------------------------------------其他数据预加载方法-----------------------------------------
//邮件数据
TArray<FEquipment_FMail_Data> UGlobalDatas::Global_SourceMailData_Mail;

void UGlobalDatas::InitCardsDatasLoader()
{
	/*----------------------------------------------防御卡数据加载----------------------------------------------------*/
	//获取攻击卡片数据
	UGameSystemFunction::GetDataTableRows<FCard_ATK_Data>(UCardBaseStruct::GetCardDataTablePath(ECardType::E_ATK), UGlobalDatas::Global_SourceData_CardATK);
	//获取生产卡片数据
	UGameSystemFunction::GetDataTableRows<FCard_Spawn_Data>(UCardBaseStruct::GetCardDataTablePath(ECardType::E_SPAWN), UGlobalDatas::Global_SourceData_Spawn);
	//获取防御卡片数据
	UGameSystemFunction::GetDataTableRows<FCard_Defence_Data>(UCardBaseStruct::GetCardDataTablePath(ECardType::E_DEFENCE), UGlobalDatas::Global_SourceData_Defence);
	//获取功能卡片数据
	UGameSystemFunction::GetDataTableRows<FCard_Function_Data>(UCardBaseStruct::GetCardDataTablePath(ECardType::E_Function), UGlobalDatas::Global_SourceData_Function);
	//获取范围攻击卡片数据
	UGameSystemFunction::GetDataTableRows<FCard_RangeATK_Data>(UCardBaseStruct::GetCardDataTablePath(ECardType::E_RangeATK), UGlobalDatas::Global_SourceData_RangeATK);


	/*----------------------------------------------材料数据加载----------------------------------------------------*/
	//获取防御卡合成配方数据
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisBlueprint_Data>(UCardBlueprintStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Blueprint);
	//获取防御卡合成配方材料数据
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisMaterial_Data>(UCardBlueprintMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_BlueprintMaterial);
	//获取防御卡转职材料数据
	UGameSystemFunction::GetDataTableRows<FMaterial_CardChangeJobMaterial_Data>(UCardChangeJobMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_ChangeJobMaterial);
	//获取香料数据
	UGameSystemFunction::GetDataTableRows<FMaterial_SpicesMaterial_Data>(USpicesMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Spices);
	//获取四叶草数据
	UGameSystemFunction::GetDataTableRows<FMaterial_CloverMaterial_Data>(UCloverMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Clover);
	//获取技能书数据
	UGameSystemFunction::GetDataTableRows<FCardSkillBooks_Data>(UCardSkillBookMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_CardSkillBooks);
	//获取兑换券数据
	UGameSystemFunction::GetDataTableRows<FMaterial_TicketMaterial_Data>(UTicketStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Tickets);
	//获取强化水晶
	UGameSystemFunction::GetDataTableRows<FMaterial_Crystal_Data>(UCrystalMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Crystals);
	//获取武器开槽钻头
	UGameSystemFunction::GetDataTableRows<FMaterial_Bit_Data>(UBitMaterialStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_Bits);
	//获取关卡钥匙
	UGameSystemFunction::GetDataTableRows<FMaterial_LevelKey_Data>(ULevelKeyStruct::DataTablePath, UGlobalDatas::Global_SourceMaterialData_LevelKeys);


	/*----------------------------------------------装备数据加载----------------------------------------------------*/
	//背包数据
	UGameSystemFunction::GetDataTableRows<FEquipment_Bag_Data>(UBagStruct::DataTablePath, UGlobalDatas::Global_SourceEquipmentData_Bag);
	//角色装备数据

	//男-着装
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Body, EPlayerEquipmentType::E_Man), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//女-着装
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Body, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//女-头发
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Hair, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//女-眼镜
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Glasses, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//不区分-帽子
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Cap, EPlayerEquipmentType::E_Base), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//不区分-眼睛
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Eye, EPlayerEquipmentType::E_Base), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//女-脸饰品
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Face, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//女-眼睛
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Eye, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//女-帽子
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Cap, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//不区分-翅膀
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_FlyItem, EPlayerEquipmentType::E_Base), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//翅膀-男
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(
		UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_FlyItem, EPlayerEquipmentType::E_Man), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//翅膀-女
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_FlyItem, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//套装-男
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Suit, EPlayerEquipmentType::E_Man), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);
	//套装-女
	UGameSystemFunction::GetDataTableRows<FEquipment_PlayerEquipment_Data>(UPlayerEquipmentStruct::GetDataTablePath(EPlayerEquipmentSlotPosition::E_Player_Suit, EPlayerEquipmentType::E_Men), UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment);

	//角色-主武器
	UGameSystemFunction::GetDataTableRows<FEquipment_WeaponFirst_Data>(UPlayerWepaonStruct::GetDataTablePath(EEquipment::E_PlayerWeaponFirst), UGlobalDatas::Global_SourceEquipmentData_WeaponFirst);
	//副武器
	UGameSystemFunction::GetDataTableRows<FEquipment_WeaponSecond_Data>(UPlayerWepaonStruct::GetDataTablePath(EEquipment::E_PlayerWeaponSecond), UGlobalDatas::Global_SourceEquipmentData_WeaponSecond);
	//超级武器
	UGameSystemFunction::GetDataTableRows<FEquipment_WeaponSuper_Data>(UPlayerWepaonStruct::GetDataTablePath(EEquipment::E_PlayerWeaponSuper), UGlobalDatas::Global_SourceEquipmentData_WeaponSuper);

	//主动技能-宝石{激光宝石类型}
	UGameSystemFunction::GetDataTableRows<FEquipment_GemLaser_Data>(UGemLaser::DataTablePath, UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Frist_Laser);
	//被动技能-宝石{攻击宝石类型}
	UGameSystemFunction::GetDataTableRows<FEquipment_GemAttack_Data>(UGemAttack::DataTablePath, UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Second_Attack);


	//礼盒数据
	UGameSystemFunction::GetDataTableRows<FEquipment_GiftBox_Data>(UGiftBoxStruct::DataTablePath, UGlobalDatas::Global_SourceEquipmentData_GiftBox);


	/*----------------------------------------------其他数据加载----------------------------------------------------*/
	//邮件数据
	UGameSystemFunction::GetDataTableRows<FEquipment_FMail_Data>(TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/MailData.MailData'"), UGlobalDatas::Global_SourceMailData_Mail);

}

void UGlobalDatas::UnLoadCardsDatasLoader()
{
	UGlobalDatas::Global_SourceData_CardATK.Empty();
	UGlobalDatas::Global_SourceData_Spawn.Empty();
	UGlobalDatas::Global_SourceData_Defence.Empty();
	UGlobalDatas::Global_SourceData_Function.Empty();
	UGlobalDatas::Global_SourceData_RangeATK.Empty();

	UGlobalDatas::Global_SourceMaterialData_Blueprint.Empty();
	UGlobalDatas::Global_SourceMaterialData_BlueprintMaterial.Empty();
	UGlobalDatas::Global_SourceMaterialData_ChangeJobMaterial.Empty();
	UGlobalDatas::Global_SourceMaterialData_Spices.Empty();
	UGlobalDatas::Global_SourceMaterialData_Clover.Empty();
	UGlobalDatas::Global_SourceMaterialData_CardSkillBooks.Empty();
	UGlobalDatas::Global_SourceMaterialData_Tickets.Empty();
	UGlobalDatas::Global_SourceMaterialData_Crystals.Empty();
	UGlobalDatas::Global_SourceMaterialData_Bits.Empty();
	UGlobalDatas::Global_SourceMaterialData_LevelKeys.Empty();

	UGlobalDatas::Global_SourceEquipmentData_Bag.Empty();
	UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment.Empty();
	UGlobalDatas::Global_SourceEquipmentData_GiftBox.Empty();
	UGlobalDatas::Global_SourceEquipmentData_WeaponFirst.Empty();
	UGlobalDatas::Global_SourceEquipmentData_WeaponSecond.Empty();
	UGlobalDatas::Global_SourceEquipmentData_WeaponSuper.Empty();
	UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Frist_Laser.Empty();
	UGlobalDatas::Global_SourceEquipmentData_WeaponGem_Second_Attack.Empty();

	UGlobalDatas::Global_SourceMailData_Mail.Empty();

	if (UFVMGameInstance::GetDebug())
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("数据-卸载完成"));
}