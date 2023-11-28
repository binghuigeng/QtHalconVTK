#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "ProjectConfig.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    initial(); //初始化
    initSignalAndSlot(); //初始化信号与槽连接
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::setWidgetContent(const QString &content)
{
    ui->lbAPP->setText(content);
}

void AboutDialog::setDefaultButton()
{
    ui->btnClose->setFocus();  //设置焦点
    ui->btnClose->setDefault(true); //设置默认按钮，设置了这个属性，当用户按下回车的时候，就会按下该按钮
}

void AboutDialog::initial()
{
    // 设置窗口标志位 隐藏窗口的最小化和最大化按钮，以及窗口置顶
    Qt::WindowFlags flags = this->windowFlags(); // 获取窗口标志位
    flags &= ~Qt::WindowMinMaxButtonsHint; // 隐藏窗口的最小化和最大化按钮
    this->setWindowFlags(flags | Qt::WindowStaysOnTopHint); // 设置窗口标志位 窗口置顶

    // 设置窗口标题
    this->setWindowTitle(QString("关于 %1").arg(TOSTRING(PROJECT_NAME))); // 设置窗口标志位 窗口置顶

    // 主体
    ui->widgetBody->setFont(QFont("Microsoft YaHei UI", 10)); //设置字体
    ui->lbAPPVersion->setAlignment(Qt::AlignCenter); //居中对齐
    ui->lbAPP->setText(TOSTRING(PROJECT_VERSION)); //APP版本

    // 关闭按钮
    ui->btnClose->setText("关闭");
}

void AboutDialog::initSignalAndSlot()
{
    connect(ui->btnClose, &QPushButton::clicked, this, &AboutDialog::close);
}
