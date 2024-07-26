// Copyright (C) 2020, @Author: AngrySteiner
// This file is mainly copied from Engine/Plugins/Experimental/GeometryProcessing/Source/GeometricObjects/Public/Polygon2.h

#pragma once

#include "CoreMinimal.h"

#include "Polygon2D.generated.h"

USTRUCT()
struct POLYGONBUTTON_API FPolygon2D
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Data")
	TArray<FVector2D> Vertices;

public:

	FPolygon2D() {}

	FPolygon2D(const FPolygon2D& Copy) : Vertices(Copy.Vertices) {}

	FPolygon2D(const TArray<FVector2D>& VertexList) : Vertices(VertexList) {}

	const FVector2D& operator[](int Index) const
	{
		return Vertices[Index];
	}

	FVector2D& operator[](int Index)
	{
		return Vertices[Index];
	}

	void Reverse()
	{
		int32 j = Vertices.Num() - 1;
		for (int32 i = 0; i < j; ++i, --j)
		{
			Swap(Vertices[i], Vertices[j]);
		}
	}

	void InsertVertex(int Index, const FVector2D& NewVertex)
	{
		Vertices.Insert(NewVertex, Index);
	}

	void RemoveVertex(int Index)
	{
		Vertices.RemoveAt(Index);
	}

	void PushVertex(const FVector2D& NewVertex)
	{
		Vertices.Push(NewVertex);
	}

	void PopVertex()
	{
		Vertices.Pop(false);
	}

	int Num() const
	{
		return Vertices.Num();
	}

	bool Contains(const FVector2D& QueryPoint) const
	{
		int WindingNumber = 0;
		int N = Vertices.Num();
		FVector2D A = Vertices[0];
		FVector2D B = FVector2D::ZeroVector;
		for (int i = 0; i < N; ++i) 
		{
			B = Vertices[(i + 1) % N];
			if (A.Y <= QueryPoint.Y)
			{
				if (B.Y > QueryPoint.Y)
				{
					if (FVector2D::CrossProduct(B - A, QueryPoint - A) > 0)
					{
						++WindingNumber;
					}
				}
			}
			else
			{
				if (B.Y <= QueryPoint.Y)
				{
					if (FVector2D::CrossProduct(B - A, QueryPoint - A) < 0)
					{
						--WindingNumber;
					}
				}
			}
			A = B;
		}
		return WindingNumber != 0;
	}
};


