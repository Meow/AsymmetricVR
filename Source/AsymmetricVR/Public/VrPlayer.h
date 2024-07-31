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
  TObjectPtr<AVrTeleportVisualizer> TeleportVisualizer;

  /** Jump Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> JumpAction;

  /** Move Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> MoveAction;

  /** Look Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> LookAction;

  /** Interaction Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> InteractAction;

  /** Grab (right hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> GrabRightAction;

  /** Grab (left hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> GrabLeftAction;

  /** Teleport (right hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> TeleportRightAction;

  /** Teleport (left hand) Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> TeleportLeftAction;

  /** Use continuous locomotion instead of teleportation. Disables teleportation. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  bool ContinuousLocomotion = true;

private:
  bool IsTeleporting = false;

  bool TeleportRightHand = false;

public:
  // Sets default values for this pawn's properties
  AVrPlayer();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void Move(const FInputActionValue &Value);

  void Look(const FInputActionValue &Value);

  void BeginTeleportRight();

  void BeginTeleportLeft();

  void BeginTeleport(bool RightHand = false);

  void Teleport();

  void DummyAction(const FInputActionValue &Value);

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
