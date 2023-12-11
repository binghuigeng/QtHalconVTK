#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QCheckBox>

#include "aboutdialog.h"
#include "setdialog.h"
#include "commondialog.h"

// HALCON headers
#include "HalconCpp.h"

// VTK headers
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /// @brief 重写窗口的关闭事件处理
    /// @param event 窗口关闭事件的信息
    void closeEvent(QCloseEvent *event) override;

    /// @brief 准备退出
    void readyQuit();

public slots:
    /// @brief 窗口置顶
    void sltWindowOnTop();

    /// @brief 渲染背景
    /// @param index Default:默认 Black:黑色 Gray:灰色 DarkBlue:深蓝色
    void sltRendererBackground(SysConfig::RendererBackground index);

private slots:
    /// @brief 打开文件
    void slt_actOpen_triggered();

    /// @brief 退出
    void slt_actQuit_triggered();

    /// @brief 重置
    void slt_actReset_triggered();

    /// @brief 添加
    void slt_actAdd_triggered();

    /// @brief 重启
    void slt_actRestart_triggered();

    /// @brief 通用
    void slt_actUniversal_triggered();

    /// @brief 关于
    void slt_actAbout_triggered();

    /// @brief 关闭窗口点击的按钮
    /// @param butClicked 抽象按钮
    void buttonClicked(QAbstractButton *butClicked);

    /// @brief 窗口询问改变
    /// @param state 窗口询问状态
    void slt_chkInquiry_stateChanged(int state);

private:
    /// @brief 初始化
    void initial();

    /// @brief 初始化配置
    void initSysConfig();

    /// @brief 初始化关闭窗口
    void initCloseWindow();

    /// @brief 初始化状态栏显示消息
    void initStatusbarMessage();

    /// @brief 初始化 VTK
    void initVTK();

    /// @brief 初始化信号与槽函数
    void initSignalsAndSlots();

    /// @brief 渲染背景
    /// @param index Default:默认 Black:黑色 Gray:灰色 DarkBlue:深蓝色
    void setRendererBackground(SysConfig::RendererBackground index);

    /// @brief 加载点云文件
    /// @return true:成功 false:失败
    bool loadPointCloudFile(QString fileName);

    /// @brief 获取点云数据
    /// @return true:成功 false:失败
    bool getPointCloudData();

    /// @brief 提取点云坐标
    void extractPointCloudCoordinates();

    /// @brief 显示点云文件信息
    /// @param fileName 文件路径
    void showPointCloudFile(QString fileName);

    /// @brief 显示点云
    void showPointCloud();

#if 0
    /// @brief 默认渲染窗口显示点云
    void showDefaultWindow();

    /// @brief VTKWidget 显示点云
    void showVTKWidget();
#endif

private:
    Ui::MainWindow *ui;

    /// @brief 状态栏消息标签
    QLabel statusLabel;

    /// @brief 关于对话框
    AboutDialog dlgAbout;

    /// @brief 设置对话框
    SetDialog dlgSet;

    /// @brief 通用对话框
    CommonDialog dlgCommon;

    /// @brief 关闭窗口消息对话框
    QMessageBox msgBox;
    QPushButton *btnAccept;
    QPushButton *btnReject;
    QCheckBox chkInquiry;
    bool bAcceptClose;

    // HALCON variables
    HalconCpp::HObjectModel3D model3D;
    HalconCpp::HTuple x, y, z;
    HalconCpp::HTuple pointCount;

    // VTK variables
    vtkSmartPointer<vtkPoints> points; // 点云数据
    vtkSmartPointer<vtkCellArray> vertices; // 顶点
    vtkSmartPointer<vtkFloatArray> scalars; // 标量数据
    vtkSmartPointer<vtkPolyData> polyData; // 多边形数据
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter; // 顶点滤波器
    vtkSmartPointer<vtkLookupTable> lut; // 颜色映射
    vtkSmartPointer<vtkPolyDataMapper> mapper; // 点云映射器
    vtkSmartPointer<vtkActor> actor; // 点云演员
    vtkSmartPointer<vtkScalarBarActor> scalarBar; // 标量条
    vtkSmartPointer<vtkRenderer> renderer; // 渲染器
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow; // 渲染窗口
    QVTKOpenGLWidget *renderWindowInteractor; // 交互器
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style; // 交互器样式
    vtkSmartPointer<vtkAxesActor> axes; // 坐标系
    vtkSmartPointer<vtkOrientationMarkerWidget> marker; // 坐标系标记
};
#endif // MAINWINDOW_H
