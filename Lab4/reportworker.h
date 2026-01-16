#ifndef REPORTWORKER_H
#define REPORTWORKER_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QDate>
#include <QString>
#include <QVector>

// 统计结果数据结构
struct StatisticsResult {
    // 病人统计
    int totalPatients = 0;
    int newPatientsThisMonth = 0;
    int newPatientsThisWeek = 0;

    // 医生统计
    int totalDoctors = 0;
    int doctorsOnDuty = 0;

    // 预约统计
    int totalAppointments = 0;
    int pendingAppointments = 0;
    int approvedAppointments = 0;
    int rejectedAppointments = 0;
    int completedAppointments = 0;

    // 药品统计
    int totalMedicines = 0;
    int lowStockMedicines = 0;

    // 科室统计
    int totalDepartments = 0;

    // 处方统计
    int totalPrescriptions = 0;

    // 近期趋势数据（用于图表）
    QVector<int> appointmentsLast7Days;
    QVector<int> newPatientsLast7Days;

    // 科室预约分布
    QMap<QString, int> appointmentsByDepartment;

    // 医生工作负荷
    QMap<QString, int> appointmentsByDoctor;

    // 统计时间范围
    QDate startDate;
    QDate endDate;
    bool isValid = false;
    QString errorMessage;
};

// 报表类型枚举
enum class ReportType {
    DailySummary,      // 每日汇总
    WeeklyStatistics,  // 周统计
    MonthlyReport,     // 月报
    DepartmentStats,   // 科室统计
    DoctorWorkload,    // 医生工作量
    AppointmentStats,  // 预约统计
    InventoryReport    // 库存报表
};

// 报表生成器类
class ReportGenerator {
public:
    static QString generateHTMLReport(const StatisticsResult& stats, ReportType type);
    static QString generateTextReport(const StatisticsResult& stats, ReportType type);
    static QString exportToCSV(const StatisticsResult& stats, ReportType type);
};

// 后台工作线程类
class ReportWorker : public QObject
{
    Q_OBJECT

public:
    explicit ReportWorker(QObject *parent = nullptr);
    ~ReportWorker();

public slots:
    // 开始执行统计任务
    void startStatistics(ReportType type, const QDate& startDate, const QDate& endDate);
    void startStatistics(ReportType type);  // 默认使用最近30天

signals:
    // 统计进度信号
    void progressChanged(int progress, const QString& message);

    // 统计完成信号
    void statisticsCompleted(const StatisticsResult& result);

    // 错误信号
    void errorOccurred(const QString& error);

private:
    // 执行实际的统计计算
    StatisticsResult calculateStatistics(ReportType type, const QDate& startDate, const QDate& endDate);

    // 各类统计方法
    StatisticsResult calculateDailySummary(const QDate& date);
    StatisticsResult calculateWeeklyStatistics(const QDate& startDate);
    StatisticsResult calculateMonthlyReport(const QDate& month);
    StatisticsResult calculateDepartmentStats();
    StatisticsResult calculateDoctorWorkload(const QDate& startDate, const QDate& endDate);
    StatisticsResult calculateAppointmentStats(const QDate& startDate, const QDate& endDate);
    StatisticsResult calculateInventoryReport();

    // 辅助方法
    int getDaysInMonth(int year, int month);
    QDate getWeekStartDate(const QDate& date);
    QDate getWeekEndDate(const QDate& date);
    QVector<QDate> getLast7Days();
};

#endif // REPORTWORKER_H
