// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StudentData.h"

UMyGameInstance::UMyGameInstance()
{
}

void UMyGameInstance::Init()
{
    Super::Init();

    // 구조체 객체 생성.
    FStudentData RawDataSource(23, TEXT("최재우"));

    // 파일로 저장하기 위한 경로 생성.
    const FString SavePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Saved"));

    // 파일 저장 테스트를 위한 구간.
    {
        // 파일 이름.
        const FString RawDataFileName(TEXT("RawData.bin"));

        // 저장 경로 + 파일 이름.
        FString RawDataAbsolutePath = FPaths::Combine(SavePath, RawDataFileName);

        UE_LOG(LogTemp, Log, TEXT("변경할 파일 전체 경로: %s"), *RawDataAbsolutePath);

        FPaths::MakeStandardFilename(RawDataAbsolutePath);

        // 직렬화 사용해서 저장.
        FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*RawDataAbsolutePath);
        if (RawFileWriterAr)
        {
            *RawFileWriterAr << RawDataSource.Order;
            *RawFileWriterAr << RawDataSource.Name;

            RawFileWriterAr->Close();
            delete RawFileWriterAr;
            RawFileWriterAr = nullptr;
        }
    }
}
