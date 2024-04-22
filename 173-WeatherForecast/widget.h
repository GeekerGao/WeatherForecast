#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMenu>
#include <QNetworkReply>
#include <QLabel>
#include "citycodeutils.h"
#include "day.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Day days[7];
    QList<QLabel *> mDayList;
    QList<QLabel *> mDateList;
    QList<QLabel *> mIconList;
    QList<QLabel *> mWeaTypeList;
    QList<QLabel *> mAiqList;
    QList<QLabel *> mFxList;
    QList<QLabel *> mFlList;

    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched,QEvent *event);
public:
    void readHttpReply(QNetworkReply *reply);
private slots:
    void on_pushButton_clicked();

    void on_lineEditCity_returnPressed();

private:
    Ui::Widget *ui;
    QMenu *menuQuit;
    QPoint mOffset;
    QNetworkReply *reply;
    QString strUrl;
    QNetworkAccessManager *manager;
    CityCodeUtils cityCodeUtils;
    void parseWeatherJsonData(QByteArray rawData);
    QMap<QString,QString> mTypeMap;
    void parseWeatherJsonDataNew(QByteArray rawData);
    void updateUI();
    void drawTempLineHigh();
    void drawTempLineLow();
};
#endif // WIDGET_H
