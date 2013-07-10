#include "specarr.h"

specArr::specArr(QObject *parent) :
    QObject(parent)
{
}

bool specArr::CreateTable(QSqlDatabase *db)
{
    QSqlQuery* query = new QSqlQuery(*db);
    QString str = "DROP TABLE spec_out";
    query->exec(str);
    str = "CREATE TABLE spec_out ("
            "seqno SERIAL PRIMARY KEY,"
            "pgp_index integer,"
            "mes_day date,"
            "mes_time time with time zone,"
            "rainc real,"
            "rainnc real,"
            "hailnc real,"
            "snownc real,"
            "graupelnc real,"
            "snowh real,"
            "psfc real,"
            "t2 real,"
            "windSpeed real,"
            "windAngle real,"
            "tRail real,"
            "tslb real,"
            "smois real,"
            "glaccyIce integer,"
            "hurricane integer);";
    if(!query->exec(str)){
        qDebug() << "Unable to create table";
        qDebug() << query->lastError();
        return false;
    }
    return true;
}

bool specArr::InsertValues(QSqlDatabase *db)
{
    /** Сначала Update-им если запись существует, если ее нет, то запрос ничего не будет делать**/
    QSqlQuery* preparedQueryUpdate = new QSqlQuery(*db);
    QString str_update;
    str_update = "UPDATE spec_out SET "
            "rainc = :rainc, "
            "rainnc = :rainnc, "
            "hailnc = :hailnc, "
            "snownc = :snownc, "
            "graupelnc = :graupelnc, "
            "snowh = :snowh, "
            "psfc = :psfc, "
            "t2 = :t2, "
            "windspeed = :windspeed, "
            "windangle = :windangle, "
            "trail = :trail, "
            "tslb = :tslb, "
            "smois = :smois, "
            "glaccyice = :glaccyice, "
            "hurricane = :hurricane ";
    str_update = str_update + "WHERE pgp_index = " + pgp_index +
            " AND mes_day = '" + date_time.date().toString("MM/dd/yyyy") +
            "' AND mes_time = '" + date_time.time().toString("hh:mm")+"';";
    preparedQueryUpdate->prepare(str_update);
    preparedQueryUpdate->bindValue( ":rainc", rainc);
    preparedQueryUpdate->bindValue( ":rainnc", rainnc);
    preparedQueryUpdate->bindValue( ":hailnc", hailnc);
    preparedQueryUpdate->bindValue( ":snownc", snownc);
    preparedQueryUpdate->bindValue( ":graupelnc", graupelnc);
    preparedQueryUpdate->bindValue( ":snowh", snowh);
    preparedQueryUpdate->bindValue( ":psfc", psfc);
    preparedQueryUpdate->bindValue( ":t2", t2);
    preparedQueryUpdate->bindValue( ":windSpeed", windSpeed);
    preparedQueryUpdate->bindValue( ":windAngle", windAngle);
    preparedQueryUpdate->bindValue( ":trail", tRail);
    preparedQueryUpdate->bindValue( ":tslb", tslb);
    preparedQueryUpdate->bindValue( ":smois", smois);
    preparedQueryUpdate->bindValue( ":glaccyice", glaccyIce);
    preparedQueryUpdate->bindValue( ":hurricane", hurricane);
    if(!preparedQueryUpdate->exec())
    {
        qDebug() << "Unable to insert";
        qDebug() << preparedQueryUpdate->lastError();
        return false;
    }
    /** Теперь вставляем при условии, что такой записи еще нет **/
    QSqlQuery* preparedQueryInsert= new QSqlQuery(*db);
    QString str_insert = " INSERT INTO spec_out(pgp_index, mes_day, mes_time, rainc, rainnc, hailnc, snownc,"
            "graupelnc, snowh, psfc, t2, windSpeed, windAngle, trail, tslb, smois, glaccyice, hurricane)"
            " SELECT :pgp_index, :mes_day, :mes_time, :rainc, :rainnc, :hailnc, :snownc,"
            ":graupelnc, :snowh, :psfc, :t2, :windSpeed, :windAngle, :trail, :tslb, :smois, :glaccyice, :hurricane"
            " WHERE NOT EXISTS (SELECT 1 FROM spec_out WHERE pgp_index = :pgp_index1 AND mes_day= :mes_day1 AND mes_time= :mes_time1);";

    preparedQueryInsert->prepare(str_insert);
    preparedQueryInsert->bindValue( ":pgp_index1", pgp_index);
    preparedQueryInsert->bindValue( ":mes_day1", date_time.date().toString("MM/dd/yyyy"));
    preparedQueryInsert->bindValue( ":mes_time1", date_time.time().toString("hh:mm"));
    preparedQueryInsert->bindValue( ":pgp_index", pgp_index);
    preparedQueryInsert->bindValue( ":mes_day", date_time.date().toString("MM/dd/yyyy"));
    preparedQueryInsert->bindValue( ":mes_time", date_time.time().toString("hh:mm"));
    preparedQueryInsert->bindValue( ":rainc", rainc);
    preparedQueryInsert->bindValue( ":rainnc", rainnc);
    preparedQueryInsert->bindValue( ":hailnc", hailnc);
    preparedQueryInsert->bindValue( ":snownc", snownc);
    preparedQueryInsert->bindValue( ":graupelnc", graupelnc);
    preparedQueryInsert->bindValue( ":snowh", snowh);
    preparedQueryInsert->bindValue( ":psfc", psfc);
    preparedQueryInsert->bindValue( ":t2", t2);
    preparedQueryInsert->bindValue( ":windSpeed", windSpeed);
    preparedQueryInsert->bindValue( ":windAngle", windAngle);
    preparedQueryInsert->bindValue( ":trail", tRail);
    preparedQueryInsert->bindValue( ":tslb", tslb);
    preparedQueryInsert->bindValue( ":smois", smois);
    preparedQueryInsert->bindValue( ":glaccyice", glaccyIce);
    preparedQueryInsert->bindValue( ":hurricane", hurricane);
    if(!preparedQueryInsert->exec())
    {
        qDebug() << "Unable to insert";
        qDebug() << preparedQueryInsert->lastError();
        return false;
    }
    return true;
}
