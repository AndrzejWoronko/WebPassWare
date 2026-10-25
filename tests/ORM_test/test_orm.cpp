#include <QObject>
#include <QString>
#include <QtTest>
#include <QSqlRecord>
#include <QSqlField>
#include <QList>
#include <QFile>
#include <QSqlDriver>
#include "Global.h"
#include "Database.h"
#include "ExceptionSql.h"
#include "OrmObject.h"
#include "OrmRepository.h"

#include <memory>

// SQLite hands back its own storage type (bool as 0/1, a QChar column as
// QString). Qt 5 compared QVariants of different types by converting them;
// Qt 6 does not, and has no QString -> QChar conversion, hence the fallback.
static bool sameStoredValue(const QVariant &stored, const QVariant &expected)
{
    QVariant converted = stored;
    if (converted.convert(expected.metaType()))
    {
        return converted == expected;
    }
    return stored.toString() == expected.toString();
}

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

private:
    // QSqlDatabase::exec() is deprecated in Qt 6.
    QSqlQuery execSql(const QString &sql)
    {
        QSqlQuery query(m_db->getDb());
        query.exec(sql);
        return query;
    }

    template<class T>
    COrmRepository<T> repo()
    {
        return COrmRepository<T>(*m_db);
    }

    std::unique_ptr<CDatabase> m_db;
};

Test_ORMObject::Test_ORMObject()
{
    const QString dataPath = QDir::temp().filePath(QString("webpassware-orm-test-data-%1").arg(QUuid::createUuid().toString(QUuid::Id128)));
    QDir().mkpath(dataPath);
    qputenv("XDG_DATA_HOME", QFile::encodeName(dataPath));

    m_db = std::make_unique<CDatabase>();

    m_db->setEchoQuery(true);
    m_db->setLogQuery(true);
    m_db->setEchoError(true);
    m_db->setLogError(true);

    m_db->setQueryLog();
    //Rejestracja typów

    qRegisterMetaType<Value>("Value");
    qRegisterMetaType<Price>("Price");
    qRegisterMetaType<Amount>("Amount");
    qRegisterMetaType<Indicator>("Indicator");
    qRegisterMetaType<Varchar255>("Varchar255");
    qRegisterMetaType<Timestamp>("Timestamp");

    DEBUG_WITH_LINE << m_db->getDb().driverName();
}

Test_ORMObject::~Test_ORMObject()
{   
    const QString path = m_db->getBaseNameFullPath();
    m_db.reset();
    QFile::remove(path);
}

void Test_ORMObject::test_createTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;

    QCOMPARE(repo<MyModel>().createTable(), true);
//    QCOMPARE(repo<MyModel>().createTable(), false);
    QCOMPARE(repo<CarDriver>().createTable(), true);
    QCOMPARE(repo<DriverLicense>().createTable(), true);
    QCOMPARE(repo<Car>().createTable(), true);
}

void Test_ORMObject::test_save()
{
    MyModel model;
    QTime time = QTime::currentTime();

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
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
    QCOMPARE(repo<MyModel>().save(&model), true);
    QSqlQuery query = execSql(QString("SELECT * FROM %1;").arg(model.getTableName()));
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
            QCOMPARE(query.value(i).toDateTime(), model.gettimestp());
        else
        {
            const QVariant expected = model.property(query.record().fieldName(i).toLocal8Bit().constData());
            QVERIFY2(sameStoredValue(query.value(i), expected),
                     qPrintable(query.record().fieldName(i) + ": " + query.value(i).toString() + " != " + expected.toString()));
        }
    }
}

void Test_ORMObject::test_update()
{
    MyModel model, model2;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    model2.setnameInt(10);
    QCOMPARE(repo<MyModel>().update(&model2), false);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), true);
    repo<MyModel>().save(&model2);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), false);
    model.setId(model2.getId());
    model.setnameInt(15);
    QCOMPARE(repo<MyModel>().update(&model), true);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), false);
    QCOMPARE(repo<MyModel>().first()->getnameInt(), 15);
}

