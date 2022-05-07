// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDTPlayerController.generated.h"

UCLASS()
class TDTRIS_API ATDTPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDTPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPauseGame();
};
