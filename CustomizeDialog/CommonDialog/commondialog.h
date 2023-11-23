#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QWidget>

namespace Ui {
class CommonDialog;
}

class CommonDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CommonDialog(QWidget *parent = nullptr);
    ~CommonDialog();

    /// @brief 设置标题栏的图标
    /// @param iconPath 图标的路径
    void setWidgetIcon(const QString &iconPath);

    /// @brief 设置标题栏标题
    /// @param title 设置的标题
    void setWidgetTitle(const QString &title);

    /// @brief 设置内容
    /// @param content 内容
    void setWidgetContent(const QString &content);

    /// @brief 隐藏取消按钮
    void hiddenCancelButton();

    /// @brief 设置默认按钮
    void setDefaultButton();

signals:
    /// @brief 选择确定按钮
    void sigChooseSureButton();

private slots:
    void slt_btnSure_clicked();

private:
    Ui::CommonDialog *ui;

    /// @brief 初始化
    void initial();

    /// @brief 初始化信号与槽连接
    void initSignalAndSlot();
};

#endif // COMMONDIALOG_H
