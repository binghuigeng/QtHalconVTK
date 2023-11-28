#include "commondialog.h"
#include "ui_commondialog.h"

CommonDialog::CommonDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonDialog)
{
    ui->setupUi(this);

    initial(); //初始化
    initSignalAndSlot(); //初始化信号与槽连接
}

CommonDialog::~CommonDialog()
{
    delete ui;
}

void CommonDialog::setWidgetIcon(const QString &iconPath)
{
    ui->lbWindowIcon->setPixmap(QPixmap(iconPath));
}

void CommonDialog::setWidgetTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void CommonDialog::setWidgetContent(const QString &content)
{
    ui->lbContent->setText(content);
}

void CommonDialog::setDefaultButton()
{
    ui->btnSure->setFocus();  //设置焦点
    ui->btnSure->setDefault(true); //设置默认按钮，设置了这个属性，当用户按下回车的时候，就会按下该按钮
}

void CommonDialog::initial()
{
    // 设置窗口标志位 隐藏窗口的最小化和最大化按钮，以及窗口置顶
    Qt::WindowFlags flags = this->windowFlags(); // 获取窗口标志位
    flags &= ~Qt::WindowMinMaxButtonsHint; // 隐藏窗口的最小化和最大化按钮
    this->setWindowFlags(flags | Qt::WindowStaysOnTopHint); // 设置窗口标志位 窗口置顶

    // 图标
    ui->lbWindowIcon->setPixmap(QPixmap(":/Dialog/qmessagebox-warn.png"));
    ui->lbWindowIcon->setAlignment(Qt::AlignCenter); //居中对齐

    // 内容
    ui->lbContent->setFont(QFont("Microsoft YaHei UI", 12)); //设置字体
    ui->lbContent->setAlignment(Qt::AlignCenter); //居中对齐
    ui->lbContent->setWordWrap(true); //自动换行
    ui->lbContent->setText("无内容"); //内容
}

void CommonDialog::initSignalAndSlot()
{
    connect(ui->btnSure, &QPushButton::clicked, this, &CommonDialog::close);
}
