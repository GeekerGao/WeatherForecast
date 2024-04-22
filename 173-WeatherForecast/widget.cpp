#include "widget.h"
#include "ui_widget.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFont(QFont("Arial",15));
    setFixedSize(593,956);
    setWindowFlag(Qt::FramelessWindowHint);
    //新建一个菜单组件
    menuQuit = new QMenu(this);
    //设置菜单项文字颜色
    menuQuit->setStyleSheet("QMenu::item{color:white}");
    //为菜单组件添加一个动作
    QAction *closeAct = new QAction(QIcon(":/new/prefix1/res/close.png"),tr("退出"),this);
    menuQuit->addAction(closeAct);
    //用户选择菜单项并点击后回调函数，退出程序
    connect(menuQuit,&QMenu::triggered,this,[=]{
        this->close();
    });
    //由QNetworkAccessManager发起get请求
    manager = new QNetworkAccessManager(this);
    strUrl = "http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=47546632&appsecret=bOHHPCP1";
    QUrl urlTianQi(strUrl);
    //指定请求的url地址
    QNetworkRequest res(urlTianQi);
    reply = manager->get(res);
    //QNetworkRequest网络请求后进行数据读取
    connect(manager,&QNetworkAccessManager::finished,this,&Widget::readHttpReply);

    mDayList << ui->labelday0 << ui->labelday1
             << ui->labelday2 << ui->labelday3
             << ui->labelday4 << ui->labelday5;
    mDateList << ui->labeldate0 << ui->labeldate1
              << ui->labeldate2 << ui->labeldate3
              << ui->labeldate4 << ui->labeldate5;
    mIconList << ui->labelWeatherIcon0 << ui->labelWeatherIcon1
              << ui->labelWeatherIcon2 << ui->labelWeatherIcon3
              << ui->labelWeatherIcon4 << ui->labelWeatherIcon5;
    mWeaTypeList << ui->labelWeatherTypeDate0 << ui->labelWeatherTypeDate1
                 << ui->labelWeatherTypeDate2 << ui->labelWeatherTypeDate3
                 << ui->labelWeatherTypeDate4 << ui->labelWeatherTypeDate5;
    mAiqList << ui->labelaiq0 << ui->labelaiq1
             << ui->labelaiq2 << ui->labelaiq3
             << ui->labelaiq4 << ui->labelaiq5;
    mFxList << ui->labelFX0 << ui->labelFX1
            << ui->labelFX2 << ui->labelFX3
            << ui->labelFX4 << ui->labelFX5;
    mFlList << ui->labelFL0 << ui->labelFL1
            << ui->labelFL2 << ui->labelFL3
            << ui->labelFL4 << ui->labelFL5;

    //根据keys,设置icon的路径
    mTypeMap.insert("暴雪",":/new/prefix1/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/new/prefix1/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/new/prefix1/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/new/prefix1/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/new/prefix1/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/new/prefix1/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/new/prefix1/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/new/prefix1/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/new/prefix1/res/type/DongYu.png");
    mTypeMap.insert("多云",":/new/prefix1/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/new/prefix1/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/new/prefix1/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/new/prefix1/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/new/prefix1/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/new/prefix1/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/new/prefix1/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/new/prefix1/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/new/prefix1/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/new/prefix1/res/type/undefined.png");
    mTypeMap.insert("雾",":/new/prefix1/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/new/prefix1/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/new/prefix1/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/new/prefix1/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/new/prefix1/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/new/prefix1/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/new/prefix1/res/type/YangSha.png");
    mTypeMap.insert("阴",":/new/prefix1/res/type/Yin.png");
    mTypeMap.insert("雨",":/new/prefix1/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/new/prefix1/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/new/prefix1/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/new/prefix1/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/new/prefix1/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/new/prefix1/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/new/prefix1/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/new/prefix1/res/type/ZhongYu.png");

    ui->widget0404->installEventFilter(this);
    ui->widget0405->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        menuQuit->exec(QCursor::pos());
    }
    //鼠标当前位置event.globalPos();
    //窗口当前位置this.pos()
    //窗口新位置event->globalPos() - mOffset
    if(event->button() == Qt::LeftButton){
        mOffset = event->globalPos() - this->pos();
    }
}
//鼠标左键按下后的移动，导致这个事件被调用，设置窗口新位置
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);//鼠标当前位置-偏移值
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget0404 && event->type() == QEvent::Paint){
        drawTempLineHigh();
        return true;//表示事件已被处理
    }
    if(watched == ui->widget0405 && event->type() == QEvent::Paint){
        drawTempLineLow();
        return true;//表示事件已被处理
    }
    return QWidget::eventFilter(watched, event);
}

