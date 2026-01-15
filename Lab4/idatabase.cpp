#include "idatabase.h"
#include<QUuid>
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
    return departmentTabModel->rowCount() - 1;
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
