// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/ObstacleActor.h"

void AObstacleActor::SetLine(FLine Line)
{
	this->CurLine = Line;
}

FLine AObstacleActor::GetLine()
{
	return this->CurLine;
}
