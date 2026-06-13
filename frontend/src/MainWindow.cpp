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
#include<QCoreApplication>
#ifdef Q_OS_WIN
#include<windows.h>
#endif
// #include<QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("ImgShift-Tool");
    resize(600,400);

    /////   process
    process=new QProcess(this);

    #ifdef Q_OS_WIN
    process->setCreateProcessArgumentsModifier(
        [](QProcess::CreateProcessArguments *args) {
            args->flags |= CREATE_NO_WINDOW;
        }
    );
    #endif

    stackedwidgets=new QStackedWidget(this);

    setupHomePage();
    setupTransPage();
    setupZipPage();
    setupGifMakerPage();
    setupGifSpliterPage();

    stackedwidgets->addWidget(homePage);
    stackedwidgets->addWidget(transPage);
    stackedwidgets->addWidget(zipPage);
    stackedwidgets->addWidget(gifMakerPage);
    stackedwidgets->addWidget(gifSpliterPage);

    stackedwidgets->setCurrentWidget(homePage);

    connect(process,&QProcess::readyReadStandardOutput,this,[this](){
        pyOutput=QString::fromLocal8Bit(process->readAllStandardOutput());
    });
    connect(process,&QProcess::readyReadStandardError,this,[this](){
        pyOutput=QString::fromLocal8Bit(process->readAllStandardError());
    });

    connect(process,&QProcess::finished,this,[this](){
        zipRunButton->setEnabled(true);
        transRunButton->setEnabled(true);
        gifMakerRunButton->setEnabled(true);
        gifSpliterRunButton->setEnabled(true);
        if(process->exitCode()==0) {
            QMessageBox::information(
                this,
                "成功",
                pyOutput
            );
        }
        else {
            QMessageBox::warning(
                this,
                "错误",
                pyOutput
            );
        }
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

    auto *line1=new QHBoxLayout;
    auto *transBottom=new QPushButton("格式转换",homePage);
    transBottom->setFixedHeight(100);
    auto *zipBottom=new QPushButton("图片压缩",homePage);
    zipBottom->setFixedHeight(100);
    line1->addWidget(transBottom,1);
    line1->addWidget(zipBottom,1);

    auto *line2=new QHBoxLayout;
    auto *gifMakerBottom=new QPushButton("GIF 生成",homePage);
    gifMakerBottom->setFixedHeight(100);
    auto *gifSpliterBottom=new QPushButton("GIF 拆分",homePage);
    gifSpliterBottom->setFixedHeight(100);
    line2->addWidget(gifMakerBottom,1);
    line2->addWidget(gifSpliterBottom,1);

    layout->addStretch(1);
    layout->addWidget(titleLabel,1);
    layout->addLayout(line1);
    layout->addLayout(line2);
    layout->addStretch(1);

    connect(transBottom,&QPushButton::clicked,this,&MainWindow::showTransPage);
    connect(zipBottom,&QPushButton::clicked,this,&MainWindow::showZipPage);
    connect(gifMakerBottom,&QPushButton::clicked,this,&MainWindow::showGifMakerPage);
    connect(gifSpliterBottom,&QPushButton::clicked,this,&MainWindow::showGifSpliterPage);
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

void MainWindow::setupGifMakerPage() {
    gifMakerPage=new QWidget(this);
    auto *layout=new QVBoxLayout(gifMakerPage);

    /////   back
    auto *backButton=new QPushButton("返回",gifMakerPage);
    backButton->setFixedSize(60,32);

    layout->addWidget(backButton,1);

    connect(backButton,&QPushButton::clicked,this,&MainWindow::showHomePage);

    /////   inputFile
    auto inputLayout=new QHBoxLayout;

    gifMakerInputDirLabel=new QLabel("请选择输入路径",gifMakerPage);
    gifMakerInputDirLabel->setFixedHeight(32);

    auto inputChooseButton=new QPushButton("选择路径",gifMakerPage);
    inputChooseButton->setFixedSize(100,32);

    inputLayout->addWidget(inputChooseButton,1);
    inputLayout->addWidget(gifMakerInputDirLabel,1);

    layout->addLayout(inputLayout);

    connect(inputChooseButton,&QPushButton::clicked,this,&MainWindow::gifMakerChooseInputDir);

    /////   outputDir
    auto *outputLayout=new QHBoxLayout;

    gifMakerOutputDirLabel=new QLabel("请选择输出路径",gifMakerPage);
    gifMakerOutputDirLabel->setFixedHeight(32);

    auto *outputChooseButton=new QPushButton("选择路径",gifMakerPage);
    outputChooseButton->setFixedSize(100,32);

    outputLayout->addWidget(outputChooseButton,1);
    outputLayout->addWidget(gifMakerOutputDirLabel,1);

    layout->addLayout(outputLayout);

    connect(outputChooseButton,&QPushButton::clicked,this,&MainWindow::gifMakerChooseOutputDir);

    /////   duration
    auto *durationLayout=new QHBoxLayout;

    auto *durationLabel=new QLabel("每帧时间间隔，单位毫秒，默认 300",gifMakerPage);
    durationLabel->setFixedHeight(32);

    durationSpin=new QSpinBox(gifMakerPage);
    durationSpin->setFixedSize(300,32);
    durationSpin->setRange(1,10000);
    durationSpin->setValue(300);
    durationSpin->setSingleStep(1);
    durationSpin->setSuffix("ms");

    durationLayout->addWidget(durationLabel,1);
    durationLayout->addWidget(durationSpin,1);

    layout->addLayout(durationLayout);

    /////   runButton
    gifMakerRunButton=new QPushButton("运行",gifMakerPage);
    gifMakerRunButton->setFixedSize(100,32);

    layout->addWidget(gifMakerRunButton,1);

    connect(gifMakerRunButton,&QPushButton::clicked,this,&MainWindow::runGifMaker);
}

void MainWindow::setupGifSpliterPage() {
    gifSpliterPage=new QWidget(this);
    auto *layout=new QVBoxLayout(gifSpliterPage);

    /////   back
    auto *backButton=new QPushButton("返回",gifSpliterPage);
    backButton->setFixedSize(60,32);

    layout->addWidget(backButton,1);

    connect(backButton,&QPushButton::clicked,this,&MainWindow::showHomePage);

    /////   inputFile
    auto inputLayout=new QHBoxLayout;

    gifSpliterInputFileLabel=new QLabel("请选择输入路径",gifSpliterPage);
    gifSpliterInputFileLabel->setFixedHeight(32);

    auto inputChooseButton=new QPushButton("选择路径",gifSpliterPage);
    inputChooseButton->setFixedSize(100,32);

    inputLayout->addWidget(inputChooseButton,1);
    inputLayout->addWidget(gifSpliterInputFileLabel,1);

    layout->addLayout(inputLayout);

    connect(inputChooseButton,&QPushButton::clicked,this,&MainWindow::gifSpliterChooseInputFile);

    /////   outputDir
    auto *outputLayout=new QHBoxLayout;

    gifSpliterOutputDirLabel=new QLabel("请选择输出路径",gifSpliterPage);
    gifSpliterOutputDirLabel->setFixedHeight(32);

    auto *outputChooseButton=new QPushButton("选择路径",gifSpliterPage);
    outputChooseButton->setFixedSize(100,32);

    outputLayout->addWidget(outputChooseButton,1);
    outputLayout->addWidget(gifSpliterOutputDirLabel,1);

    layout->addLayout(outputLayout);

    connect(outputChooseButton,&QPushButton::clicked,this,&MainWindow::gifSpliterChooseOutputDir);

    /////   runButton
    gifSpliterRunButton=new QPushButton("运行",gifSpliterPage);
    gifSpliterRunButton->setFixedSize(100,32);

    layout->addWidget(gifSpliterRunButton,1);

    connect(gifSpliterRunButton,&QPushButton::clicked,this,&MainWindow::runGifSpliter);
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

void MainWindow::showGifMakerPage() {
    stackedwidgets->setCurrentWidget(gifMakerPage);
}

void MainWindow::showGifSpliterPage() {
    stackedwidgets->setCurrentWidget(gifSpliterPage);
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

void MainWindow::gifMakerChooseInputDir() {
    gifMakerInputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输入路径",
        QString()
    );
    if(!gifMakerInputDir.isEmpty()) {
        QDir dir(gifMakerInputDir);
        gifMakerInputDirLabel->setText(dir.dirName());
        gifMakerInputDirLabel->setToolTip(gifMakerInputDir);
    }
    else {
        gifMakerInputDirLabel->setText("请选择输入路径");
        gifMakerInputDirLabel->setToolTip(nullptr);
    }
}

void MainWindow::gifSpliterChooseInputFile() {
    gifSpliterInputFile=QFileDialog::getOpenFileName(
        this,
        "选择输入动图",
        QString()
    );
    if(!gifSpliterInputFile.isEmpty()) {
        QFileInfo fileInfo(gifSpliterInputFile);
        gifSpliterInputFileLabel->setText(fileInfo.fileName());
        gifSpliterInputFileLabel->setToolTip(gifSpliterInputFile);
    }
    else {
        gifSpliterInputFileLabel->setText("请选择输入动图");
        gifSpliterInputFileLabel->setToolTip(nullptr);
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

void MainWindow::gifMakerChooseOutputDir() {
    gifMakerOutputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输出路径",
        QString()
    );
    if(!gifMakerOutputDir.isEmpty()) {
        QDir dir(gifMakerOutputDir);
        gifMakerOutputDirLabel->setText(dir.dirName());
        gifMakerOutputDirLabel->setToolTip(gifMakerOutputDir);
    }
    else {
        gifMakerOutputDirLabel->setText("请选择输出路径");
        gifMakerOutputDirLabel->setToolTip(nullptr);
    }
}

void MainWindow::gifSpliterChooseOutputDir() {
    gifSpliterOutputDir=QFileDialog::getExistingDirectory(
        this,
        "选择输出路径",
        QString()
    );
    if(!gifSpliterOutputDir.isEmpty()) {
        QDir dir(gifSpliterOutputDir);
        gifSpliterOutputDirLabel->setText(dir.dirName());
        gifSpliterOutputDirLabel->setToolTip(gifSpliterOutputDir);
    }
    else {
        gifSpliterOutputDirLabel->setText("请选择输出路径");
        gifSpliterOutputDirLabel->setToolTip(nullptr);
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

    QString cliDir=getAppDir();
    QStringList args;
    args<<"trans";
    args<<transInputFile;
    args<<"-f"<<imgFormat;
    args<<"-o"<<transOutputDir;
    args<<"-q"<<QString::number(quality);
    if(nofast) args<<"-no-fast";
    if(progressive) args<<"-progressive";

    pyOutput.clear();
    process->start(cliDir,args);
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

    QString cliDir=getAppDir();
    QStringList args;
    args<<"zip";
    args<<zipInputFile;
    args<<"-o"<<zipOutputDir;
    args<<"-q"<<QString::number(quality);
    args<<"-level"<<QString::number(level);
    if(lossless) args<<"-lossless";
    if(progressive) args<<"-progressive";

    pyOutput.clear();
    process->start(cliDir,args);
}

void MainWindow::runGifMaker() {
    if(gifMakerInputDir.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输入路径"
        );
        return ;
    }
    if(gifMakerOutputDir.isEmpty()) {
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

    gifMakerRunButton->setEnabled(false);

    int duration=durationSpin->value();

    QString cliDir=getAppDir();
    QStringList args;
    args<<"gifmake";
    args<<gifMakerInputDir;
    args<<"-o"<<gifMakerOutputDir;
    args<<"-duration"<<QString::number(duration);

    pyOutput.clear();
    process->start(cliDir,args);
}

void MainWindow::runGifSpliter() {
    if(gifSpliterInputFile.isEmpty()) {
        QMessageBox::warning(
            this,
            "提示",
            "请选择输入路径"
        );
        return ;
    }
    if(gifSpliterOutputDir.isEmpty()) {
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

    gifSpliterRunButton->setEnabled(false);

    QString cliDir=getAppDir();
    QStringList args;
    args<<"gifsplit";
    args<<gifSpliterInputFile;
    args<<"-o"<<gifSpliterOutputDir;

    pyOutput.clear();
    process->start(cliDir,args);
}

QString MainWindow::getAppDir() {
    QDir appDir(QCoreApplication::applicationDirPath());
    #ifdef Q_OS_WIN
    return appDir.filePath("image_cli.exe");
    #else
    return appDir.filePath("image_cli");
    #endif
}
