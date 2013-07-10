#ifndef XML_NAMELIST_H
#define XML_NAMELIST_H

#include <QObject>
#include "QDomDocument"
#include "QFile"
#include "QDebug"
#include "QDate"

/** Класс для работы с namelist.wfs*/
class xml_namelist : public QObject
{
    Q_OBJECT
private:
    /** Указатель на домовскую модель*/
    QDomDocument* p_domDocument;
    /** Текущий файл*/
    QString curXML;
    /** Найти элемент в домовской модели*/
    QDomElement findElement(QDomElement parent, const QString &textToFind);
    /** Записать элемент XML*/
    void writeFolderElement(const QDomElement &element, QTextStream &device);
    void SetNumAndStepElement(const QDomElement &element, QString x_num, QString dx, QString y_num, QString dy);
    /** Задать начальную дату и время*/
    void SetStartDateTime(QDateTime* p_start);
    /** Задать конечную дату и время*/
    void SetEndDateTime(QDateTime* p_end);
public:
    /** Конструктов*/
    explicit xml_namelist(QObject *parent = 0);
    /** Проверка GRIB файлов*/
    bool SetTimeDateFromGRIB(QString grib_files);
    /** Получить время из GRIB файла*/
    bool GetDateTimeFromGrib2(QString fileName, QDateTime *dateTime);
    /** Задать дату и время*/
    void SetDates(QDateTime* p_start, QDateTime* p_end);

    void SetTimeAnalysis(QDateTime* p_start, int deltaInSeconds);

    void GetParentIDAndRatio(QString &s_id, QString &s_ratio);

    void SetParentIDAndRatio(QString s_id, QString s_ratio);

    /** Задать центр домена*/
    void SetCentreDomain(float ref_lat, float ref_lon);
    /** Прочесть центр домена*/
    void GetCentreDomain(float &ref_lat,float &ref_lon);
    /** Задать шаг и величину шага*/
    void SetNumAndStep(QString x_num, QString dx, QString y_num, QString dy);
    /** Прочесть шаг и величину шага*/
    void GetNumAndStep(QString &x_num, QString &dx, QString &y_num, QString &dy);
    /** Получить дату и время начала счета*/
    QDateTime GetDateTimeStart();
    /** Получить дату и время окончания счета*/
    QDateTime GetDateTimeEnd();
    /** Задать рабочую папку */
    void SetWorkFolder(QString path);
    /** Задать выходную папку */
    void SetOutputFolder(QString path);
    /** Задать папку с Геоданными */
    void SetGeogPath(QString path);
    /** Прочесть XML*/
    bool ReadXml(QString file_name);
    /** Записать XML*/
    bool writeXML(QString file_name);
    /** Ссылка на внешнюю домовскую модель*/
    void SetDomDocument(QDomDocument*);
    /** Записать файл WPS*/
    bool writeasciiWps(QString);
    /** Записать файл WRF*/
    bool writeasciiWrf(QString);
    /** Записать файл WRF*/
    bool writeasciiOBSPROC(QString);
    /** Записать файл ARWPost*/
    bool writeasciiARWPost(QString);

    /** Вложенные сетки **/
    /** Задать количество доменов*/
    void SetDomainCount(int n_count);
    /** Получить количество доменов*/
    int GetDomainCount();
    void SetParentStart(QString s_i, QString s_j);
    void GetParentStart(QString &s_i, QString & s_j);


signals:

public slots:

};

#endif // XML_NAMELIST_H
