// Fill out your copyright notice in the Description page of Project Settings.


#include "DesktopPlayer.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

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
}

// Called when the game starts or when spawned
void ADesktopPlayer::BeginPlay() {
  Super::BeginPlay();
}

void ADesktopPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // Set up action bindings
  if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    // Jumping
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    // Moving
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Move);

    // Looking
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADesktopPlayer::Look);
  }
}

void ADesktopPlayer::Move(const FInputActionValue& Value) {
  // input is a Vector2D
  FVector2D MovementVector = Value.Get<FVector2D>();

  //UE_LOG(LogTemp, Warning, TEXT("MovementVector(%f %f)"), MovementVector.X, MovementVector.Y);
  //UE_LOG(LogTemp, Warning, TEXT("GetActorForwardVector(%f %f %f)"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z);
  //UE_LOG(LogTemp, Warning, TEXT("GetActorRightVector(%f %f %f)"), GetActorRightVector().X, GetActorRightVector().Y, GetActorRightVector().Z);

  if (Controller != nullptr) {
    // add movement 
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
  }
}

void ADesktopPlayer::Look(const FInputActionValue& Value) {
  // input is a Vector2D
  FVector2D LookAxisVector = Value.Get<FVector2D>();

  if (Controller != nullptr) {
    // add yaw and pitch input to controller
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
  }
}

// Called every frame
void ADesktopPlayer::Tick(const float DeltaTime) {
  Super::Tick(DeltaTime);
}
