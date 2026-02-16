#include "oneparameterdialog.h"
#include "ui_oneparameterdialog.h"

#include "utilites.h"

#include <QLabel>
#include <QDateEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QMessageBox>

OneParameterDialog::OneParameterDialog(QWidget *parent, Database& db, QDate date, int id) :
	QDialog(parent),
    ui(new Ui::OneParameterDialog),
	_db(db),
	_id(id)
{
    ui->setupUi(this);

    loadParametersList();
    loadParameter();

	QDateEdit* dateEdit = findChild<QDateEdit*>("dateExecution");
	dateEdit->setDate(date);

	QLineEdit* thisAmount = findChild<QLineEdit*>("textAmount");
	thisAmount->setValidator(createUniversalDoubleValidator());
}

OneParameterDialog::~OneParameterDialog()
{
	delete ui;
}

void OneParameterDialog::selectParameter(QListWidgetItem*)
{

}

void OneParameterDialog::chkShowAllStateChanged(int)
{
    loadParametersList();
}

void OneParameterDialog::loadParametersList()
{
    QListWidget *lst = findChild<QListWidget*>("listParameters");
    lst->clear();

    QCheckBox* showAll = findChild<QCheckBox*>("chkShowAll");
    IdToString exs = _db.fetchParametersList(showAll->isChecked());
    for (IdToString::const_iterator i = exs.begin(); i != exs.end(); ++i)
	{
		QListWidgetItem* pItem = new QListWidgetItem(i.value());
		pItem->setData(Qt::UserRole, i.key());
		lst->addItem(pItem);
	}
	lst->sortItems();
}

void OneParameterDialog::loadParameter()
{
    if (_id != -1)
	{
		QDate date;
		int foodId;
		double amount;
		QString comment;
		_db.fetchFoodIntake(_id, date, foodId, amount, comment);

		QDateEdit* dateEdit = findChild<QDateEdit*>("dateExecution");
		dateEdit->setDate(date);

        QListWidget *lst = findChild<QListWidget*>("listParameters");
		selectItemById(lst, foodId);

		QLineEdit* thisAmount = findChild<QLineEdit*>("textAmount");
		thisAmount->setText(QString::number(amount));

		QPlainTextEdit* thisComment = findChild<QPlainTextEdit*>("textComment");
		thisComment->setPlainText(comment);

		thisAmount->setFocus();
	}
	else
	{
        QListWidget *lst = findChild<QListWidget*>("listParameters");
		lst->setFocus();
	}
}

void OneParameterDialog::accept()
{
	QDateEdit* dateEdit = findChild<QDateEdit*>("dateExecution");

    QListWidget *lst = findChild<QListWidget*>("listParameters");
	if (lst->selectedItems().size() == 0)
	{
        QMessageBox::warning(this,"Недопустимый параметр", "Не выбран параметр");
		return;
	}
	int id = lst->selectedItems()[0]->data(Qt::UserRole).toInt();

	QLineEdit* thisAmount = findChild<QLineEdit*>("textAmount");

	QString s = thisAmount->text().replace(',','.');
	double amount = s.toDouble();
	if (amount < 0.0000001)
	{
		QMessageBox::warning(this,"Недопустимый параметр", "Не заполнено количество");
		return;
	}

	QPlainTextEdit* thisComment = findChild<QPlainTextEdit*>("textComment");

	if (_id == -1)
	{
        _db.addParameterValue(dateEdit->date(), id, amount, thisComment->toPlainText());
	}
	else
	{
		_db.saveFoodIntake(_id, dateEdit->date(), id, amount, thisComment->toPlainText());
	}
	QDialog::accept();
}
