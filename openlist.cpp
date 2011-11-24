 #include <QtGui >

#include "openlist.h"


ListDialog::ListDialog(QWidget *parent):QDialog(parent)
 {

list=new QListWidget();
list->show();


 connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),this, SLOT(this->getCliked(QListWidgetItem*)));

}

void ListDialog::getCliked(QListWidgetItem * item)
{
 num=item->text().length();
}


