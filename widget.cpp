
#include "widget.h"
#include "ui_widget.h"
#include <QDate>
#include<QFileDialog>
#include<QPrintDialog>
#include<QPrinter>
#include<QDir>
#include<QDesktopServices>
#include<QMessageBox>
#include<QListWidget>
#include<QTextCodec>
#include<QCalendarWidget>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ///qRegExp
    ui->setupUi(this);
    QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
    title_report=codec->toUnicode(QString("Відомість по нарахуванню заробітної плати за").toStdString().c_str());
    ui->lineEdit_8->setText(title_report);
    ui->dateEdit->setDate(QDate::currentDate());
    // calendar->close();
    ui->pushButton->hide();
    ui->list->hide();
    ui->pushButton_2->hide();
    ui->table->setColumnWidth(1,200);
    ui->table->setColumnWidth(3,165);
    ui->table2->setColumnWidth(0,275);
    ui->table2->setColumnWidth(1,300);
    ui->label->hide();

    ui->pushButton_7->hide();



    /*
      1111
    QDir dir(QString("in\\"));
    QString entryAbsPath;
    QStringList filter;
    filter << "*.txt";
    dir.setNameFilters(filter);
    bool rem=false;

    if(!dir.entryList().isEmpty())
    {
        int file_count=dir.entryList().count();
        int vect_count=file_name_v.count();
        for(int i=0;i<file_count;i++)
        {
            rem=false;

            for(int j=0;j<vect_count;j++)
            {
                 if(dir.entryList().at(i)==file_name_v.at(j)+".txt")break;
                if(dir.entryList().at(i)!=file_name_v.at(j)+".txt") rem=true;
            }
            QFile::remove(dir.absolutePath() + "/" + dir.entryList().at(i));
        }

    }
*/




    QFile inFile("settings.ini");
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&inFile);
    QString name1,name2,name3,simple;
    name1=in.readLine(100);
    name2=in.readLine(100);
    name3=in.readLine(100);

    simple=in.readLine(100);

    out_dir=in.readLine(100);
    out_dir2=in.readLine(100);


    QString auto_openS=in.readLine(100);

    if(name1.contains("name1"))name1=name1.mid(5,name1.length());
    if(name2.contains("name2"))name2=name2.mid(5,name2.length());
    if(name3.contains("name3"))name3=name3.mid(5,name3.length());
    if(simple.contains("simple"))simple=simple.mid(6,simple.length());
    if(auto_openS.contains("auto_open:"))auto_openS=auto_openS.mid(10,auto_openS.length());
    ui->lineEdit_3->setText(name1);
    ui->lineEdit_2->setText(name2);
    ui->lineEdit->setText(name3);

    if(simple.contains("on"))ui->checkBox->setChecked(true);
    else ui->checkBox->setChecked(false);
    if(out_dir.contains("out_dir:"))out_dir=out_dir.mid(8,out_dir.length());


    if(out_dir2.contains("out_dir2:"))out_dir2=out_dir2.mid(9,out_dir2.length());
    ui->dir1->setText(out_dir);
    ui->dir2->setText(out_dir2);
    /*if(ui->checkBox->checkState())
    {
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->pushButton_6->hide();
    }*/
    auto_open=false;
    if(auto_openS.contains("on"))auto_open=true; else auto_open=false;



    loadBase();


    bool del=false;

    QVector<QString>::Iterator i = file_name_v.begin();
    while (i != file_name_v.end()) {
        if(!QFile::exists(QString("in\\%1").arg(*i+".txt"))){file_name_v.erase(i); del=true;i--;}
        ++i;

    }
    if(del){
        ui->pushButton_6->click();
        loadBase();}



    if(auto_open && ui->checkBox->checkState()){

        on_pushButton_4_clicked();

        ui->toolButton->setEnabled(false);
        ui->toolButton_2->setEnabled(false);
        ui->toolButton_3->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    }
    if(auto_open)ui->checkBox_3->setChecked(true);
    z_count=0;






}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked(QString filename) //.txt  openning
{
    input_file=filename;
    if(ui->checkBox->checkState())filename=QString("in\\%1").arg(filename);
    units_v.clear();
    //    QFileDialog *fdialog=new QFileDialog();
    //    fdialog->setFilter(QString("Text files(*.txt)"));
    //     filename=fdialog->getOpenFileName();
    //Unit units[50];
    QString OUT;
    QFile inFile(filename);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&inFile);
    in.setCodec("CP866");
    int counter=0;
    QString id,name,suma;


    QString line;
    line=in.read(30);

    date=line.mid(19,8);
    file_name=line.mid(5,11);


    //ui->label->setText(QString(" %1 ,<br> %2 <br> %3").arg(date).arg(file_name).arg(units[3].getName()));

    in.seek(368);
    while(!in.atEnd() &&counter<150)
    {

        line=in.read(159);


        id=line.mid(6,14);
        suma=line.mid(29,18);

        name=line.mid(47,100);
        int len=suma.length();

        for(int i=0;i<len;i++)
        {
            if(!(suma[i]=='0'))break;
            if(suma[i]=='0')suma[i]=' ';
        }
        suma=suma.trimmed();
        suma=suma.insert(suma.length()-2,",");



        units[counter].Add(id,name.trimmed(),suma,"");


        units_v.push_back(units[counter]);
        counter++;

        ui->label->setText(QString(" %1 <br> %2 ").arg(date).arg(file_name));
    }
    count=counter;
    ui->table->size();
    ui->table->clearContents();
    ui->table->setRowCount(counter-1);

    for(int i=0;i<counter;i++)
    {


        QTableWidgetItem *item1 =new QTableWidgetItem();
        item1->setText(units[i].getId());

        QTableWidgetItem *item2 =new QTableWidgetItem();
        item2->setText(units[i].getName());
        QTableWidgetItem *item3 =new QTableWidgetItem();
        item3->setText(units[i].getSuma());




        ui->table->setItem(i,0,item1);

        ui->table->setItem(i,1,item2);

        ui->table->setItem(i,2,item3);
    }

    for(int i=0;i<units_v.size();i++)
    {


        QTableWidgetItem *item1 =new QTableWidgetItem();
        item1->setText(units_v[i].getId());

        QTableWidgetItem *item2 =new QTableWidgetItem();
        item2->setText(units_v[i].getName());
        QTableWidgetItem *item3 =new QTableWidgetItem();
        item3->setText(units_v[i].getSuma());




        ui->table->setItem(i,0,item1);

        ui->table->setItem(i,1,item2);

        ui->table->setItem(i,2,item3);
    }

    ui->table->update();

    ui->status_lb->setText(QString("Файл %1 відкрито").arg(filename));
}

