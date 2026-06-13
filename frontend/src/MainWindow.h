#pragma once

#include<QMainWindow>
#include<QProcess>

class QPushButton;
class QLabel;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QStackedWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public :
    explicit MainWindow(QWidget *parent=nullptr);

private slots:
    void transChooseInputFile();
    void transChooseOutputDir();
    void zipChooseInputFile();
    void zipChooseOutputDir();
    void gifMakerChooseInputDir();
    void gifMakerChooseOutputDir();
    void gifSpliterChooseInputFile();
    void gifSpliterChooseOutputDir();
    void runTrans();
    void runZip();
    void runGifMaker();
    void runGifSpliter();
    void showHomePage();
    void showTransPage();
    void showZipPage();
    void showGifMakerPage();
    void showGifSpliterPage();

private :
    QString transInputFile;
    QLabel *transInputFileLabel;
    QString transOutputDir;
    QLabel *transOutputDirLabel;

    QString zipInputFile;
    QLabel *zipInputFileLabel;
    QString zipOutputDir;
    QLabel *zipOutputDirLabel;

    QString gifMakerInputDir;
    QLabel *gifMakerInputDirLabel;
    QString gifMakerOutputDir;
    QLabel *gifMakerOutputDirLabel;

    QString gifSpliterInputFile;
    QLabel *gifSpliterInputFileLabel;
    QString gifSpliterOutputDir;
    QLabel *gifSpliterOutputDirLabel;

    QComboBox *formatCombo;
    
    QSpinBox *transQualitySpin;
    QSpinBox *zipQualitySpin;
    QSpinBox *levelSpin;
    QSpinBox *durationSpin;

    QCheckBox *no_fastCheck;
    QCheckBox *transProgressiveCheck;
    QCheckBox *zipProgressiveCheck;
    QCheckBox *losslessCheck;

    QPushButton *transRunButton;
    QPushButton *zipRunButton;
    QPushButton *gifMakerRunButton;
    QPushButton *gifSpliterRunButton;

    QProcess *process;
    QString pyOutput;

    /////   pages
    QStackedWidget *stackedwidgets;
    QWidget *homePage;
    QWidget *transPage;
    QWidget *zipPage;
    QWidget *gifMakerPage;
    QWidget *gifSpliterPage;

    QString getAppDir();

    void setupHomePage();
    void setupTransPage();
    void setupZipPage();
    void setupGifMakerPage();
    void setupGifSpliterPage();
};
