#pragma once


#include "TDTCoreTypes.generated.h"

// figures

USTRUCT(BlueprintType)
struct FFigureData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float UpperXBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float LowerXBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float UpperYBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float LowerYBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float UpperZBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    float LowerZBound = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Figure")
    int32 BlocksCount = 4;
};


