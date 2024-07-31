// Fill out your copyright notice in the Description page of Project Settings.

#include "VrPlayer.h"
#include "HeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ParentRelativeAttachmentComponent.h"

// Sets default values
AVrPlayer::AVrPlayer() {
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVrPlayer::BeginPlay() {
  Super::BeginPlay();

  // if (GEngine) {
  //   HMD    = GEngine->XRSystem->GetHMDDevice();
  //   Stereo = GEngine->XRSystem->GetStereoRenderingDevice();
  // } else {
  //   UE_LOG(LogTemp, Warning, TEXT("Engine pointer is not available"));
  //   return;
  // }

  // if (!HMD || !Stereo) {
  //   UE_LOG(LogTemp, Warning, TEXT("VR is not available"));
  //   return;
  // }

  // if (HMD->IsHMDEnabled() && HMD->IsHMDConnected() && Stereo->IsStereoEnabled())
  //   GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Type::Stage);
}

// Called every frame
void AVrPlayer::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void AVrPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // Set up action bindings
  if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    // Moving
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVrPlayer::Move);

    // Looking
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVrPlayer::Look);

    // Teleporting
    EnhancedInputComponent->BindAction(TeleportLeftAction, ETriggerEvent::Started, this, &AVrPlayer::BeginTeleportLeft);
    EnhancedInputComponent->BindAction(TeleportLeftAction, ETriggerEvent::Completed, this, &AVrPlayer::Teleport);

    EnhancedInputComponent->BindAction(
      TeleportRightAction, ETriggerEvent::Started, this, &AVrPlayer::BeginTeleportRight
    );
    EnhancedInputComponent->BindAction(TeleportRightAction, ETriggerEvent::Completed, this, &AVrPlayer::Teleport);

    // Jumping
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    // Grabbing
    EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Triggered, this, &AVrPlayer::DummyAction);
    EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Triggered, this, &AVrPlayer::DummyAction);

    // Interacting
    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AVrPlayer::DummyAction);
  }
}

void AVrPlayer::Move(const FInputActionValue &Value) {
  if (IsTeleporting || !ContinuousLocomotion)
    return;

  FVector2D MovementVector = Value.Get<FVector2D>();

  if (Controller != nullptr) {
    AddMovementInput(ParentRelativeAttachment->GetForwardVector(), MovementVector.Y);
    AddMovementInput(ParentRelativeAttachment->GetRightVector(), MovementVector.X);
  }
}

void AVrPlayer::Look(const FInputActionValue &Value) {
  FVector2D LookAxisVector = Value.Get<FVector2D>();

  if (Controller != nullptr) {
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
  }
}

void AVrPlayer::BeginTeleportRight() { this->BeginTeleport(true); }

void AVrPlayer::BeginTeleportLeft() { this->BeginTeleport(false); }

void AVrPlayer::BeginTeleport(bool RightHand) {
  // prevent triggering by another hand while teleporting with one hand
  if (IsTeleporting || ContinuousLocomotion)
    return;

  UE_LOG(
    LogTemp, Warning, TEXT("VrPlayer - Begin teleport! Using right hand - %s"), RightHand ? TEXT("yes") : TEXT("no")
  );

  IsTeleporting     = true;
  TeleportRightHand = RightHand;
}

void AVrPlayer::Teleport() {
  // prevent accidental triggering
  if (!IsTeleporting || ContinuousLocomotion)
    return;

  UE_LOG(
    LogTemp,
    Warning,
    TEXT("VrPlayer - End teleport! Using right hand - %s"),
    TeleportRightHand ? TEXT("yes") : TEXT("no")
  );

  IsTeleporting = false;
}

void AVrPlayer::DummyAction(const FInputActionValue &Value) {
  UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Dummy action triggered"));
}
