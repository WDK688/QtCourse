// #include "idatabase.h"

// #include<QUuid>
// void IDatabase::ininDatabase()
// {
//     database = QSqlDatabase::addDatabase("QSQLITE");
//     QString aFile = "E:/QtCode/lab3/lab3.db";
//     database.setDatabaseName(aFile);


//     if (!database.open()) {
//         qDebug() << "failed to open database";
//     } else {
//         qDebug() << "open database is ok";
//     }
// }

// bool IDatabase::initPatienntModel()
// {
//     patientTabModel = new QSqlTableModel(this, database);
//     patientTabModel->setTable("patient");
//     patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

//     patientTabModel->setSort(patientTabModel->fieldIndex("name"), Qt::AscendingOrder);

//     if (!(patientTabModel->select())) {
//         return false;
//     }

//     thePatientSelection = new QItemSelectionModel(patientTabModel);

//     return true;
// }

// int IDatabase::addNewPatient()
// {
//     patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());

//     QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = patientTabModel->record(curRecNo);
//     curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

//     patientTabModel->setRecord(curRecNo, curRec);
//     if (!patientTabModel->submitAll()) {
//         // 输出错误信息到控制台（调试用）
//         // qDebug() << "提交失败：" << patientTabModel->lastError().text();
//     } else {
//         // qDebug() << "提交成功";
//     }
//     return curIndex.row();
// }

// bool IDatabase::searchPatient(QString filter)
// {
//     patientTabModel->setFilter(filter);
//     return patientTabModel->select();
// }

// bool IDatabase::deleteCurrentPatient()
// {
//     // QModelIndex curIndex = thePatientSelection->currentIndex();
//     // patientTabModel->removeRow(curIndex.row());
//     // patientTabModel->submitAll();
//     // patientTabModel->select();
//     // try {
//     //     QModelIndex curIndex = thePatientSelection->currentIndex();
//     //     // 先判断索引是否有效，避免空指针/无效索引导致崩溃
//     //     if (!curIndex.isValid()) {
//     //         return false; // 索引无效，删除失败
//     //     }
//     //     // 执行删除行操作
//     //     bool isRemoved = patientTabModel->removeRow(curIndex.row());
//     //     if (isRemoved) {
//     //         patientTabModel->select(); // 刷新数据
//     //         return true; // 删除成功
//     //     } else {
//     //         return false; // 删除失败
//     //     }
//     // } catch (...) {
//     //     // 捕获异常，避免程序崩溃
//     //     return false;
//     // }
//     try {
//         QModelIndex curIndex = thePatientSelection->currentIndex();
//         if (!curIndex.isValid()) {
//             return false;
//         }

//         // 1. 移除模型中的行（内存中标记删除）
//         bool isRemoved = patientTabModel->removeRow(curIndex.row());
//         if (!isRemoved) {
//             return false;
//         }

//         // 2. 提交操作到数据库（关键步骤：将删除写入数据库）
//         bool isSubmitted = patientTabModel->submitAll();
//         if (!isSubmitted) {
//             // 提交失败时回滚，恢复模型中的行
//             patientTabModel->revertAll();
//             return false;
//         }

//         // 3. 刷新模型数据，同步数据库最新状态
//         patientTabModel->select();
//         return true;

//     } catch (...) {
//         return false;
//     }
// }

// bool IDatabase::submitPatientEdit()
// {
//     return patientTabModel->submitAll();
// }

// void IDatabase::revertPatientEdit()
// {
//     patientTabModel->revertAll();
// }

// QString IDatabase::userLogin(QString userName, QString password)
// {
//     //return "loginOK";
//     QSqlQuery query;
//     query.prepare("select username, password from user where username = :USER");
//     query.bindValue(":USER", userName);

//     query.exec();

//     if (query.first() && query.value("username").isValid()) {
//         QString passwd = query.value("password").toString();
//         if (passwd == password) {
//             return "loginOk";
//         } else {
//             qDebug() << "wrong password";
//             return "wrongPassword";
//         }
//     } else {
//         qDebug() << "no such user: " << userName;
//         return "wrongUsername";
//     }
// }


// IDatabase::IDatabase(QObject *parent)
//     : QObject{parent}
// {
//     ininDatabase();
// }

// bool IDatabase::initDepartmentModel()
// {
//     departmentTabModel = new QSqlTableModel(this, database);
//     departmentTabModel->setTable("department");
//     departmentTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     departmentTabModel->setSort(departmentTabModel->fieldIndex("name"), Qt::AscendingOrder);

//     if (!(departmentTabModel->select())) {
//         return false;
//     }

//     theDepartmentSelection = new QItemSelectionModel(departmentTabModel);
//     return true;
// }
// int IDatabase::addNewDepartment()
// {
//     departmentTabModel->insertRow(departmentTabModel->rowCount(), QModelIndex());

