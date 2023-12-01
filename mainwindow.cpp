#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标志位
    if (SysConfig::getWindowTop()) {
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint); // 窗口置顶
    }
    this->setWindowTitle("点云编辑器 - guchi"); // 设置窗口标题

    initial(); // 初始化
    initSysConfig(); // 初始化配置
    initCloseWindow(); // 初始化关闭窗口
    initStatusbarMessage(); // 初始化状态栏显示消息
    initVTK(); // 初始化VTK
    initSignalsAndSlots(); // 初始化信号与槽函数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#if 0
    // 基于静态函数创建消息对话框
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                                tr("Are you sure you want to close?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore(); // 忽略关闭事件
    } else {
        event->accept(); // 接受关闭事件
    }
#endif

#if 0
    // 基于属性的 API 创建消息对话框
    QMessageBox msgBox;
    msgBox.setWindowTitle(this->windowTitle());
    msgBox.setText("Are you sure you want to close?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);

    // 显示对话框，并在用户点击确认按钮时关闭窗口
    if (msgBox.exec() == QMessageBox::Yes) {
        event->accept(); // 接受关闭事件
    } else {
        event->ignore(); // 忽略关闭事件
    }
#endif

    if (SysConfig::getWindowClose()) {
        QApplication::setActiveWindow(&msgBox); // 将指定窗口设置为活动窗口并将焦点设置到该窗口
        if (msgBox.isVisible() && bAcceptClose) {
            if (bAcceptClose) {
                readyQuit(); // 准备退出
                event->accept(); // 接受关闭事件
            }
        } else {
            event->ignore(); // 忽略关闭事件
            msgBox.show();
        }
    } else {
        readyQuit(); // 准备退出
        event->accept(); // 接受关闭事件
    }
}

void MainWindow::readyQuit()
{
    // 关闭关于对话框
    if (dlgAbout.isVisible()) {
        dlgAbout.close();
    }

    // 关闭设置对话框
    if (dlgSet.isVisible()) {
        dlgSet.close();
    }

    // 关闭通用对话框
    if (dlgCommon.isVisible()) {
        dlgCommon.close();
    }
}

void MainWindow::sltWindowOnTop()
{
    // 设置窗口标志位 隐藏窗口的最小化和最大化按钮，以及窗口置顶
    if (this->windowFlags() & Qt::WindowStaysOnTopHint) {
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint); // 取消窗口置顶
    } else {
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint); // 窗口置顶
    }
    this->show(); // 重新显示窗口

//    // 设置窗口标志位 隐藏窗口的最小化和最大化按钮，以及窗口置顶
//    Qt::WindowFlags flags = this->windowFlags(); // 获取窗口标志位
//    if (enable) {
//        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint); // 窗口置顶
//    } else {
//        this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint); // 取消窗口置顶
//    }
//    this->show(); // 重新显示窗口
}

void MainWindow::sltRendererBackground(SysConfig::RendererBackground index)
{
    // 设置渲染器背景颜色
    switch (index) {
    case SysConfig::Default:
        renderer->SetBackground(0.1, 0.2, 0.4); // 深蓝色
        break;
    case SysConfig::Black:
        renderer->SetBackground(0.0, 0.0, 0.0); // 黑色
        break;
    case SysConfig::Gray:
        renderer->SetBackground(0.5, 0.5, 0.5); // 灰色
        break;
    default:
        renderer->SetBackground(0.1, 0.2, 0.4); // 深蓝色
        break;
    }
    renderWindow->Render(); // 刷新渲染窗口
}

void MainWindow::slt_actOpen_triggered()
{
    // 选择 .ply 文件
    QString filename = QFileDialog::getOpenFileName(
                this, "打开文件", "", "点云文件 (*.ply)");

    if (!filename.isEmpty()) {
        // 加载点云文件
        if (loadPointCloudFile(filename))
        {
            // 获取点云数据
            if (getPointCloudData()) {
                extractPointCloudCoordinates(); // 提取点云坐标
                showPointNum(); // 显示点云个数
                showPointCloud(); // 显示点云
            }
        }
    }
}

