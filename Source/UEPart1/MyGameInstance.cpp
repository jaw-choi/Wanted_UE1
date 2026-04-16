// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StudentData.h"
#include "Student.h"
#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"

// static 변수 초기화.
// /Game -> 프로젝트 경로 /Content 폴더 가리킴
const FString UMyGameInstance::PackageName(TEXT("/Game/Student"));
const FString UMyGameInstance::AssetName(TEXT("Student"));

void PrintStudentInfo(
    const UStudent* InStudent,
    const FString& InTag)
{
    UE_LOG(
        LogTemp,
        Log,
        TEXT("[%s] 이름: %s, 순번: %d"),
        *InTag,
        *InStudent->GetName(),
        InStudent->GetOrder()
    );
}

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
            //*RawFileWriterAr << RawDataSource.Order;
            //*RawFileWriterAr << RawDataSource.Name;
            *RawFileWriterAr << RawDataSource;

            RawFileWriterAr->Close();
            delete RawFileWriterAr;
            RawFileWriterAr = nullptr;
        }

        // 저장한 파일에서 읽어오기.
        // 역직렬화(Deserialization) 사용해서 읽어오기.
        FStudentData RawDataDeserialized;

        // 파일 읽기를 위한 아카이브 얻어오기.
        FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*RawDataAbsolutePath);
        if (RawFileReaderAr)
        {
            // 구조체로 값 전달.일고 쓰기 모두 << 연산자 오버로딩으로 처리.
            *RawFileReaderAr << RawDataDeserialized;

            // 파일 닫기.
            RawFileReaderAr->Close();

            // 객체 메모리 해제.
            delete RawFileReaderAr;
            RawFileReaderAr = nullptr;

            UE_LOG(
                LogTemp,
                Log,
                TEXT("[RawData] 이름: %s, 순번: %d"),
                *RawDataDeserialized.Name,
                RawDataDeserialized.Order);
        }
    }

    // 언리얼 오브젝트 생성.
    StudentSource = NewObject<UStudent>();
    StudentSource->SetName(TEXT("최재우"));
    StudentSource->SetOrder(325);

    // 직렬화 테스트를 위한 구간 나누기.
    {
        // 저장할 파일 이름 값.
        const FString& ObjectDataFileName(TEXT("ObjectData.bin"));

        // 파일이름을 포함한 경로 생성.
        FString ObjectDataAbsolutePath = FPaths::Combine(SavePath, ObjectDataFileName);

        // 경로 값 정리.
        FPaths::MakeStandardFilename(ObjectDataAbsolutePath);

        // 메모리를 담을 버퍼.
        TArray<uint8> BufferArray;
        FMemoryWriter MemoryWriter(BufferArray);
        // MemoryWriter를 사용해서
        // 언리얼 오브젝트를 직렬화해 값을 기록.
        StudentSource->Serialize(MemoryWriter);

        // 파일에 기록.
        TUniquePtr<FArchive> FileWriterAr
            = TUniquePtr<FArchive>(
                IFileManager::Get().CreateFileWriter(
                    *ObjectDataAbsolutePath)
            );
        // 파일을 잘 열었는지 확인.
        if (FileWriterAr)
        {
            // 파일에 기록 처리.
            *FileWriterAr << BufferArray;
            // 파일 닫기.
            FileWriterAr->Close();
        }

        // 언리얼 오브젝트로 복원.
        TArray<uint8> BufferArrayDeserialized;
        TUniquePtr<FArchive> FileReaderAr
            = TUniquePtr<FArchive>(
                IFileManager::Get().CreateFileReader(
                    *ObjectDataAbsolutePath)
            );

        if (FileReaderAr)
        {
            // 파일에서 읽어오기.
            *FileReaderAr << BufferArrayDeserialized;
            // 파일 닫기.
            FileReaderAr->Close();

            // 메모리를 담을 버퍼로부터 언리얼 오브젝트로 역직렬화 처리.
            FMemoryReader MemoryReader(BufferArrayDeserialized);

            UStudent* StudentDest = NewObject<UStudent>();
            StudentDest->Serialize(MemoryReader);

            UE_LOG(
                LogTemp,
                Log,
                TEXT("[ObjectData] 이름: %s, 순번: %d"),
                *StudentDest->GetName(),
                StudentDest->GetOrder());
        }
    }

    // JSON(JavaScript Object Notation) 직렬화.
    {
        const FString JsonDataFileName(TEXT("StudentJsonData.txt"));

        // 저장할 파일 경로 값.
        FString JsonDataAbsolutePath = FPaths::Combine(SavePath, JsonDataFileName);
        FPaths::MakeStandardFilename(JsonDataAbsolutePath);

        // JSON 직렬화 객체 생성.
        // UObject -> JSON object -> JSON string -> 파일 저장
        TSharedRef<FJsonObject> JsonObjectRef = MakeShared<FJsonObject>();

        FJsonObjectConverter::UStructToJsonObject(
            StudentSource->StaticClass(),
            StudentSource,
            JsonObjectRef
        );

        // 직렬화.
        FString JsonOutString;

        // Archive의 일종
        TSharedRef<TJsonWriter<TCHAR>> JsonWriterAr
            = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
        if (FJsonSerializer::Serialize(JsonObjectRef, JsonWriterAr))
        {
            // 성공한 경우 파일에 저장.
            FFileHelper::SaveStringToFile(JsonOutString, *JsonDataAbsolutePath);

            //JsonWriterAr->Close();
        }

        // 역직렬화.
        FString JsonInString;
        // 파일에서 문자열로 읽어오기.
        FFileHelper::LoadFileToString(JsonInString, *JsonDataAbsolutePath);

        UE_LOG(LogTemp, Log, TEXT("읽어온 JSON 문자열: %s"), *JsonInString);

        // 역직렬화를 위한 아카이브 생성.
        TSharedRef<TJsonReader<TCHAR>> JsonReaderAr
            = TJsonReaderFactory<TCHAR>::Create(JsonInString);

        TSharedPtr<FJsonObject> JsonObjectDeserialized = MakeShared<FJsonObject>();
        if (FJsonSerializer::Deserialize(JsonReaderAr, JsonObjectDeserialized))
        {
            // JSON object -> JSON string -> UObject
            UStudent* StudentDest = NewObject<UStudent>();

            if (FJsonObjectConverter::JsonObjectToUStruct(
                JsonObjectDeserialized.ToSharedRef(),
                StudentDest->StaticClass(),
                StudentDest
            ))
            {
                //UE_LOG(
                //    LogTemp,
                //    Log,
                //    TEXT("[JsonData] 이름: %s, 순번: %d"),
                //    *StudentDest->GetName(),
                //    StudentDest->GetOrder());
                PrintStudentInfo(StudentDest, TEXT("JsonData"));
            }
        }
    }

    //SaveStudentPackage();
    //LoadStudentPackage();

    // 경로 값을 사용해 오브젝트를 로드하는 함수 실행.
    LoadStudentObject();
}

