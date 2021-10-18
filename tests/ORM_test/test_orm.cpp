#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include <QList>
#include <QFile>
#include <QSqlDriver>
#include "Global.h"
#include "Tools.cpp"
#include "Logger.cpp"
#include "ExceptionSql.cpp"
#include "Exception.cpp"
#include "Database.cpp"
#include "SqlQuery.cpp"
#include "OrmObject.h"

#define CLEAR_LIST \
    for(int i = 0; i < list.size(); i++) \
        delete list.takeAt(i);

class MyModel : public COrmObject<MyModel>
{
    Q_OBJECT
    ORM_PROPERTY(bool, nameBool)
    ORM_PROPERTY(int, nameInt)
    ORM_PROPERTY(uint, nameUint)
    ORM_PROPERTY(qlonglong, nameLonglong)
    ORM_PROPERTY(qulonglong, nameUlonglong)
    ORM_PROPERTY(double, namedouble)
    ORM_PROPERTY(QByteArray, nameBlob)
    ORM_PROPERTY(QChar, nameChar)
    ORM_PROPERTY(QDate, nameDate)
    ORM_PROPERTY(QTime, nameTime)
    ORM_PROPERTY(QDateTime, nameDatetime)
    ORM_PROPERTY(QString, nameString)
    ORM_PROPERTY(Price, namePrice)
    ORM_PROPERTY(Amount, nameAmount)
    ORM_PROPERTY(Value, nameValue)
    ORM_PROPERTY(QString, nameVarchar)

public:
    MyModel() {
                initORMObject();
                ADD_SQL_DESC("nameVarchar", "VARCHAR(50) NOT NULL DEFAULT ''")
                setnameChar('A');
                clearUpdateList();
              }
};

class Car : public COrmObject<Car>
{
    Q_OBJECT
    ORM_PROPERTY(QString, Number)

public:
    Car() { initORMObject(); }
};

class DriverLicense : public COrmObject<DriverLicense>
{
    Q_OBJECT
    ORM_PROPERTY(int, Number)

public:
    DriverLicense() { initORMObject(); }
};

class CarDriver : public COrmObject<CarDriver>
{
    Q_OBJECT
    //ORM_HAS_ONE(DriverLicense)
    //ORM_HAS_MANY(Car)
    ORM_PROPERTY(QString, Name)

public:
    CarDriver() {  initORMObject(); }
};

class Test_ORMObject : public QObject
{
    Q_OBJECT
    
public:
    Test_ORMObject();
    ~Test_ORMObject();
    
private:    


private Q_SLOTS:
    void test_createTable();
    void test_save();
    void test_find();
    void test_findAll();
    void test_first();
    void test_last();
    void test_findByValue();
    void test_findByValues();
    void test_findByParams();   
    void test_update();
    void test_updateProperty();
    void test_exists();
    void test_existsById();    
    void test_remove();
    void test_removeBy();
    void test_removeAll();
    void test_count();
    void test_countByFieldName();   
    void test_toString();
    void test_hasUnsavedChanges();
    void test_dropTable();
};

Test_ORMObject::Test_ORMObject()
{
    DB.getInstance();

    DB.setEchoQuery(true);
    DB.setLogQuery(true);
    DB.setEchoError(true);
    DB.setLogError(true);

    DB.setQueryLog();
    //Rejestracja typów

    qRegisterMetaType<Value>("Value");
    qRegisterMetaType<Price>("Price");
    qRegisterMetaType<Amount>("Amount");
    qRegisterMetaType<Indicator>("Indicator");
    qRegisterMetaType<Varchar255>("Varchar255");
    qRegisterMetaType<Timestamp>("Timestamp");

    DEBUG_WITH_LINE << DB.getDb().driverName();
}

Test_ORMObject::~Test_ORMObject()
{   
    QFile file(DB.getBaseNameFullPath());
    file.remove();    
}

void Test_ORMObject::test_createTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;

    QCOMPARE(model.createTable(), true);
//    QCOMPARE(model.createTable(), false);
    QCOMPARE(driver.createTable(), true);
    QCOMPARE(license.createTable(), true);
    QCOMPARE(car.createTable(), true);
}

