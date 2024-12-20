#include "utilities/WinUtils.hpp"

#include "utilities/config.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellScalingApi.h>
#include "WinUtils.hpp"
#endif

#ifdef _WIN32
float getWindowScale()
{
    typedef HRESULT(WINAPI *SetProcessDpiAwarenessContextFunc)(DPI_AWARENESS_CONTEXT);
    typedef HRESULT(WINAPI *SetProcessDpiAwarenessFunc)(PROCESS_DPI_AWARENESS);
    typedef BOOL(WINAPI *SetProcessDPIAwareFunc)();

    HMODULE hUser32 = LoadLibrary(TEXT("user32.dll"));
    if (hUser32)
    {
        SetProcessDpiAwarenessContextFunc SetProcessDpiAwarenessContext = 
            (SetProcessDpiAwarenessContextFunc)GetProcAddress(hUser32, "SetProcessDpiAwarenessContext");
        if (SetProcessDpiAwarenessContext)
        {
            SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        else
        {
            HMODULE hShcore = LoadLibrary(TEXT("shcore.dll"));
            if (hShcore)
            {
                SetProcessDpiAwarenessFunc SetProcessDpiAwareness = 
                    (SetProcessDpiAwarenessFunc)GetProcAddress(hShcore, "SetProcessDpiAwareness");
                if (SetProcessDpiAwareness)
                {
                    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
                }
                else
                {
                    SetProcessDPIAwareFunc SetProcessDPIAware = 
                        (SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");
                    if (SetProcessDPIAware)
                    {
                        SetProcessDPIAware();
                    }
                }
                FreeLibrary(hShcore);
            }
        }
        FreeLibrary(hUser32);
    }
    
    HDC screen = GetDC(0);
    int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(0, screen);
    return dpiX / 96.0f;
}
void enableResize(sf::RenderWindow &window)
{
    sf::WindowHandle handle = window.getSystemHandle();
    // If the window is not in fullscreen mode
    if(GetWindowLong(handle, GWL_STYLE) & WS_OVERLAPPEDWINDOW)
    {
        LONG style = GetWindowLong(handle, GWL_STYLE);
        style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
        SetWindowLong(handle, GWL_STYLE, style);
        SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}
void disableResize(sf::RenderWindow &window)
{
    sf::WindowHandle handle = window.getSystemHandle();
    // If the window is not in fullscreen mode
    if(GetWindowLong(handle, GWL_STYLE) & WS_OVERLAPPEDWINDOW)
    {
        LONG style = GetWindowLong(handle, GWL_STYLE);
        style &= ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;
        SetWindowLong(handle, GWL_STYLE, style);
        SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}
#else
void enableResize(sf::RenderWindow &window) {}
void disableResize(sf::RenderWindow &window) {}
float getWindowScale() { return 1.0f; }
#endif
void toggleFullscreen(sf::RenderWindow &window)
{
    static auto size = window.getSize();
    // If window is not in fullscreen mode
    if(window.getSize().x != sf::VideoMode::getDesktopMode().width
    || window.getSize().y != sf::VideoMode::getDesktopMode().height)
    {
        window.close();
        window.create(sf::VideoMode::getDesktopMode(), 
            sf::String::fromUtf8(config.gameWindowTitle.begin(), config.gameWindowTitle.end()),
            sf::Style::Fullscreen);
    }
    else
    {
        window.close();
        window.create(sf::VideoMode(size.x, size.y), 
            sf::String::fromUtf8(config.gameWindowTitle.begin(), config.gameWindowTitle.end()));
    }
}