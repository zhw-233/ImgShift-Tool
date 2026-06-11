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
    void runTrans();
    void runZip();
    void showHomePage();
    void showTransPage();
    void showZipPage();

private :
    QString transInputFile;
    QLabel *transInputFileLabel;
    QString transOutputDir;
    QLabel *transOutputDirLabel;

    QString zipInputFile;
    QLabel *zipInputFileLabel;
    QString zipOutputDir;
    QLabel *zipOutputDirLabel;

    QComboBox *formatCombo;
    
    QSpinBox *transQualitySpin;
    QSpinBox *zipQualitySpin;
    QSpinBox *levelSpin;

    QCheckBox *no_fastCheck;
    QCheckBox *transProgressiveCheck;
    QCheckBox *zipProgressiveCheck;
    QCheckBox *losslessCheck;

    QPushButton *transRunButton;
    QPushButton *zipRunButton;
    QProcess *process;

    /////   pages
    QStackedWidget *stackedwidgets;
    QWidget *homePage;
    QWidget *transPage;
    QWidget *zipPage;

    void setupHomePage();
    void setupTransPage();
    void setupZipPage();
};