void Test_ORMObject::test_save()
{
    MyModel model;
    QTime time = QTime::currentTime();

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameBool(true);
    model.setnameBlob(QByteArray(10000, '1'));
    model.setnameChar('A');
    model.setnameDate(QDate(2013, 03, 26));
    model.setnameDatetime(QDateTime(QDate(2013, 03, 26), time));
    model.setnamedouble(0.1);
    model.setnameInt(10);
    model.setnameLonglong(1234567890);
    model.setnameString("Hello world!");
    model.setnameTime(time);
    model.setnameUint(60000);
    model.setnameUlonglong(123456789123456789);
    QCOMPARE(model.save(), true);
    QSqlQuery query = DB.getDb().exec(QString("SELECT * FROM %1;").arg(model.getTableName()));
    if (query.lastError().isValid())
        {
            DEBUG_WITH_LINE << "Error: " << QString("SELECT * FROM %1;").arg(model.getTableName()) <<" : " << query.lastError().databaseText();
        }
    DEBUG_WITH_LINE <<  query.isActive() <<  "" << query.isSelect();
    QCOMPARE(query.next(), true);
    DEBUG_WITH_LINE << "Size: " << query.size() << " valid: "<< query.isValid();

    for(int i = 0; i < query.record().count(); i++)
    {
        DEBUG_WITH_LINE << "Field: " << query.record().fieldName(i) << "Value: " << query.value(i);
//        DEBUG_WITH_LINE << query.record().fieldName(i);
        DEBUG_WITH_LINE << model.property(query.record().fieldName(i).toLocal8Bit().constData());
//        DEBUG_WITH_LINE << query.value(i);
        if (query.record().fieldName(i) == QString("id_%1").arg(model.getTableName()))
            QCOMPARE(query.value(i).toInt(), model.getId());
        else if (query.record().fieldName(i) == QString("deleted"))
            QCOMPARE(query.value(i), model.getdeleted());
        else if (query.record().fieldName(i) == QString("timestp"))
            QCOMPARE(query.value(i), model.gettimestp());
        else
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
    }
}

void Test_ORMObject::test_update()
{
    MyModel model, model2;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model2.setnameInt(10);
    QCOMPARE(model2.update(), false);
    QCOMPARE(model.findAll().isEmpty(), true);
    model2.save();
    QCOMPARE(model.findAll().isEmpty(), false);
    model.setId(model2.getId());
    model.setnameInt(15);
    QCOMPARE(model.update(), true);
    QCOMPARE(model2.findAll().isEmpty(), false);
    QCOMPARE(model2.first()->getnameInt(), 15);
}

void Test_ORMObject::test_find()
{
    MyModel model;
    MyModel *pointer;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.getId(), -1);    
    QCOMPARE(model.save(), true);
    int id = model.getId();
    QVERIFY(id >= 0);
    QCOMPARE((pointer = model.find(id))->getId(), id);
    delete pointer;
    QVERIFY(model.find(13423) == 0);
    QCOMPARE(model.getId(), id);
    QSqlQuery query = DB.getDb().exec(QString("SELECT * FROM %1 WHERE id_%1 = %2;").arg(model.getTableName()).arg(id));
    query.next();
    for(int i = 0; i < query.record().count(); i++)
    {
        DEBUG_WITH_LINE << "Field: " << query.record().fieldName(i) << "Value: " << query.value(i);
        //        DEBUG_WITH_LINE << query.record().fieldName(i);
        DEBUG_WITH_LINE << model.property(query.record().fieldName(i).toLocal8Bit().constData());
        //        DEBUG_WITH_LINE << query.value(i);
        if (query.record().fieldName(i) == QString("id_%1").arg(model.getTableName()))
            QCOMPARE(query.value(i).toInt(), model.getId());
        else if (query.record().fieldName(i) == QString("deleted"))
            QCOMPARE(query.value(i), model.getdeleted());
        else if (query.record().fieldName(i) == QString("timestp"))
            QCOMPARE(query.value(i), model.gettimestp());
        else
            QCOMPARE(query.value(i), model.property(query.record().fieldName(i).toLocal8Bit().constData()));
    }
}

void Test_ORMObject::test_findAll()
{
    MyModel model, model1, model2, model3, resultModel;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(resultModel.findAll().isEmpty(), true);
    model.setnameInt(1);
    model.setnameString("a");
    model1.setnameInt(2);
    model1.setnameString("b");
    model2.setnameInt(3);
    model2.setnameString("a");
    model3.setnameInt(4);
    model3.setnameString("c");
    model.save();
    model1.save();
    model2.save();
    model3.save();
    QList<MyModel*> list;
    list = resultModel.findAll();
    QCOMPARE(list.size(), 4);
    CLEAR_LIST
    model1.updateProperty("nameString", "a");
    model2.updateProperty("nameInt", "0");
    list = resultModel.findAll(QString("nameInt"));
    QCOMPARE(list.first()->getnameInt(), 0);
    CLEAR_LIST
    list = resultModel.findAll(QString("nameString"));
    QCOMPARE(list.size(), 4);
    QCOMPARE(list.first()->getnameString(), "a");
    CLEAR_LIST
}

