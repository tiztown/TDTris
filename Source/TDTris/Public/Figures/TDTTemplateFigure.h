// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Figures/TDTBaseFigure.h"
#include "TDTTemplateFigure.generated.h"

class ATDTPlayableFigure;
UCLASS()
class TDTRIS_API ATDTTemplateFigure : public ATDTBaseFigure
{
    GENERATED_BODY()


public:
    ATDTTemplateFigure();

    void GenerateTemplateFigure();

    virtual void SetTemplateFigureVisual(ATDTPlayableFigure* ParentFigure);

    void SyncFigures();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ATDTPlayableFigure* ParentFigure;
};