void MainWindow::slt_actQuit_triggered()
{
    this->close();
}

void MainWindow::slt_actReset_triggered()
{
    // 清空插入的点
    points->Reset();

    // 刷新渲染窗口
    renderWindow->Render();
    renderWindowInteractor->GetInteractor()->Render();
}

void MainWindow::slt_actAdd_triggered()
{
    // 清空插入的点
    points->Reset();

    // 插入点云数据
    points->InsertNextPoint(0, 0, 0); // 添加点坐标
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(1, 1, 1);

    // 假设 points 已经被改变
    points->Modified(); // 标记 points 数据已经被修改

    // 清空插入的顶点
    vertices->Reset();

    // 插入顶点 创建一个包含单个顶点的单元，并将点云数据中的每个点与这些单元关联起来，从而定义了点云数据的拓扑结构
    for (int i = 0; i < points->GetNumberOfPoints(); i++)
    {
        /********************************************************************************
        ** @brief 向 vtkCellArray 中插入一个新的单元
        **
        ** 在这里，参数 1 表示这是一个包含一个顶点的单元（在 VTK 中，点云数据中的每个点被视为一个单元）
        ** 这个函数会返回一个整数值，表示新插入的单元在 vtkCellArray 中的索引
        ********************************************************************************/
        vertices->InsertNextCell(1);
        // 将点的索引 i 添加到刚刚插入的单元中。这样就将点云数据中的每个点与一个单元关联起来了。
        vertices->InsertCellPoint(i);
    }

    // 假设 vertices 已经被改变
    vertices->Modified(); // 标记 vertices 数据已经被修改

    // 根据点云数据自适应地设置相机位置和视角
    renderer->ResetCamera();

    // 刷新渲染窗口以显示新的点云数据
    renderWindow->Render();
    renderWindowInteractor->GetInteractor()->Render();
}

void MainWindow::slt_actRestart_triggered()
{
#if 0
    qDebug() << "applicationFilePath " << QCoreApplication::applicationFilePath();
    qDebug() << "arguments " << QCoreApplication::arguments();

    /// 法一
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

    /// 法二
    QString program = QCoreApplication::applicationFilePath(); // 应用程序的路径
    QStringList arguments = QCoreApplication::arguments(); // 应用程序的参数列表
    QProcess::startDetached(program, arguments);
    qApp->exit();

    /// 法三
    // 退出当前进程
    QCoreApplication::exit(0);
    // 启动新的进程
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
#endif

    // 退出应用程序的主事件循环，从而关闭应用程序
    QCoreApplication::quit();

    // 启动新的进程来运行应用程序
    QProcess::startDetached(QCoreApplication::applicationFilePath(), QCoreApplication::arguments());
}

void MainWindow::slt_actUniversal_triggered()
{
    // 设置对话框
    if (!dlgSet.isVisible()) {
        dlgSet.show(); //非模态显示对话框
        dlgSet.setDefaultButton(); //设置默认按钮
    }
}

void MainWindow::slt_actAbout_triggered()
{
    // 关于对话框
    if (!dlgAbout.isVisible()) {
        dlgAbout.show(); //非模态显示对话框
        dlgAbout.setDefaultButton(); //设置默认按钮
    }
}

void MainWindow::buttonClicked(QAbstractButton *butClicked)
{
    if(butClicked == (QAbstractButton*)btnAccept){
        bAcceptClose = true;
        this->close();
    } else if (butClicked == (QAbstractButton*)btnReject) {
//        this->close();
    }
}

void MainWindow::slt_chkInquiry_stateChanged(int state)
{
    if (state == Qt::Checked) {
        SysConfig::setWindowClose(false);
        dlgSet.setWidgetContent(false);
    } else if (state == Qt::Unchecked) {
        SysConfig::setWindowClose(true);
        dlgSet.setWidgetContent(true);
    }
}

