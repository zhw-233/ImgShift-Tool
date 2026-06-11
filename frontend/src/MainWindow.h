#pragma once

#include<QMainWindow>
#include<QProcess>

class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QSpinBox;
class QCheckBox;

class MainWindow : public QMainWindow {
    Q_OBJECT

public :
    explicit MainWindow(QWidget *parent=nullptr);

private slots:
    void chooseInputFile();
    void chooseOutputDir();
    void runTrans();

private :
    QString inputFile;
    QLabel *inputFileLabel;

    QString outputDir;
    QLabel *outputDirLabel;

    QComboBox *formatCombo;
    
    QSpinBox *qualitySpin;

    QCheckBox *no_fastCheck;
    QCheckBox *progressiveCheck;

    QPushButton *runButton;

    QProcess *process;
};