//     QModelIndex curIndex = departmentTabModel->index(departmentTabModel->rowCount()-1, 1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = departmentTabModel->record(curRecNo);
//     // 为新科室生成唯一ID
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

//     departmentTabModel->setRecord(curRecNo, curRec);
//     if (!departmentTabModel->submitAll()) {
//         qDebug() << "科室提交失败：" << departmentTabModel->lastError().text();
//     }
//     return curIndex.row();
// }
// bool IDatabase::searchDepartment(QString filter) {
//     departmentTabModel->setFilter(filter);
//     return departmentTabModel->select();
// }
// bool IDatabase::deleteCurrentDepartment() { return true; }
// bool IDatabase::submitDepartmentEdit() { return departmentTabModel->submitAll(); }
// void IDatabase::revertDepartmentEdit() { departmentTabModel->revertAll(); }
// bool IDatabase::initDoctorModel()
// {
//     doctorTabModel = new QSqlTableModel(this, database);
//     doctorTabModel->setTable("doctor");
//     doctorTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

//     doctorTabModel->setSort(doctorTabModel->fieldIndex("name"), Qt::AscendingOrder);

//     if (!(doctorTabModel->select())) {
//         return false;
//     }

//     theDoctorSelection = new QItemSelectionModel(doctorTabModel);

//     return true;
// }
// int IDatabase::addNewDoctor()
// {
//     doctorTabModel->insertRow(doctorTabModel->rowCount(), QModelIndex());

//     QModelIndex curIndex = doctorTabModel->index(doctorTabModel->rowCount()-1, 1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = doctorTabModel->record(curRecNo);
//     // 为新医生生成唯一ID
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

//     doctorTabModel->setRecord(curRecNo, curRec);
//     if (!doctorTabModel->submitAll()) {
//         qDebug() << "医生提交失败：" << doctorTabModel->lastError().text();
//     }
//     return curIndex.row();
// }
// bool IDatabase::searchDoctor(QString filter)
// {
//     doctorTabModel->setFilter(filter);
//     return doctorTabModel->select();
// }

// bool IDatabase::deleteCurrentDoctor()
// {
//     try {
//         QModelIndex curIndex = theDoctorSelection->currentIndex();
//         if (!curIndex.isValid()) {
//             return false;
//         }

//         bool isRemoved = doctorTabModel->removeRow(curIndex.row());
//         if (!isRemoved) {
//             return false;
//         }

//         bool isSubmitted = doctorTabModel->submitAll();
//         if (!isSubmitted) {
//             doctorTabModel->revertAll();
//             return false;
//         }

//         doctorTabModel->select();
//         return true;

//     } catch (...) {
//         return false;
//     }
// }

// bool IDatabase::submitDoctorEdit()
// {
//     return doctorTabModel->submitAll();
// }

// void IDatabase::revertDoctorEdit()
// {
//     doctorTabModel->revertAll();
// }

// bool IDatabase::initMedicineModel()
// {
//     medicineTabModel = new QSqlTableModel(this, database);
//     medicineTabModel->setTable("MEDICINE");
//     medicineTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     medicineTabModel->setSort(medicineTabModel->fieldIndex("NAME"), Qt::AscendingOrder);

//     if (!(medicineTabModel->select())) {
//         return false;
//     }

//     theMedicineSelection = new QItemSelectionModel(medicineTabModel);
//     return true;
// }

// int IDatabase::addNewMedicine()
// {
//     medicineTabModel->insertRow(medicineTabModel->rowCount(), QModelIndex());

//     QModelIndex curIndex = medicineTabModel->index(medicineTabModel->rowCount()-1, 1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = medicineTabModel->record(curRecNo);
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
//     curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

//     medicineTabModel->setRecord(curRecNo, curRec);
//     if (!medicineTabModel->submitAll()) {
//         qDebug() << "药品提交失败：" << medicineTabModel->lastError().text();
//     }
//     return curIndex.row();
// }

// bool IDatabase::searchMedicine(QString filter)
// {
//     medicineTabModel->setFilter(filter);
//     return medicineTabModel->select();
// }

// bool IDatabase::deleteCurrentMedicine()
// {
//     try {
//         QModelIndex curIndex = theMedicineSelection->currentIndex();
//         if (!curIndex.isValid()) {
//             return false;
//         }

//         bool isRemoved = medicineTabModel->removeRow(curIndex.row());
//         if (!isRemoved) {
//             return false;
//         }

//         bool isSubmitted = medicineTabModel->submitAll();
//         if (!isSubmitted) {
//             medicineTabModel->revertAll();
//             return false;
//         }

//         medicineTabModel->select();
//         return true;

//     } catch (...) {
//         return false;
//     }
// }

// bool IDatabase::submitMedicineEdit()
// {
//     return medicineTabModel->submitAll();
// }