void Test_ORMObject::test_find()
{
    MyModel model;
    MyModel *pointer;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(model.getId(), -1);    
    QCOMPARE(repo<MyModel>().save(&model), true);
    int id = model.getId();
    QVERIFY(id >= 0);
    QCOMPARE((pointer = repo<MyModel>().find(id))->getId(), id);
    delete pointer;
    QVERIFY(repo<MyModel>().find(13423) == 0);
    QCOMPARE(model.getId(), id);
    QSqlQuery query = execSql(QString("SELECT * FROM %1 WHERE id_%1 = %2;").arg(model.getTableName()).arg(id));
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
            QCOMPARE(query.value(i).toDateTime(), model.gettimestp());
        else
        {
            const QVariant expected = model.property(query.record().fieldName(i).toLocal8Bit().constData());
            QVERIFY2(sameStoredValue(query.value(i), expected),
                     qPrintable(query.record().fieldName(i) + ": " + query.value(i).toString() + " != " + expected.toString()));
        }
    }
}

void Test_ORMObject::test_findAll()
{
    MyModel model, model1, model2, model3, resultModel;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), true);
    model.setnameInt(1);
    model.setnameString("a");
    model1.setnameInt(2);
    model1.setnameString("b");
    model2.setnameInt(3);
    model2.setnameString("a");
    model3.setnameInt(4);
    model3.setnameString("c");
    repo<MyModel>().save(&model);
    repo<MyModel>().save(&model1);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    QList<MyModel*> list;
    list = repo<MyModel>().findAll();
    QCOMPARE(list.size(), 4);
    CLEAR_LIST
    repo<MyModel>().updateProperty(&model1, "nameString", "a");
    repo<MyModel>().updateProperty(&model2, "nameInt", "0");
    list = repo<MyModel>().findAll(QString("nameInt"));
    QCOMPARE(list.first()->getnameInt(), 0);
    CLEAR_LIST
    list = repo<MyModel>().findAll(QString("nameString"));
    QCOMPARE(list.size(), 4);
    QCOMPARE(list.first()->getnameString(), "a");
    CLEAR_LIST
}

void Test_ORMObject::test_findByValue()
{
    MyModel model;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameInt(15);
    model.setnameString("a");
    repo<MyModel>().save(&model);
    QList<MyModel*> list;
    list = repo<MyModel>().findBy("nameInt", QVariant(15));
    QCOMPARE(list.first()->getnameInt(), 15);
    MyModel model2;
    list = repo<MyModel>().findBy("nameString", QVariant("sdjkfhsjk"));
    QCOMPARE(model2.getId(), -1);
    QCOMPARE(list.size(), 0);
    CLEAR_LIST
    list = repo<MyModel>().findBy("nameInt", 15);
    QCOMPARE(list.size(), 1);
    CLEAR_LIST
    MyModel model3;
    model3.setnameInt(15);
    model3.setnameString("b");
    repo<MyModel>().save(&model3);
    list = repo<MyModel>().findBy("nameInt", 15);
    QCOMPARE(list.size(), 2);
    CLEAR_LIST
    list = repo<MyModel>().findBy("nameInt", 15, QString("id_%1").arg(model3.getTableName()));
    QCOMPARE(list.first()->getnameString(), QString("a"));
    CLEAR_LIST
}