void Widget::on_pushButton_2_clicked(QString filename)
{

    //filename.replace(".uf",".txt");
    //    QFileDialog *fdialog=new QFileDialog();
    //    fdialog->setFilter(QString("*.txt"));

    //    filename=fdialog->getSaveFileName();
    QFile outFile(filename);

    QString preFilename=filename;
    preFilename= filename.mid(filename.lastIndexOf("\\")+1,filename.length()-filename.lastIndexOf("\\"));

    preFilename.remove(preFilename.length()-4,4);

    if(preFilename.length()>=7){
        preFilename=preFilename.remove(7,preFilename.length()-7);
        preFilename+=".TXT";
        filename=preFilename;}
    else  {
        int len=preFilename.length();
        for(int i=0;i<(7-len);i++)
            preFilename+="0";
        preFilename+=".TXT";
        filename=preFilename;
    }

    file_name=filename;
    //file_name=file_name.mid(file_name.lastIndexOf("/")+1,file_name.length()-file_name.lastIndexOf("/"));
    QDate today;
    date=today.currentDate().toString("dd/MM/yy");

    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outFile);
    out.setCodec("CP866");



    out<<"01017"<<file_name<<"368"<<date;
    QString number_of_rows=ui->label_11->text().mid(14,ui->label_11->text().length()-7-14);
    // ui->label_10->setText(number_of_rows);
    int len=number_of_rows.length();
    QString temp=number_of_rows;
    number_of_rows="";
    for(int i=0;i<(6-len);i++)
    {
        number_of_rows+="0";
    }
    number_of_rows+=temp;
    out<<number_of_rows;
    //ui->label_7->setText(number_of_rows);
    out<<"000000000000000000000000000000000000 ";
    /*
    QString suma_out=QString::number(sum_all,'f',2);
    suma_out.remove('.');
    len=number_of_rows.length();
    suma_out="";
    for(int i=0;i<(19-len);i++)
    {
        suma_out+="0";
    }*/
    QString suma_out;
    suma_out+=QString::number(sum_all,'f',2);
    suma_out.remove('.');
    len=suma_out.length();
    suma_out="";
    if(len<=19){
        for(int i=0;i<(19-len);i++)
        {
            suma_out+="0";
        }
    }
    QString pr="Найменування платника";
    //pr="Ќ???Ґ?гў???п Ї??в?ЁЄ?";

    QTextCodec * codec1 = QTextCodec::codecForName("Windows-1251");
    pr=codec1->fromUnicode(pr);
    suma_out+=QString::number(sum_all,'f',2);
    suma_out.remove('.');
    out<<suma_out;
    //ui->label_7->setText(suma_out);
    out<<"01           "<<pr<<"      1017                       ";

    QString priznachenya=ui->comboBox->itemText(ui->comboBox->currentIndex());

    len=priznachenya.length();
    for(int i=0;i<(160-len);i++)
        priznachenya+=" ";
    out<<priznachenya.toAscii();

    out<<"0000028                                            ";
    out<<"\n";
    for(int i=0;i<count;i++)
    {

        if(!ui->checkBox_2->checkState() && units[i].getSuma()=="0.00")continue;
        if(units[i].getName().isEmpty() || units[i].getId().toULongLong()==0)continue;

        out<<"00000"<<units[i].getId()<<"00001028";

        QString st=units[i].getSuma();
        st=st.remove(st.length()-3,1);
        st=st.remove(',');
        st=st.remove('.');
        int len=st.length();
        QString stSuma;
        for(int j=0;j<(19-len);j++)
            stSuma+="0";
        stSuma+=st;

        out<<stSuma;
        st=units[i].getName();
        len=st.length();
        for(int i=0;i<(100-len);i++)
            st+=" ";
        out<<st<<"000000000000"<<"\n";
    }
    outFile.close();
    //if(ui->checkBox->checkState())on_pushButton_3_clicked("out");
}

