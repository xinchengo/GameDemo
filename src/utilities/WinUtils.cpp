#include "utilities/WinUtils.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
void enableResize(sf::RenderWindow &window)
{
    sf::WindowHandle handle = window.getSystemHandle();
    LONG style = GetWindowLong(handle, GWL_STYLE);
    style |= WS_SIZEBOX | WS_MAXIMIZEBOX;
    SetWindowLong(handle, GWL_STYLE, style);
    SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
void disableResize(sf::RenderWindow &window)
{
    sf::WindowHandle handle = window.getSystemHandle();
    LONG style = GetWindowLong(handle, GWL_STYLE);
    style &= ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;
    SetWindowLong(handle, GWL_STYLE, style);
    SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
#endif