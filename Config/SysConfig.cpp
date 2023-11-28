#include "SysConfig.h"
#include <QSettings>

SysConfig::SysConfig(QObject *parent) : QObject(parent)
{

}

SysConfig::~SysConfig()
{

}

void SysConfig::setWindowTop(bool checked)
{
    QSettings configIniWrite("./Config/config.ini", QSettings::IniFormat);
    configIniWrite.setValue("/Universal/WindowTop", checked);
}

bool SysConfig::getWindowTop()
{
    QSettings configIniRead("./Config/config.ini", QSettings::IniFormat);
    return configIniRead.value("/Universal/WindowTop", false).toBool();
}

void SysConfig::setWindowClose(bool checked)
{
    QSettings configIniWrite("./Config/config.ini", QSettings::IniFormat);
    configIniWrite.setValue("/Universal/WindowClose", checked);
}

bool SysConfig::getWindowClose()
{
    QSettings configIniRead("./Config/config.ini", QSettings::IniFormat);
    return configIniRead.value("/Universal/WindowClose", false).toBool();
}

void SysConfig::setRendererBackground(SysConfig::RendererBackground index)
{
    QSettings configIniWrite("./Config/config.ini", QSettings::IniFormat);
    configIniWrite.setValue("/Universal/RendererBackground", index);
}

int SysConfig::getRendererBackground()
{
    QSettings configIniRead("./Config/config.ini", QSettings::IniFormat);
    return configIniRead.value("/Universal/RendererBackground", SysConfig::Default).toInt();
}
