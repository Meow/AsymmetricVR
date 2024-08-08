// Fill out your copyright notice in the Description page of Project Settings.

#include "VrPlayer.h"
#include "HeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ParentRelativeAttachmentComponent.h"
#include "CoreUObject.h"
#include "GripMotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Grippables/GrippableActor.h"
#include "Interactible.h"

// Sets default values
AVrPlayer::AVrPlayer() {
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  LeftGrabSphere = CreateOptionalDefaultSubobject<USphereComponent>("LeftGrabSphere");

  if (LeftGrabSphere) {
    LeftGrabSphere->SetupAttachment(LeftMotionController);
    LeftGrabSphere->SetSphereRadius(10.0f);
    LeftGrabSphere->SetRelativeTransform(FTransform(FVector(0.0, 5.0, 0.0)));
  }

  RightGrabSphere = CreateOptionalDefaultSubobject<USphereComponent>("RightGrabSphere");

  if (RightGrabSphere) {
    RightGrabSphere->SetupAttachment(RightMotionController);
    RightGrabSphere->SetSphereRadius(10.0f);
    RightGrabSphere->SetRelativeTransform(FTransform(FVector(0.0, -5.0, 0.0)));
  }
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
    EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVrPlayer::BeginGrabLeft);
    EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVrPlayer::ReleaseGrabLeft);

    EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVrPlayer::BeginGrabRight);
    EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Completed, this, &AVrPlayer::ReleaseGrabRight);

    // Interacting
    EnhancedInputComponent->BindAction(InteractLeftAction, ETriggerEvent::Triggered, this, &AVrPlayer::InteractLeft);
    EnhancedInputComponent->BindAction(InteractRightAction, ETriggerEvent::Triggered, this, &AVrPlayer::InteractRight);
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

void AVrPlayer::BeginGrip(const USphereComponent *const Sphere, UGripMotionControllerComponent *const GripController) const {
  UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Begin grip!"));

  // No grippy if we already gripped something!
  if (GripController->HasGrippedObjects())
    return;

  TArray<AActor *> OverlappingActors;

  Sphere->GetOverlappingActors(OverlappingActors);

  for (auto Actor : OverlappingActors) {
    if (IsValid(Actor) && Actor != this) {
      FTransform Offset(GripController->GetComponentLocation());

      Offset.SetRotation(Actor->GetActorRotation().Quaternion());

      UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Found something to grip, doing so!"));

      if (GripController->GripActor(Actor, Offset))
        break;
    }
  }
}

void AVrPlayer::ReleaseGrip(UGripMotionControllerComponent *const GripController) const {
  UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Release grip!"));

  if (!GripController->HasGrippedObjects())
    return;

  TArray<FBPActorGripInformation> ActorGrips;

  GripController->GetAllGrips(ActorGrips);

  for (const auto &GrippedObject : ActorGrips) {
    auto Actor = GrippedObject.GetGrippedActor();

    UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Releasing grip on held object"));

    if (IsValid(Actor)) {
      GripController->DropActor(Actor, false);
    }
  }
}

void AVrPlayer::BeginGrabRight() { BeginGrip(RightGrabSphere, RightMotionController); }

void AVrPlayer::BeginGrabLeft() { BeginGrip(LeftGrabSphere, LeftMotionController); }

void AVrPlayer::ReleaseGrabRight() { ReleaseGrip(RightMotionController); }

void AVrPlayer::ReleaseGrabLeft() { ReleaseGrip(LeftMotionController); }

void AVrPlayer::Interact(const USphereComponent *const Sphere) {
  TArray<AActor *> OverlappingActors;

  Sphere->GetOverlappingActors(OverlappingActors);

  for (auto Actor : OverlappingActors) {
    if (IsValid(Actor) && Actor != this) {
      auto InteractibleComponent = Actor->GetComponentByClass<UInteractible>();

      if (IsValid(InteractibleComponent)) {
        InteractibleComponent->OnInteract.Broadcast(this, nullptr);
      }
    }
  }
}

void AVrPlayer::InteractRight() { Interact(RightGrabSphere); }

void AVrPlayer::InteractLeft() { Interact(LeftGrabSphere); }

void AVrPlayer::DummyAction(const FInputActionValue &Value) {
  UE_LOG(LogTemp, Warning, TEXT("VrPlayer - Dummy action triggered"));
}