// void IDatabase::revertMedicineEdit()
// {
//     medicineTabModel->revertAll();
// }
// bool IDatabase::initPrescriptionModel()
// {
//     prescriptionTabModel = new QSqlTableModel(this, database);
//     prescriptionTabModel->setTable("PRESCRIPTION");
//     prescriptionTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     prescriptionTabModel->setSort(prescriptionTabModel->fieldIndex("ID"), Qt::AscendingOrder);

//     // 设置字段显示名称
//     prescriptionTabModel->setHeaderData(1, Qt::Horizontal, "患者ID");
//     prescriptionTabModel->setHeaderData(2, Qt::Horizontal, "医生ID");
//     prescriptionTabModel->setHeaderData(3, Qt::Horizontal, "药品ID");
//     prescriptionTabModel->setHeaderData(4, Qt::Horizontal, "数量");
//     prescriptionTabModel->setHeaderData(5, Qt::Horizontal, "用法用量");
//     prescriptionTabModel->setHeaderData(6, Qt::Horizontal, "创建时间");

//     if (!(prescriptionTabModel->select())) {
//         return false;
//     }

//     thePrescriptionSelection = new QItemSelectionModel(prescriptionTabModel);
//     return true;
// }

// int IDatabase::addNewPrescription()
// {
//     prescriptionTabModel->insertRow(prescriptionTabModel->rowCount(), QModelIndex());

//     QModelIndex curIndex = prescriptionTabModel->index(prescriptionTabModel->rowCount()-1, 1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = prescriptionTabModel->record(curRecNo);
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
//     curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

//     prescriptionTabModel->setRecord(curRecNo, curRec);
//     if (!prescriptionTabModel->submitAll()) {
//         qDebug() << "处方提交失败：" << prescriptionTabModel->lastError().text();
//     }
//     return curIndex.row();
// }

// bool IDatabase::searchPrescription(QString filter)
// {
//     prescriptionTabModel->setFilter(filter);
//     return prescriptionTabModel->select();
// }

// bool IDatabase::deleteCurrentPrescription()
// {
//     try {
//         QModelIndex curIndex = thePrescriptionSelection->currentIndex();
//         if (!curIndex.isValid()) {
//             return false;
//         }

//         bool isRemoved = prescriptionTabModel->removeRow(curIndex.row());
//         if (!isRemoved) {
//             return false;
//         }

//         bool isSubmitted = prescriptionTabModel->submitAll();
//         if (!isSubmitted) {
//             prescriptionTabModel->revertAll();
//             return false;
//         }

//         prescriptionTabModel->select();
//         return true;

//     } catch (...) {
//         return false;
//     }
// }

// bool IDatabase::submitPrescriptionEdit()
// {
//     return prescriptionTabModel->submitAll();
// }

// void IDatabase::revertPrescriptionEdit()
// {
//     prescriptionTabModel->revertAll();
// }

// QStringList IDatabase::getAllPatientNames()
// {
//     QStringList names;
//     QSqlQuery query(database);
//     query.prepare("SELECT NAME FROM patient");
//     if (query.exec()) {
//         while (query.next()) {
//             names << query.value(0).toString();
//         }
//     }
//     return names;
// }

// QStringList IDatabase::getAllDoctorNames()
// {
//     QStringList names;
//     QSqlQuery query(database);
//     query.prepare("SELECT NAME FROM doctor");
//     if (query.exec()) {
//         while (query.next()) {
//             names << query.value(0).toString();
//         }
//     }
//     return names;
// }

// QStringList IDatabase::getAllMedicineNames()
// {
//     QStringList names;
//     QSqlQuery query(database);
//     query.prepare("SELECT NAME FROM MEDICINE");
//     if (query.exec()) {
//         while (query.next()) {
//             names << query.value(0).toString();
//         }
//     }
//     return names;
// }

// QString IDatabase::getPatientIdByName(QString name)
// {
//     QSqlQuery query(database);
//     query.prepare("SELECT ID FROM patient WHERE NAME = :NAME");
//     query.bindValue(":NAME", name);
//     if (query.exec() && query.first()) {
//         return query.value(0).toString();
//     }
//     return "";
// }

// QString IDatabase::getDoctorIdByName(QString name)
// {
//     QSqlQuery query(database);
//     query.prepare("SELECT ID FROM doctor WHERE NAME = :NAME");
//     query.bindValue(":NAME", name);
//     if (query.exec() && query.first()) {
//         return query.value(0).toString();
//     }
//     return "";
// }

// QString IDatabase::getMedicineIdByName(QString name)
// {
//     QSqlQuery query(database);
//     query.prepare("SELECT ID FROM MEDICINE WHERE NAME = :NAME");
//     query.bindValue(":NAME", name);
//     if (query.exec() && query.first()) {
//         return query.value(0).toString();
//     }
//     return "";
// }

