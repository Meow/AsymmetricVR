// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabbable.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class ASYMMETRICVR_API UGrabbable : public USceneComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UGrabbable();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbable)
  bool IsHeld = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbable)
  bool PhysicsOnDrop = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbable)
  bool Snap = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbable)
  FRotator GrabRotation;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void
    TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

  UFUNCTION(BlueprintCallable, Category = Grabbable)
  void Grab();

  UFUNCTION(BlueprintCallable, Category = Grabbable)
  void Release();
};
