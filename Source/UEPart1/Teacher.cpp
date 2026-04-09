// Fill out your copyright notice in the Description page of Project Settings.


#include "Teacher.h"

UTeacher::UTeacher()
{
    Name = TEXT("최재우");
    Year = 4;
    Id = 2;
}

void UTeacher::DoLesson()
{
    UE_LOG(LogTemp, Log, TEXT("%d년차 선생님 %s님이 수업을 진행합니다."), Year, *Name);
}