void MainWindow::initial()
{
    // 设置字体、字号、粗体、斜体
    ui->lbPointNum->setFont(QFont("Microsoft YaHei UI", 10, QFont::Normal, false));
}

void MainWindow::initSysConfig()
{
    dlgSet.setControlShow(SysConfig::getWindowTop(), SysConfig::getWindowClose(), SysConfig::getRendererBackground());
}

void MainWindow::initCloseWindow()
{
    bAcceptClose = false;
    msgBox.setWindowTitle(this->windowTitle()); // 设置消息对话框的标题
    msgBox.setText("您确定要关闭吗？"); // 设置消息对话框的具体内容
    msgBox.setIcon(QMessageBox::Question); // 设置消息对话框的图标
    // 自定义两个按钮
    btnAccept = msgBox.addButton("是(Y)", QMessageBox::AcceptRole); // 使用给定文本创建一个按钮，将其添加到指定角色的消息框中
    btnReject = msgBox.addButton("否(N)", QMessageBox::RejectRole); // 使用给定文本创建一个按钮，将其添加到指定角色的消息框中
    msgBox.setDefaultButton(btnAccept); // 设置消息对话框的默认按钮，即按下回车键会触发的按钮
//    chkInquiry = new QCheckBox("不再询问", &msgBox);
    chkInquiry.setText("不再询问");
    msgBox.setCheckBox(&chkInquiry); // 设置消息对话框的复选框
    /********************************************************************************
    ** @brief 设置窗口的模态性与设置窗口的标志执行顺序不能互换，否则会引发一连串问题
    **
    ** 1. 窗口置顶：初次启动窗口置顶生效，点击其他窗口窗口置顶失效
    ** 2. 窗口标志：标题栏会显示 Dialog 的 ? 按钮
    ********************************************************************************/
    msgBox.setWindowModality(Qt::NonModal); // 设置消息对话框的模态 -> 非模态
    msgBox.setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint); // 设置消息对话框的标志 -> 显示关闭按钮窗口置顶
}

void MainWindow::initStatusbarMessage()
{
//    ui->statusbar->showMessage(tr("Ready"));

    // 创建一个标签并设置居中对齐
    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setText("<a href=\"https://www.baidu.com\" style=\"text-decoration: none; color: #000000;\">Copyright 2022-2023 The guchi Company Ltd. All rights reserved.</a>");
    statusLabel->setOpenExternalLinks(true);

    // 将标签添加到状态栏，并设置其占用状态栏的比例为1
    ui->statusbar->addWidget(statusLabel, 1);
}