void Widget::on_table_cellChanged(int row, int column)
{
    QString numbers="0123456789";
    if(column==0 && ui->table->item(row,column)->text().length()>14)ui->table->item(row,column)->setText(ui->table->item(row,column)->text().mid(0,14));
    QString temp=ui->table->item(row,column)->text();
    if(column==0){
        QString prob;
        int len_temp=temp.length();
        if(temp.length()<14)
        {
            for(int i=0;i<14-len_temp;i++)
        {
            prob+="0";
        }
        }
        prob+=temp.trimmed();
        temp=prob;
        for(int i=0;i<14;i++)
        {

            if(!temp[i].isDigit())temp[i]='0';
        }
        ui->table->item(row,column)->setText(temp);
    }

    if(column==3 && ui->table->item(row,column)->text().length()>10)ui->table->item(row,column)->setText(ui->table->item(row,column)->text().mid(0,10));
    temp=ui->table->item(row,column)->text();
    if(column==3){
        for(int i=0;i<10;i++)
        {

            if(!temp[i].isDigit() && temp[i]!=' ')temp[i]='0';
        }
        ui->table->item(row,column)->setText(temp);
    }


    if(column==2 && ui->table->item(row,column)->text().length()>18)ui->table->item(row,column)->setText(ui->table->item(row,column)->text().mid(0,18));
    if(column==2){
        temp=ui->table->item(row,column)->text();
        for(int i=0;i<temp.length();i++)
        {

            if(!temp[i].isDigit() && !( temp[i]==',' || temp[i]=='.'))temp[i]='0';
            if(temp[i]==',')temp[i]='.';

        }
        if(!(temp.contains(",")|| temp.contains(".")))temp+=".00";
        temp=QString::number(temp.toDouble(),'f',2);
        ui->table->item(row,column)->setText(temp);
    }


    ///iiii

    if(column==1){
        temp=ui->table->item(row,column)->text();
        for(int i=0;i<temp.length();i++)
        {

            if(temp[i].toAscii()=='?')temp[i]='i';//капец


        }

        ui->table->item(row,column)->setText(temp);
    }
    ///////





    if(column==0) units[row].Add(ui->table->item(row,column)->text(),units[row].getName(),units[row].getSuma(),units[row].getIdentificator());
    if(column==1) units[row].Add(units[row].getId(),ui->table->item(row,column)->text(),units[row].getSuma(),units[row].getIdentificator());
    if(column==2) units[row].Add(units[row].getId(),units[row].getName(),ui->table->item(row,column)->text(),units[row].getIdentificator());
    if(column==3) units[row].Add(units[row].getId(),units[row].getName(),units[row].getSuma(),ui->table->item(row,column)->text());
    sum_all=0.00;
    z_count=0;
    for(int i=0;i<count;i++)
    {
        sum_all+=units[i].getSuma().toDouble();
        if(units[i].getSuma().toDouble()==0 && !units[i].getId().trimmed().isEmpty())z_count+=1;
    }

    ui->label_5->setText(QString("<font size=11 color=red> %1</font>").arg(QString::number(sum_all,'f',2)).toAscii());

    ui->label_7->setText(QString("<font size=11>")+QString::number(ui->table->rowCount())+QString("</font>"));
    ui->label_11->setText(QString("<font size=11>")+QString::number(ui->table->rowCount()-z_count)+QString("</font>"));
}

void Widget::on_toolButton_clicked()
{
    //if(ui->table->rowCount()<1)return;
    ui->table->setRowCount(ui->table->rowCount()+1);
    count=ui->table->rowCount()+1;

    QModelIndex newIndex = ui->table->model()->index(count, 1);
    ui->table->setCurrentIndex(newIndex);
    ui->table->setCurrentCell(count,1,QItemSelectionModel::SelectCurrent);

    ui->table->selectRow(count);



}

void Widget::on_pushButton_3_clicked()
{




    QString filename;

    if(!ui->checkBox->checkState()) {
        QFileDialog *fdialog=new QFileDialog();

        if(fdialog->Accept)    filename=fdialog->getSaveFileName(this,QString("Збереження бази"),QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),QString(" Текстовий файл(*.txt)"));
        else return;}
    else {
        //filename=QString("out\\zp_out.uf"); on_pushButton_2_clicked(filename);

        QString temp_s=file_name;
        temp_s=temp_s.mid(temp_s.indexOf("\\")+1,temp_s.length());

        if(!out_dir.isEmpty())filename=QString("%1\\%2").arg(out_dir).arg(input_file);

        if(ui->checkBox_2->checkState()){
            filename=QString("in\\%1").arg(input_file);
            ui->status_lb->setText(QString("Зберігаємо %1 ").arg(filename));
            on_pushButton_2_clicked(filename);
            return;
        }

    }

    ui->status_lb->setText(QString("Зберігаємо %1 ").arg(filename));
    /*QFile outFile(filename);
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outFile);
    out.setCodec("CP866");
    QString name_add;
    QString spaces;
    QString org_txt_str=ui->org_txt->text();
    spaces.fill(' ',130-org_txt_str.length());
    org_txt_str+=spaces;
    out<<"000000"<<org_txt_str<<"000000";
    for(int i=0;i<count;i++)
    {
        if(!ui->checkBox_2->checkState() && units[i].getSuma().toDouble()==0)continue;
        if(units[i].getId().trimmed().isEmpty()){ continue;}
        out<<"\n";
        out<<units[i].getId().trimmed()<<units[i].getName().trimmed()<<name_add.fill(' ',(100-units[i].getName().trimmed().length()));
        name_add="";
        units[i].getIdentificator();
        out<<units[i].getIdentificator().trimmed()<<name_add.fill(' ',(10-units[i].getIdentificator().trimmed().length()));
        name_add="";
        out<<units[i].getSuma().trimmed()<<name_add.fill(' ',(18-units[i].getSuma().trimmed().length()));
    }

    outFile.close();
*/
    QString temp_s=file_name;
    temp_s=temp_s.mid(temp_s.indexOf("\\")+1,temp_s.length());

    QString filename2=QString("%1\\%2").arg(out_dir2).arg(input_file);

    /* if(ui->checkBox->checkState() && !ui->checkBox_2->checkState())
    {




        QFile outFile2(filename2);
        outFile2.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out2(&outFile2);
        out2.setCodec("CP866");

        for(int i=0;i<count;i++)
        {
            if(!ui->checkBox_2->checkState() && units[i].getSuma()=="0.00")
            if(!ui->checkBox_2->checkState() && units[i].getSuma().toDouble()==0)continue;
            if(units[i].getId().trimmed().isEmpty()){ continue;}
            out2<<"\n";
            out2<<units[i].getId().trimmed()<<units[i].getName().trimmed()<<name_add.fill(' ',(100-units[i].getName().trimmed().length()));
            name_add="";
            units[i].getIdentificator();
            out2<<units[i].getIdentificator().trimmed()<<name_add.fill(' ',(10-units[i].getIdentificator().trimmed().length()));
            name_add="";
            out2<<units[i].getSuma().trimmed()<<name_add.fill(' ',(18-units[i].getSuma().trimmed().length()));
        }

        outFile2.close();

    }*/

    on_pushButton_2_clicked(filename);

    on_pushButton_2_clicked(filename2);


    ui->status_lb->setText(QString(tr("Збережено %1 , %2")).arg(filename).arg(filename2));
}

