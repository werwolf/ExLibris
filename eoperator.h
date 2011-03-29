#ifndef EOPERATOR_H
#define EOPERATOR_H

#include <QTabWidget>
#include "euser.h"

namespace Ui {
    class EOperator;
}

class EOperator : public QWidget, public EUser
{
    Q_OBJECT

public:
    explicit EOperator(EUser& user, QWidget* parent = 0);
    ~EOperator();

private:
    Ui::EOperator *ui;
};

#endif // EOPERATOR_H