void Test_ORMObject::test_findByValue()
{
    MyModel model;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameInt(15);
    model.setnameString("a");
    model.save();
    QList<MyModel*> list;
    list = model.findBy("nameInt", QVariant(15));
    QCOMPARE(list.first()->getnameInt(), 15);
    MyModel model2;
    list = model2.findBy("nameString", QVariant("sdjkfhsjk"));
    QCOMPARE(model2.getId(), -1);
    QCOMPARE(list.size(), 0);
    CLEAR_LIST
    list = model2.findBy("nameInt", 15);
    QCOMPARE(list.size(), 1);
    CLEAR_LIST
    MyModel model3;
    model3.setnameInt(15);
    model3.setnameString("b");
    model3.save();
    list = model2.findBy("nameInt", 15);
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
    list = model3.findBy("nameInt", 15, QString("id_%1").arg(model3.getTableName()));
    QCOMPARE(list.first()->getnameString(), QString("a"));
    CLEAR_LIST
}

void Test_ORMObject::test_findByValues()
{
    MyModel model1, model2, model3, resultModel;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model1.getTableName()));
    model1.setnameInt(10);
    model2.setnameInt(11);
    model3.setnameInt(12);
    model1.save();
    model2.save();
    model3.save();
    QVector<QVariant> vector;
    vector.append(10);
    vector.append(11);
    QList<MyModel*> list;
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getId(), model1.getId());
    vector.clear();
    CLEAR_LIST
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    vector.append(20);
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    CLEAR_LIST
    vector.clear();
    vector.append(10);
    vector.append(11);
    model3.updateProperty("nameInt", 10);
    list = resultModel.findBy("nameInt", vector);
    QCOMPARE(list.size(), 3);
    CLEAR_LIST
    list = resultModel.findBy("nameInt", vector, QString("nameInt"));
    QCOMPARE(list.size(), 3);
    QCOMPARE(list.first()->getnameInt(), 10);
    CLEAR_LIST
}

void Test_ORMObject::test_findByParams()
{
    MyModel model, model2, model3, resultModel;

    DB.dropTable(model.getTableName());
    model.createTable();
    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameString("Hello");
    model2.setnameInt(3);
    model3.setnamedouble(1.23);
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.save(), true);
    QHash<QString, QVariant> findHash;
    findHash.insert("nameString", QVariant("Hello"));
    findHash.insert("nameInt", QVariant(3));
    findHash.insert("nameDouble", QVariant(1.23));
    findHash.insert("nameChar", QVariant('X'));
    QList<MyModel*> list;
    list = resultModel.findByOr(findHash);
    QCOMPARE(list.size(), 3);
    for(int i = 0; i < list.size(); i++)
    {
        QVERIFY(model.getId() == list.value(i)->getId() || model2.getId() == list.value(i)->getId()
                || model3.getId() == list.value(i)->getId());
        if(list.value(i)->getId() == 1)
            QCOMPARE(list.value(i)->getnameString(), QString("Hello"));
        if(list.value(i)->getId() == 2)
        {
            QCOMPARE(list.value(i)->getnameInt(), 3);
            QCOMPARE(list.value(i)->getnameString().isEmpty(), true);
        }
        if(list.value(i)->getId() == 3)
            QCOMPARE(list.value(i)->getnamedouble(), 1.23);
    }
    findHash.clear();
    findHash.insert(QString("id_%1").arg(model.getTableName()), QVariant(-100));
    CLEAR_LIST
    list = resultModel.findByAnd(findHash);
    QCOMPARE(list.isEmpty(), true);
}

void Test_ORMObject::test_updateProperty()
{
    MyModel model, model2;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameInt(10);
    model.save();
    QCOMPARE(model.updateProperty("nameString", "Hello"), true);
    QCOMPARE(model.updateProperty("ololoProperty", 123), false);
    QCOMPARE(model2.updateProperty("nameInt", 20), false);
    QCOMPARE(model2.first()->getnameString(), QString("Hello"));
}