void Widget::on_pushButton_4_clicked()
{
    ui->list->clear();
    if(auto_open && ui->checkBox->checkState()){

        ui->toolButton->setEnabled(false);
        ui->toolButton_2->setEnabled(false);
        ui->toolButton_3->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    }

    ui->toolButton->setEnabled(true);
    ui->toolButton_2->setEnabled(true);
    ui->toolButton_3->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);

    ui->org_txt->clear();
    QString filename;


    QDir dir(QString("in\\"));



    QStringList filters1;
    filters1 << "*.txt";
    dir.setNameFilters(filters1);


    if(!ui->checkBox->checkState()){
        QFileDialog *fdialog=new QFileDialog();


        filename=fdialog->getOpenFileName(this,tr("Відкрити файл"),QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),QString("Формат зарахування (*.txt)"));
        if(filename.isEmpty())return;
        on_pushButton_clicked(filename);

    }
    else {


        /* if(!dir.entryList().isEmpty() && dir.entryList().count()>1 )
        {
            filename=QString("in\\%1").arg(dir.entryList()[0]);


            QStringList filters;

            QDir dir2(QString("in\\"));

            filters << "*.uf";
            dir2.setNameFilters(filters);


            ui->list->clear();
            //        list= new QListWidget;
            QStringList dirs=dir2.entryList();
            for(int i=0;i<dirs.count();++i){
                QFile inFile("in\\"+dirs[i]);
                inFile.open(QIODevice::ReadOnly | QIODevice::Text);
                QTextStream in(&inFile);
                in.setCodec("CP866");
                QString line;
                QString result;
                line=in.readLine(140);

                result=line.mid(6,130).trimmed();

                //  list->setProperty(result,&r);
                QString nuls;
                nuls.fill(' ',350);
                result+=nuls+":in\\"+dirs[i];
                ui->list->addItem(result);
                inFile.close();
            }*/

        /* QObject::connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),///connect with open funcion
                                         this,  SLOT(on_pushButton_7_clicked(QListWidgetItem*)));

        QObject::connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),///connect with open funcion

                                   list,  SLOT(hide()));*/

        if(!file_name_v.isEmpty()){
            for(int i=0;i<file_name_v.size();i++)
                ui->list->addItem(org_name_v.at(i));
            ui->list->show();
            ui->list->setFocus();}

        return ;
    }

    // if(dir.entryList().count()==1) filename=QString("in\\%1").arg(dir.entryList()[0]);
    /*if(dir.entryList().isEmpty())
        {
            filters1 << "*.uf"<<"*.txt";
            dir.setNameFilters(filters1);
            if(!dir.entryList().isEmpty()) filename=QString("in\\%1").arg(dir.entryList()[0]);
        }*/

    on_pushButton_clicked(filename);
    /* if( dir.entryList().isEmpty()) {

            switch( QMessageBox::warning( this, tr("Зарахування"),
                                          QString(tr("Помилка відкриття файлу.\n"
                                                     "Можливо файл бази не існує або "
                                                     "не підтримується даним програмним забезпеченням\n\n")).toAscii(),
                                          "Retry",
                                          "Cancel", 0, 0, 1 ) ) {
            case 0: on_pushButton_4_clicked();
                break;
            case 1: return;
                break;
            }


        }*/

    ui->status_lb->setText(QString("Відкривається: %1").arg(filename));
    //if(!QFile::exists(filename.replace(".txt",".uf")))filename.replace(".uf",".txt");

    //  if(filename.contains(".txt"))input_file=filename; else {input_file=filename; input_file.replace(".uf",".txt");}

    /*
    QFile inFile(filename);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&inFile);
    in.setCodec("CP866");
    QString line;
    QString id,name,suma,identificator;
    count=0;
    ui->table->setRowCount(0);
    line=in.readLine();
    ui->org_txt->setText(line.mid(6,130).trimmed());
    units_v.clear();
    while(!in.atEnd() && count<150)
    {

        line=in.readLine();
        if(line.trimmed().isEmpty()) continue;
        id=line.mid(0,14);
        name=line.mid(14,100);
        identificator=line.mid(114,10);


        suma=line.mid(124,18);
        suma=QString::number(suma.toDouble(),'f',2);
        units[count].Add(id,name,suma,identificator);
        units_v.push_back(units[count]);
        count+=1;
        // if(suma.toDouble()!=0)z_count+=1;
    }
    inFile.close();

    ui->table->clearContents();
    ui->table->setRowCount(count-1);


    for(int i=0;i<count;i++)
    {


        QTableWidgetItem *item1 =new QTableWidgetItem();
        item1->setText(units[i].getId());

        QTableWidgetItem *item2 =new QTableWidgetItem();
        item2->setText(units[i].getName());
        QTableWidgetItem *item3 =new QTableWidgetItem();
        item3->setText(units[i].getSuma());
        QTableWidgetItem *item4 =new QTableWidgetItem();
        if(units[i].getIdentificator()=="0000000000")item4->setText("");
        else   item4->setText(units[i].getIdentificator());



        ui->table->setItem(i,0,item1);

        ui->table->setItem(i,1,item2);

        ui->table->setItem(i,2,item3);

        ui->table->setItem(i,3,item4);
    }
    ui->table->update();
    ui->status_lb->setText(QString(tr("Файл %1 відкрито")).arg(filename));*/
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);


}

