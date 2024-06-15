#include <iostream>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), udpSocket(new QUdpSocket(this))
{
    ui->setupUi(this);
    ui->chatTextEdit->setReadOnly(true);

    // Bind the socket to any available port
	if (!udpSocket->bind(0)) {	
        qFatal("Failed to bind UDP socket");
    }

    // Connect the readyRead signal to the slot to handle incoming messages
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams);
	// Connect returnPressed signal of QLineEdit to on_sendButton_clicked slot
	connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_sendButton_clicked);
	
	ui->messageLineEdit->setFocus();
	
    // Timer for handling timeout
    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, &MainWindow::handleTimeout);
	
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->messageLineEdit->text();
	QString server = ui->serverAddressLineEdit->text();
    QByteArray data = message.toUtf8();

    // Send the datagram to localhost on port 8080
	std::cout << "Send " << message.toStdString() << "\n"; 
    //udpSocket->writeDatagram(data, QHostAddress::LocalHost, 8080);
	udpSocket->writeDatagram(data, QHostAddress(server), 8080);
	
    // Start the timer for timeout (3 seconds)
    timeoutTimer->start(3000);
	
    ui->messageLineEdit->clear();
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString message = QString::fromUtf8(datagram);
        ui->chatTextEdit->append("Message from " + sender.toString() + ":" + QString::number(senderPort) + " - " + message);
		
        // If we receive a response, stop the timeout timer
        timeoutTimer->stop();
    }
}

void MainWindow::handleTimeout()
{
    ui->chatTextEdit->append("No response from server - server might be unavailable.");
}


