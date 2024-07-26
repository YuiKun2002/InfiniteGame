// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/Synthesis/SynModel_Structures.h"
#include "UI/WidgetBase.h"
#include <Components/Image.h>
#include <Components/ScrollBox.h>
#include <Components/UniformGridPanel.h>
#include <Components/Button.h>
#include <Components/Widget.h>

void USynModel_Structures::InitComponentBindGemSlot(UWidgetBase* _widget, FSynModel_GemSlot& _comps, const TArray<FString>& _CompsName)
{
	if (IsValid(_widget))
	{
		_comps.M_ItemHead = _widget->GetWidgetComponent<UImage>(_widget, _CompsName[0]);
		_comps.M_ItemGrade = _widget->GetWidgetComponent<UImage>(_widget, _CompsName[1]);
		_comps.M_ItemBorderStyle = _widget->GetWidgetComponent<UImage>(_widget, _CompsName[2]);
		_comps.M_ItemAttachmentButton = _widget->GetWidgetComponent<UButton>(_widget, _CompsName[3]);
		_comps.M_ItemClickButton = _widget->GetWidgetComponent<UButton>(_widget, _CompsName[4]);
	}
}

void USynModel_Structures::InitComponentBindGemSlotButton(UObject* _Object, FSynModel_GemSlot& _comps, const TArray<FString>& _FunctionName)
{
	FScriptDelegate Button_0, Button_1;
	if (!_FunctionName[0].Equals(TEXT("")))
	{
		if (IsValid(_comps.M_ItemAttachmentButton))
		{
			Button_0.BindUFunction(_Object, *_FunctionName[0]);
			_comps.M_ItemAttachmentButton->OnClicked.Add(Button_0);
		}
	}

	if (!_FunctionName[1].Equals(TEXT("")))
	{
		if (IsValid(_comps.M_ItemClickButton))
		{
			Button_1.BindUFunction(_Object, *_FunctionName[1]);
			_comps.M_ItemClickButton->OnClicked.Add(Button_1);
		}
	}
}
