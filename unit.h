
class Unit
{
private:
	QString id_number;
	QString name;
	QString suma; 
        QString identificator;

public:
Unit()
{
id_number=" ";
name=" ";
suma=" ";
identificator=" ";
}
        Unit(QString Id_number,QString Name,QString Suma,QString Identificator)
	{
		id_number=Id_number;
		name=Name;
		suma=Suma;
                identificator=Identificator;
	}
        void Add(QString Id_number,QString Name, QString Suma,QString Identificator)
	{
		id_number=Id_number;
		name=Name;
		suma=Suma;
                identificator=Identificator;
	}

	QString getId(){return id_number;}
	QString getName(){return name;}
	QString getSuma(){return suma;}
        QString getIdentificator(){ return identificator;}
        void clear()
        {
            id_number=" ";
            name=" ";
            suma=" ";
            identificator=" ";
        }
};



