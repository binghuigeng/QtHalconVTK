#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <QObject>

/*
 * @file SysConfig.h
 * @brief 与配置文件相关的函数
 */
class SysConfig : public QObject
{
    Q_OBJECT
public:
    explicit SysConfig(QObject *parent = nullptr);
    ~SysConfig();

    /// @brief 渲染背景
    enum RendererBackground {
        Default,
        Gray,
        NavyBlue,
        DarkBlue
    };

    /// @brief 设置窗口置顶
    /// @param checked true:窗口置顶 false:取消窗口置顶
    static void setWindowTop(bool checked);

    /// @brief 获取窗口置顶
    /// @return true:窗口置顶 false:取消窗口置顶
    static bool getWindowTop();

    /// @brief 设置窗口询问
    /// @param checked true:窗口询问 false:取消窗口询问
    static void setWindowClose(bool checked);

    /// @brief 获取窗口询问
    /// @return true:窗口询问 false:取消窗口询问
    static bool getWindowClose();

    /// @brief 设置渲染背景
    /// @param index Default:默认 Gray:灰色 NavyBlue:藏青色 DarkBlue:深蓝色
    static void setRendererBackground(SysConfig::RendererBackground index);

    /// @brief 获取渲染背景
    /// @return Default:默认 Gray:灰色 NavyBlue:藏青色 DarkBlue:深蓝色
    static RendererBackground getRendererBackground();
};

#endif // SYSCONFIG_H