// bool IDatabase::initAppointmentModel()
// {
//     // 首先检查表是否存在，如果不存在则创建
//     QSqlQuery query(database);
//     if (!database.tables().contains("APPOINTMENT")) {
//         QString createTableSql = R"(
//             CREATE TABLE APPOINTMENT (
//                 ID VARCHAR(36) PRIMARY KEY,
//                 PATIENT_ID VARCHAR(36) NOT NULL,
//                 DOCTOR_ID VARCHAR(36) NOT NULL,
//                 APPOINTMENT_TIME DATETIME NOT NULL,
//                 STATUS VARCHAR(20) DEFAULT '待审核',
//                 AUDIT_COMMENT VARCHAR(200),
//                 AUDIT_TIME DATETIME,
//                 CREATEDTIMESTAMP DATETIME,
//                 SYNC_FLAG INTEGER DEFAULT 0
//             )
//         )";
//         if (!query.exec(createTableSql)) {
//             qDebug() << "创建预约表失败：" << query.lastError().text();
//             return false;
//         }
//         qDebug() << "预约表创建成功";
//     }

//     appointmentTabModel = new QSqlTableModel(this, database);
//     appointmentTabModel->setTable("APPOINTMENT");
//     appointmentTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     appointmentTabModel->setSort(appointmentTabModel->fieldIndex("APPOINTMENT_TIME"), Qt::AscendingOrder);

//     // 设置字段显示名称
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("ID"), Qt::Horizontal, "预约ID");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("PATIENT_ID"), Qt::Horizontal, "患者ID");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("DOCTOR_ID"), Qt::Horizontal, "医生ID");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("APPOINTMENT_TIME"), Qt::Horizontal, "预约时间");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("STATUS"), Qt::Horizontal, "状态");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("AUDIT_COMMENT"), Qt::Horizontal, "审核意见");
//     appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("CREATEDTIMESTAMP"), Qt::Horizontal, "创建时间");

//     if (!(appointmentTabModel->select())) {
//         return false;
//     }

//     theAppointmentSelection = new QItemSelectionModel(appointmentTabModel);
//     return true;
// }

// int IDatabase::addNewAppointment()
// {
//     appointmentTabModel->insertRow(appointmentTabModel->rowCount(), QModelIndex());

//     QModelIndex curIndex = appointmentTabModel->index(appointmentTabModel->rowCount()-1, 1);
//     int curRecNo = curIndex.row();
//     QSqlRecord curRec = appointmentTabModel->record(curRecNo);
//     curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
//     curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
//     curRec.setValue("STATUS", "待审核");

//     appointmentTabModel->setRecord(curRecNo, curRec);
//     if (!appointmentTabModel->submitAll()) {
//         qDebug() << "预约提交失败：" << appointmentTabModel->lastError().text();
//     }
//     return curIndex.row();
// }

// bool IDatabase::searchAppointment(QString filter)
// {
//     appointmentTabModel->setFilter(filter);
//     return appointmentTabModel->select();
// }

// bool IDatabase::deleteCurrentAppointment()
// {
//     try {
//         QModelIndex curIndex = theAppointmentSelection->currentIndex();
//         if (!curIndex.isValid()) {
//             return false;
//         }

//         bool isRemoved = appointmentTabModel->removeRow(curIndex.row());
//         if (!isRemoved) {
//             return false;
//         }

//         bool isSubmitted = appointmentTabModel->submitAll();
//         if (!isSubmitted) {
//             appointmentTabModel->revertAll();
//             return false;
//         }

//         appointmentTabModel->select();
//         return true;

//     } catch (...) {
//         return false;
//     }
// }

// bool IDatabase::submitAppointmentEdit()
// {
//     return appointmentTabModel->submitAll();
// }

// void IDatabase::revertAppointmentEdit()
// {
//     appointmentTabModel->revertAll();
// }

// bool IDatabase::checkTimeConflict(QString doctorId, QString appointmentTime, QString excludeId)
// {
//     QSqlQuery query(database);
//     QString sql = "SELECT ID FROM APPOINTMENT WHERE DOCTOR_ID = :DOCTOR_ID "
//                   "AND STATUS != '已拒绝' "
//                   "AND APPOINTMENT_TIME = :APPOINTMENT_TIME";

//     if (!excludeId.isEmpty()) {
//         sql += " AND ID != :EXCLUDE_ID";
//     }

//     query.prepare(sql);
//     query.bindValue(":DOCTOR_ID", doctorId);
//     query.bindValue(":APPOINTMENT_TIME", appointmentTime);

//     if (!excludeId.isEmpty()) {
//         query.bindValue(":EXCLUDE_ID", excludeId);
//     }

//     if (query.exec() && query.next()) {
//         // 找到冲突的预约
//         return true;
//     }
//     return false;
// }

// bool IDatabase::approveAppointment(int rowNo)
// {
//     if (rowNo < 0 || rowNo >= appointmentTabModel->rowCount()) {
//         return false;
//     }

//     QSqlRecord record = appointmentTabModel->record(rowNo);
//     record.setValue("STATUS", "已通过");
//     record.setValue("AUDIT_TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
//     appointmentTabModel->setRecord(rowNo, record);

