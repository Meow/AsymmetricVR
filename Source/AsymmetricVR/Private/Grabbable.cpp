// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabbable.h"

// Sets default values for this component's properties
UGrabbable::UGrabbable() {
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
  // features off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}

// Called when the game starts
void UGrabbable::BeginPlay() {
  Super::BeginPlay();

  // ...
}

// Called every frame
void UGrabbable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}