//解析天气Json数据
void Widget:: parseWeatherJsonData(QByteArray rawData)
{
    QJsonDocument jsonObj = QJsonDocument::fromJson(rawData);
    if(!jsonObj.isNull() && jsonObj.isObject()){
        QJsonObject objRoot = jsonObj.object();
        //解析日期
        QString date = objRoot["date"].toString();
        QString week = objRoot["week"].toString();
        ui->labelCurrentDate->setText(date+"  "+week);
        //解析城市名
        QString cityName = objRoot["city"].toString();
        ui->labelCity->setText(cityName+"市");
        //解析当前温度
        QString currentTmp = objRoot["tem"].toString();
        ui->labelTemp->setText(currentTmp+"℃");
        ui->labelTempRange->setText(objRoot["tem2"].toString()+"℃"+"~"+
                                    objRoot["tem1"].toString()+"℃");
        //解析天气类型
        ui->labelWeatherType->setText(objRoot["wea"].toString());
        ui->labelWeatherIcon->setPixmap(mTypeMap[objRoot["wea"].toString()]);
        //感冒指数
        ui->labelGanMao->setText(objRoot["air_tips"].toString());
        //解析风向
        ui->labelFXType->setText(objRoot["win"].toString());
        //解析风力
        ui->labelFXData->setText(objRoot["win_speed"].toString());
        //解析PM2.5
        ui->labelPM25Data->setText(objRoot["air_pm25"].toString());
        //解析湿度
        ui->labelShiduData->setText(objRoot["humidity"].toString());
        //解析空气质量
        ui->labelAirData->setText(objRoot["air_level"].toString());
    }
}
//解析7天天气Json数据
void Widget::parseWeatherJsonDataNew(QByteArray rawData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData);
    if(!jsonDoc.isNull() && jsonDoc.isObject()){
        QJsonObject jsonRoot = jsonDoc.object();
        days[0].mCity = jsonRoot["city"].toString();
        days[0].mpm25 = jsonRoot["aqi"].toObject()["pm25"].toString();

        if(jsonRoot.contains("data") && jsonRoot["data"].isArray()){
           QJsonArray weaArray = jsonRoot["data"].toArray();
            for(int i=0;i<weaArray.size();i++){
                QJsonObject obj = weaArray[i].toObject();
                days[i].mDate = obj["date"].toString();
                days[i].mWeek = obj["week"].toString();
                days[i].mWeaType = obj["wea"].toString();
                days[i].mTemp = obj["tem"].toString();
                days[i].mTempLow = obj["tem2"].toString();
                days[i].mTempHigh = obj["tem1"].toString();
                days[i].mFx = obj["win"].toArray()[0].toString();
                days[i].mFl = obj["win_speed"].toString();
                days[i].mAirq = obj["air_level"].toString();
                days[i].mTips = obj["index"].toArray()[3].toObject()["desc"].toString();
                days[i].mHu = obj["humidity"].toString();
            }
        }
    }
    updateUI();
}

