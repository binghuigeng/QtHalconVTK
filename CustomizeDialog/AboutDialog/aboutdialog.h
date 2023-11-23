#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

    /// @brief 设置内容
    /// @param content 内容
    void setWidgetContent(const QString &content);

    /// @brief 设置默认按钮
    void setDefaultButton();

private:
    Ui::AboutDialog *ui;

    /// @brief 初始化
    void initial();

    /// @brief 初始化信号与槽连接
    void initSignalAndSlot();
};

#endif // ABOUTDIALOG_H
