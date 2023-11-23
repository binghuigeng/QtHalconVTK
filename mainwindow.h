#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "aboutdialog.h"

// HALCON headers
#include "HalconCpp.h"

// VTK headers
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

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
    /// @brief // 重写窗口的关闭事件处理
    void closeEvent(QCloseEvent *event) override;

private slots:
    /// @brief 打开文件
    void slt_actOpen_triggered();

    /// @brief 重置
    void slt_actReset_triggered();

    /// @brief 添加
    void slt_actAdd_triggered();

    /// @brief 关于
    void slt_actAbout_triggered();

private:
    /// @brief 加载点云文件
    /// @return true:成功 false:失败
    bool loadPointCloudFile(QString fileName);

    /// @brief 获取点云数据
    /// @return true:成功 false:失败
    bool getPointCloudData();

    /// @brief 提取点云坐标
    void extractPointCloudCoordinates();

    /// @brief 显示点云
    void showPointCloud();

    /// @brief 初始化 VTK
    void initVTK();

    /// @brief 初始化状态栏显示消息
    void initStatusbarMessage();

    /// @brief 初始化信号与槽函数
    void initSignalsAndSlots();

#if 0
    /// @brief 默认渲染窗口显示点云
    void showDefaultWindow();

    /// @brief VTKWidget 显示点云
    void showVTKWidget();
#endif

private:
    Ui::MainWindow *ui;

    /// @brief 状态栏消息标签
    QLabel *statusLabel;

    /// @brief 关于对话框
    AboutDialog dlgAbout;

    // HALCON variables
    HalconCpp::HObjectModel3D model3D;
    HalconCpp::HTuple x, y, z;
    HalconCpp::HTuple pointCount;

    // VTK variables
    vtkSmartPointer<vtkPoints> points; // 点云数据
    vtkSmartPointer<vtkCellArray> vertices; // 顶点
    vtkSmartPointer<vtkPolyData> polyData; // 多边形数据
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter; // 顶点滤波器
    vtkSmartPointer<vtkPolyDataMapper> mapper; // 点云映射器
    vtkSmartPointer<vtkActor> actor; // 点云演员
    vtkSmartPointer<vtkRenderer> renderer; // 渲染器
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow; // 渲染窗口
    QVTKOpenGLWidget *renderWindowInteractor; // 交互器
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style; // 交互器样式
};
#endif // MAINWINDOW_H
