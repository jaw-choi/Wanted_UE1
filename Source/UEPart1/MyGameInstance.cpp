// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "Teacher.h"
//#include "Staff.h"

UMyGameInstance::UMyGameInstance()
{
    // 기본 값은 CDO라는 특별한 템플릿 객체에 저장됨.
    SchoolName = TEXT("기본학교");
}

void UMyGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("================================================"));

    // TArray는 언리얼 엔진이 지원하는 동적배열
    // STL의 std::vector와 유사한 기능을 제공.
    // 언리얼 오브젝트에 특화된 동적 배열.
    TArray<UPerson*> Persons =
    {
        NewObject<UStudent>(),
        NewObject<UTeacher>(),
        //NewObject<UStaff>()
    };

    for (const auto* Person : Persons)
    {
        UE_LOG(LogTemp, Log, TEXT("구성원 이름: %s"), *Person->GetName());
    }
    
    UE_LOG(LogTemp, Log, TEXT("================================================"));

    for (auto* Person : Persons)
    {
        // 인터페이스로 형변환.
        ILessonInterface* LessonInterface = Cast<ILessonInterface>(Person);
        if (LessonInterface)
        {
            LessonInterface->DoLesson();
            UE_LOG(LogTemp, Log, TEXT("%s 님은 쉅에 참여할 수 있습니다."), *Person->GetName());
        }
        else
        {
            LessonInterface->DoLesson();
            UE_LOG(LogTemp, Log, TEXT("%s 님은 쉅에 참여할 수 없 습니다."), *Person->GetName());
        }
    }
    UE_LOG(LogTemp, Log, TEXT("================================================"));



}