void MainWindow::initVTK()
{
    // 创建点云数据
    points = vtkSmartPointer<vtkPoints>::New();
//    points->InsertNextPoint(0, 0, 0); // 添加点坐标
//    points->InsertNextPoint(1, 0, 0);
//    points->InsertNextPoint(0, 1, 0);
//    points->InsertNextPoint(1, 1, 1);

    // 创建顶点
    vertices = vtkSmartPointer<vtkCellArray>::New();
//    for (int i = 0; i < points->GetNumberOfPoints(); i++)
//    {
//        vertices->InsertNextCell(1);
//        vertices->InsertCellPoint(i);
//    }

    // 创建多边形数据
    polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetVerts(vertices);

    // 创建顶点滤波器
    vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(polyData);

    // 创建点云映射器
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(vertexFilter->GetOutputPort());

    // 创建点云演员
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
//    actor->GetProperty()->SetPointSize(5);

    // 创建渲染器
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
//    renderer->SetBackground(0.1, 0.2, 0.4); // 设置渲染器背景颜色
    setRendererBackground(SysConfig::getRendererBackground()); // 依据配置文件设置渲染器背景颜色

    // 创建渲染窗口
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer); // 将渲染器添加到渲染窗口中
//    renderWindow->SetSize(800, 600); // 设置渲染窗口的大小为 800x600

    // 创建交互器
    renderWindowInteractor = new QVTKOpenGLWidget(this);
    renderWindowInteractor->SetRenderWindow(renderWindow); // 设置渲染窗口

    // 创建坐标系
    axes = vtkSmartPointer<vtkAxesActor>::New();

    // 创建坐标系标记
    marker = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    marker->SetOrientationMarker(axes); // 设置坐标系
    marker->SetInteractor(renderWindowInteractor->GetInteractor()); // 设置交互器
    marker->SetViewport(0.0, 0.0, 0.2, 0.2); // 设置坐标系标记的位置和大小
    marker->SetEnabled(1); // 启用坐标系标记
    marker->InteractiveOn(); // 启用或激活交互功能。当交互功能被启用时，用户可以使用鼠标来旋转、缩放和平移坐标轴。

    // 创建交互器样式
    style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->GetInteractor()->SetInteractorStyle(style);

    // 开启渲染和交互
    renderWindow->Render();
    /*************************************************************************
    ** @brief 移除 qvtkWidget->GetInteractor()->Start(); 的原因
    ** 因为在使用 QVTKOpenGLWidget 时，不需要显式调用 Start() 方法
    ** 相反，我们直接调用 qvtkWidget->GetInteractor()->Render() 来触发交互器的渲染
    *************************************************************************/
    renderWindowInteractor->GetInteractor()->Render();

    // 添加到布局中
    ui->verticalLayout->addWidget(renderWindowInteractor);
}

void MainWindow::initSignalsAndSlots()
{
    /************ QAction 项对应的槽 ************/
    // 打开文件
    connect(ui->actOpen, &QAction::triggered, this, &MainWindow::slt_actOpen_triggered);
    // 退出
    connect(ui->actQuit, &QAction::triggered, this, &MainWindow::slt_actQuit_triggered);
    // 重置
    connect(ui->actReset, &QAction::triggered, this, &MainWindow::slt_actReset_triggered);
    // 添加
    connect(ui->actAdd, &QAction::triggered, this, &MainWindow::slt_actAdd_triggered);
    // 重启
    connect(ui->actRestart, &QAction::triggered, this, &MainWindow::slt_actRestart_triggered);
    // 通用
    connect(ui->actUniversal, &QAction::triggered, this, &MainWindow::slt_actUniversal_triggered);
    // 关于
    connect(ui->actAbout, &QAction::triggered, this, &MainWindow::slt_actAbout_triggered);
    // 连接信号与槽，监听用户点击的按钮，如果用户同意关闭，则程序退出
    connect(&msgBox, &QMessageBox::buttonClicked, this, &MainWindow::buttonClicked);
    // 窗口询问改变
    connect(&chkInquiry, &QCheckBox::stateChanged, this, &MainWindow::slt_chkInquiry_stateChanged);
    /************ 设置 ************/
    // 窗口置顶
    // 渲染背景
    connect(&dlgSet, &SetDialog::sigWindowOnTop, this, &MainWindow::sltWindowOnTop);
    connect(&dlgSet, &SetDialog::sigRendererBackground, this, &MainWindow::sltRendererBackground);
}

void MainWindow::setRendererBackground(int index)
{
    // 设置渲染器背景颜色
    switch (index) {
    case 0:
        renderer->SetBackground(0.1, 0.2, 0.4); // 深蓝色
        break;
    case 1:
        renderer->SetBackground(0.0, 0.0, 0.0); // 黑色
        break;
    case 2:
        renderer->SetBackground(0.5, 0.5, 0.5); // 灰色
        break;
    default:
        renderer->SetBackground(0.1, 0.2, 0.4); // 深蓝色
        break;
    }
}