//     return appointmentTabModel->submitAll();
// }

// bool IDatabase::rejectAppointment(int rowNo)
// {
//     if (rowNo < 0 || rowNo >= appointmentTabModel->rowCount()) {
//         return false;
//     }

//     QSqlRecord record = appointmentTabModel->record(rowNo);
//     record.setValue("STATUS", "已拒绝");
//     record.setValue("AUDIT_TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
//     appointmentTabModel->setRecord(rowNo, record);

//     return appointmentTabModel->submitAll();
// }
#include "idatabase.h"

#include<QUuid>
#include<QCoreApplication>
#include<QDir>
void IDatabase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "E:/QtCode/lab3/lab3.db";
    database.setDatabaseName(aFile);


    if (!database.open()) {
        qDebug() << "failed to open database";
    } else {
        qDebug() << "open database is ok";
    }
}

bool IDatabase::initPatienntModel()
{
    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    patientTabModel->setSort(patientTabModel->fieldIndex("name"), Qt::AscendingOrder);

    if (!(patientTabModel->select())) {
        return false;
    }

    thePatientSelection = new QItemSelectionModel(patientTabModel);

    return true;
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());

    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo, curRec);
    if (!patientTabModel->submitAll()) {
        // 输出错误信息到控制台（调试用）
        // qDebug() << "提交失败：" << patientTabModel->lastError().text();
    } else {
        // qDebug() << "提交成功";
    }
    return curIndex.row();
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    // QModelIndex curIndex = thePatientSelection->currentIndex();
    // patientTabModel->removeRow(curIndex.row());
    // patientTabModel->submitAll();
    // patientTabModel->select();
    // try {
    //     QModelIndex curIndex = thePatientSelection->currentIndex();
    //     // 先判断索引是否有效，避免空指针/无效索引导致崩溃
    //     if (!curIndex.isValid()) {
    //         return false; // 索引无效，删除失败
    //     }
    //     // 执行删除行操作
    //     bool isRemoved = patientTabModel->removeRow(curIndex.row());
    //     if (isRemoved) {
    //         patientTabModel->select(); // 刷新数据
    //         return true; // 删除成功
    //     } else {
    //         return false; // 删除失败
    //     }
    // } catch (...) {
    //     // 捕获异常，避免程序崩溃
    //     return false;
    // }
    try {
        QModelIndex curIndex = thePatientSelection->currentIndex();
        if (!curIndex.isValid()) {
            return false;
        }

        // 1. 移除模型中的行（内存中标记删除）
        bool isRemoved = patientTabModel->removeRow(curIndex.row());
        if (!isRemoved) {
            return false;
        }

        // 2. 提交操作到数据库（关键步骤：将删除写入数据库）
        bool isSubmitted = patientTabModel->submitAll();
        if (!isSubmitted) {
            // 提交失败时回滚，恢复模型中的行
            patientTabModel->revertAll();
            return false;
        }

        // 3. 刷新模型数据，同步数据库最新状态
        patientTabModel->select();
        return true;

    } catch (...) {
        return false;
    }
}

bool IDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

QString IDatabase::userLogin(QString userName, QString password)
{
    //return "loginOK";
    QSqlQuery query;
    query.prepare("select username, password from user where username = :USER");
    query.bindValue(":USER", userName);

    query.exec();

    if (query.first() && query.value("username").isValid()) {
        QString passwd = query.value("password").toString();
        if (passwd == password) {
            return "loginOk";
        } else {
            qDebug() << "wrong password";
            return "wrongPassword";
        }
    } else {
        qDebug() << "no such user: " << userName;
        return "wrongUsername";
    }
}


IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}

