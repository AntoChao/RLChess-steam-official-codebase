#include "HUDOptions.h"

void UHUDOptions::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize texts
    // InitializeTexts();

    // Optionally load settings or apply initial configuration
    LoadSettings();
}

void UHUDOptions::LoadSettings()
{
    // Initialize common resolutions
    AllResolutions = {
        FIntPoint(1280, 720),
        FIntPoint(1366, 768),
        FIntPoint(1600, 900),
        FIntPoint(1920, 1080),       
        FIntPoint(2560, 1440)
    };

    UGameUserSettings* UserSettings = GEngine->GameUserSettings;

    CurResolution = UserSettings->GetScreenResolution();
    CurWindowMode = UserSettings->GetFullscreenMode();
    CurWindowModeIndex = static_cast<int32>(CurWindowMode);
    VsyncEnabled = UserSettings->IsVSyncEnabled();

    URLInstance* GI = Cast<URLInstance>(GetWorld()->GetGameInstance());
    if (GI)
    {
        CurLanguage = GI->getLanguage();
    }

    Refresh();
}

void UHUDOptions::UpdateLanguage(int32 Shift)
{
    CurLanguagesIndex = (CurLanguagesIndex + Shift + 3) % 3;
    CurLanguage = static_cast<ELanguage>(CurLanguagesIndex);

    // Assuming you have a method to update the language settings in your game instance
    URLInstance* GI = Cast<URLInstance>(GetWorld()->GetGameInstance());
    if (GI)
    {
        GI->setLanguage(CurLanguage);
        // Additional logic to refresh the UI or load appropriate language assets can be implemented here.
        Refresh();
    }
}

void UHUDOptions::UpdateWindow(int32 Shift)
{
    CurWindowModeIndex = (CurWindowModeIndex + Shift + 3) % 3;  // Assuming three window modes
    CurWindowMode = static_cast<EWindowMode::Type>(CurWindowModeIndex);
    Refresh();
}

void UHUDOptions::UpdateResolution(int32 Shift)
{
    int32 Index = AllResolutions.IndexOfByKey(CurResolution);
    Index = (Index + Shift + AllResolutions.Num()) % AllResolutions.Num();
    CurResolution = AllResolutions[Index];
    Refresh();
}

void UHUDOptions::ToggleVsync()
{
    VsyncEnabled = !VsyncEnabled;
    Refresh();
}

void UHUDOptions::ApplySettings()
{
    UGameUserSettings* UserSettings = GEngine->GameUserSettings;
    UserSettings->SetScreenResolution(CurResolution);
    UserSettings->SetFullscreenMode(CurWindowMode);
    UserSettings->SetVSyncEnabled(VsyncEnabled);
    UserSettings->ApplySettings(true);
    UserSettings->SaveSettings();
}

/*
void UHUDOptions::InitializeTexts()
{
    // Initialize texts for Window Mode, Resolution, and Vsync
    LanguageTexts.English = "Language";
    LanguageTexts.Chinese = "语言";
    LanguageTexts.Spanish = "Idioma";

    LanguageOptionTexts.English = "English";
    LanguageOptionTexts.Chinese = "中文";
    LanguageOptionTexts.Spanish = "Español";

    WindowModeTexts.Description.English = "Window Mode";
    WindowModeTexts.Description.Chinese = "窗口模式";
    WindowModeTexts.Description.Spanish = "Modo Ventana";

    WindowModeTexts.FullScreen.English = "Full Screen";
    WindowModeTexts.FullScreen.Chinese = "全屏";
    WindowModeTexts.FullScreen.Spanish = "Pantalla Completa";

    WindowModeTexts.Windowed.English = "Windowed";
    WindowModeTexts.Windowed.Chinese = "窗口化";
    WindowModeTexts.Windowed.Spanish = "Ventanado";

    WindowModeTexts.WindowedFullscreen.English = "Windowed Fullscreen";
    WindowModeTexts.WindowedFullscreen.Chinese = "窗口化全屏";
    WindowModeTexts.WindowedFullscreen.Spanish = "Pantalla Completa Ventanada";

    // Initialize resolution texts
    ResolutionTexts.Description.English = "Resolution";
    ResolutionTexts.Description.Chinese = "分辨率";
    ResolutionTexts.Description.Spanish = "Resolución";

    // Initialize Vsync texts
    VsyncTexts.Description.English = "VSync";
    VsyncTexts.Description.Chinese = "垂直同步";
    VsyncTexts.Description.Spanish = "VSync";

    VsyncTexts.Enabled.English = "Enabled";
    VsyncTexts.Enabled.Chinese = "启用";
    VsyncTexts.Enabled.Spanish = "Habilitado";

    VsyncTexts.Disabled.English = "Disabled";
    VsyncTexts.Disabled.Chinese = "禁用";
    VsyncTexts.Disabled.Spanish = "Deshabilitado";
}*/

void UHUDOptions::Refresh()
{
    URLInstance* GI = Cast<URLInstance>(GetWorld()->GetGameInstance());
    if (GI)
    {
        CurLanguage = GI->getLanguage();
    }

    LanguageDescriptionText = LanguageTexts.GetTextForCurrentLanguage(CurLanguage);
    LanguageOptionText = LanguageOptionTexts.GetTextForCurrentLanguage(CurLanguage);

    WindowModeDescriptionText = WindowModeTexts.Description.GetTextForCurrentLanguage(CurLanguage);
    WindowModeOptionText = GetCurrentWindowModeText();

    ResolutionDescriptionText = ResolutionTexts.Description.GetTextForCurrentLanguage(CurLanguage);
    ResolutionOptionText = FString::Printf(TEXT("%dx%d"), CurResolution.X, CurResolution.Y);

    VsyncDescriptionText = VsyncTexts.Description.GetTextForCurrentLanguage(CurLanguage);
    VsyncOptionText = VsyncEnabled ? VsyncTexts.Enabled.GetTextForCurrentLanguage(CurLanguage) : VsyncTexts.Disabled.GetTextForCurrentLanguage(CurLanguage);
}

FString UHUDOptions::GetCurrentWindowModeText()
{
    switch (CurWindowMode)
    {
    case EWindowMode::Fullscreen:
        return WindowModeTexts.FullScreen.GetTextForCurrentLanguage(CurLanguage);
    case EWindowMode::Windowed:
        return WindowModeTexts.Windowed.GetTextForCurrentLanguage(CurLanguage);
    case EWindowMode::WindowedFullscreen:
        return WindowModeTexts.WindowedFullscreen.GetTextForCurrentLanguage(CurLanguage);
    default:
        return WindowModeTexts.FullScreen.GetTextForCurrentLanguage(CurLanguage);  // Default case
    }
}