#ifndef MODELTABLECHECK_H
#define MODELTABLECHECK_H

#include "Global.h"
#include "OrmObject.h"
#include "ExceptionSql.h"

class ModelTableCheck
{

public:
    ModelTableCheck();

    //Sprawdzenie wszystkich tabel
    static void checkAllTables();

    template<class T>
    static void checkOneTable(COrmObject<T> *ormobj)
    {
        bool isChangeOccured = false;

        if (!DB.checkTableInDatabase(ormobj->getTableName()))
        {
            try
            {
                ormobj->createTable();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
            try
            {
                ormobj->createIndexes();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
            try
            {
                ormobj->createUniqueIndexes();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
        }
        if (!DB.checkTableInDatabase(ormobj->getTableName()))
        {
            DEBUG_WITH_LINE << "Table error :" << ormobj->getTableName();
        }
        //Sprawdzenie kolumn
        QSqlRecord cols = DB.getTableFields(ormobj->getTableName());

        for(int i = 1; i < ormobj->metaObject()->propertyCount(); i++)
        {
            QString propertyName = QString::fromLatin1(ormobj->metaObject()->property(i).name());

            if (!cols.contains(propertyName))
            {
                try
                {
                    ormobj->alterTableAddColumn(propertyName);
                }
                catch (CExceptionSql *e)
                {
                    DB.showErrorInfo(e);
                }
                isChangeOccured = true;
            }
        }
        //Sprawdzenie indeksów
        //Sprawdzamy czy indeksy getUniqueIndexFields i  getIndexFields są w ormobj->getTableIndexes()

        QHash <QString, QString> table_index_list = ormobj->getTableIndexesNames();

        Q_FOREACH( auto idxName, ormobj->getIndexFields().keys())
        {
            QString idxFullName = QString("%1_%2_idx").arg(ormobj->getTableName()).arg(idxName);
            if (!table_index_list.contains(idxFullName))
            {
                isChangeOccured = true;
                break; //Wychodzimy bo i tak robimy wszystkie indeksy od początku
            }
        }

        Q_FOREACH( auto idxName, ormobj->getUniqueIndexFields().keys())
        {
            QString idxFullName = QString("%1_%2_idx").arg(ormobj->getTableName()).arg(idxName);
            if (!table_index_list.contains(idxFullName))
            {
                isChangeOccured = true;
                break; //Wychodzimy bo i tak robimy wszystkie indeksy od początku
            }
        }

        if (isChangeOccured) //Dodane zostały pola robimy jeszcze raz indeksy
        {
            try
            {
                ormobj->dropIndexes();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
            try
            {
                ormobj->createIndexes();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
            try
            {
                ormobj->createUniqueIndexes();
            }
            catch (CExceptionSql *e)
            {
                DB.showErrorInfo(e);
            }
        }
    }


};

#endif // MODELTABLECHECK_H
