// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VrTeleportVisualizer.h"
#include "VRCharacter.h"
#include "VrPlayer.generated.h"

class UInputAction;
struct FInputActionValue;

UCLASS()

class ASYMMETRICVR_API AVrPlayer : public AVRCharacter {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsymmetricVr)
  AVrTeleportVisualizer *TeleportVisualizer;

  /** Jump Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *JumpAction;

  /** Move Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *MoveAction;

  /** Look Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *LookAction;

  /** Interaction Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *InteractAction;

  /** Grab (right hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *GrabRightAction;

  /** Grab (left hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *GrabLeftAction;

  /** Teleport (right hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *TeleportRightAction;

  /** Teleport (left hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *TeleportLeftAction;

// private:
//   IHeadMountedDisplay *HMD;
//   TSharedPtr<IStereoRendering, ESPMode::ThreadSafe> Stereo;

public:
  // Sets default values for this pawn's properties
  AVrPlayer();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void Move(const FInputActionValue &Value);

  void Look(const FInputActionValue &Value);

  void DummyAction(const FInputActionValue &Value);

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
