 #ifndef OPENLIST

#define OPENLIST

#include <QDialog>
#include <QListWidget>

 class QListWidget;
 
 class ListDialog : public QDialog
 {
 Q_OBJECT
 public:
 ListDialog(QWidget *parent = 0);
 int getNum(){return num;}

 
 signals:
 void clicked();

 private:
 int num;
  QListWidget *list;
 private slots:
 void getCliked(QListWidgetItem*);
 };
 #endif
