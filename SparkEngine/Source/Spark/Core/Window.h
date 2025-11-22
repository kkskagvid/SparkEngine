#pragma once

namespace Spark::Core
{
    /**
     * @brief 窗口的状态
     */
    enum class WindowState
    {
        Normal,
        Minimized,
        Maximized,
        FullScreen
    };

    /**
     * @brief 窗口的属性
     */
    struct WindowProperties
    {
        /**
         * @brief 窗口标题
         */
        std::string title;

        /**
         * @brief 窗口宽度
         */
        unsigned int width;

        /**
         * @brief 窗口宽度
         */
        unsigned int height;

        /**
         * @brief 窗口左边缘的 X 坐标
         */
        unsigned int left;

        /**
         * @brief 窗口上边缘的 Y 坐标
         */
        unsigned int top;

        /**
         * @brief 窗口状态
         */
        WindowState windowState;
    };

    class IWindow
    {
    public:
        IWindow() = default;
        virtual ~IWindow() = default;

        /**
         * @brief 获取窗口属性
         * @return 窗口的属性
         */
        virtual WindowProperties GetProperties() const = 0;

        /**
         * @brief 创建一个窗口并根据指定属性进行初始化
         * @param properties 窗口的属性
         * @return 如果窗口创建成功，则返回 true；否则返回 false
         */
        virtual bool Create(const WindowProperties& properties);

        /**
         * @brief 销毁窗口
         */
        virtual void Destroy() = 0;

        /**
         * @brief 更新窗口
         */
        virtual void Update() = 0;

        /**
         * @brief 获取底层原生窗口句柄
         * @return 返回指向原生窗口句柄的指针，若未实现或资源无效则为 nullptr
         */
        virtual void* GetNativeHandle() const = 0;

        /**
         * @brief 获取窗口标题
         * @return 窗口标题
         */
        virtual const std::string GetTitle() const = 0;

        /**
         * @brief 设置窗口标题
         * @param title 新的窗口标题
         */
        virtual void SetTitle(const std::string& title) = 0;

        /**
         * @brief 获取窗口宽度
         * @return 返回窗口宽度
         */
        virtual unsigned int GetWidth() const = 0;

        /**
         * @brief 获取窗口高度
         * @return 返回窗口高度
         */
        virtual unsigned int GetHeight() const = 0;

        /**
         * @brief 设置窗口大小
         * @param width 新的窗口宽度
         * @param height 新的窗口高度
         */
        virtual void SetSize(unsigned int width, unsigned int height) = 0;

        /**
         * @brief 判断是否启用垂直同步（VSync）
         * @return 如果启用了垂直同步，则返回 true；否则返回 false
         */
        virtual bool IsVSyncEnabled() const = 0;

        /**
         * @brief 设置是否启用垂直同步（VSync）
         * @param enable 如果为 true，则启用垂直同步；如果为 false，则禁用垂直同步
         */
        virtual void SetVSyncEnabled(bool enable) = 0;
    };
}
