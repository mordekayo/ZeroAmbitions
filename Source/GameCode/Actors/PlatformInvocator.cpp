// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformInvocator.h"

#include "Kismet/GameplayStatics.h"


void APlatformInvocator::Invoke()
{
	OnInvoctorActivated.Broadcast();

	APlayerController* FirstPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GEngine->AddOnScreenDebugMessage(701, 10.0f, FColor::Blue, FString::Printf(TEXT("1C %i"),IsValid(FirstPlayerController)));
	APlayerController* SecondPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	GEngine->AddOnScreenDebugMessage(702, 10.0f, FColor::Blue, FString::Printf(TEXT("1C %i"),IsValid(SecondPlayerController)));
	APlayerController* ThirdPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 2);
	GEngine->AddOnScreenDebugMessage(703, 10.0f, FColor::Blue, FString::Printf(TEXT("1C %i"),IsValid(ThirdPlayerController)));
}

