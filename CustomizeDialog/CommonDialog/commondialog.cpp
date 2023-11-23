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
    ui->lbWindowIcon->setStyleSheet(iconPath);
}

void CommonDialog::setWidgetTitle(const QString &title)
{
    ui->lbWindowTitle->setText(title);
}

void CommonDialog::setWidgetContent(const QString &content)
{
    ui->lbContent->setText(content);
}

void CommonDialog::hiddenCancelButton()
{
    ui->btnCancel->setHidden(true);
}

void CommonDialog::setDefaultButton()
{
    ui->btnSure->setFocus();  //设置焦点
    ui->btnSure->setDefault(true); //设置默认按钮，设置了这个属性，当用户按下回车的时候，就会按下该按钮
}

void CommonDialog::slt_btnSure_clicked()
{
//    qDebug() << "slt_btnSure_clicked";
    this->close();
    emit sigChooseSureButton(); //选择确定按钮
}

void CommonDialog::initial()
{
    // 设置窗口无边框，隐藏标题栏 | 窗口置顶 | Windows平台上，使窗体具有更窄的边框，用于固定对话框大小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);

    // 图标
    ui->lbWindowIcon->setStyleSheet("QLabel { border-image: url(:/Logo/Logo.png); }");

    // 标题
    ui->lbWindowTitle->setFont(QFont("Microsoft YaHei UI", 10, QFont::Bold, false)); //设置字体、字号、粗体、斜体
    ui->lbWindowTitle->setText("无标题"); //标题

    // 内容
    ui->lbContent->setFont(QFont("Microsoft YaHei UI", 12)); //设置字体
    ui->lbContent->setAlignment(Qt::AlignCenter); //居中对齐
    ui->lbContent->setWordWrap(true); //自动换行
    ui->lbContent->setText("无内容"); //内容
}

void CommonDialog::initSignalAndSlot()
{
    connect(ui->btnCancel, &QPushButton::clicked, this, &CommonDialog::close);
    connect(ui->btnSure, &QPushButton::clicked, this, &CommonDialog::slt_btnSure_clicked);
}