void Widget::on_pushButton_5_clicked()
{
     title_report=ui->lineEdit_8->text();

            QTextBrowser *print=new QTextBrowser();
    print->setFontPointSize(qreal(15));
    QFont d;
    d.setPixelSize(15);
    print->setFont(d);
    print->setMinimumSize(800,600);
    print->setWindowTitle(QString("Звіт"));
    print->show();
    QString months[13];
    months[0]="Error";
    months[1]="січня";
    months[2]="лютого";
    months[3]="березня";
    months[4]="квітня";
    months[5]="травня";
    months[6]="червня";
    months[7]="липня";
    months[8]="серпня";
    months[9]="вересня";
    months[10]="жовтня";
    months[11]="листопада";
    months[12]="грудня";
    QDate today=ui->dateEdit->date();
    QString date=today.toString("M");
    QString all;
    all+="<style type=\"text/css\">left {text-align:justify }  right {text-align:right }     </style>";
    all+=QString("<table width=\"100%\" cellspacing=0><tr><td width=\"100%\"> %1 <br>").arg(ui->lineEdit->text());
    all+=ui->org_txt->text();
    all+=QString("</td> <td >Чуднівське ТВБВ №10005/053  <br> ВАТ \"Ощадбанк\"</td></tr> </table> <br>");
    all+="<font size=16><center> "+title_report+" "+today.toString("d")+"  "+months[date.toInt()]+" "+today.toString("yyyy")+" року</center></font>";

    all+="<table width=\"100%\" border=1 border-color=red cellspacing=0>\n";
    //print->insertHtml(all);
    //  print->append(all);
    all+=QString("<tr><td>№ <td>%1 <td>%2 <td>%3<td> %4</tr>").arg(QString("<b>Номер рахунку</b>")).arg(QString("<b>ПІБ</b>")).arg(QString("<b>Ідентифікаційний номер</b>")).arg(QString("<b>Сума</b>"));
    sum_all=0.0;
    int numerical=0;
    for(int i=0;i<count;i++)
    {
        if(units[i].getId().trimmed().isEmpty())continue;
        if( units[i].getSuma().toDouble()==0)continue;
        numerical+=1;
        all+=QString("<tr><td>%5<td> %1 <td>%2 <td>%3<td>%4</tr>").arg(units[i].getId().trimmed()).arg(units[i].getName().trimmed()).arg(units[i].getIdentificator().trimmed()).arg(units[i].getSuma().trimmed()).arg(numerical);
        // sum_all+=units[i].getSuma().toDouble();
    }

    /*for(int i=0;i<units_v.size();i++)
    {
        if(units_v[i].getId().trimmed().isEmpty())continue;
        if( units_v[i].getSuma().toDouble()==0)continue;
        numerical+=1;
        all+=QString("<tr><td>%5<td> %1 <td>%2 <td>%3<td>%4</tr>").arg(units_v[i].getId().trimmed()).arg(units_v[i].getName().trimmed()).arg(units_v[i].getIdentificator().trimmed()).arg(units_v[i].getSuma().trimmed()).arg(numerical);
        sum_all+=units_v[i].getSuma().toDouble();
    }*/
    sum_all=0.0;
    for(int i=0;i<ui->table->rowCount();i++)
    {
        if(ui->table->item(i,2)->text().toDouble()==0)z_count+=1;
        sum_all+= ui->table->item(i,2)->text().toDouble();
    }

    all+=QString("<tr> <td> <td> <td> <td>%1  <td> %2</tr>").arg("Сума:").arg(QString::number(sum_all,'f',2));
    all+="</table>";

    //print->insertHtml(all);
    all+=QString("<center><br> <br><br> <b>Керівник:________________ %1 </b> <br><br> <br><b>Головний бухгалтер:________________ %2</b></center>").arg(ui->lineEdit_3->text()).arg(ui->lineEdit_2->text());
    print->append(all);


    QPrinter printer;

    QPrintDialog dialog(&printer,print);
    if (dialog.exec() == QDialog::Accepted)print->close();
    else return;
    print->print(&printer);

}

