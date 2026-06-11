#include "MainWindow.h"

#include<QDir>
#include<QLabel>
#include<QWidget>
#include<QFileDialog>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QPushButton>
#include<QComboBox>
#include<QSpinBox>
#include<QCheckBox>
#include<QMessageBox>
#include<QStackedWidget>
// #include<QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("ImgShift-Tool");
    resize(600,400);

    /////   process
    process=new QProcess(this);

    stackedwidgets=new QStackedWidget(this);

    setupHomePage();
    setupTransPage();
    setupZipPage();

    stackedwidgets->addWidget(homePage);
    stackedwidgets->addWidget(transPage);
    stackedwidgets->addWidget(zipPage);

    stackedwidgets->setCurrentWidget(homePage);

    connect(process,&QProcess::finished,this,[this](){
        zipRunButton->setEnabled(true);
        transRunButton->setEnabled(true);
    });

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

    /////   show
    setCentralWidget(stackedwidgets);
};

void MainWindow::setupHomePage() {
    homePage=new QWidget(this);
    auto *layout=new QVBoxLayout(homePage);

    auto *titleLabel=new QLabel("请选择模式",homePage);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(40);
    auto *transBottom=new QPushButton("格式转换",homePage);
    transBottom->setFixedHeight(100);
    auto *zipBottom=new QPushButton("图片压缩",homePage);
    zipBottom->setFixedHeight(100);

    layout->addStretch(1);
    layout->addWidget(titleLabel,1);
    layout->addWidget(transBottom,1);
    layout->addWidget(zipBottom,1);
    layout->addStretch(1);

    connect(transBottom,&QPushButton::clicked,this,&MainWindow::showTransPage);
    connect(zipBottom,&QPushButton::clicked,this,&MainWindow::showZipPage);
}

void MainWindow::setupTransPage() {
    transPage=new QWidget(this);
    auto *layout=new QVBoxLayout(transPage);

    /////   back
    auto *backButton=new QPushButton("返回",transPage);
    backButton->setFixedSize(60,32);

    layout->addWidget(backButton,1);

    connect(backButton,&QPushButton::clicked,this,&MainWindow::showHomePage);

    /////   inputFile
    auto *inputLayout=new QHBoxLayout;

    transInputFileLabel=new QLabel("请选择输入图片",transPage);
    transInputFileLabel->setFixedHeight(32);

    auto *inputChooseButton=new QPushButton("选择图片",transPage);
    inputChooseButton->setFixedSize(100,32);

    inputLayout->addWidget(inputChooseButton,1);
    inputLayout->addWidget(transInputFileLabel,1);

    layout->addLayout(inputLayout);

    connect(inputChooseButton,&QPushButton::clicked,this,&MainWindow::transChooseInputFile);

    /////   outputDir
    auto *outputLayout=new QHBoxLayout;

    transOutputDirLabel=new QLabel("请选择输出路径",transPage);
    transOutputDirLabel->setFixedHeight(32);

    auto *outputChooseButton=new QPushButton("选择路径",transPage);
    outputChooseButton->setFixedSize(100,32);

    outputLayout->addWidget(outputChooseButton,1);
    outputLayout->addWidget(transOutputDirLabel,1);

    layout->addLayout(outputLayout);

    connect(outputChooseButton,&QPushButton::clicked,this,&MainWindow::transChooseOutputDir);

    /////   format
    auto *formatComboLayout=new QHBoxLayout;

    auto *formatComboLabel=new QLabel("目标格式",transPage);
    formatComboLabel->setFixedHeight(32);

    formatCombo=new QComboBox(transPage);
    formatCombo->setFixedSize(300,32);
    formatCombo->addItem("JPG / JPEG","JPEG");
    formatCombo->addItem("PNG","PNG");
    formatCombo->addItem("WEBP","WEBP");

    formatComboLayout->addWidget(formatComboLabel,1);
    formatComboLayout->addWidget(formatCombo,1);

    layout->addLayout(formatComboLayout);
    
    /////   quality
    auto *qualitySpinLayout=new QHBoxLayout;

    auto *qualitySpinLabel=new QLabel("图片质量",transPage);
    qualitySpinLabel->setFixedHeight(32);

    transQualitySpin=new QSpinBox(transPage);
    transQualitySpin->setFixedSize(300,32);
    transQualitySpin->setRange(0,100);
    transQualitySpin->setValue(95);
    transQualitySpin->setSingleStep(5);
    transQualitySpin->setSuffix("%");

    qualitySpinLayout->addWidget(qualitySpinLabel,1);
    qualitySpinLayout->addWidget(transQualitySpin,1);

    layout->addLayout(qualitySpinLayout);

    /////   no_fast
    auto *no_fastLayout=new QHBoxLayout;

    auto *no_fastLabel=new QLabel("关闭快速模式",transPage);
    no_fastLabel->setFixedHeight(32);

    no_fastCheck=new QCheckBox(transPage);

    no_fastLayout->addWidget(no_fastCheck,1);
    no_fastLayout->addWidget(no_fastLabel,1);

    layout->addLayout(no_fastLayout);

    /////   progressive
    auto *progressiveLayout=new QHBoxLayout;

    auto *progressiveLabel=new QLabel("生成渐进式图片（ JPG / JPEG ）",transPage);
    progressiveLabel->setFixedHeight(32);

    transProgressiveCheck=new QCheckBox(transPage);
    
    progressiveLayout->addWidget(transProgressiveCheck,1);
    progressiveLayout->addWidget(progressiveLabel,1);

    layout->addLayout(progressiveLayout);

    /////   runButton
    transRunButton=new QPushButton("运行",transPage);
    transRunButton->setFixedSize(100,32);

    layout->addWidget(transRunButton,1);

    connect(transRunButton,&QPushButton::clicked,this,&MainWindow::runTrans);
}

