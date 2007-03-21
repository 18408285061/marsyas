#include "MarControlGUI.h"

#ifdef MARSYAS_QT

#include "realvec.h"

using namespace Marsyas;
using namespace std;

MarControlGUI::MarControlGUI(MarControlPtr control, QWidget *parent)
	: control_(control), QWidget(parent)
{
	nullWidget_ = NULL;
	realWidget_ = NULL;
	boolWidget_= NULL;
	naturalWidget_= NULL;
	stringWidget_= NULL;
	vecWidget_ = NULL;
	vecTable_ = NULL;

	cname_ = control_->getName();
	//control_ = control;

	editing_ = false;

	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	createWidget();
}

MarControlGUI::~MarControlGUI()
{

}

QSize
MarControlGUI::sizeHint() const
{
	return sizeHint_;
}

void
MarControlGUI::createWidget()
{
	string ctype = control_->getType();
	
	//replace by more efficient approach (e.g. use MarControlManager) [!]
	if(ctype == "mrs_real")
	{
		realWidget();
		return;
	}
	if(ctype == "mrs_natural")
	{
		naturalWidget();
		return;
	}
	if(ctype == "mrs_bool")
	{
		boolWidget();
		return;
	}
	if(ctype == "mrs_string")
	{
		stringWidget();
		return;
	}
	if(ctype == "mrs_realvec")
	{
		vecWidget();
		return;
	}
	if(ctype == "mrs_unknown")
	{
		nullWidget(); //[!]must be changed to provide custom widgets!
	}
}

void
MarControlGUI::nullWidget()
{
	//delete nullWidget_;
	nullWidget_ = new QLabel(this);
	nullWidget_->setText("CUSTOM CONTROL");
	sizeHint_ = nullWidget_->sizeHint();
}

void
MarControlGUI::realWidget()
{
	//delete realWidget_;
	realWidget_ = new QLineEdit(this);
	realWidget_->setText(QString::number(double(control_->toReal())));
	
	sizeHint_ = realWidget_->sizeHint();

	connect(realWidget_, SIGNAL(textEdited(const QString &)), 
		this, SLOT(editingStarted()));

	connect(realWidget_, SIGNAL(editingFinished()), 
										this, SLOT(double2MarControl()));
}

void
MarControlGUI::boolWidget()
{
	//delete boolWidget_;
	boolWidget_ = new QCheckBox(this);
	boolWidget_->setChecked(control_->toBool());
	sizeHint_ = boolWidget_->sizeHint();
	
	connect(boolWidget_, SIGNAL(toggled(bool)), 
		this, SLOT(toMarControl(bool)));
}

void
MarControlGUI::naturalWidget()
{
	//delete naturalWidget_;
	naturalWidget_ = new QLineEdit(this);
	naturalWidget_->setText(QString::number((int)(control_->toNatural())));

	sizeHint_ = naturalWidget_->sizeHint();

	connect(naturalWidget_, SIGNAL(textEdited(const QString &)), 
		this, SLOT(editingStarted()));

	connect(naturalWidget_, SIGNAL(editingFinished()), 
		this, SLOT(int2MarControl()));
}

void
MarControlGUI::stringWidget()
{
	//delete stringWidget_;
	stringWidget_ = new QLineEdit(this);
	stringWidget_->setText(QString::fromStdString(control_->toString()));
	sizeHint_ = stringWidget_->sizeHint();

	connect(stringWidget_, SIGNAL(textEdited(const QString &)), 
		this, SLOT(editingStarted()));

	connect(stringWidget_, SIGNAL(editingFinished()), 
		this, SLOT(string2MarControl()));
}

void
MarControlGUI::vecWidget()
{
	//delete vecWidget_;
	vecWidget_ = new QToolButton(this);
	vecWidget_->setText("...");
	sizeHint_ = vecWidget_->sizeHint();

	connect(vecWidget_, SIGNAL(clicked(bool)),
										this, SLOT(showVectorTable()));
}

void
MarControlGUI::showVectorTable()
{
	if(control_->getType() != "mrs_realvec") //if not a vector, do nothing
		return;
	
	realvec vec = control_->toVec();

	if(vec.getSize() == 0)
	{
		QMessageBox::information(this, "Empty vector!",QString::fromStdString(cname_),QMessageBox::Ok,
			 										QMessageBox::NoButton,QMessageBox::NoButton);
		return;
	}

	int rows = (int)(vec.getRows());
	int cols = (int)(vec.getCols());
	
	vecTable_ = new QTableWidget(rows, cols, NULL);
	vecTable_->setWindowTitle(QString::fromStdString(cname_));
	vecTable_->setAttribute(Qt::WA_DeleteOnClose, true);
	
	for(int r = 0; r < rows; r++)
		for(int c = 0; c < cols; c++)
		{
			QString svalue = QString::number(vec(r,c));
			QTableWidgetItem *cell = new QTableWidgetItem(svalue);
			//cell->setFlags(Qt::ItemFlags(cell->flags() & (!Qt::ItemIsEditable))); //[!]
			vecTable_->setItem(r, c, cell);
		}

	vecTable_->resizeColumnsToContents();

	connect(vecTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
										this, SLOT(toMarControl(QTableWidgetItem*)));

	connect(vecTable_, SIGNAL(destroyed(QObject*)),
		this, SLOT(vecTableDestroyed()));

	QDialog* vecDialog = new QDialog(this);
	QGridLayout* layout = new QGridLayout(vecDialog);
	layout->addWidget(vecTable_);
	vecDialog->show();
}

