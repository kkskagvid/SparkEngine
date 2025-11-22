#include "SparkPCH.h"

#include "Spark/Core/Window.h"
#include "Spark/Core/String/StringUtils.h"

#include "Spark/Platforms/Windows/WindowsWindow.h"

namespace Spark::Core
{
    WindowProperties WindowsWindow::GetProperties() const
    {
        return m_Properties;
    }

    bool WindowsWindow::Create(const WindowProperties& properties)
    {
        m_Properties = properties;

        m_HInstance = ::GetModuleHandleW(nullptr);

        std::string className;
        className += properties.title;
        className += " Window Class";

        std::wstring wTitle = Spark::Core::ConvertCStringToWString(properties.title.c_str());
        std::wstring wClassName = ConvertCStringToWString(className.c_str());

        RECT windowRect = {};
        windowRect.left = (long)0;                          // Set Left Value To 0
        windowRect.right = (long)m_Properties.width;        // Set Right Value To Requested Width
        windowRect.top = (long)0;                           // Set Top Value To 0
        windowRect.bottom = (long)m_Properties.height;      // Set Bottom Value To Requested Height

        WNDCLASSEX windowClass = {};
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = m_HInstance;
        windowClass.hIcon = ::LoadIconW(m_HInstance, MAKEINTRESOURCE(105));            // default icon
        windowClass.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);                       // default arrow
        windowClass.hbrBackground = nullptr;                                           // don't need background
        windowClass.lpszMenuName = nullptr;                                            // no menu
        windowClass.lpszClassName = wClassName.c_str();
        windowClass.hIconSm = ::LoadIconW(m_HInstance, MAKEINTRESOURCE(101));          // windows logo small icon

        if (!::RegisterClassExW(&windowClass))
        {
            return false;
        }

        DWORD windowStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
        DWORD windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        ::AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowExStyle);     // Adjust Window To True Requested Size

        m_HWnd = ::CreateWindowExW(
            0,
            wClassName.c_str(),
            wTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,     // width
            windowRect.bottom - windowRect.top,     // height
            nullptr,
            nullptr,
            m_HInstance,
            this
        );

        if (!m_HWnd)
        {
            return false;
        }

        ShowWindow(m_HWnd, SW_SHOW);
        UpdateWindow(m_HWnd);

        return true;
    }

    void WindowsWindow::Destroy()
    {
        if (m_HWnd)
        {
            ::DestroyWindow(m_HWnd);
            m_HWnd = nullptr;
        }
    }

    void WindowsWindow::Update()
    {
        ProcessMessages();
        SwapBuffers();
    }

    void* WindowsWindow::GetNativeHandle() const
    {
        return nullptr;
    }

    const std::string WindowsWindow::GetTitle() const
    {
        return m_Properties.title;
    }

    void WindowsWindow::SetTitle(const std::string& title)
    {
        m_Properties.title = title;
    }

    unsigned int WindowsWindow::GetWidth() const
    {
        return m_Properties.width;
    }

    unsigned int WindowsWindow::GetHeight() const
    {
        return m_Properties.height;
    }

    void WindowsWindow::SetSize(unsigned int width, unsigned int height)
    {
        m_Properties.width = width;
        m_Properties.height = height;
    }

    bool WindowsWindow::IsVSyncEnabled() const
    {
        if(!GetSwapIntervalEXT) {
            //std::cerr << "[VSyncController] wglGetSwapIntervalEXT 不可用。\n";
            return false;
        }

        int interval = GetSwapIntervalEXT();
        return interval != 0;
    }

    void WindowsWindow::SetVSyncEnabled(bool enable)
    {
        if (!SwapIntervalEXT)
        {
            //std::cerr << "[VSyncController] wglSwapIntervalEXT 不可用，垂直同步设置失败。\n";
            return;
        }

        int interval = enable ? 1 : 0;
        if (!SwapIntervalEXT(interval))
        {
            //std::cerr << "[VSyncController] 设置垂直同步失败，可能由于扩展不支持或上下文无效。\n";
        }
    }

    void WindowsWindow::InitializeWGLExtensions()
    {
        SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        GetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
    }

    void WindowsWindow::ProcessMessages()
    {
        while (::PeekMessageW(&m_Msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&m_Msg);
            ::DispatchMessageW(&m_Msg);
        }
    }

    void WindowsWindow::SwapBuffers()
    {
        ::SwapBuffers(GetDC(m_HWnd));
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE:
        {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            WindowsWindow* pThis = reinterpret_cast<WindowsWindow*>(cs->lpCreateParams);
            ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            break;
        }

        case WM_NCDESTROY:
        {
            ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
            break;
        }

        default:
        {
            WindowsWindow* pThis = reinterpret_cast<WindowsWindow*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
            if (pThis)
            {
                return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
            }
        }
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    LRESULT WindowsWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE:         // window creation
            break;

        case WM_DESTROY:            // window destroy
        case WM_QUIT:
            ::CloseWindow(hWnd);
            break;
        case WM_CLOSE:                  // windows is closing

            // deselect rendering context and delete it
            //wglMakeCurrent(hDC, NULL);
            //wglDeleteContext(hRC);
            // send WM_QUIT to message queue
            ::PostQuitMessage(0);
            break;

        case WM_SIZE:
            break;

        case WM_ACTIVATEAPP:        // activate app
            break;

        case WM_PAINT:              // paint
            PAINTSTRUCT ps;
            ::BeginPaint(hWnd, &ps);
            ::EndPaint(hWnd, &ps);
            break;

        case WM_LBUTTONDOWN:        // left mouse button
            break;

        case WM_RBUTTONDOWN:        // right mouse button
            break;

        case WM_MOUSEMOVE:          // mouse movement
            break;

        case WM_LBUTTONUP:          // left button release
            break;

        case WM_RBUTTONUP:          // right button release
            break;

        case WM_KEYUP:
            break;

        case WM_KEYDOWN:
            break;

        default:
            break;
        }
        return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}