bool MainWindow::loadPointCloudFile(QString fileName)
{
    /************************************************************************************************
    ** @brief 加载 .ply 文件
    **
    ** ReadObjectModel3d 函数的作用是从文件中读取一个 3D 对象模型。下面是各个参数的含义：
    ** 1. const char* FileName
    **     这是一个 C 风格的字符串，表示要加载的文件的完整路径和文件名。
    ** 2. const char* Scale
    **     这是一个字符串，表示模型的缩放参数。通常可以指定为 "m"（米）、"cm"（厘米）等，用于指定模型的尺度单位。
    ** 3. const char* GenParamName
    **     这是一个字符串，表示通用的加载参数的名称。可以用于指定加载模型时的一些额外参数。
    ** 4. const char* GenParamValue
    **     这是一个字符串，表示通用的加载参数的值。与上面的参数一起使用，用于指定加载模型时的一些额外参数的值。
    **
    ** 函数的返回值是一个 HString 对象，表示加载模型的结果。
    **
    ** 总的来说，ReadObjectModel3d 函数的作用是从文件中读取一个 3D 对象模型，可以指定模型的缩放参数和其他加载参数。
    ************************************************************************************************/
    HalconCpp::HString result = model3D.ReadObjectModel3d(fileName.toStdString().c_str(), "m", HalconCpp::HTuple(), HalconCpp::HTuple());

    // 检查读取结果
    if (result.IsEmpty()) {
//        std::cout << "3D object model read successfully." << std::endl;
    } else {
//        std::cout << "Failed to read 3D object model: " << result << std::endl;

        // 通用对话框
        dlgCommon.setWidgetTitle("读取 3D 对象模型失败");
        dlgCommon.setWidgetIcon(":/Dialog/qmessagebox-crit.png");
        dlgCommon.setWidgetContent(fileName);
        if (!dlgCommon.isVisible()) {
            dlgCommon.show(); //非模态显示对话框
        }
        dlgCommon.setDefaultButton(); //设置默认按钮

        return false;
    }

    return true;
}

bool MainWindow::getPointCloudData()
{
    // 获取点云数据
    x = model3D.GetObjectModel3dParams("point_coord_x");
    y = model3D.GetObjectModel3dParams("point_coord_y");
    z = model3D.GetObjectModel3dParams("point_coord_z");

//    std::cout << x.Length() << " "
//              << y.Length() << " "
//              << z.Length() << std::endl;

    if (x.Length() == y.Length() && y.Length() == z.Length()) {
//        std::cout << "object model 3dParams get successfully." << std::endl;
    } else {
//        std::cout << "Failed to get object model 3dParams." << std::endl;

        // 通用对话框
        dlgCommon.setWidgetTitle("获取点云数据个数不一致");
        dlgCommon.setWidgetIcon(":/Dialog/qmessagebox-crit.png");
        dlgCommon.setWidgetContent(QString("x: %1\ny: %2\nz: %3\n").arg(x.Length()).arg(y.Length()).arg(z.Length()));
        if (!dlgCommon.isVisible()) {
            dlgCommon.show(); //非模态显示对话框
        }
        dlgCommon.setDefaultButton(); //设置默认按钮

        return false;
    }

    return true;
}

void MainWindow::extractPointCloudCoordinates()
{
    // 提取点云坐标
    pointCount = x.Length();
//    for (int i = 0; i < pointCount; i++)
//    {
//        double pointX = x[i].D();
//        double pointY = y[i].D();
//        double pointZ = z[i].D();

//        // 在这里，你可以使用点云坐标
//        std::cout << pointX << " "
//                  << pointY << " "
//                  << pointZ << " " << std::endl;
    //    }
}

void MainWindow::showPointNum()
{
    ui->lbPointNum->setText(QString("点云个数：%1").arg(x.Length()));
}