void MainWindow::setupZipPage() {
    zipPage=new QWidget(this);
    auto *layout=new QVBoxLayout(zipPage);

    /////   back
    auto *backButton=new QPushButton("返回",zipPage);
    backButton->setFixedSize(60,32);

    layout->addWidget(backButton,1);

    connect(backButton,&QPushButton::clicked,this,&MainWindow::showHomePage);

    /////   inputFile
    auto *inputLayout=new QHBoxLayout;

    zipInputFileLabel=new QLabel("请选择输入图片",zipPage);
    zipInputFileLabel->setFixedHeight(32);

    auto *inputChooseButton=new QPushButton("选择图片",zipPage);
    inputChooseButton->setFixedSize(100,32);

    inputLayout->addWidget(inputChooseButton,1);
    inputLayout->addWidget(zipInputFileLabel,1);

    layout->addLayout(inputLayout);

    connect(inputChooseButton,&QPushButton::clicked,this,&MainWindow::zipChooseInputFile);

    /////   outputDir
    auto *outputLayout=new QHBoxLayout;

    zipOutputDirLabel=new QLabel("请选择输出路径",zipPage);
    zipOutputDirLabel->setFixedHeight(32);

    auto *outputChooseButton=new QPushButton("选择路径",zipPage);
    outputChooseButton->setFixedSize(100,32);

    outputLayout->addWidget(outputChooseButton,1);
    outputLayout->addWidget(zipOutputDirLabel,1);

    layout->addLayout(outputLayout);

    connect(outputChooseButton,&QPushButton::clicked,this,&MainWindow::zipChooseOutputDir);
    
    /////   quality
    auto *qualitySpinLayout=new QHBoxLayout;

    auto *qualitySpinLabel=new QLabel("图片质量（ 仅对 JPG / JPEG / WEBP 生效 ）",zipPage);
    qualitySpinLabel->setFixedHeight(32);

    zipQualitySpin=new QSpinBox(zipPage);
    zipQualitySpin->setFixedSize(300,32);
    zipQualitySpin->setRange(0,100);
    zipQualitySpin->setValue(95);
    zipQualitySpin->setSingleStep(5);
    zipQualitySpin->setSuffix("%");

    qualitySpinLayout->addWidget(qualitySpinLabel,1);
    qualitySpinLayout->addWidget(zipQualitySpin,1);

    layout->addLayout(qualitySpinLayout);

    /////   level
    auto *levelLayout=new QHBoxLayout;

    auto *levelLabel=new QLabel("压缩等级（ 仅对 PNG 生效 ）",zipPage);
    levelLabel->setFixedHeight(32);

    levelSpin=new QSpinBox(zipPage);
    levelSpin->setFixedSize(300,32);
    levelSpin->setRange(0,9);
    levelSpin->setValue(3);
    levelSpin->setSingleStep(1);
    levelSpin->setSuffix("级");

    levelLayout->addWidget(levelLabel,1);
    levelLayout->addWidget(levelSpin,1);

    layout->addLayout(levelLayout);

    ///// lossless
    auto losslessLayout=new QHBoxLayout;

    auto *losslessLabel=new QLabel("启用无损压缩（ 仅对 WEBP 生效 ）",zipPage);
    losslessLabel->setFixedHeight(32);

    losslessCheck=new QCheckBox(zipPage);

    losslessLayout->addWidget(losslessCheck,1);
    losslessLayout->addWidget(losslessLabel,1);

    layout->addLayout(losslessLayout);

    /////   progressive
    auto *progressiveLayout=new QHBoxLayout;

    auto *progressiveLabel=new QLabel("生成渐进式图片（ JPG / JPEG ）",zipPage);
    progressiveLabel->setFixedHeight(32);

    zipProgressiveCheck=new QCheckBox(zipPage);
    
    progressiveLayout->addWidget(zipProgressiveCheck,1);
    progressiveLayout->addWidget(progressiveLabel,1);

    layout->addLayout(progressiveLayout);

    /////   runButton
    zipRunButton=new QPushButton("运行",zipPage);
    zipRunButton->setFixedSize(100,32);

    layout->addWidget(zipRunButton,1);

    connect(zipRunButton,&QPushButton::clicked,this,&MainWindow::runZip);
}

