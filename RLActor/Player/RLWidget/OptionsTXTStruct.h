#pragma once

#include "CoreMinimal.h"
#include "../../../CommonEnum.h"
#include "OptionsTXTStruct.generated.h"

USTRUCT(BlueprintType)
struct FLocalizedText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString English;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString Chinese;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString Spanish;

    FString GetTextForCurrentLanguage(ELanguage CurrentLanguage) const
    {
        switch (CurrentLanguage)
        {
        case ELanguage::EEnglish:
            return English;
        case ELanguage::EChinese:
            return Chinese;
        case ELanguage::ESpanish:
            return Spanish;
        default:
            return English; // Default case can default to English or an empty string
        }
    }
};

USTRUCT(BlueprintType)
struct FWindowModeText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText FullScreen;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Windowed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText WindowedFullscreen;
};

USTRUCT(BlueprintType)
struct FResolutionText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Description;

    // Resolutions are less likely to need localization, but the description does
    // You might want to add more fields if you have descriptions for each resolution
};

USTRUCT(BlueprintType)
struct FGraphicsQualityText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Ultra;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Epic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText High;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Medium;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Low;
};

USTRUCT(BlueprintType)
struct FVsyncText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Enabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText Disabled;
};