// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonInterface.h"

// Add default functionality here for any ILessonInterface functions that are not pure virtual.
void ILessonInterface::DoLesson()
{
    UE_LOG(LogTemp, Log, TEXT("수업에 참여합니다."));
}