bool IDatabase::initDepartmentModel()
{
    departmentTabModel = new QSqlTableModel(this, database);
    departmentTabModel->setTable("department");
    departmentTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    departmentTabModel->setSort(departmentTabModel->fieldIndex("name"), Qt::AscendingOrder);

    if (!(departmentTabModel->select())) {
        return false;
    }

    theDepartmentSelection = new QItemSelectionModel(departmentTabModel);
    return true;
}
int IDatabase::addNewDepartment()
{
    departmentTabModel->insertRow(departmentTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = departmentTabModel->index(departmentTabModel->rowCount()-1, 1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = departmentTabModel->record(curRecNo);
    // 为新科室生成唯一ID
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    departmentTabModel->setRecord(curRecNo, curRec);
    if (!departmentTabModel->submitAll()) {
        qDebug() << "科室提交失败：" << departmentTabModel->lastError().text();
    }
    return curIndex.row();
}
bool IDatabase::searchDepartment(QString filter) {
    departmentTabModel->setFilter(filter);
    return departmentTabModel->select();
}
bool IDatabase::deleteCurrentDepartment() { return true; }
bool IDatabase::submitDepartmentEdit() { return departmentTabModel->submitAll(); }
void IDatabase::revertDepartmentEdit() { departmentTabModel->revertAll(); }
bool IDatabase::initDoctorModel()
{
    doctorTabModel = new QSqlTableModel(this, database);
    doctorTabModel->setTable("doctor");
    doctorTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    doctorTabModel->setSort(doctorTabModel->fieldIndex("name"), Qt::AscendingOrder);

    if (!(doctorTabModel->select())) {
        return false;
    }

    theDoctorSelection = new QItemSelectionModel(doctorTabModel);

    return true;
}
int IDatabase::addNewDoctor()
{
    doctorTabModel->insertRow(doctorTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = doctorTabModel->index(doctorTabModel->rowCount()-1, 1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = doctorTabModel->record(curRecNo);
    // 为新医生生成唯一ID
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    doctorTabModel->setRecord(curRecNo, curRec);
    if (!doctorTabModel->submitAll()) {
        qDebug() << "医生提交失败：" << doctorTabModel->lastError().text();
    }
    return curIndex.row();
}

bool IDatabase::searchDoctor(QString filter)
{
    doctorTabModel->setFilter(filter);
    return doctorTabModel->select();
}

bool IDatabase::deleteCurrentDoctor()
{
    try {
        QModelIndex curIndex = theDoctorSelection->currentIndex();
        if (!curIndex.isValid()) {
            return false;
        }

        bool isRemoved = doctorTabModel->removeRow(curIndex.row());
        if (!isRemoved) {
            return false;
        }

        bool isSubmitted = doctorTabModel->submitAll();
        if (!isSubmitted) {
            doctorTabModel->revertAll();
            return false;
        }

        doctorTabModel->select();
        return true;

    } catch (...) {
        return false;
    }
}

bool IDatabase::submitDoctorEdit()
{
    return doctorTabModel->submitAll();
}

void IDatabase::revertDoctorEdit()
{
    doctorTabModel->revertAll();
}

bool IDatabase::initMedicineModel()
{
    medicineTabModel = new QSqlTableModel(this, database);
    medicineTabModel->setTable("MEDICINE");
    medicineTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    medicineTabModel->setSort(medicineTabModel->fieldIndex("NAME"), Qt::AscendingOrder);

    if (!(medicineTabModel->select())) {
        return false;
    }

    theMedicineSelection = new QItemSelectionModel(medicineTabModel);
    return true;
}

int IDatabase::addNewMedicine()
{
    medicineTabModel->insertRow(medicineTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = medicineTabModel->index(medicineTabModel->rowCount()-1, 1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = medicineTabModel->record(curRecNo);
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    medicineTabModel->setRecord(curRecNo, curRec);
    if (!medicineTabModel->submitAll()) {
        qDebug() << "药品提交失败：" << medicineTabModel->lastError().text();
    }
    return curIndex.row();
}

bool IDatabase::searchMedicine(QString filter)
{
    medicineTabModel->setFilter(filter);
    return medicineTabModel->select();
}

bool IDatabase::deleteCurrentMedicine()
{
    try {
        QModelIndex curIndex = theMedicineSelection->currentIndex();
        if (!curIndex.isValid()) {
            return false;
        }

        bool isRemoved = medicineTabModel->removeRow(curIndex.row());
        if (!isRemoved) {
            return false;
        }

        bool isSubmitted = medicineTabModel->submitAll();
        if (!isSubmitted) {
            medicineTabModel->revertAll();
            return false;
        }

        medicineTabModel->select();
        return true;

    } catch (...) {
        return false;
    }
}

bool IDatabase::submitMedicineEdit()
{
    return medicineTabModel->submitAll();
}

void IDatabase::revertMedicineEdit()
{
    medicineTabModel->revertAll();
}
bool IDatabase::initPrescriptionModel()
{
    prescriptionTabModel = new QSqlTableModel(this, database);
    prescriptionTabModel->setTable("PRESCRIPTION");
    prescriptionTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prescriptionTabModel->setSort(prescriptionTabModel->fieldIndex("ID"), Qt::AscendingOrder);

    // 设置字段显示名称
    prescriptionTabModel->setHeaderData(1, Qt::Horizontal, "患者ID");
    prescriptionTabModel->setHeaderData(2, Qt::Horizontal, "医生ID");
    prescriptionTabModel->setHeaderData(3, Qt::Horizontal, "药品ID");
    prescriptionTabModel->setHeaderData(4, Qt::Horizontal, "数量");
    prescriptionTabModel->setHeaderData(5, Qt::Horizontal, "用法用量");
    prescriptionTabModel->setHeaderData(6, Qt::Horizontal, "创建时间");

    if (!(prescriptionTabModel->select())) {
        return false;
    }

    thePrescriptionSelection = new QItemSelectionModel(prescriptionTabModel);
    return true;
}

int IDatabase::addNewPrescription()
{
    prescriptionTabModel->insertRow(prescriptionTabModel->rowCount(), QModelIndex());

    QModelIndex curIndex = prescriptionTabModel->index(prescriptionTabModel->rowCount()-1, 1);
    int curRecNo = curIndex.row();
    QSqlRecord curRec = prescriptionTabModel->record(curRecNo);
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    prescriptionTabModel->setRecord(curRecNo, curRec);
    if (!prescriptionTabModel->submitAll()) {
        qDebug() << "处方提交失败：" << prescriptionTabModel->lastError().text();
    }
    return curIndex.row();
}

bool IDatabase::searchPrescription(QString filter)
{
    prescriptionTabModel->setFilter(filter);
    return prescriptionTabModel->select();
}

bool IDatabase::deleteCurrentPrescription()
{
    try {
        QModelIndex curIndex = thePrescriptionSelection->currentIndex();
        if (!curIndex.isValid()) {
            return false;
        }

        bool isRemoved = prescriptionTabModel->removeRow(curIndex.row());
        if (!isRemoved) {
            return false;
        }

        bool isSubmitted = prescriptionTabModel->submitAll();
        if (!isSubmitted) {
            prescriptionTabModel->revertAll();
            return false;
        }

        prescriptionTabModel->select();
        return true;

    } catch (...) {
        return false;
    }
}

bool IDatabase::submitPrescriptionEdit()
{
    return prescriptionTabModel->submitAll();
}

void IDatabase::revertPrescriptionEdit()
{
    prescriptionTabModel->revertAll();
}

QStringList IDatabase::getAllPatientNames()
{
    QStringList names;
    QSqlQuery query(database);
    query.prepare("SELECT NAME FROM patient");
    if (query.exec()) {
        while (query.next()) {
            names << query.value(0).toString();
        }
    }
    return names;
}

QStringList IDatabase::getAllDoctorNames()
{
    QStringList names;
    QSqlQuery query(database);
    query.prepare("SELECT NAME FROM doctor");
    if (query.exec()) {
        while (query.next()) {
            names << query.value(0).toString();
        }
    }
    return names;
}

QStringList IDatabase::getAllMedicineNames()
{
    QStringList names;
    QSqlQuery query(database);
    query.prepare("SELECT NAME FROM MEDICINE");
    if (query.exec()) {
        while (query.next()) {
            names << query.value(0).toString();
        }
    }
    return names;
}

QString IDatabase::getPatientIdByName(QString name)
{
    QSqlQuery query(database);
    query.prepare("SELECT ID FROM patient WHERE NAME = :NAME");
    query.bindValue(":NAME", name);
    if (query.exec() && query.first()) {
        return query.value(0).toString();
    }
    return "";
}

QString IDatabase::getDoctorIdByName(QString name)
{
    QSqlQuery query(database);
    query.prepare("SELECT ID FROM doctor WHERE NAME = :NAME");
    query.bindValue(":NAME", name);
    if (query.exec() && query.first()) {
        return query.value(0).toString();
    }
    return "";
}

QString IDatabase::getMedicineIdByName(QString name)
{
    QSqlQuery query(database);
    query.prepare("SELECT ID FROM MEDICINE WHERE NAME = :NAME");
    query.bindValue(":NAME", name);
    if (query.exec() && query.first()) {
        return query.value(0).toString();
    }
    return "";
}

QMap<QString, QString> IDatabase::getPatientNameIdMap()
{
    QMap<QString, QString> map;
    QSqlQuery query(database);
    query.prepare("SELECT ID, NAME FROM patient");
    if (query.exec()) {
        while (query.next()) {
            map[query.value(1).toString()] = query.value(0).toString();
        }
    }
    return map;
}

QMap<QString, QString> IDatabase::getDoctorNameIdMap()
{
    QMap<QString, QString> map;
    QSqlQuery query(database);
    query.prepare("SELECT ID, NAME FROM doctor");
    if (query.exec()) {
        while (query.next()) {
            map[query.value(1).toString()] = query.value(0).toString();
        }
    }
    return map;
}

QMap<QString, QString> IDatabase::getMedicineNameIdMap()
{
    QMap<QString, QString> map;
    QSqlQuery query(database);
    query.prepare("SELECT ID, NAME FROM MEDICINE");
    if (query.exec()) {
        while (query.next()) {
            map[query.value(1).toString()] = query.value(0).toString();
        }
    }
    return map;
}

bool IDatabase::initAppointmentModel()
{
    // 首先检查表是否存在，如果不存在则创建
    QSqlQuery query(database);
    if (!database.tables().contains("APPOINTMENT")) {
        QString createTableSql = R"(
            CREATE TABLE APPOINTMENT (
                ID VARCHAR(36) PRIMARY KEY,
                PATIENT_ID VARCHAR(36) NOT NULL,
                DOCTOR_ID VARCHAR(36) NOT NULL,
                APPOINTMENT_TIME DATETIME NOT NULL,
                STATUS VARCHAR(20) DEFAULT '待审核',
                AUDIT_COMMENT VARCHAR(200),
                AUDIT_TIME DATETIME,
                CREATEDTIMESTAMP DATETIME,
                SYNC_FLAG INTEGER DEFAULT 0
            )
        )";
        if (!query.exec(createTableSql)) {
            qDebug() << "创建预约表失败：" << query.lastError().text();
            return false;
        }
        qDebug() << "预约表创建成功";
    }

    appointmentTabModel = new QSqlTableModel(this, database);
    appointmentTabModel->setTable("APPOINTMENT");
    appointmentTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    appointmentTabModel->setSort(appointmentTabModel->fieldIndex("APPOINTMENT_TIME"), Qt::AscendingOrder);

    // 设置字段显示名称
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("ID"), Qt::Horizontal, "预约ID");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("PATIENT_ID"), Qt::Horizontal, "患者ID");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("DOCTOR_ID"), Qt::Horizontal, "医生ID");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("APPOINTMENT_TIME"), Qt::Horizontal, "预约时间");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("STATUS"), Qt::Horizontal, "状态");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("AUDIT_COMMENT"), Qt::Horizontal, "审核意见");
    appointmentTabModel->setHeaderData(appointmentTabModel->fieldIndex("CREATEDTIMESTAMP"), Qt::Horizontal, "创建时间");

    if (!(appointmentTabModel->select())) {
        return false;
    }

    theAppointmentSelection = new QItemSelectionModel(appointmentTabModel);
    return true;
}