void Test_ORMObject::test_findByValues()
{
    MyModel model1, model2, model3, resultModel;

    execSql(QString("DELETE FROM %1;").arg(model1.getTableName()));
    model1.setnameInt(10);
    model2.setnameInt(11);
    model3.setnameInt(12);
    repo<MyModel>().save(&model1);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    QVector<QVariant> vector;
    vector.append(10);
    vector.append(11);
    QList<MyModel*> list;
    list = repo<MyModel>().findBy("nameInt", vector);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.first()->getId(), model1.getId());
    vector.clear();
    CLEAR_LIST
    list = repo<MyModel>().findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    vector.append(20);
    list = repo<MyModel>().findBy("nameInt", vector);
    QCOMPARE(list.isEmpty(), true);
    CLEAR_LIST
    vector.clear();
    vector.append(10);
    vector.append(11);
    repo<MyModel>().updateProperty(&model3, "nameInt", 10);
    list = repo<MyModel>().findBy("nameInt", vector);
    QCOMPARE(list.size(), 3);
    CLEAR_LIST
    list = repo<MyModel>().findBy("nameInt", vector, QString("nameInt"));
    QCOMPARE(list.size(), 3);
    QCOMPARE(list.first()->getnameInt(), 10);
    CLEAR_LIST
}

void Test_ORMObject::test_findByParams()
{
    MyModel model, model2, model3, resultModel;

    m_db->dropTable(model.getTableName());
    repo<MyModel>().createTable();
    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameString("Hello");
    model2.setnameInt(3);
    model3.setnamedouble(1.23);
    QCOMPARE(repo<MyModel>().save(&model), true);
    QCOMPARE(repo<MyModel>().save(&model2), true);
    QCOMPARE(repo<MyModel>().save(&model3), true);
    QHash<QString, QVariant> findHash;
    findHash.insert("nameString", QVariant("Hello"));
    findHash.insert("nameInt", QVariant(3));
    findHash.insert("nameDouble", QVariant(1.23));
    findHash.insert("nameChar", QVariant('X'));
    QList<MyModel*> list;
    list = repo<MyModel>().findByOr(findHash);
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
    list = repo<MyModel>().findByAnd(findHash);
    QCOMPARE(list.isEmpty(), true);
}

void Test_ORMObject::test_updateProperty()
{
    MyModel model, model2;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    model.setnameInt(10);
    repo<MyModel>().save(&model);
    QCOMPARE(repo<MyModel>().updateProperty(&model, "nameString", "Hello"), true);
    QVERIFY_EXCEPTION_THROWN(repo<MyModel>().updateProperty(&model, "ololoProperty", 123), CExceptionSql);
    QCOMPARE(repo<MyModel>().updateProperty(&model2, "nameInt", 20), false);
    QCOMPARE(repo<MyModel>().first()->getnameString(), QString("Hello"));
}

void Test_ORMObject::test_exists()
{
    MyModel model;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().existsTable(), false);
    repo<MyModel>().save(&model);
    QCOMPARE(repo<MyModel>().existsTable(), true);
    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().existsTable(), false);
}

void Test_ORMObject::test_existsById()
{
    MyModel model;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().exists(1), false);
    QCOMPARE(repo<MyModel>().exists(model.getId()), false);
    repo<MyModel>().save(&model);
    QCOMPARE(repo<MyModel>().exists(model.getId()), true);
    int id = model.getId();
    repo<MyModel>().remove(&model);
    QCOMPARE(repo<MyModel>().exists(id), false);
}

void Test_ORMObject::test_remove()
{
    MyModel model, model2, model3, resultModel;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    repo<MyModel>().save(&model);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    int id = model.getId();
    int id2 = model2.getId();
    QList<MyModel*> list = repo<MyModel>().findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 3);
    QCOMPARE(repo<MyModel>().find(id)->getId(), id);
    QCOMPARE(repo<MyModel>().remove(&model), true);
    QVERIFY(repo<MyModel>().find(id) == 0);
    QCOMPARE(repo<MyModel>().find(id2)->getId(), id2);
    QCOMPARE(repo<MyModel>().remove(&model2), true);
    list = repo<MyModel>().findAll();
    QCOMPARE(list.isEmpty(), false);
    QCOMPARE(list.size(), 1);
    QVERIFY(repo<MyModel>().find(id2) == 0);
    QCOMPARE(repo<MyModel>().remove(&model3), true);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), true);
}

