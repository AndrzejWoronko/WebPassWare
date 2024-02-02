#ifndef CSERVICE_H
#define CSERVICE_H

#include "Global.h"
#include "Exception.h"

class ICService
{
public:
    virtual ~ICService() = default;
    virtual void clearError() = 0;
    virtual void showErrorInfo(CException &e) = 0;
    virtual QString getError() = 0;
    virtual void setError(const QString &) = 0;
};


class CService: public ICService
{
    QString m_error;

public:
    CService();
    ~CService() = default;

    virtual void clearError() override final;
    virtual void showErrorInfo(CException &e) override final;
    virtual void setError(const QString &) override final;
    virtual QString getError() override final;
};

#endif // CSERVICE_H