void MainWindow::showPointCloud()
{
    // 清空插入的点
    points->Reset();

    // 插入点云数据
    for (int i = 0; i < pointCount; i++)
    {
        points->InsertNextPoint(x[i].D(), y[i].D(), z[i].D()); // 添加点坐标
    }

    // 假设 points 已经被改变
    points->Modified(); // 标记 points 数据已经被修改

    // 清空插入的顶点
    vertices->Reset();

    // 插入顶点 创建一个包含单个顶点的单元，并将点云数据中的每个点与这些单元关联起来，从而定义了点云数据的拓扑结构
    for (int i = 0; i < points->GetNumberOfPoints(); i++)
    {      
        /********************************************************************************
        ** @brief 向 vtkCellArray 中插入一个新的单元
        **
        ** 在这里，参数 1 表示这是一个包含一个顶点的单元（在 VTK 中，点云数据中的每个点被视为一个单元）
        ** 这个函数会返回一个整数值，表示新插入的单元在 vtkCellArray 中的索引
        ********************************************************************************/
        vertices->InsertNextCell(1);
        // 将点的索引 i 添加到刚刚插入的单元中。这样就将点云数据中的每个点与一个单元关联起来了。
        vertices->InsertCellPoint(i);
    }

    // 假设 vertices 已经被改变
    vertices->Modified(); // 标记 vertices 数据已经被修改

    // 根据点云数据自适应地设置相机位置和视角
    renderer->ResetCamera();

    // 刷新渲染窗口以显示新的点云数据
    renderWindow->Render();
}

#if 0
void MainWindow::showDefaultWindow()
{
    // 加载.ply文件
    HalconCpp::HObjectModel3D model3D;
    HalconCpp::HString result = model3D.ReadObjectModel3d("pointcloud.ply", "m", HalconCpp::HTuple(), HalconCpp::HTuple());

    // 检查读取结果
    if (result.IsEmpty())
    {
        std::cout << "3D object model read successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to read 3D object model: " << result << std::endl;
    }

    // 获取点云数据
    HalconCpp::HTuple x, y, z;

    x = model3D.GetObjectModel3dParams("point_coord_x");
    y = model3D.GetObjectModel3dParams("point_coord_y");
    z = model3D.GetObjectModel3dParams("point_coord_z");

    std::cout << x.Length() << " "
              << y.Length() << " "
              << z.Length() << std::endl;

    // 提取点云坐标
    HalconCpp::HTuple pointCount = x.Length();
    for (int i = 0; i < pointCount; i++)
    {
        double pointX = x[i].D();
        double pointY = y[i].D();
        double pointZ = z[i].D();

        // 在这里，你可以使用点云坐标
        std::cout << pointX  << " " << pointY << " "
                  << pointZ  << " " << std::endl;
    }

#if 0
    // 初始化Halcon
    HalconCpp::HSystem::SetSystem("width", 1024);
    HalconCpp::HSystem::SetSystem("height", 512);

    // 加载点云数据
    HalconCpp::HTuple genParamName;
    HalconCpp::HTuple genParamValue;
    HalconCpp::HTuple objectModel3D; // 使用HTuple类型的变量来存储3D对象模型
//    HalconCpp::HObjectModel3D objectModel3D;
    HalconCpp::HTuple status;
    HalconCpp::ReadObjectModel3d("pointcloud.ply", 1.0, genParamName, genParamValue, &objectModel3D, &status);

    std::cout << genParamValue << std::endl;

    // 提取点云坐标
    HalconCpp::HObjectModel3D object3D;
    HalconCpp::HTuple x, y, z;
    HalconCpp::HTuple rows, cols, depths;
    HalconCpp::GetObjectModel3dParams(object3D, x, &rows);
    HalconCpp::GetObjectModel3dParams(object3D, y, &cols);
    HalconCpp::GetObjectModel3dParams(object3D, z, &depths);
    std::cout << x  << " " << rows << " "
              << depths.Length()  << " " << std::endl;
#endif

    // 创建点云数据
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < pointCount; i++)
    {
        points->InsertNextPoint(x[i].D(), y[i].D(), z[i].D());
    }
