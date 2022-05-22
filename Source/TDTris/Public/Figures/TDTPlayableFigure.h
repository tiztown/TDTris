// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Figures/TDTBaseFigure.h"
#include "TDTPlayableFigure.generated.h"

UCLASS()
class TDTRIS_API ATDTPlayableFigure : public ATDTBaseFigure
{
    GENERATED_BODY()

public:
    ATDTPlayableFigure();

    void GeneratePlayableFigure(ETDTFigureType FigureType);


protected:
    virtual void BeginPlay() override;
};
