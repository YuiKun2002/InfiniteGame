// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_CardSkillTable.h"
#include <Components/Image.h>

bool UUI_CardSkillTable::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Head = this->GetWidgetComponent<UImage>(this, TEXT("Head"));
	this->M_Pro = this->GetWidgetComponent<UImage>(this, TEXT("pro"));

	return true;
}

void UUI_CardSkillTable::InitData(FSkillBookData _Data)
{
	int32 LIndex = 0;

	if (_Data.M_LV == 0 || _Data.M_LV == 1)
		LIndex = 0;
	else if (_Data.M_LV > 1 && _Data.M_LV < 5)
		LIndex = _Data.M_LV - 1;
	else
		LIndex = 3;

	//如果等级为0设置头像为灰色
	if (_Data.M_LV == 0)
	{
		//设置头像
		UWidgetBase::SetImageBrush(this->M_Head, FString(TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/CardSkillBook/") + _Data.M_HeadNames[LIndex] + TEXT(".") + _Data.M_HeadNames[LIndex] + TEXT("'")),FVector(0.4f,0.4f,0.4f));
	}
	else {
		//设置头像
		UWidgetBase::SetImageBrush(this->M_Head, FString(TEXT("Texture2D'/Game/Resource/Texture/PlayerItems/CardSkillBook/") + _Data.M_HeadNames[LIndex] + TEXT(".") + _Data.M_HeadNames[LIndex] + TEXT("'")));
	}
	
	//设置名称
	this->M_Name = _Data.M_Name;
	//设置等级
	this->M_Lv = FString(TEXT("Lv:") + FString::FromInt(_Data.M_LV));
	//设置提示
	this->M_Tip = FString(TEXT("(") + _Data.M_Tip + TEXT(")"));
	//经验
	this->M_Ex = FString(FString::FromInt(_Data.M_CurrentEx) + TEXT("/") + FString::FromInt(_Data.M_CurrentTotalEx));
	//更新进度条
	UWidgetBase::SetWidgetTransformProgress(this->M_Pro, 3.f, 219.f, ((_Data.M_CurrentEx + 0.f) / _Data.M_CurrentTotalEx));
}

void UUI_CardSkillTable::PlayAnimation_1()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

void UUI_CardSkillTable::LoadData()
{

}