void Widget::on_toolButton_2_clicked()
{
    if(ui->table->rowCount()<1)return;

    for(int i=0;i<count;i++)
    {
        if(units[i].getId()==ui->table->item(ui->table->currentRow(),0)->text())
        {units[i].clear();break;}

    }
    ui->table->removeRow(ui->table->currentRow());
    //units[ui->table->currentRow()].clear();
    //units_v[ui->table->currentRow()].clear();
    sum_all=0.00;
    //for(int i=0;i<count;i++)
    //   sum_all+=units[i].getSuma().toDouble();
    ui->label_5->setText(QString("<font size=8 color=red>  %1</font>").arg(QString::number(sum_all,'f',2)));


    z_count=0;
    for(int i=0;i<ui->table->rowCount();i++)
    {
        if(ui->table->item(i,2)->text().toDouble()==0)z_count+=1;
        sum_all+= ui->table->item(i,2)->text().toDouble();
    }
    ui->label_5->setText(QString("<font size=11 color=red>  %1</font>").arg(QString::number(sum_all,'f',2)).toAscii());

    ui->label_7->setText(QString("<font size=11>")+QString::number(ui->table->rowCount())+QString("</font>"));
    ui->label_11->setText(QString("<font size=11>")+QString::number(ui->table->rowCount()-z_count)+QString("</font>"));


}






void Widget::on_pushButton_6_clicked()
{
    out_dir=ui->dir1->text();
    out_dir2=ui->dir2->text();


    QFile inFile("settings.ini");
    inFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream in(&inFile);
    in<<"name1"<<ui->lineEdit_3->text()<<"\n";
    in<<"name2"<<ui->lineEdit_2->text()<<"\n";
    in<<"name3"<<ui->lineEdit->text()<<"\n";
    if(ui->checkBox->isChecked()) in<<"simple"<<" on"<<"\n";
    else in<<"simple"<<" off"<<"\n";

    in<<"out_dir:"<<out_dir<<"\n";
    in<<"out_dir2:"<<out_dir2<<"\n";

    if(auto_open) in<<"auto_open:"<<" on"<<"\n";
    else in<<"auto_open:"<<" off"<<"\n";

    inFile.close();




    inFile.setFileName("base.ini");
    inFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&inFile);


    int c=file_name_v.size();
    out<<c<<"\n";
    for(int i=0;i<c;i++)
        out<<file_name_v[i]<<";"<<org_name_v[i]<<"\n";


    inFile.close();
}

void Widget::on_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{

}



void Widget::on_checkBox_stateChanged(int )
{
    /*if(!ui->checkBox->checkState())
    {
        ui->lineEdit->show();
        ui->lineEdit_2->show();
        ui->lineEdit_3->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->pushButton_6->show();
    }

    if(ui->checkBox->checkState())
    {
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->pushButton_6->hide();
    }*/

}

void Widget::on_comboBox_currentIndexChanged(QString )
{
    if(ui->comboBox->currentText()==QString("..."))ui->comboBox->setEditable(true);
    else ui->comboBox->setEditable(false);
}

void Widget::on_checkBox_3_stateChanged(int )
{
    auto_open=ui->checkBox_3->checkState();
}

void Widget::on_checkBox_clicked()
{

}







void Widget::on_pushButton_7_clicked()
{
    /*indian code its so beautiful))
QString filename=item->text().mid(item->text().lastIndexOf(":"),item->text().length()).trimmed();
    ui->org_txt->clear();


        ui->status_lb->setText(QString("Відкривається: %1").arg(filename));
            if(!QFile::exists(filename.replace(".txt",".uf")))filename.replace(".uf",".txt");

        //if(filename.contains(".txt"))input_file=filename; else {input_file=filename; input_file.replace(".uf",".txt");}
        //if(!filename.contains(".uf")){ on_pushButton_clicked(filename);return;}

        QFile inFile(filename);
        inFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&inFile);
        in.setCodec("CP866");
        QString line;
        QString id,name,suma,identificator;
        count=0;
        ui->table->setRowCount(0);
          line=in.readLine();
    ui->org_txt->setText(line.mid(6,130).trimmed());
        while(!in.atEnd() && count<150)
        {

            line=in.readLine();
            if(line.trimmed().isEmpty()) continue;
            id=line.mid(0,14);
            name=line.mid(14,100);
            identificator=line.mid(114,10);


            suma=line.mid(124,18);
            suma=QString::number(suma.toDouble(),'f',2);
            units[count].Add(id,name,suma,identificator);
            count+=1;
            // if(suma.toDouble()!=0)z_count+=1;
        }
        inFile.close();
        ui->table->clearContents();
        ui->table->setRowCount(count-1);


        for(int i=0;i<count;i++)
        {


            QTableWidgetItem *item1 =new QTableWidgetItem();
            item1->setText(units[i].getId());

            QTableWidgetItem *item2 =new QTableWidgetItem();
            item2->setText(units[i].getName());
            QTableWidgetItem *item3 =new QTableWidgetItem();
            item3->setText(units[i].getSuma());
            QTableWidgetItem *item4 =new QTableWidgetItem();
            if(units[i].getIdentificator()=="0000000000")item4->setText("");
            else   item4->setText(units[i].getIdentificator());



            ui->table->setItem(i,0,item1);

            ui->table->setItem(i,1,item2);

            ui->table->setItem(i,2,item3);

            ui->table->setItem(i,3,item4);
        }
        ui->table->update();
     ui->status_lb->setText(QString(tr("Файл %1 відкрито")).arg(filename));
*/
}

