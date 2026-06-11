#include "MainWindow.h"

#include<QDir>
#include<QLabel>
#include<QWidget>
#include<QFileDialog>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include<QComboBox>
#include<QSpinBox>
#include<QCheckBox>
#include<QMessageBox>
// #include<QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("ImgShift-Tool");
    resize(600,400);

    auto *central=new QWidget(this);
    auto *layout=new QVBoxLayout(central);

    /////   inputFile
    auto *inputLayout=new QHBoxLayout;

    inputFileLabel=new QLabel("请选择输入图片",central);
    inputFileLabel->setFixedHeight(32);

    auto *inputChooseButton=new QPushButton("选择图片",central);
    inputChooseButton->setFixedSize(100,32);

    inputLayout->addWidget(inputChooseButton,1);
    inputLayout->addWidget(inputFileLabel,1);

    layout->addLayout(inputLayout);

    connect(inputChooseButton,&QPushButton::clicked,this,&MainWindow::chooseInputFile);

    /////   outputDir
    auto *outputLayout=new QHBoxLayout;

    outputDirLabel=new QLabel("请选择输出路径",central);
    outputDirLabel->setFixedHeight(32);

    auto *outputChooseButton=new QPushButton("选择路径",central);
    outputChooseButton->setFixedSize(100,32);

    outputLayout->addWidget(outputChooseButton,1);
    outputLayout->addWidget(outputDirLabel,1);

    layout->addLayout(outputLayout);

    connect(outputChooseButton,&QPushButton::clicked,this,&MainWindow::chooseOutputDir);

    /////   format
    auto *formatComboLayout=new QHBoxLayout;

    auto *formatComboLabel=new QLabel("目标格式",central);
    formatComboLabel->setFixedHeight(32);

    formatCombo=new QComboBox(central);
    formatCombo->setFixedSize(300,32);
    formatCombo->addItem("JPG / JPEG","JPEG");
    formatCombo->addItem("PNG","PNG");
    formatCombo->addItem("WEBP","WEBP");

    formatComboLayout->addWidget(formatComboLabel,1);
    formatComboLayout->addWidget(formatCombo,1);

    layout->addLayout(formatComboLayout);
    
    /////   quality
    auto *qualitySpinLayout=new QHBoxLayout;

    auto *qualitySpinLabel=new QLabel("图片质量",central);
    qualitySpinLabel->setFixedHeight(32);

    qualitySpin=new QSpinBox(central);
    qualitySpin->setFixedSize(300,32);
    qualitySpin->setRange(0,100);
    qualitySpin->setValue(95);
    qualitySpin->setSingleStep(5);
    qualitySpin->setSuffix("%");

    qualitySpinLayout->addWidget(qualitySpinLabel,1);
    qualitySpinLayout->addWidget(qualitySpin,1);

    layout->addLayout(qualitySpinLayout);

    /////   no_fast
    auto *no_fastLayout=new QHBoxLayout;

    auto *no_fastLabel=new QLabel("关闭快速模式",central);
    no_fastLabel->setFixedHeight(32);

    no_fastCheck=new QCheckBox(central);

    no_fastLayout->addWidget(no_fastCheck,1);
    no_fastLayout->addWidget(no_fastLabel,1);

    layout->addLayout(no_fastLayout);

    /////   progressive
    auto *progressiveLayout=new QHBoxLayout;

    auto *progressiveLabel=new QLabel("生成渐进式图片（ JPG / JPEG ）",central);
    progressiveLabel->setFixedHeight(32);

    progressiveCheck=new QCheckBox(central);
    
    progressiveLayout->addWidget(progressiveCheck,1);
    progressiveLayout->addWidget(progressiveLabel,1);

    layout->addLayout(progressiveLayout);

    /////   runButton
    runButton=new QPushButton("运行",central);
    runButton->setFixedSize(100,32);

    layout->addWidget(runButton,1);

    connect(runButton,&QPushButton::clicked,this,&MainWindow::runTrans);

    /////   process
    process=new QProcess(this);

    connect(process,&QProcess::readyReadStandardOutput,this,[this](){
        QString pyOutput=QString::fromLocal8Bit(process->readAllStandardOutput());
        QMessageBox::information(
            this,
            "成功",
            pyOutput
        );
    });
    connect(process,&QProcess::readyReadStandardError,this,[this](){
        QString pyOutput=QString::fromLocal8Bit(process->readAllStandardError());
        QMessageBox::warning(
            this,
            "错误",
            pyOutput
        );
    });
    connect(process,&QProcess::finished,this,[this](){
        runButton->setEnabled(true);
    });

    /////   show
    setCentralWidget(central);
};

void MainWindow::chooseInputFile() {
    inputFile=QFileDialog::getOpenFileName(
        this,
        "选择输入图片",
        QString(),
        "Images(*.jpg *.jpeg *.png *.webp)"
    );
    if(!inputFile.isEmpty()) {
        QFileInfo fileInfo(inputFile);
        inputFileLabel->setText(fileInfo.fileName());
        inputFileLabel->setToolTip(inputFile);
    }
    else {
        inputFileLabel->setText("请选择输入图片");
        inputFileLabel->setToolTip(nullptr);
    }
}

void MainWindow::chooseOutputDir() {
    outputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输出路径",
        QString()
    );
    if(!outputDir.isEmpty()) {
        QDir dir(outputDir);
        outputDirLabel->setText(dir.dirName());
        outputDirLabel->setToolTip(outputDir);
    }
    else {
        outputDirLabel->setText("请选择输出路径");
        outputDirLabel->setToolTip(nullptr);
    }
}

void MainWindow::runTrans() {
    if(inputFile.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输入文件"
        );
        // qDebug("请选择输入文件");
        return ;
    }
    if(outputDir.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输出路径"
        );
        // qDebug("请选择输出路径");
        return ;
    }
    if(process->state()!=QProcess::NotRunning) {
        QMessageBox::warning(
            this,
            "提示",
            "当前已有任务正在运行"
        );
        return ;
    }

    runButton->setEnabled(false);

    QString imgFormat=formatCombo->currentData().toString();
    int quality=qualitySpin->value();
    bool nofast=no_fastCheck->isChecked();
    bool progress=progressiveCheck->isChecked();

    QString language;
    QStringList args;

    #ifdef Q_OS_WIN
    language="py";
    args<<"-3";
    #else
    language="python3";
    #endif

    args<<"backend/image_cli.py";
    args<<"trans";
    args<<inputFile;
    args<<"-f"<<imgFormat;
    args<<"-o"<<outputDir;
    args<<"-q"<<QString::number(quality);
    if(nofast) args<<"-no-fast";
    if(progress) args<<"-progressive";

    process->start(language,args);
}