void Test_ORMObject::test_removeBy()
{
    MyModel model1, model2, model3, resultModel;

    execSql(QString("DELETE FROM %1;").arg(model1.getTableName()));
    model1.setnameInt(10);
    model1.setnameString("foo");
    model2.setnameInt(15);
    model3.setnameString("foo");
    repo<MyModel>().save(&model1);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), false);
    QCOMPARE(repo<MyModel>().removeAll(), true);
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), true);
}

void Test_ORMObject::test_removeAll()
{
    MyModel model1, model2, model3;

    execSql(QString("DELETE FROM %1;").arg(model1.getTableName()));
    repo<MyModel>().save(&model1);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    QList<MyModel*> list;
    list = repo<MyModel>().findAll();
    QCOMPARE(list.size(), 3);
    repo<MyModel>().removeAll();
    QCOMPARE(repo<MyModel>().findAll().isEmpty(), true);
}

void Test_ORMObject::test_count()
{
    MyModel model, model2, model3;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().count(), 0);
    repo<MyModel>().save(&model);
    QCOMPARE(repo<MyModel>().count(), 1);
    repo<MyModel>().save(&model2);
    QCOMPARE(repo<MyModel>().count(), 2);
    repo<MyModel>().save(&model3);
    QCOMPARE(repo<MyModel>().count(), 3);
    repo<MyModel>().remove(&model2);
    repo<MyModel>().remove(&model3);
    QCOMPARE(repo<MyModel>().count(), 1);
}

void Test_ORMObject::test_countByFieldName()
{
    MyModel model1, model2, model3;

    execSql(QString("DELETE FROM %1;").arg(model1.getTableName()));
    QCOMPARE(repo<MyModel>().count("nameint"), 0);
    model1.setnameInt(1);
    model2.setnameInt(1);
    model3.setnameInt(1);
    model1.setnameString("Hello!");
    model2.setnameString("Hello!");
    model3.setnameString("Hello!");
    repo<MyModel>().save(&model1);
    repo<MyModel>().save(&model2);
    repo<MyModel>().save(&model3);
    QCOMPARE(repo<MyModel>().count("nameInt"), 3);
    QCOMPARE(repo<MyModel>().count("nameString"), 3);
    QVERIFY_EXCEPTION_THROWN(repo<MyModel>().count("someField"), CExceptionSql);
}

void Test_ORMObject::test_first()
{
    MyModel model, model2, model3;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().save(&model), true);
    QCOMPARE(repo<MyModel>().save(&model2), true);
    QCOMPARE(repo<MyModel>().first()->getId(), model.getId());
    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QVERIFY(repo<MyModel>().first() == 0);
}

void Test_ORMObject::test_last()
{
    MyModel model, model2, model3;

    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QCOMPARE(repo<MyModel>().save(&model), true);
    QCOMPARE(repo<MyModel>().save(&model2), true);
    QCOMPARE(repo<MyModel>().last()->getId(), model2.getId());
    execSql(QString("DELETE FROM %1;").arg(model.getTableName()));
    QVERIFY(repo<MyModel>().last() == 0);
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
    repo<MyModel>().removeAll();
    QCOMPARE(model.getId(), -1);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameString("1234ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    model.setnameInt(123);
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(repo<MyModel>().save(&model), true);
    QVERIFY(model.getId() >= 0);
    QCOMPARE(model.hasUnsavedChanges(), false);
    model.setnameString("123ABC");
    QCOMPARE(model.hasUnsavedChanges(), true);
    QCOMPARE(repo<MyModel>().update(&model), true);
    QCOMPARE(model.hasUnsavedChanges(), false);
}

void Test_ORMObject::test_dropTable()
{
    MyModel model;
    CarDriver driver;
    DriverLicense license;
    Car car;
    QCOMPARE(repo<MyModel>().dropTable(), true);    
    QCOMPARE(repo<DriverLicense>().dropTable(), true);
    QCOMPARE(repo<Car>().dropTable(), true);
    QCOMPARE(repo<CarDriver>().dropTable(), true);
}

QTEST_MAIN(Test_ORMObject)

#include "test_orm.moc"
