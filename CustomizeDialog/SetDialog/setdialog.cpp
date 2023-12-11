#include "setdialog.h"
#include "ui_setdialog.h"

SetDialog::SetDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);

    initial(); //初始化
    initSignalAndSlot(); //初始化信号与槽连接
}

SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::setWidgetContent(bool windowInquiry)
{
    ui->chkInquiry->setChecked(windowInquiry);
}

void SetDialog::setDefaultButton()
{
//    ui->btnClose->setFocus();  //设置焦点
//    ui->btnClose->setDefault(true); //设置默认按钮，设置了这个属性，当用户按下回车的时候，就会按下该按钮
}

void SetDialog::setControlShow(bool windowTop, bool windowInquiry, int index)
{
    ui->chkTop->setChecked(windowTop);
    ui->chkInquiry->setChecked(windowInquiry);
    switch (index) {
    case SysConfig::Default:
        ui->rdoDefault->setChecked(true);
        break;
    case SysConfig::Black:
        ui->rdoBlack->setChecked(true);
        break;
    case SysConfig::Gray:
        ui->rdoGray->setChecked(true);
        break;
    case SysConfig::DarkBlue:
        ui->rdoDarkBlue->setChecked(true);
        break;
    default:
        ui->rdoDefault->setChecked(true);
        break;
    }
}

void SetDialog::slt_chkTop_stateChanged(int state)
{
    if (state == Qt::Checked) {
        SysConfig::setWindowTop(true);
    } else if (state == Qt::Unchecked) {
        SysConfig::setWindowTop(false);
    }

    emit sigWindowOnTop();
}

void SetDialog::slt_chkInquiry_stateChanged(int state)
{
    if (state == Qt::Checked) {
        SysConfig::setWindowClose(true);
    } else if (state == Qt::Unchecked) {
        SysConfig::setWindowClose(false);
    }
}

void SetDialog::slt_rdoDefault_clicked()
{
    SysConfig::setRendererBackground(SysConfig::Default);
    emit sigRendererBackground(SysConfig::Default);
}

void SetDialog::slt_rdoBlack_clicked()
{
    SysConfig::setRendererBackground(SysConfig::Black);
    emit sigRendererBackground(SysConfig::Black);
}

void SetDialog::slt_rdoGray_clicked()
{
    SysConfig::setRendererBackground(SysConfig::Gray);
    emit sigRendererBackground(SysConfig::Gray);
}

void SetDialog::slt_rdoDarkBlue_clicked()
{
    SysConfig::setRendererBackground(SysConfig::DarkBlue);
    emit sigRendererBackground(SysConfig::DarkBlue);
}

void SetDialog::initial()
{
    // 设置窗口标志位 隐藏窗口的最小化和最大化按钮，以及窗口置顶
    Qt::WindowFlags flags = this->windowFlags(); // 获取窗口标志位
    flags &= ~Qt::WindowMinMaxButtonsHint; // 隐藏窗口的最小化和最大化按钮
    this->setWindowFlags(flags | Qt::WindowStaysOnTopHint); // 设置窗口标志位 窗口置顶

    // 设置窗口标题
    this->setWindowTitle("设置"); // 设置窗口标志位 窗口置顶

//    // 关闭按钮
//    ui->btnClose->setText("关闭");
}

void SetDialog::initSignalAndSlot()
{
    // 窗口置顶
    connect(ui->chkTop, &QCheckBox::stateChanged, this, &SetDialog::slt_chkTop_stateChanged);
    // 窗口询问
    connect(ui->chkInquiry, &QCheckBox::stateChanged, this, &SetDialog::slt_chkInquiry_stateChanged);
    // 渲染背景
    connect(ui->rdoDefault, &QRadioButton::clicked, this, &SetDialog::slt_rdoDefault_clicked);
    connect(ui->rdoBlack, &QRadioButton::clicked, this, &SetDialog::slt_rdoBlack_clicked);
    connect(ui->rdoGray, &QRadioButton::clicked, this, &SetDialog::slt_rdoGray_clicked);
    connect(ui->rdoDarkBlue, &QRadioButton::clicked, this, &SetDialog::slt_rdoDarkBlue_clicked);
}