void UMyGameInstance::LoadStudentPackage() const
{
    // 패키지 로드.
    UPackage* StudentPackage
        = LoadPackage(nullptr, *PackageName, LOAD_None);

    // 패키지 로드 실패 처리.
    if (!StudentPackage)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("패키지를 찾을 수 없습니다.")
        );
        return;
    }

    // 완전히 로드되도록 함수 실행.
    StudentPackage->FullyLoad();

    // 패키지 안에 있는 언리얼 오브젝트(애셋) 검색.
    UStudent* Student
        = FindObject<UStudent>(StudentPackage, *AssetName);

    if (Student)
    {
        // 로드한 결과 출력.
        PrintStudentInfo(Student, TEXT("FindObject Asset"));
    }
}

void UMyGameInstance::LoadStudentObject() const
{
    // 오브젝트 경로 값.
    const FString SoftObjectPath
        = FString::Printf(TEXT("%s.%s"),
            *PackageName, *AssetName
        );

    // 오브젝트 로드.
    UStudent* Student
        = LoadObject<UStudent>(nullptr, *SoftObjectPath);
    if (Student)
    {
        // 로드한 오브젝트 정보 출력.
        PrintStudentInfo(Student, TEXT("LoadObject Asset"));
    }
}

void UMyGameInstance::SaveStudentPackage() const
{
    // 패키지 생성.
    UPackage* StudentPackage = LoadPackage(nullptr,*PackageName,LOAD_None);

    if (StudentPackage)
    {
        StudentPackage->FullyLoad();

    }

    StudentPackage = CreatePackage(*PackageName);

    // 패키지에 사용할 플래그 설정.
    // RF_Public | RF_Standalone 두 플래그 값이 가장 일반적.
    EObjectFlags PackageFlags = RF_Public | RF_Standalone;

    // 패키지에 저장할 언리얼 오브젝트 생성.
    UStudent* Student = NewObject<UStudent>(
        StudentPackage,
        UStudent::StaticClass(),
        *AssetName,
        PackageFlags
    );

    Student->SetName(TEXT("최재우"));
    Student->SetOrder(11);

    // 서브 오브젝트 추가.
    const int32 SubObjectCount = 10;
    for (int32 i = 1; i <= SubObjectCount; i++)
    {
        FString SubObjectName = FString::Printf(TEXT("%s_Student%d"), *AssetName, i);
        // 객체 생성.
        UStudent* SubStudent = NewObject<UStudent>(
            Student,
            UStudent::StaticClass(),
            *SubObjectName,
            PackageFlags
        );

        SubStudent->SetName(FString::Printf(TEXT("학생%d"), i));
        SubStudent->SetOrder(i);
    }

    // 패키지 저장.
    FString PackageFileName = FPackageName::LongPackageNameToFilename(
        PackageName,
        FPackageName::GetAssetPackageExtension()
    );

    // 참고로 이렇게도 가능.
    //FString PackageFileName2
    //    = FPaths::Combine(
    //        FPlatformMisc::ProjectDir(),
    //        TEXT("Content"),
    //        FString::Printf(TEXT("%s%s"),
    //            *AssetName,
    //            *FPackageName::GetAssetPackageExtension())
    //    );
    FPaths::MakeStandardFilename(PackageFileName);

    // 저장할 옵션 설정.
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = PackageFlags;

    // 패키지 저장.
        if (UPackage::SavePackage(
                StudentPackage,
                nullptr,
                *PackageFileName,
                SaveArgs
        ))
        {
                UE_LOG(LogTemp, Log, TEXT("패키지 저장 성공: %s"), *PackageFileName);
        }
        else
        {
                UE_LOG(LogTemp, Log, TEXT("패키지 저장 실패: %s"), *PackageFileName);
        }

}

