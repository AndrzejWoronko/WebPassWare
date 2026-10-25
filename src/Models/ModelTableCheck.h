#ifndef MODELTABLECHECK_H
#define MODELTABLECHECK_H

#include "Global.h"
#include "OrmRepository.h"
#include "ExceptionSql.h"

class ModelTableCheck
{

public:
    ModelTableCheck();

    //Sprawdzenie wszystkich tabel
    static void checkAllTables(CDatabase &db);

    template<class T>
    static void checkOneTable(COrmRepository<T> &repo)
    {
        bool isChangeOccured = false;
        T *ormobj = repo.prototype();

        if (!repo.database().checkTableInDatabase(ormobj->getTableName()))
        {
            try
            {
                repo.createTable();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
            try
            {
                repo.createIndexes();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
            try
            {
                repo.createUniqueIndexes();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
        }
        if (!repo.database().checkTableInDatabase(ormobj->getTableName()))
        {
            DEBUG_WITH_LINE << "Table error :" << ormobj->getTableName();
        }
        //Sprawdzenie kolumn
        QSqlRecord cols = repo.database().getTableFields(ormobj->getTableName());

        for(int i = 1; i < ormobj->metaObject()->propertyCount(); i++)
        {
            QString propertyName = QString::fromLatin1(ormobj->metaObject()->property(i).name());

            if (!cols.contains(propertyName))
            {
                try
                {
                    repo.alterTableAddColumn(propertyName);
                }
                catch (CExceptionSql &e)
                {
                    CDatabase::showErrorInfo(e);
                }
                isChangeOccured = true;
            }
        }
        //Sprawdzenie indeksów
        //Sprawdzamy czy indeksy getUniqueIndexFields i  getIndexFields są w ormobj->getTableIndexes()

        QHash <QString, QString> table_index_list = repo.getTableIndexesNames();

        for (auto idxName : ormobj->getIndexFields().keys())
        {
            QString idxFullName = QString("%1_%2_idx").arg(ormobj->getTableName()).arg(idxName);
            if (!table_index_list.contains(idxFullName))
            {
                isChangeOccured = true;
                break; //Wychodzimy bo i tak robimy wszystkie indeksy od początku
            }
        }

        for (auto idxName : ormobj->getUniqueIndexFields().keys())
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
                repo.dropIndexes();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
            try
            {
                repo.createIndexes();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
            try
            {
                repo.createUniqueIndexes();
            }
            catch (CExceptionSql &e)
            {
                CDatabase::showErrorInfo(e);
            }
        }
    }


};

#endif // MODELTABLECHECK_H
