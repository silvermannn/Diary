#pragma once

#include "database.h"

#include <QDialog>
#include <QListWidget>

namespace Ui {
class OneParameterDialog;
}

class OneParameterDialog : public QDialog
{
	Q_OBJECT

public:
    explicit OneParameterDialog(QWidget *parent, Database& db, QDate date, int id);
    ~OneParameterDialog();

public slots:
    void selectParameter(QListWidgetItem* item);
	void chkShowAllStateChanged(int);

private:
    Ui::OneParameterDialog *ui;
	Database& _db;
	int _id;

    void loadParametersList();
    void loadParameter();

	// QDialog interface
public slots:
	void accept();
};