int IDatabase::addNewAppointment()
{
    appointmentTabModel->insertRow(appointmentTabModel->rowCount(), QModelIndex());
    int newRow = appointmentTabModel->rowCount() - 1;
    QSqlRecord newRec = appointmentTabModel->record(newRow);

    newRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));
    newRec.setValue("STATUS", "待审核");
    newRec.setValue("SYNC_FLAG", 0);
    // 新增：设置创建时间戳
    newRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    appointmentTabModel->setRecord(newRow, newRec);

    // 立即提交到数据库
    if (!appointmentTabModel->submitAll()) {
        qDebug() << "新增预约初始提交失败：" << appointmentTabModel->lastError().text();
    }

    return newRow;
}
bool IDatabase::searchAppointment(QString filter)
{
    appointmentTabModel->setFilter(filter);
    return appointmentTabModel->select();
}

bool IDatabase::deleteCurrentAppointment()
{
    try {
        QModelIndex curIndex = theAppointmentSelection->currentIndex();
        if (!curIndex.isValid()) {
            return false;
        }

        bool isRemoved = appointmentTabModel->removeRow(curIndex.row());
        if (!isRemoved) {
            return false;
        }

        bool isSubmitted = appointmentTabModel->submitAll();
        if (!isSubmitted) {
            appointmentTabModel->revertAll();
            return false;
        }

        appointmentTabModel->select();
        return true;

    } catch (...) {
        return false;
    }
}