void
MarControlGUI::vecTableDestroyed()
{
	vecTable_ = NULL;
}

void
MarControlGUI::double2MarControl()
{
	double val = realWidget_->text().toDouble();

	if(val == (double)control_->toReal())
		return;

	control_->setValue(val); //this is thread safe!
	//emit controlChanged(cname_, control_);

	editing_ = false;
}

void
MarControlGUI::toMarControl(bool val)
{
	if(val == control_->toBool())
		return;

	control_->setValue(val);//this is thread safe!
	//emit controlChanged(cname_, control_);
}

void
MarControlGUI::int2MarControl()
{
	int val = naturalWidget_->text().toInt();
	
	if(val == (int)control_->toNatural())
		return;

	control_->setValue((mrs_natural)val);//this is thread safe!
	//emit controlChanged(cname_, control_);

	editing_ = false;
}

void
MarControlGUI::string2MarControl()
{
	string val = stringWidget_->text().toStdString();
	
	if(val == control_->toString())
		return;

	control_->setValue(val); //this is thread safe!
	//emit controlChanged(cname_, control_);

	editing_ = false;
}

void
MarControlGUI::toMarControl(QTableWidgetItem* cell)
{
	QTableWidget* table = cell->tableWidget();
	int col = table->column(cell);
	int row = table->row(cell);
	
	realvec vec = control_->toVec();
	vec(row, col) = (mrs_real)(cell->text().toDouble());
	
	control_->setValue(vec);//this is thread safe!
	//emit controlChanged(cname_, control_);
}

void
MarControlGUI::updControl(MarControlPtr control)
{
	//if this control is currently being edited by the user
	//do not update its widget
	if(editing_)
		return;
	
	//check control name and if it's the correct type
	if((control_->getType() != control->getType()) &&
			(cname_ != control->getName()))
		return;

	//if no changes are needed, do nothing
// 	if(!(*control != control_))
// 		return;

	//update internal MarControlPtr copy
	//control_ = control;
	
	//update corresponding widget
	
	string ctype = control_->getType();
	if(ctype == "mrs_real")
	{
		realWidget_->setText(QString::number((double)control_->toReal()));//does not trigger a signal!
		return;
	}
	if(ctype == "mrs_natural")
	{
		naturalWidget_->setText(QString::number((int)control_->toNatural()));//does not trigger a signal!
		return;
	}
	if(ctype == "mrs_bool")
	{
		//we have to temporary disconnect the QCheckBox signal otherwise
		//we'll enter an infinite signal->slot loop...
		disconnect(boolWidget_, SIGNAL(toggled(bool)), 
			this, SLOT(toMarControl(bool))); //[!]
		boolWidget_->setChecked(control_->toBool());//This does trigger a signal!!
		connect(boolWidget_, SIGNAL(toggled(bool)), 
			this, SLOT(toMarControl(bool)));
		return;
	}
	if(ctype == "mrs_string")
	{
		stringWidget_->setText(QString::fromStdString(control_->toString()));//does not trigger a signal!
		return;
	}
	if(ctype == "mrs_realvec")
	{
		// 		if(vecTable_)
		// 		{
		// 			Q_ASSERT(disconnect(vecTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
		// 				this, SLOT(toMarControl(QTableWidgetItem*))));
		// 
		// 			realvec vec = control_->toVec();
		// 			int rows = (int)(vec.getRows());
		// 			int cols = (int)(vec.getCols());
		// 
		// 			for(int r = 0; r < rows; r++)
		// 				for(int c = 0; c < cols; c++)
		// 				{
		// 					QString svalue = QString::number(vec(r,c));
		// 					QTableWidgetItem *cell = vecTable_->item(r,c);
		// 					cell->setText(svalue);
		// 				}
		// 			
		// 			vecTable_->resizeColumnsToContents();
		// 			
		// 			Q_ASSERT(connect(vecTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
		// 					this, SLOT(toMarControl(QTableWidgetItem*))));
		// 		}
		return;
	}
	if(ctype == "mrs_unknown")
	{
		//[!]must be changed to provide custom widgets!
	}
}

void
MarControlGUI::editingStarted()
{
	editing_ = true;
}

#endif //MARSYAS_QT
