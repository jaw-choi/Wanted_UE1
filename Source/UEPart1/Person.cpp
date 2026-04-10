// Fill out your copyright notice in the Description page of Project Settings.


#include "Person.h"

UPerson::UPerson()
{
    // 기본 값 설정.
    Name = TEXT("홍길동"); //-> CDO에 의해 관리됨
    Year = 1;
}

void UPerson::DoLesson()
{
    // 구현 내용 (비어있어도 됩니다)
}