void Widget::on_list_itemDoubleClicked(QListWidgetItem* item)
{

    //    ui->toolButton->setEnabled(true);
    //    ui->toolButton_2->setEnabled(true);
    //    ui->toolButton_3->setEnabled(true);
    //    ui->pushButton_3->setEnabled(true);
    //    ui->pushButton_5->setEnabled(true);



    //    /*indian code its so beautiful))*/
    //    QString org_name=item->text().trimmed();
    //    QString filename;
    //    ui->org_txt->clear();

    //    //if(filename.contains(".txt"))input_file=filename; else {input_file=filename; input_file.replace(".uf",".txt");}
    //    for(int i=0;i<org_name_v.size();i++)
    //    {

    //        if(org_name_v.at(i).trimmed()==org_name) filename=file_name_v.at(i);
    //    }
    //    filename+=".txt";

    //    ui->status_lb->setText(QString("Відкривається: %1").arg(filename));
    //    //if(!QFile::exists(filename.replace(".txt",".uf")))filename.replace(".uf",".txt");

    //    //if(filename.contains(".txt"))input_file=filename; else {input_file=filename; input_file.replace(".uf",".txt");}
    //    on_pushButton_clicked(filename);
    //    ui->org_txt->setText(org_name);

    //    /*le_name=filename;
    //    // ui->label_6->setText(file_name);
    //    QFile inFile(filename);
    //    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    //    QTextStream in(&inFile);
    //    in.setCodec("CP866");
    //    QString line;
    //    QString id,name,suma,identificator;
    //    count=0;
    //    in.seek(0);
    //    ui->table->setRowCount(0);
    //    line=in.readLine();
    //    ui->org_txt->setText(line.mid(6,129).trimmed());
    //    for(int i=0; i<150; i++)
    //        units[i].clear();

    //    units_v.clear();
    //    while(!in.atEnd() && count<150)
    //    {

    //        line=in.readLine();
    //        if(line.trimmed().isEmpty()) continue;
    //        id=line.mid(0,14);
    //        name=line.mid(14,100);
    //        identificator=line.mid(114,10);


    //        suma=line.mid(124,18);
    //        suma=QString::number(suma.toDouble(),'f',2);
    //        units[count].Add(id,name,suma,identificator);
    //        units_v.push_back(units[count]);

    //        count+=1;
    //        // if(suma.toDouble()!=0)z_count+=1;
    //    }
    //    inFile.close();
    //    ui->table->clearContents();
    //    ui->table->setRowCount(count);//count-1   maybe


    //    for(int i=0;i<=count;i++)
    //    {


    //        QTableWidgetItem *item1 =new QTableWidgetItem();
    //        item1->setText(units[i].getId());

    //        QTableWidgetItem *item2 =new QTableWidgetItem();
    //        item2->setText(units[i].getName());
    //        QTableWidgetItem *item3 =new QTableWidgetItem();
    //        item3->setText(units[i].getSuma());
    //        QTableWidgetItem *item4 =new QTableWidgetItem();
    //        if(units[i].getIdentificator()=="0000000000")item4->setText("");
    //        else   item4->setText(units[i].getIdentificator());



    //        ui->table->setItem(i,0,item1);

    //        ui->table->setItem(i,1,item2);

    //        ui->table->setItem(i,2,item3);

    //        ui->table->setItem(i,3,item4);
    //    }
    //    ui->table->update();
    //    ui->status_lb->setText(QString(tr("Файл %1 відкрито")).arg(filename));
    //*/
    //    ui->list->hide();


}

void Widget::on_list_doubleClicked(QModelIndex index)
{

}

void Widget::on_toolButton_3_clicked()
{

    if(ui->table->rowCount()<1)return;
    for(int i=0;i<=count;i++)
    {
        QTableWidgetItem *item1 =new QTableWidgetItem("0");
        ui->table->setItem(i,2,item1);
    }
}


bool Widget::loadBase()
{


    QFile inFile("base.ini");
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&inFile);

    QString str_read,a1,a2;
    str_read=in.readLine();
    int c=str_read.toInt();

    for(int i=0;i<c;i++)
    {
        str_read=in.readLine();
        int l=str_read.length()-str_read.indexOf(";");
        a1=str_read.mid(0,str_read.indexOf(";"));
        a2=str_read.mid(str_read.indexOf(";")+1,l);

        file_name_v.push_back(a1);
        org_name_v.push_back(a2);

    }
    inFile.close();


    ui->table2->size();
    ui->table2->clearContents();
    ui->table2->setRowCount(c);

    for(int i=0;i<c;i++)
    {
        QTableWidgetItem *item1 =new QTableWidgetItem(file_name_v[i]);
        item1->setText(file_name_v[i]);
        ui->table2->setItem(i,0,item1);

        QTableWidgetItem *item2 =new QTableWidgetItem(org_name_v[i]);
        item2->setText(org_name_v[i]);
        ui->table2->setItem(i,1,item2);
    }
    ui->table2->update();
    return true;
}