#if 0
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(1, 1, 1);
#endif

    // 创建顶点
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < points->GetNumberOfPoints(); i++)
    {
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }

    // 创建多边形数据
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetVerts(vertices);

    // 创建顶点滤波器
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(polyData);

    // 创建点云映射器
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(vertexFilter->GetOutputPort());

    // 创建点云演员
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
//    actor->GetProperty()->SetPointSize(5);

    // 创建渲染器
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.4); // 设置渲染器背景颜色

    // 创建渲染窗口
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer); // 将渲染器添加到渲染窗口中
    renderWindow->SetSize(800, 600); // 设置渲染窗口的大小为 800x600

    // 创建交互器
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);  // 设置渲染窗口

    // 创建交互器样式
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(style);

    // 开启渲染和交互
    renderWindow->Render();
    renderWindowInteractor->Start();
}

void MainWindow::showVTKWidget()
{
    // 加载.ply文件
    HalconCpp::HObjectModel3D model3D;
    HalconCpp::HString result = model3D.ReadObjectModel3d("pointcloud.ply", "m", HalconCpp::HTuple(), HalconCpp::HTuple());

    // 检查读取结果
    if (result.IsEmpty())
    {
        std::cout << "3D object model read successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to read 3D object model: " << result << std::endl;
    }

    // 获取点云数据
    HalconCpp::HTuple x, y, z;

    x = model3D.GetObjectModel3dParams("point_coord_x");
    y = model3D.GetObjectModel3dParams("point_coord_y");
    z = model3D.GetObjectModel3dParams("point_coord_z");

    std::cout << x.Length() << " "
              << y.Length() << " "
              << z.Length() << std::endl;

    // 提取点云坐标
    HalconCpp::HTuple pointCount = x.Length();
    for (int i = 0; i < pointCount; i++)
    {
        double pointX = x[i].D();
        double pointY = y[i].D();
        double pointZ = z[i].D();

        // 在这里，你可以使用点云坐标
        std::cout << pointX  << " " << pointY << " "
                  << pointZ  << " " << std::endl;
    }

    // 创建点云数据
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < pointCount; i++)
    {
        points->InsertNextPoint(x[i].D(), y[i].D(), z[i].D());
    }
#if 0
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(1, 1, 1);
#endif

    // 创建顶点
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < points->GetNumberOfPoints(); i++)
    {
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }

    // 创建多边形数据
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetVerts(vertices);

    // 创建顶点滤波器
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(polyData);

    // 创建点云映射器
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(vertexFilter->GetOutputPort());

    // 创建点云演员
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
//    actor->GetProperty()->SetPointSize(5);

    // 创建渲染器
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.4); // 设置渲染器背景颜色

    // 创建渲染窗口
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer); // 将渲染器添加到渲染窗口中
//    renderWindow->SetSize(800, 600); // 设置渲染窗口的大小为 800x600

    // 创建交互器
    QVTKOpenGLWidget *renderWindowInteractor = new QVTKOpenGLWidget();
    renderWindowInteractor->SetRenderWindow(renderWindow);  // 设置渲染窗口

    // 创建交互器样式
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->GetInteractor()->SetInteractorStyle(style);

    // 开启渲染和交互
    renderWindow->Render();
    /*************************************************************************
    ** @brief 移除 qvtkWidget->GetInteractor()->Start(); 的原因
    ** 因为在使用 QVTKOpenGLWidget 时，不需要显式调用 Start() 方法
    ** 相反，我们直接调用 qvtkWidget->GetInteractor()->Render() 来触发交互器的渲染
    *************************************************************************/
    renderWindowInteractor->GetInteractor()->Render();

    // 添加到布局中
    ui->verticalLayout->addWidget(renderWindowInteractor);
}
#endif
