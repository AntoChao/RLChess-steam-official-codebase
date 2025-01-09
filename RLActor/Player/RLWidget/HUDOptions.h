#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../CommonEnum.h"
#include "GameFramework/GameUserSettings.h"
#include "../../../RLHighLevel/RLInstance.h"
#include "OptionsTXTStruct.h"
#include "HUDOptions.generated.h"

UCLASS()
class UHUDOptions : public UUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;

    // settings control
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    int32 CurLanguagesIndex;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    ELanguage CurLanguage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    int32 CurWindowModeIndex;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    TEnumAsByte<EWindowMode::Type> CurWindowMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    TArray<FIntPoint> AllResolutions;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    FIntPoint CurResolution;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    bool VsyncEnabled;

    UFUNCTION(BlueprintCallable)
    void LoadSettings();
    UFUNCTION(BlueprintCallable)
    void UpdateWindow(int32 Shift);
    UFUNCTION(BlueprintCallable)
    void UpdateResolution(int32 Shift);
    UFUNCTION(BlueprintCallable)
    void UpdateLanguage(int32 Shift);
    UFUNCTION(BlueprintCallable)
    void ToggleVsync();
    UFUNCTION(BlueprintCallable)
    void ApplySettings();

    // display control
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString LanguageDescriptionText; 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString LanguageOptionText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString WindowModeDescriptionText;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString WindowModeOptionText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString ResolutionDescriptionText;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString ResolutionOptionText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString VsyncDescriptionText;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Localization")
    FString VsyncOptionText;

    // all text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText LanguageTextsTest;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText LanguageTexts;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FLocalizedText LanguageOptionTexts;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FWindowModeText WindowModeTexts;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FResolutionText ResolutionTexts;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FVsyncText VsyncTexts;

    // Initialization method to set up localized texts
    /*
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void InitializeTexts();*/

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void Refresh();

    UFUNCTION(BlueprintCallable, Category = "RLFactory")
    FString GetCurrentWindowModeText();
};

