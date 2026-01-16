#include "reportworker.h"
#include "idatabase.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ReportWorker::ReportWorker(QObject *parent) : QObject(parent)
{
}

ReportWorker::~ReportWorker()
{
}

void ReportWorker::startStatistics(ReportType type, const QDate& startDate, const QDate& endDate)
{
    emit progressChanged(0, "开始执行统计计算...");

    try {
        StatisticsResult result = calculateStatistics(type, startDate, endDate);
        emit progressChanged(100, "统计计算完成");
        emit statisticsCompleted(result);
    } catch (const std::exception& e) {
        emit errorOccurred(QString("统计过程发生异常: %1").arg(e.what()));
    } catch (...) {
        emit errorOccurred("统计过程发生未知错误");
    }
}

void ReportWorker::startStatistics(ReportType type)
{
    // 默认统计最近30天的数据
    QDate endDate = QDate::currentDate();
    QDate startDate = endDate.addDays(-30);
    startStatistics(type, startDate, endDate);
}

StatisticsResult ReportWorker::calculateStatistics(ReportType type, const QDate& startDate, const QDate& endDate)
{
    StatisticsResult result;
    result.startDate = startDate;
    result.endDate = endDate;

    switch (type) {
    case ReportType::DailySummary:
        result = calculateDailySummary(endDate);
        break;
    case ReportType::WeeklyStatistics:
        result = calculateWeeklyStatistics(startDate);
        break;
    case ReportType::MonthlyReport:
        result = calculateMonthlyReport(endDate);
        break;
    case ReportType::DepartmentStats:
        result = calculateDepartmentStats();
        break;
    case ReportType::DoctorWorkload:
        result = calculateDoctorWorkload(startDate, endDate);
        break;
    case ReportType::AppointmentStats:
        result = calculateAppointmentStats(startDate, endDate);
        break;
    case ReportType::InventoryReport:
        result = calculateInventoryReport();
        break;
    default:
        result.errorMessage = "未知的报表类型";
        return result;
    }

    result.isValid = result.errorMessage.isEmpty();
    return result;
}