void MainWindow::showHomePage() {
    stackedwidgets->setCurrentWidget(homePage);
}

void MainWindow::showTransPage() {
    stackedwidgets->setCurrentWidget(transPage);
}

void MainWindow::showZipPage() {
    stackedwidgets->setCurrentWidget(zipPage);
}

void MainWindow::transChooseInputFile() {
    transInputFile=QFileDialog::getOpenFileName(
        this,
        "选择输入图片",
        QString(),
        "Images(*.jpg *.jpeg *.png *.webp)"
    );
    if(!transInputFile.isEmpty()) {
        QFileInfo fileInfo(transInputFile);
        transInputFileLabel->setText(fileInfo.fileName());
        transInputFileLabel->setToolTip(transInputFile);
    }
    else {
        transInputFileLabel->setText("请选择输入图片");
        transInputFileLabel->setToolTip(nullptr);
    }
}

void MainWindow::zipChooseInputFile() {
    zipInputFile=QFileDialog::getOpenFileName(
        this,
        "选择输入图片",
        QString(),
        "Images(*.jpg *.jpeg *.png *.webp)"
    );
    if(!zipInputFile.isEmpty()) {
        QFileInfo fileInfo(zipInputFile);
        zipInputFileLabel->setText(fileInfo.fileName());
        zipInputFileLabel->setToolTip(zipInputFile);
    }
    else {
        zipInputFileLabel->setText("请选择输入图片");
        zipInputFileLabel->setToolTip(nullptr);
    }
}

void MainWindow::transChooseOutputDir() {
    transOutputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输出路径",
        QString()
    );
    if(!transOutputDir.isEmpty()) {
        QDir dir(transOutputDir);
        transOutputDirLabel->setText(dir.dirName());
        transOutputDirLabel->setToolTip(transOutputDir);
    }
    else {
        transOutputDirLabel->setText("请选择输出路径");
        transOutputDirLabel->setToolTip(nullptr);
    }
}

void MainWindow::zipChooseOutputDir() {
    zipOutputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输出路径",
        QString()
    );
    if(!zipOutputDir.isEmpty()) {
        QDir dir(zipOutputDir);
        zipOutputDirLabel->setText(dir.dirName());
        zipOutputDirLabel->setToolTip(zipOutputDir);
    }
    else {
        zipOutputDirLabel->setText("请选择输出路径");
        zipOutputDirLabel->setToolTip(nullptr);
    }
}

void MainWindow::runTrans() {
    if(transInputFile.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输入文件"
        );
        // qDebug("请选择输入文件");
        return ;
    }
    if(transOutputDir.isEmpty()) {
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

    transRunButton->setEnabled(false);

    QString imgFormat=formatCombo->currentData().toString();
    int quality=transQualitySpin->value();
    bool nofast=no_fastCheck->isChecked();
    bool progressive=transProgressiveCheck->isChecked();

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
    args<<transInputFile;
    args<<"-f"<<imgFormat;
    args<<"-o"<<transOutputDir;
    args<<"-q"<<QString::number(quality);
    if(nofast) args<<"-no-fast";
    if(progressive) args<<"-progressive";

    process->start(language,args);
}

void MainWindow::runZip() {
    if(zipInputFile.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输入文件"
        );
        return ;
    }
    if(zipOutputDir.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输出路径"
        );
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

    zipRunButton->setEnabled(false);
    
    int quality=zipQualitySpin->value();
    int level=levelSpin->value();
    bool lossless=losslessCheck->isChecked();
    bool progressive=zipProgressiveCheck->isChecked();

    QString language;
    QStringList args;

    #ifdef Q_OS_WIN
    language="py";
    args<<"-3";
    #else
    language="python3";
    #endif

    args<<"backend/image_cli.py";
    args<<"zip";
    args<<zipInputFile;
    args<<"-o"<<zipOutputDir;
    args<<"-q"<<QString::number(quality);
    args<<"-level"<<QString::number(level);
    if(lossless) args<<"-lossless";
    if(progressive) args<<"-progressive";

    process->start(language,args);
}
