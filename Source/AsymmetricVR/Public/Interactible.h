// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactible.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
  FInteractibleActionSignature,
  AActor *,
  InteractingActor,
  UPrimitiveComponent *,
  InteractingComponent
);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class ASYMMETRICVR_API UInteractible : public UActorComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UInteractible();

  UFUNCTION(BlueprintImplementableEvent, Category = "AsymmetricVR")
  void OnInteract(class AActor *InteractingActor);

  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AsymmetricVr)
  //FInteractibleActionSignature OnInteract;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void
    TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
