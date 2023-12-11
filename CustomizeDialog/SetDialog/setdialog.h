#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QWidget>
#include "SysConfig.h"

namespace Ui {
class SetDialog;
}

class SetDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = nullptr);
    ~SetDialog();

    /// @brief 设置内容
    /// @param windowInquiry 窗口询问状态
    void setWidgetContent(bool windowInquiry);

    /// @brief 设置默认按钮
    void setDefaultButton();

    /// @brief 设置控件显示
    /// @param windowTop 窗口置顶状态
    /// @param windowInquiry 窗口询问状态
    /// @param index 渲染背景选择
    void setControlShow(bool windowTop, bool windowInquiry, int index);

signals:
    /// @brief 窗口置顶
    void sigWindowOnTop();

    /// @brief 渲染背景
    void sigRendererBackground(SysConfig::RendererBackground index);

private slots:
    /// @brief 窗口置顶改变
    /// @param state 窗口置顶状态
    void slt_chkTop_stateChanged(int state);

    /// @brief 窗口询问改变
    /// @param state 窗口询问状态
    void slt_chkInquiry_stateChanged(int state);

    /// @brief 默认渲染背景
    void slt_rdoDefault_clicked();

    /// @brief 黑色渲染背景
    void slt_rdoBlack_clicked();

    /// @brief 白色渲染背景
    void slt_rdoGray_clicked();

    /// @brief 深蓝色渲染背景
    void slt_rdoDarkBlue_clicked();

private:
    Ui::SetDialog *ui;

    /// @brief 初始化
    void initial();

    /// @brief 初始化信号与槽连接
    void initSignalAndSlot();
};

#endif // SETDIALOG_H