bool IDatabase::submitAppointmentEdit()
{
    return appointmentTabModel->submitAll();
}

void IDatabase::revertAppointmentEdit()
{
    appointmentTabModel->revertAll();
}

bool IDatabase::checkTimeConflict(QString doctorId, QString appointmentTime, QString excludeId)
{
    QSqlQuery query(database);
    QString sql = "SELECT ID FROM APPOINTMENT WHERE DOCTOR_ID = :DOCTOR_ID "
                  "AND STATUS != '已拒绝' "
                  "AND APPOINTMENT_TIME = :APPOINTMENT_TIME";

    if (!excludeId.isEmpty()) {
        sql += " AND ID != :EXCLUDE_ID";
    }

    query.prepare(sql);
    query.bindValue(":DOCTOR_ID", doctorId);
    query.bindValue(":APPOINTMENT_TIME", appointmentTime);

    if (!excludeId.isEmpty()) {
        query.bindValue(":EXCLUDE_ID", excludeId);
    }

    if (query.exec() && query.next()) {
        // 找到冲突的预约
        return true;
    }
    return false;
}

bool IDatabase::approveAppointment(int rowNo)
{
    if (rowNo < 0 || rowNo >= appointmentTabModel->rowCount()) {
        return false;
    }

    QSqlRecord record = appointmentTabModel->record(rowNo);
    record.setValue("STATUS", "已通过");
    record.setValue("AUDIT_TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    appointmentTabModel->setRecord(rowNo, record);

    return appointmentTabModel->submitAll();
}

bool IDatabase::rejectAppointment(int rowNo)
{
    if (rowNo < 0 || rowNo >= appointmentTabModel->rowCount()) {
        return false;
    }

    QSqlRecord record = appointmentTabModel->record(rowNo);
    record.setValue("STATUS", "已拒绝");
    record.setValue("AUDIT_TIME", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    appointmentTabModel->setRecord(rowNo, record);

    return appointmentTabModel->submitAll();
}
