// Fill out your copyright notice in the Description page of Project Settings.

#include "AsymmetricVrGameMode.h"

AAsymmetricVrGameMode::AAsymmetricVrGameMode() : Super() {
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_DesktopPlayer"));
  DefaultPawnClass = PlayerPawnClassFinder.Class;
}