StatisticsResult ReportWorker::calculateDailySummary(const QDate& date)
{
    StatisticsResult result;
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        // 统计今日新增病人
        QString todayStr = date.toString("yyyy-MM-dd");
        QSqlQuery query(database);

        // 病人总数
        query.exec("SELECT COUNT(*) FROM patient");
        if (query.next()) {
            result.totalPatients = query.value(0).toInt();
        }

        // 今日新增病人
        query.prepare("SELECT COUNT(*) FROM patient WHERE CREATEDTIMESTAMP = :today");
        query.bindValue(":today", todayStr);
        if (query.exec() && query.next()) {
            result.newPatientsThisMonth = query.value(0).toInt();
        }

        // 医生总数
        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) {
            result.totalDoctors = query.value(0).toInt();
        }

        // 预约统计
        query.exec("SELECT COUNT(*) FROM appointment");
        if (query.next()) {
            result.totalAppointments = query.value(0).toInt();
        }

        // 待审核预约
        query.exec("SELECT COUNT(*) FROM appointment WHERE status IS NULL OR status = ''");
        if (query.next()) {
            result.pendingAppointments = query.value(0).toInt();
        }

        // 已通过预约
        query.exec("SELECT COUNT(*) FROM appointment WHERE status = 'approved'");
        if (query.next()) {
            result.approvedAppointments = query.value(0).toInt();
        }

        // 已拒绝预约
        query.exec("SELECT COUNT(*) FROM appointment WHERE status = 'rejected'");
        if (query.next()) {
            result.rejectedAppointments = query.value(0).toInt();
        }

        // 药品总数
        query.exec("SELECT COUNT(*) FROM medicine");
        if (query.next()) {
            result.totalMedicines = query.value(0).toInt();
        }

        // 科室总数
        query.exec("SELECT COUNT(*) FROM department");
        if (query.next()) {
            result.totalDepartments = query.value(0).toInt();
        }

        // 处方总数
        query.exec("SELECT COUNT(*) FROM prescription");
        if (query.next()) {
            result.totalPrescriptions = query.value(0).toInt();
        }

        // 计算最近7天的预约趋势
        // result.appointmentsLast7Days = getLast7Days().indices(
        //     [&date, &database](const QDate& d) {
        //         QSqlQuery query(database);
        //         QString dateStr = d.toString("yyyy-MM-dd");
        //         query.prepare("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) = :date");
        //         query.bindValue(":date", dateStr);
        //         if (query.exec() && query.next()) {
        //             return query.value(0).toInt();
        //         }
        //         return 0;
        //     }
        //     );

    } catch (...) {
        result.errorMessage = "统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateWeeklyStatistics(const QDate& startDate)
{
    StatisticsResult result;
    QDate endDate = startDate.addDays(6);
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 按天统计预约
        QDate currentDate = startDate;
        int dayIndex = 0;
        while (currentDate <= endDate && dayIndex < 7) {
            QString dateStr = currentDate.toString("yyyy-MM-dd");
            query.prepare("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) = :date");
            query.bindValue(":date", dateStr);
            if (query.exec() && query.next()) {
                if (dayIndex < result.appointmentsLast7Days.size()) {
                    result.appointmentsLast7Days[dayIndex] = query.value(0).toInt();
                }
            }
            currentDate = currentDate.addDays(1);
            dayIndex++;
        }

        // 统计本周新增病人
        query.prepare("SELECT COUNT(*) FROM patient WHERE date(CREATEDTIMESTAMP) >= :start AND date(CREATEDTIMESTAMP) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.newPatientsThisWeek = query.value(0).toInt();
        }

        // 基本统计
        query.exec("SELECT COUNT(*) FROM patient");
        if (query.next()) result.totalPatients = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment");
        if (query.next()) result.totalAppointments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM medicine");
        if (query.next()) result.totalMedicines = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM department");
        if (query.next()) result.totalDepartments = query.value(0).toInt();

    } catch (...) {
        result.errorMessage = "周统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateMonthlyReport(const QDate& month)
{
    StatisticsResult result;
    QDate startDate(month.year(), month.month(), 1);
    QDate endDate = startDate.addMonths(1).addDays(-1);
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 本月新增病人
        query.prepare("SELECT COUNT(*) FROM patient WHERE date(CREATEDTIMESTAMP) >= :start AND date(CREATEDTIMESTAMP) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.newPatientsThisMonth = query.value(0).toInt();
        }

        // 本月预约统计
        query.prepare("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) >= :start AND date(appointmentTime) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.totalAppointments = query.value(0).toInt();
        }

        // 基本统计
        query.exec("SELECT COUNT(*) FROM patient");
        if (query.next()) result.totalPatients = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM medicine");
        if (query.next()) result.totalMedicines = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM department");
        if (query.next()) result.totalDepartments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM prescription");
        if (query.next()) result.totalPrescriptions = query.value(0).toInt();

        // 统计各状态预约数量
        query.prepare("SELECT COUNT(*) FROM appointment WHERE status = 'approved' AND date(appointmentTime) >= :start AND date(appointmentTime) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.approvedAppointments = query.value(0).toInt();
        }

        query.prepare("SELECT COUNT(*) FROM appointment WHERE status = 'rejected' AND date(appointmentTime) >= :start AND date(appointmentTime) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.rejectedAppointments = query.value(0).toInt();
        }

        // 库存不足药品（假设库存字段为stock）
        query.exec("SELECT COUNT(*) FROM medicine WHERE stock < 10");
        if (query.next()) {
            result.lowStockMedicines = query.value(0).toInt();
        }

    } catch (...) {
        result.errorMessage = "月报统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateDepartmentStats()
{
    StatisticsResult result;
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 统计每个科室的医生数量
        query.exec("SELECT d.name, COUNT(doc.id) as doctor_count "
                   "FROM department d LEFT JOIN doctor doc ON d.id = doc.department_id "
                   "GROUP BY d.id");
        while (query.next()) {
            QString deptName = query.value(0).toString();
            int doctorCount = query.value(1).toInt();
            result.appointmentsByDepartment[deptName] = doctorCount;
        }

        // 统计每个科室的预约数量
        query.exec("SELECT d.name, COUNT(a.id) as appointment_count "
                   "FROM department d LEFT JOIN doctor doc ON d.id = doc.department_id "
                   "LEFT JOIN appointment a ON doc.id = a.doctor_id "
                   "GROUP BY d.id");
        while (query.next()) {
            QString deptName = query.value(0).toString();
            int appointmentCount = query.value(1).toInt();
            result.appointmentsByDepartment[deptName] += appointmentCount * 10; // 用于显示权重
        }

        // 基本统计
        query.exec("SELECT COUNT(*) FROM department");
        if (query.next()) result.totalDepartments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment");
        if (query.next()) result.totalAppointments = query.value(0).toInt();

    } catch (...) {
        result.errorMessage = "科室统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateDoctorWorkload(const QDate& startDate, const QDate& endDate)
{
    StatisticsResult result;
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 统计每个医生的预约数量
        query.prepare("SELECT d.name, COUNT(a.id) as appointment_count "
                      "FROM doctor d LEFT JOIN appointment a ON d.id = a.doctor_id "
                      "WHERE a.appointmentTime IS NULL OR "
                      "(date(a.appointmentTime) >= :start AND date(a.appointmentTime) <= :end) "
                      "GROUP BY d.id");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));

        if (query.exec()) {
            while (query.next()) {
                QString doctorName = query.value(0).toString();
                int appointmentCount = query.value(1).toInt();
                result.appointmentsByDoctor[doctorName] = appointmentCount;
            }
        }

        // 基本统计
        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) >= :start AND date(appointmentTime) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.next()) result.totalAppointments = query.value(0).toInt();

    } catch (...) {
        result.errorMessage = "医生工作量统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateAppointmentStats(const QDate& startDate, const QDate& endDate)
{
    StatisticsResult result;
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 统计预约状态分布
        query.exec("SELECT COUNT(*) FROM appointment WHERE status IS NULL OR status = ''");
        if (query.next()) result.pendingAppointments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment WHERE status = 'approved'");
        if (query.next()) result.approvedAppointments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment WHERE status = 'rejected'");
        if (query.next()) result.rejectedAppointments = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM appointment WHERE status = 'completed'");
        if (query.next()) result.completedAppointments = query.value(0).toInt();

        // 时间范围内的预约统计
        query.prepare("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) >= :start AND date(appointmentTime) <= :end");
        query.bindValue(":start", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":end", endDate.toString("yyyy-MM-dd"));
        if (query.exec() && query.next()) {
            result.totalAppointments = query.value(0).toInt();
        }

        // 每日预约趋势
        QDate currentDate = startDate;
        int dayIndex = 0;
        while (currentDate <= endDate && dayIndex < 7) {
            QString dateStr = currentDate.toString("yyyy-MM-dd");
            query.prepare("SELECT COUNT(*) FROM appointment WHERE date(appointmentTime) = :date");
            query.bindValue(":date", dateStr);
            if (query.exec() && query.next()) {
                if (dayIndex < result.appointmentsLast7Days.size()) {
                    result.appointmentsLast7Days[dayIndex] = query.value(0).toInt();
                }
            }
            currentDate = currentDate.addDays(1);
            dayIndex++;
        }

        // 基本统计
        query.exec("SELECT COUNT(*) FROM patient");
        if (query.next()) result.totalPatients = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

    } catch (...) {
        result.errorMessage = "预约统计计算过程中发生错误";
    }

    return result;
}

