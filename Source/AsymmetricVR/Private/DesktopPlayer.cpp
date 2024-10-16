// Fill out your copyright notice in the Description page of Project Settings.

#include "DesktopPlayer.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Interactible.h"

// Sets default values
ADesktopPlayer::ADesktopPlayer() {
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // Set size for collision capsule
  GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

  // Create a CameraComponent
  FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
  FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
  FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  InteractionBox = CreateOptionalDefaultSubobject<UBoxComponent>("InteractionBox");

  if (InteractionBox) {
    InteractionBox->SetupAttachment(GetCapsuleComponent());
    InteractionBox->SetRelativeTransform(FTransform(FVector(80.0, 0.0, 60.0)));
  }
}

// Called when the game starts or when spawned
void ADesktopPlayer::BeginPlay() { Super::BeginPlay(); }

void ADesktopPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // Set up action bindings
  if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    // Jumping
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    // Moving
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Move);

    // Looking
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Look);

    // Grabbing
    EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Grab);

    // Interacting
    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Interact);
  }
}

void ADesktopPlayer::Move(const FInputActionValue &Value) {
  FVector2D MovementVector = Value.Get<FVector2D>();

  if (Controller != nullptr) {
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
  }
}

void ADesktopPlayer::Look(const FInputActionValue &Value) {
  FVector2D LookAxisVector = Value.Get<FVector2D>();

  if (Controller != nullptr) {
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
  }
}

void ADesktopPlayer::Interact() {
  TArray<AActor *> OverlappingActors;

  this->InteractionBox->GetOverlappingActors(OverlappingActors);

  for (auto Actor : OverlappingActors) {
    if (IsValid(Actor) && Actor != this) {
      auto InteractibleComponent = Actor->GetComponentByClass<UInteractible>();

      if (IsValid(InteractibleComponent))
        InteractibleComponent->OnInteract(this);
    }
  }
}

void ADesktopPlayer::Grab() {
  if (this->GrabbedObject != nullptr && IsValid(this->GrabbedObject)) {
    this->GrabbedObject->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
    this->GrabbedObject = nullptr;
    return;
  }

  TArray<AActor *> OverlappingActors;

  this->InteractionBox->GetOverlappingActors(OverlappingActors);

  for (auto Actor : OverlappingActors) {
    if (IsValid(Actor) && Actor != this) {
      Actor->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
      this->GrabbedObject = Actor;
      break;
    }
  }
}

// Called every frame
void ADesktopPlayer::Tick(const float DeltaTime) { Super::Tick(DeltaTime); }

void ADesktopPlayer::DummyAction(const FInputActionValue &Value) {
  UE_LOG(LogTemp, Warning, TEXT("DesktopPlayer - Dummy action triggered"));
}
