#include "reportview.h"
#include "ui_reportview.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QTextStream>
#include <QDebug>
#include <QThread>

ReportView::ReportView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportView)
{
    ui->setupUi(this);

    // 初始化后台工作线程
    initWorkerThread();

    // 连接信号槽
    connect(ui->btnGenerate, &QPushButton::clicked, this, &ReportView::on_btnGenerate_clicked);
    connect(ui->btnExportHtml, &QPushButton::clicked, this, &ReportView::on_btnExportHtml_clicked);
    connect(ui->btnExportCsv, &QPushButton::clicked, this, &ReportView::on_btnExportCsv_clicked);

    // 初始化进度条
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);

    // 设置初始状态
    ui->textBrowser->setHtml(QString(R"(
        <div style="text-align: center; padding: 50px; color: #7f8c8d;">
            <h2>统计报表系统</h2>
            <p>请选择报表类型并点击"生成报表"按钮开始统计</p>
            <p>系统将在后台执行统计计算，不会影响主界面的正常操作</p>
        </div>
    )"));
}

ReportView::~ReportView()
{
    cleanupWorkerThread();
    delete ui;
}

void ReportView::initWorkerThread()
{
    // 创建后台工作对象
    worker = new ReportWorker();

    // 将工作对象移动到后台线程
    worker->moveToThread(&workerThread);

    // 连接工作线程的信号槽
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ReportView::destroyed, &workerThread, &QThread::quit);

    // 连接工作对象的信号到界面槽函数
    connect(worker, &ReportWorker::progressChanged, this, &ReportView::onProgressChanged);
    connect(worker, &ReportWorker::statisticsCompleted, this, &ReportView::onStatisticsCompleted);
    connect(worker, &ReportWorker::errorOccurred, this, &ReportView::onErrorOccurred);

    // 启动后台线程
    workerThread.start();
}

void ReportView::cleanupWorkerThread()
{
    // 退出线程
    if (workerThread.isRunning()) {
        workerThread.quit();
        workerThread.wait();
    }
}

void ReportView::on_btnGenerate_clicked()
{
    generateReport();
}

void ReportView::on_btnExportHtml_clicked()
{
    exportToHtml();
}

void ReportView::on_btnExportCsv_clicked()
{
    exportToCsv();
}

void ReportView::onProgressChanged(int progress, const QString& message)
{
    // 更新进度条
    ui->progressBar->setValue(progress);

    // 更新状态提示
    ui->textBrowser->append(QString(R"(<div style="color: #3498db;">%1... (%2%)</div>)").arg(message).arg(progress));
}

void ReportView::onStatisticsCompleted(const StatisticsResult& result)
{
    // 隐藏进度条
    ui->progressBar->setVisible(false);

    // 保存当前结果
    currentResult = result;

    // 检查是否有错误
    if (!result.errorMessage.isEmpty()) {
        ui->textBrowser->append(QString(R"(<div style="color: #e74c3c;">错误: %1</div>)").arg(result.errorMessage));
        return;
    }

    // 显示统计结果
    displayResults();

    // 显示完成提示
    ui->textBrowser->append(QString(R"(
        <div style="text-align: center; padding: 20px; margin-top: 20px; background-color: #d5f5e3; border-radius: 5px;">
            <span style="color: #27ae60; font-weight: bold;">✓ 报表生成完成！</span>
        </div>
    )"));
}

void ReportView::onErrorOccurred(const QString& error)
{
    // 隐藏进度条
    ui->progressBar->setVisible(false);

    // 显示错误信息
    ui->textBrowser->append(QString(R"(
        <div style="padding: 15px; background-color: #fadbd8; border-radius: 5px; margin-top: 10px;">
            <h3 style="color: #e74c3c; margin: 0 0 10px 0;">统计过程发生错误</h3>
            <p style="color: #c0392b; margin: 0;">%1</p>
        </div>
    )").arg(error));
}

void ReportView::generateReport()
{
    // 获取选择的报表类型
    int index = ui->comboReportType->currentIndex();
    currentReportType = static_cast<ReportType>(index);

    // 清空显示区域
    ui->textBrowser->clear();
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    // 添加开始提示
    ui->textBrowser->append(QString(R"(
        <div style="text-align: center; padding: 20px; background-color: #ebf5fb; border-radius: 5px; margin-bottom: 20px;">
            <h2 style="color: #2980b9; margin: 0;">%1</h2>
            <p style="color: #7f8c8d;">开始执行统计计算...</p>
        </div>
    )").arg(getReportTypeName(currentReportType)));

    // 发送信号开始统计
    QDate startDate = QDate::currentDate().addDays(-30);
    QDate endDate = QDate::currentDate();

    QMetaObject::invokeMethod(worker, [this, startDate, endDate]() {
        worker->startStatistics(currentReportType, startDate, endDate);
    }, Qt::QueuedConnection);
}

void ReportView::displayResults()
{
    // 生成HTML报告
    QString html = ReportGenerator::generateHTMLReport(currentResult, currentReportType);

    // 显示在浏览器中
    ui->textBrowser->setHtml(html);
}

void ReportView::exportToHtml()
{
    // 检查是否有数据
    if (!currentResult.isValid) {
        QMessageBox::warning(this, "导出失败", "请先生成报表数据");
        return;
    }

    // 获取保存文件路径
    QString fileName = QFileDialog::getSaveFileName(this, "保存HTML报表",
                                                    QString("report_%1.html").arg(QDate::currentDate().toString("yyyyMMdd")),
                                                    "HTML文件 (*.html)");

    if (fileName.isEmpty()) {
        return;
    }

    // 生成HTML内容
    QString html = ReportGenerator::generateHTMLReport(currentResult, currentReportType);

    // 保存到文件
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << html;
        file.close();

        QMessageBox::information(this, "导出成功", QString("报表已保存到: %1").arg(fileName));
    } else {
        QMessageBox::critical(this, "导出失败", "无法保存文件，请检查权限");
    }
}

void ReportView::exportToCsv()
{
    // 检查是否有数据
    if (!currentResult.isValid) {
        QMessageBox::warning(this, "导出失败", "请先生成报表数据");
        return;
    }

    // 获取保存文件路径
    QString fileName = QFileDialog::getSaveFileName(this, "保存CSV报表",
                                                    QString("report_%1.csv").arg(QDate::currentDate().toString("yyyyMMdd")),
                                                    "CSV文件 (*.csv)");

    if (fileName.isEmpty()) {
        return;
    }

    // 生成CSV内容
    QString csv = ReportGenerator::exportToCSV(currentResult, currentReportType);

    // 保存到文件
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << csv;
        file.close();

        QMessageBox::information(this, "导出成功", QString("报表已保存到: %1").arg(fileName));
    } else {
        QMessageBox::critical(this, "导出失败", "无法保存文件，请检查权限");
    }
}

QString ReportView::getReportTypeName(ReportType type)
{
    switch (type) {
    case ReportType::DailySummary:
        return "每日汇总报表";
    case ReportType::WeeklyStatistics:
        return "周统计报表";
    case ReportType::MonthlyReport:
        return "月度报表";
    case ReportType::DepartmentStats:
        return "科室统计报表";
    case ReportType::DoctorWorkload:
        return "医生工作量报表";
    case ReportType::AppointmentStats:
        return "预约统计报表";
    case ReportType::InventoryReport:
        return "库存报表";
    default:
        return "统计报表";
    }
}