void Widget::updateUI()
{
    QPixmap pixmap;
    //解析日期
    ui->labelCurrentDate->setText(days[0].mDate+"  "+days[0].mWeek);
    //解析城市名
    ui->labelCity->setText(days[0].mCity+"市");
    //解析当前温度
    ui->labelTemp->setText(days[0].mTemp+"℃");
    ui->labelTempRange->setText(days[0].mTempLow+"℃"+"~"+
                                days[0].mTempHigh+"℃");
    //解析天气类型
    ui->labelWeatherType->setText(days[0].mWeaType);
    ui->labelWeatherIcon->setPixmap(mTypeMap[days[0].mWeaType]);
    //感冒指数
    ui->labelGanMao->setText(days[0].mTips);
    //解析风向
    ui->labelFXType->setText(days[0].mFx);
    //解析风力
    ui->labelFXData->setText(days[0].mFl);
    //解析PM2.5
    ui->labelPM25Data->setText(days[0].mpm25);
    //解析湿度
    ui->labelShiduData->setText(days[0].mHu);
    //解析空气质量
    ui->labelAirData->setText(days[0].mAirq);
    //解析未来几天天气
    for(int i=0;i<6;i++){

        mDayList[i]->setText(days[i].mWeek);
        mDayList[0]->setText("今天");
        mDayList[1]->setText("明天");
        mDayList[2]->setText("后天");

        QStringList dayList = days[i].mDate.split("-");
        mDateList[i]->setText(dayList.at(1)+"-"+dayList.at(2));

        //缩放图片大小和label匹配
        int index = days[i].mWeaType.indexOf("转");
        if(index != -1){
            pixmap = mTypeMap[days[i].mWeaType.left(index)];
        }else{
            pixmap = mTypeMap[days[i].mWeaType];
        }
        pixmap = pixmap.scaled(mIconList[i]->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

        mIconList[i]->setMaximumHeight(50);
        mIconList[i]->setMaximumWidth(ui->widget0402->width()/6.5);
        mIconList[i]->setPixmap(pixmap);
        mWeaTypeList[i]->setText(days[i].mWeaType);

        QString airQ = days[i].mAirq;
        mAiqList[i]->setText(airQ);
        if(airQ == "优"){
            mAiqList[i]->setStyleSheet("background-color:rgb(150,213,32);border-radius:7px;color:rgb(230,230,230)");
        }
        if(airQ == "良"){
            mAiqList[i]->setStyleSheet("background-color:rgb(208,107,39);border-radius:7px;color:rgb(230,230,230)");
        }
        if(airQ == "轻度"){
            mAiqList[i]->setStyleSheet("background-color:rgb(255，199，199);border-radius:7px;color:rgb(230,230,230)");
        }
        if(airQ == "中度"){
            mAiqList[i]->setStyleSheet("background-color:rgb(255，17，17);border-radius:7px;color:rgb(230,230,230)");
        }
        if(airQ == "重度"){
            mAiqList[i]->setStyleSheet("backgroud-color:rgb(153，0，0);border-radius:7px;color:rgb(230,230,230)");
        }

        mFxList[i]->setText(days[i].mFx);
        index = days[i].mFl.indexOf("转");
        if(index != -1){
            mFlList[i]->setText(days[i].mFl.left(index));
        }else{
            mFlList[i]->setText(days[i].mFl);
        }
    }
    update();
}

void Widget::drawTempLineHigh()
{
    QPainter painter(ui->widget0404);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);

    int ave;
    int sum = 0;
    int offSet = 0;
    int middle = ui->widget0404->height()/2;
    for(int i=0;i<6;i++){
        sum += days[i].mTempHigh.toInt();
    }
    ave = sum / 6;
    //定义出6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAiqList[i]->x()+mAiqList[i]->width()/2);
        offSet = (days[i].mTempHigh.toInt() - ave) * 3;
        points[i].setY(middle - offSet);
        //画出6个点
        painter.drawEllipse(QPoint(points[i]),3,3);
        //画出当天温度
        painter.drawText(points[i].x()-15,points[i].y()-13,days[i].mTempHigh+"°");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }
}

void Widget::drawTempLineLow()
{
    QPainter painter(ui->widget0405);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QColor(70,192,203));
    painter.setPen(QColor(70,192,203));

    int ave;
    int sum = 0;
    int offSet = 0;
    int middle = ui->widget0404->height()/2;
    for(int i=0;i<6;i++){
        sum += days[i].mTempLow.toInt();
    }
    ave = sum / 6;
    //定义出6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAiqList[i]->x()+mAiqList[i]->width()/2);
        offSet = (days[i].mTempLow.toInt() - ave) * 3;
        points[i].setY(middle - offSet);
        //画出6个点
        painter.drawEllipse(QPoint(points[i]),3,3);
        //画出当天温度
        painter.drawText(points[i].x()-15,points[i].y()-13,days[i].mTempLow+"°");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }
}
//读取Http回应
void Widget::readHttpReply(QNetworkReply *reply)
{
    int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError && resCode == 200){
        //大多数服务器返回的字符编码格式是UTF8
        QByteArray data = reply->readAll();
        //parseWeatherJsonData(data);
        parseWeatherJsonDataNew(data);
    }else{
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("网络请求失败");
        mes.setStyleSheet("QPushButton{color:red}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }
}

void Widget::on_pushButton_clicked()
{
    QString cityNameFromUser = ui->lineEditCity->text();
    QString cityCode = cityCodeUtils.getCityCodeFromName(cityNameFromUser);
    if(cityCode != NULL){
        strUrl += "&cityid=" + cityCode;
        manager->get(QNetworkRequest(QUrl(strUrl)));
    }else{
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("请输入正确的城市名");
        mes.setStyleSheet("QPushButton{color:red}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }
}

void Widget::on_lineEditCity_returnPressed()
{
    on_pushButton_clicked();
}
