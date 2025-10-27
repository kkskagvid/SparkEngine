#pragma once

namespace Spark
{
    class WindowsWindow : IWindow
    {
    public:
        WindowsWindow() = default;
        ~WindowsWindow() override = default;

        WindowProperties GetProperties() const override;

        bool Create(const WindowProperties& properties) override;
        void Destroy() override;
        void Update() override;

        void* GetNativeHandle() const;

        const std::string GetTitle() const;
        void SetTitle(const std::string& title);

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        void SetSize(unsigned int width, unsigned int height);

        bool IsVSyncEnabled() const;
        void SetVSyncEnabled(bool enable);

    private:
        WindowProperties m_Properties;

        bool m_VSync = true;

        MSG m_Msg = {};
        HWND m_HWnd = nullptr;
        HINSTANCE m_HInstance = nullptr;

        typedef BOOL    (APIENTRY* PFNWGLSWAPINTERVALEXTPROC)(int interval);
        typedef int     (APIENTRY* PFNWGLGETSWAPINTERVALEXTPROC)(void);

        PFNWGLSWAPINTERVALEXTPROC SwapIntervalEXT;
        PFNWGLGETSWAPINTERVALEXTPROC GetSwapIntervalEXT;

        void InitializeWGLExtensions();
        void ProcessMessages();
        void SwapBuffers();

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
