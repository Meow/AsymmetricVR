// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DesktopPlayer.generated.h"

class UCameraComponent;
class UInputAction;
class UBoxComponent;
struct FInputActionValue;

UCLASS(config = Game)

class ASYMMETRICVR_API ADesktopPlayer : public ACharacter {
  GENERATED_BODY()

  /** First person camera */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
  UCameraComponent *FirstPersonCameraComponent;

  /** Jump Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *JumpAction;

  /** Move Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *MoveAction;

  /** Look Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction *LookAction;

  /** Interaction Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *InteractAction;

  /** Grab Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction *GrabAction;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UBoxComponent> InteractionBox;

public:
  // Sets default values for this character's properties
  ADesktopPlayer();

protected:
  AActor *GrabbedObject;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  /** Called for movement input */
  void Move(const FInputActionValue &Value);

  /** Called for looking input */
  void Look(const FInputActionValue &Value);

  /** Called when interacting with stuff */
  void Interact();

  /* Called when grabbing stuff */
  void Grab();

  /** Placeholder: called for any unimplemented inputs */
  void DummyAction(const FInputActionValue &Value);

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
