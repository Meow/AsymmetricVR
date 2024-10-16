// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AsymmetricVrPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()

class ASYMMETRICVR_API AAsymmetricVrPlayerController
  : public APlayerController {
  GENERATED_BODY()

protected:
  /** Input Mapping Context to be used for player input */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
  UInputMappingContext *InputMappingContext;

  virtual void BeginPlay() override;
};