void Test_ORMObject::test_exists()
{
    MyModel model;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.existsTable(), false);
    model.save();
    QCOMPARE(model.existsTable(), true);
    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.existsTable(), false);
}

void Test_ORMObject::test_existsById()
{
    MyModel model;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.exists(1), false);
    QCOMPARE(model.exists(model.getId()), false);
    model.save();
    QCOMPARE(model.exists(model.getId()), true);
    int id = model.getId();
    model.remove();
    QCOMPARE(model.exists(id), false);
}

void Test_ORMObject::test_remove()
{
    MyModel model, model2, model3, resultModel;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.save();
    model2.save();
    model3.save();
    int id = model.getId();
    int id2 = model2.getId();
    QList<MyModel*> list = resultModel.findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 3);
    QCOMPARE(resultModel.find(id)->getId(), id);
    QCOMPARE(model.remove(), true);
    QVERIFY(resultModel.find(id) == 0);
    QCOMPARE(resultModel.find(id2)->getId(), id2);
    QCOMPARE(model2.remove(), true);
    list = resultModel.findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 1);
    QVERIFY(resultModel.find(id2) == 0);
    QCOMPARE(model3.remove(), true);
    QCOMPARE(resultModel.findAll().isEmpty(), true);
}

void Test_ORMObject::test_removeBy()
{
    MyModel model1, model2, model3, resultModel;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model1.getTableName()));
    model1.setnameInt(10);
    model1.setnameString("foo");
    model2.setnameInt(15);
    model3.setnameString("foo");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(resultModel.findAll().isEmpty(), false);
    QCOMPARE(resultModel.removeAll(), true);
    QCOMPARE(resultModel.findAll().isEmpty(), true);
}

void Test_ORMObject::test_removeAll()
{
    MyModel model1, model2, model3;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model1.getTableName()));
    model1.save();
    model2.save();
    model3.save();
    QList<MyModel*> list;
    list = model1.findAll();
    QCOMPARE(list.size(), 3);
    model1.removeAll();
    QCOMPARE(model1.findAll().isEmpty(), true);
}

void Test_ORMObject::test_count()
{
    MyModel model, model2, model3;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.count(), 0);
    model.save();
    QCOMPARE(model.count(), 1);
    model2.save();
    QCOMPARE(model.count(), 2);
    model3.save();
    QCOMPARE(model.count(), 3);
    model2.remove();
    model3.remove();
    QCOMPARE(model.count(), 1);
}

void Test_ORMObject::test_countByFieldName()
{
    MyModel model1, model2, model3;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model1.getTableName()));
    QCOMPARE(model2.count("nameint"), 0);
    model1.setnameInt(1);
    model2.setnameInt(1);
    model3.setnameInt(1);
    model1.setnameString("Hello!");
    model2.setnameString("Hello!");
    model3.setnameString("Hello!");
    model1.save();
    model2.save();
    model3.save();
    QCOMPARE(model1.count("nameInt"), 3);
    QCOMPARE(model2.count("nameString"), 3);
    QCOMPARE(model2.count("someField"), -1);
}

void Test_ORMObject::test_first()
{
    MyModel model, model2, model3;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.first()->getId(), model.getId());
    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QVERIFY(model.first() == 0);
}

void Test_ORMObject::test_last()
{
    MyModel model, model2, model3;

    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.save(), true);
    QCOMPARE(model2.save(), true);
    QCOMPARE(model3.last()->getId(), model2.getId());
    DB.getDb().exec(QString("DELETE FROM %1;").arg(model.getTableName()));
    QVERIFY(model.last() == 0);
}

void Test_ORMObject::test_toString()
{
    Car model;
    model.setNumber("123");
    model.setId(10);
    QString result = QString("car\n    id_%1 : 10\n    Number : 123\n").arg(model.getTableName());
    QCOMPARE(model.toString(), result);
}

void Test_ORMObject::test_hasUnsavedChanges()
{
    MyModel model;
    model.removeAll();
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameString("1234ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameInt(123);
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(model.save(), true);
    QVERIFY(model.getId() >= 0);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(model.update(), true);
    QCOMPARE(model.hasUnsavedChanges(), false);
}

void Test_ORMObject::test_dropTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;
    QCOMPARE(model.dropTable(), true);    
    QCOMPARE(license.dropTable(), true);
    QCOMPARE(car.dropTable(), true);
    QCOMPARE(driver.dropTable(), true);
}

QTEST_MAIN(Test_ORMObject)

#include "test_orm.moc"