void Widget::on_table2_cellChanged(int row, int column)
{
    if(column==0)
        file_name_v[row]= ui->table2->item(row,column)->text();

    if(column==1)
        org_name_v[row]= ui->table2->item(row,column)->text();
}

void Widget::on_toolButton_4_clicked()
{
    //if(ui->table2->rowCount()<1)return;
    file_name_v.push_back("");
    org_name_v.push_back("");
    ui->table2->setRowCount(ui->table2->rowCount()+1);
    QTableWidgetItem *item1 =new QTableWidgetItem(file_name_v.last());
    item1->setText(file_name_v.last());
    ui->table2->setItem(file_name_v.size(),0,item1);

    QTableWidgetItem *item2 =new QTableWidgetItem(org_name_v.last());
    item2->setText(org_name_v.last());
    ui->table2->setItem(org_name_v.size(),1,item2);


}

void Widget::on_toolButton_5_clicked()
{
    //  file_name_v[ui->table2->currentRow()]="";
    //org_name_v[ui->table2->currentRow()]="";
    if(ui->table2->rowCount()<1)return;
    org_name_v.remove(ui->table2->currentRow(),1);
    file_name_v.remove(ui->table2->currentRow(),1);
    ui->table2->removeRow(ui->table2->currentRow());

}

void Widget::on_table2_cellDoubleClicked(int row, int column)
{
    if(column==0){
        QFileDialog *fdialog=new QFileDialog();
        QString filename=fdialog->getOpenFileName(this,tr("Додати файл до бази"),QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),QString("Формат зарахування (*.txt)"));
        if(filename.isEmpty())return;
        /*reading file to the memory*/
        QFile inFile(filename);
        inFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&inFile);

        QString str_read;
        str_read=in.readAll();
        inFile.close();
        /*saving file at new location*/

        QString temp_s=filename;

        // temp_s=temp_s.mid(temp_s.indexOf("\\")+1,temp_s.length());
        temp_s= temp_s.split("/").last();

        QFile outFile(QApplication::applicationDirPath()+"/in/"+temp_s);
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&outFile);
        out<<str_read;
        outFile.close();

        //ui->table2->item(row,column)->setText(temp_s.remove(temp_s.length()-4,4));
        file_name_v[row]=temp_s.remove(temp_s.length()-4,4);
        QTableWidgetItem *item =new QTableWidgetItem(file_name_v.last());
        item->setText(file_name_v[row]);
        ui->table2->setItem(row,0,item);


    }
}

void Widget::on_list_itemClicked(QListWidgetItem *item)
{
    ui->toolButton->setEnabled(true);
    ui->toolButton_2->setEnabled(true);
    ui->toolButton_3->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);



    /*indian code its so beautiful))*/
    QString org_name=item->text().trimmed();
    QString filename;
    ui->org_txt->clear();


    for(int i=0;i<org_name_v.size();i++)
    {

        if(org_name_v.at(i).trimmed()==org_name) filename=file_name_v.at(i);
    }
    filename+=".txt";

    ui->status_lb->setText(QString("Відкривається: %1").arg(filename));

    on_pushButton_clicked(filename); //file reading
    ui->org_txt->setText(org_name);

    ui->list->hide();
}

void Widget::on_pushButton_8_clicked()
{



    if( finded_rows.isEmpty()){

        if( ui->lineEdit_4->text()!=word ) finded_rows.clear();
        bool finded=false;

        for(int i=0;i<ui->table->rowCount();i++){

            finded=ui->table->item(i,1)->text().contains(ui->lineEdit_4->text(),Qt::CaseInsensitive);
            if(finded) finded_rows.push_back(i);
        }
        word=ui->lineEdit_4->text();
    }

    if(ui->pushButton_8->text()==QString("Шукати далі") && !finded_rows.isEmpty())
    {
        QModelIndex newIndex = ui->table->model()->index(finded_rows.front(), 1);
        ui->table->setCurrentIndex(newIndex);
        ui->table->setCurrentCell(finded_rows.front(),1,QItemSelectionModel::SelectCurrent);

        ui->table->selectRow(finded_rows.front());
        finded_rows.pop_front();
    }



    if(!finded_rows.isEmpty()) {
        QModelIndex newIndex = ui->table->model()->index(finded_rows.front(), 1);
        ui->table->setCurrentIndex(newIndex);
        ui->table->setCurrentCell(finded_rows.front(),1,QItemSelectionModel::SelectCurrent);

        ui->table->selectRow(finded_rows.front());
        finded_rows.pop_front();
    }

    if(!finded_rows.isEmpty())ui->pushButton_8->setText(QString("Шукати далі"));
    else ui->pushButton_8->setText(QString("Пошук"));


}

void Widget::on_dateEdit_customContextMenuRequested(const QPoint &pos)
{


}

void Widget::on_dateEdit_dateChanged(const QDate &date)
{
    /*calendar=new QCalendarWidget(this);
    QObject::connect(calendar,SIGNAL(clicked(QDate)),ui->dateEdit,SLOT(setDate(QDate)));
    QObject::connect(calendar,SIGNAL(clicked(QDate)),calendar,SLOT(close()));

            calendar->move(ui->dateEdit->pos());
    calendar->show();*/
}

void Widget::on_dateEdit_editingFinished()
{

}

void Widget::on_lineEdit_8_textChanged(const QString &arg1)
{

}

void Widget::on_list_clicked(const QModelIndex &index)
{

}
