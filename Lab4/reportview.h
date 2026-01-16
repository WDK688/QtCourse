#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <QWidget>
#include "reportworker.h"
#include <QThread>

namespace Ui {
class ReportView;
}

class ReportView : public QWidget
{
    Q_OBJECT

public:
    explicit ReportView(QWidget *parent = nullptr);
    ~ReportView();

private slots:
    void on_btnGenerate_clicked();
    void on_btnExportHtml_clicked();
    void on_btnExportCsv_clicked();

    // 接收后台线程的信号
    void onProgressChanged(int progress, const QString& message);
    void onStatisticsCompleted(const StatisticsResult& result);
    void onErrorOccurred(const QString& error);

private:
    Ui::ReportView *ui;

    // 后台线程相关
    QThread workerThread;
    ReportWorker* worker;

    // 当前统计结果
    StatisticsResult currentResult;
    ReportType currentReportType;

    // 初始化后台线程
    void initWorkerThread();
    void cleanupWorkerThread();

    // 生成报表
    void generateReport();
    void displayResults();

    // 导出报表
    void exportToHtml();
    void exportToCsv();

    // 获取报表类型名称
    QString getReportTypeName(ReportType type);
};

#endif // REPORTVIEW_H