StatisticsResult ReportWorker::calculateInventoryReport()
{
    StatisticsResult result;
    IDatabase& db = IDatabase::getInstance();
    QSqlDatabase database = db.getDatabase();

    try {
        QSqlQuery query(database);

        // 药品总数
        query.exec("SELECT COUNT(*) FROM medicine");
        if (query.next()) result.totalMedicines = query.value(0).toInt();

        // 库存不足药品（假设库存字段为stock）
        query.exec("SELECT COUNT(*) FROM medicine WHERE stock < 10");
        if (query.next()) result.lowStockMedicines = query.value(0).toInt();

        // 基本统计
        query.exec("SELECT COUNT(*) FROM prescription");
        if (query.next()) result.totalPrescriptions = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM patient");
        if (query.next()) result.totalPatients = query.value(0).toInt();

        query.exec("SELECT COUNT(*) FROM doctor");
        if (query.next()) result.totalDoctors = query.value(0).toInt();

    } catch (...) {
        result.errorMessage = "库存报表统计计算过程中发生错误";
    }

    return result;
}

int ReportWorker::getDaysInMonth(int year, int month)
{
    QDate date(year, month, 1);
    return date.daysInMonth();
}

QDate ReportWorker::getWeekStartDate(const QDate& date)
{
    int daysSinceMonday = date.dayOfWeek() - 1;
    return date.addDays(-daysSinceMonday);
}

