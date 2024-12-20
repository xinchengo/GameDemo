#include "utilities/WinUtils.hpp"

#include "utilities/config.hpp"

#ifdef _WIN32
#include <windows.h>
#include "WinUtils.hpp"
#endif

#ifdef _WIN32
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