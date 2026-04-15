#pragma once

#include "CoreMinimal.h"

struct FStudentData
{
    FStudentData();
    FStudentData(int32 InOrder, const FString& InName)
        : Order(InOrder), Name(InName) 
    {
    }

    int32 Order = -1;
    FString Name = TEXT("최재우");
};