QDate ReportWorker::getWeekEndDate(const QDate& date)
{
    int daysUntilSunday = 7 - date.dayOfWeek();
    return date.addDays(daysUntilSunday);
}

QVector<QDate> ReportWorker::getLast7Days()
{
    QVector<QDate> dates;
    QDate today = QDate::currentDate();
    for (int i = 6; i >= 0; i--) {
        dates.append(today.addDays(-i));
    }
    return dates;
}

// 报表生成实现
QString ReportGenerator::generateHTMLReport(const StatisticsResult& stats, ReportType type)
{
    QString reportTypeName;
    switch (type) {
    case ReportType::DailySummary: reportTypeName = "每日汇总报表"; break;
    case ReportType::WeeklyStatistics: reportTypeName = "周统计报表"; break;
    case ReportType::MonthlyReport: reportTypeName = "月度报表"; break;
    case ReportType::DepartmentStats: reportTypeName = "科室统计报表"; break;
    case ReportType::DoctorWorkload: reportTypeName = "医生工作量报表"; break;
    case ReportType::AppointmentStats: reportTypeName = "预约统计报表"; break;
    case ReportType::InventoryReport: reportTypeName = "库存报表"; break;
    default: reportTypeName = "统计报表"; break;
    }

    QString html = QString(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>%1</title>
    <style>
        body { font-family: 'Microsoft YaHei', Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }
        .container { max-width: 900px; margin: 0 auto; background-color: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #2c3e50; text-align: center; border-bottom: 3px solid #3498db; padding-bottom: 10px; }
        .info { color: #7f8c8d; text-align: center; margin-bottom: 20px; }
        .section { margin: 20px 0; padding: 15px; border: 1px solid #ecf0f1; border-radius: 5px; background-color: #fafafa; }
        .section h2 { color: #3498db; margin-top: 0; border-left: 4px solid #3498db; padding-left: 10px; }
        .stat-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; margin-top: 15px; }
        .stat-item { background-color: white; padding: 15px; border-radius: 5px; text-align: center; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
        .stat-value { font-size: 28px; font-weight: bold; color: #2c3e50; }
        .stat-label { color: #7f8c8d; margin-top: 5px; }
        .chart { margin: 20px 0; padding: 20px; background-color: #f8f9fa; border-radius: 5px; }
        table { width: 100%%; border-collapse: collapse; margin-top: 15px; }
        th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ecf0f1; }
        th { background-color: #3498db; color: white; }
        tr:hover { background-color: #f5f5f5; }
        .footer { margin-top: 30px; padding-top: 20px; border-top: 1px solid #ecf0f1; text-align: center; color: #95a5a6; }
        .status-approved { color: #27ae60; font-weight: bold; }
        .status-pending { color: #f39c12; font-weight: bold; }
        .status-rejected { color: #e74c3c; font-weight: bold; }
    </style>
</head>
<body>
    <div class="container">
        <h1>%1</h1>
        <div class="info">
            <p>统计时间: %2 至 %3</p>
            <p>生成时间: %4</p>
        </div>

        <div class="section">
            <h2>基本统计概览</h2>
            <div class="stat-grid">
                <div class="stat-item">
                    <div class="stat-value">%5</div>
                    <div class="stat-label">病人总数</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">%6</div>
                    <div class="stat-label">医生总数</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">%7</div>
                    <div class="stat-label">科室数量</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value">%8</div>
                    <div class="stat-label">药品数量</div>
                </div>
            </div>
        </div>

        <div class="section">
            <h2>预约统计</h2>
            <div class="stat-grid">
                <div class="stat-item">
                    <div class="stat-value">%9</div>
                    <div class="stat-label">预约总数</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value status-pending">%10</div>
                    <div class="stat-label">待审核</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value status-approved">%11</div>
                    <div class="stat-label">已通过</div>
                </div>
                <div class="stat-item">
                    <div class="stat-value status-rejected">%12</div>
                    <div class="stat-label">已拒绝</div>
                </div>
            </div>
        </div>

        <div class="section">
            <h2>处方统计</h2>
            <div class="stat-grid">
                <div class="stat-item">
                    <div class="stat-value">%13</div>
                    <div class="stat-label">处方总数</div>
                </div>
            </div>
        </div>
)"
                           ).arg(reportTypeName)
                       .arg(stats.startDate.toString("yyyy年MM月dd日"))
                       .arg(stats.endDate.toString("yyyy年MM月dd日"))
                       .arg(QDateTime::currentDateTime().toString("yyyy年MM月dd日 HH:mm:ss"))
                       .arg(stats.totalPatients)
                       .arg(stats.totalDoctors)
                       .arg(stats.totalDepartments)
                       .arg(stats.totalMedicines)
                       .arg(stats.totalAppointments)
                       .arg(stats.pendingAppointments)
                       .arg(stats.approvedAppointments)
                       .arg(stats.rejectedAppointments)
                       .arg(stats.totalPrescriptions);

    // 如果有科室统计数据，添加表格
    if (!stats.appointmentsByDepartment.isEmpty()) {
        html += R"(
        <div class="section">
            <h2>科室统计</h2>
            <table>
                <tr><th>科室名称</th><th>统计值</th></tr>
)";
        for (auto it = stats.appointmentsByDepartment.begin(); it != stats.appointmentsByDepartment.end(); ++it) {
            html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(it.key()).arg(it.value());
        }
        html += R"(</table></div>)";
    }

    // 如果有医生工作量数据，添加表格
    if (!stats.appointmentsByDoctor.isEmpty()) {
        html += R"(
        <div class="section">
            <h2>医生工作量</h2>
            <table>
                <tr><th>医生姓名</th><th>预约数量</th></tr>
)";
        for (auto it = stats.appointmentsByDoctor.begin(); it != stats.appointmentsByDoctor.end(); ++it) {
            html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(it.key()).arg(it.value());
        }
        html += R"(</table></div>)";
    }

    html += R"(
        <div class="footer">
            <p>医院管理系统 - 报表生成系统</p>
            <p>Powered by MiniMax Agent</p>
        </div>
    </div>
</body>
</html>
)";

    return html;
}

QString ReportGenerator::generateTextReport(const StatisticsResult& stats, ReportType type)
{
    QString reportTypeName;
    switch (type) {
    case ReportType::DailySummary: reportTypeName = "每日汇总报表"; break;
    case ReportType::WeeklyStatistics: reportTypeName = "周统计报表"; break;
    case ReportType::MonthlyReport: reportTypeName = "月度报表"; break;
    case ReportType::DepartmentStats: reportTypeName = "科室统计报表"; break;
    case ReportType::DoctorWorkload: reportTypeName = "医生工作量报表"; break;
    case ReportType::AppointmentStats: reportTypeName = "预约统计报表"; break;
    case ReportType::InventoryReport: reportTypeName = "库存报表"; break;
    default: reportTypeName = "统计报表"; break;
    }

    QString report;
    QTextStream out(&report);

    out << "================================================\n";
    out << "              " << reportTypeName << "\n";
    out << "================================================\n\n";
    out << "统计时间: " << stats.startDate.toString("yyyy年MM月dd日")
        << " 至 " << stats.endDate.toString("yyyy年MM月dd日") << "\n";
    out << "生成时间: " << QDateTime::currentDateTime().toString("yyyy年MM月dd日 HH:mm:ss") << "\n\n";

    out << "【基本统计概览】\n";
    out << "------------------------------------------------\n";
    out << QString("  病人总数:     %1\n").arg(stats.totalPatients);
    out << QString("  医生总数:     %1\n").arg(stats.totalDoctors);
    out << QString("  科室数量:     %1\n").arg(stats.totalDepartments);
    out << QString("  药品数量:     %1\n").arg(stats.totalMedicines);
    out << QString("  处方总数:     %1\n").arg(stats.totalPrescriptions);
    out << "\n";

    out << "【预约统计】\n";
    out << "------------------------------------------------\n";
    out << QString("  预约总数:     %1\n").arg(stats.totalAppointments);
    out << QString("  待审核:       %1\n").arg(stats.pendingAppointments);
    out << QString("  已通过:       %1\n").arg(stats.approvedAppointments);
    out << QString("  已拒绝:       %1\n").arg(stats.rejectedAppointments);
    out << QString("  已完成:       %1\n").arg(stats.completedAppointments);
    out << "\n";

    if (!stats.appointmentsByDepartment.isEmpty()) {
        out << "【科室统计】\n";
        out << "------------------------------------------------\n";
        for (auto it = stats.appointmentsByDepartment.begin(); it != stats.appointmentsByDepartment.end(); ++it) {
            out << QString("  %1: %2\n").arg(it.key(), -15).arg(it.value());
        }
        out << "\n";
    }

    if (!stats.appointmentsByDoctor.isEmpty()) {
        out << "【医生工作量】\n";
        out << "------------------------------------------------\n";
        for (auto it = stats.appointmentsByDoctor.begin(); it != stats.appointmentsByDoctor.end(); ++it) {
            out << QString("  %1: %2个预约\n").arg(it.key(), -15).arg(it.value());
        }
        out << "\n";
    }

    out << "================================================\n";
    out << "              报表结束\n";
    out << "================================================\n";

    return report;
}

QString ReportGenerator::exportToCSV(const StatisticsResult& stats, ReportType type)
{
    Q_UNUSED(type)
    QString csv;
    QTextStream out(&csv);

    // CSV表头
    out << "统计项目,数值\n";

    // 基本统计
    out << "病人总数," << stats.totalPatients << "\n";
    out << "医生总数," << stats.totalDoctors << "\n";
    out << "科室数量," << stats.totalDepartments << "\n";
    out << "药品数量," << stats.totalMedicines << "\n";
    out << "处方总数," << stats.totalPrescriptions << "\n";

    // 预约统计
    out << "预约总数," << stats.totalAppointments << "\n";
    out << "待审核," << stats.pendingAppointments << "\n";
    out << "已通过," << stats.approvedAppointments << "\n";
    out << "已拒绝," << stats.rejectedAppointments << "\n";
    out << "已完成," << stats.completedAppointments << "\n";

    // 统计时间
    out << "统计开始日期," << stats.startDate.toString("yyyy-MM-dd") << "\n";
    out << "统计结束日期," << stats.endDate.toString("yyyy-MM-dd") << "\n";

    return csv;
}
