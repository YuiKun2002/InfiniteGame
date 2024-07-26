// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/Fx/FxBase.h"

void AFxBase::PoolInit(class UObjectPoolManager* PoolManager)
{
	Super::PoolInit(PoolManager);

	this->OnInit();
}
