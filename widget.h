#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QFile>
#include<QTextStream>
#include"unit.h"
#include<QTextBrowser>
#include<QListWidget>
#include <QVector>
#include<QCalendarWidget>
namespace Ui {
    class Widget;

}


class Widget : public QWidget
{
    Q_OBJECT

public:
    QString title_report;
    Unit units[150];
    QVector<Unit> units_v;
//QCalendarWidget *calendar;
    QVector<QString>file_name_v;
    QVector<QString>org_name_v;
    QVector<int>finded_rows;
    QString word;
    QString date,file_name;
    int count;
    qreal sum_all;
    QString input_file,out_dir,out_dir2;

    bool auto_open;
    int z_count;

    bool loadBase();
    explicit Widget(QWidget *parent = 0);
    ~Widget();
//QListWidget *list;
private:
    Ui::Widget *ui;


private slots:

    void on_list_doubleClicked(QModelIndex index);
    void on_list_itemDoubleClicked(QListWidgetItem* item);
    void on_pushButton_7_clicked();
    void on_checkBox_clicked();
    void on_checkBox_3_stateChanged(int );
    void on_comboBox_currentIndexChanged(QString );
    void on_checkBox_stateChanged(int );
    void on_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_pushButton_6_clicked();


    void on_toolButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_toolButton_clicked();
    void on_table_cellChanged(int row, int column);
    void on_pushButton_2_clicked(QString filename);
    void on_pushButton_clicked(QString filename);

    void on_toolButton_3_clicked();


    void on_table2_cellChanged(int row, int column);
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void on_table2_cellDoubleClicked(int row, int column);
    void on_list_itemClicked(QListWidgetItem *item);
    void on_pushButton_8_clicked();
    void on_dateEdit_customContextMenuRequested(const QPoint &pos);
    void on_dateEdit_dateChanged(const QDate &date);
    void on_dateEdit_editingFinished();
    void on_lineEdit_8_textChanged(const QString &arg1);
    void on_list_clicked(const QModelIndex &index);
};

#endif // WIDGET_H
