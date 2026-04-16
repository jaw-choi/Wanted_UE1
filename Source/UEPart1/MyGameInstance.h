// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPART1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    UMyGameInstance();

protected:
    virtual void Init() override;

    void SaveStudentPackage() const;

    void LoadStudentPackage() const;

    void LoadStudentObject() const;
private:

    static const FString PackageName;

    static const FString AssetName;

    UPROPERTY()
    TObjectPtr<class UStudent> StudentSource;
	
};
