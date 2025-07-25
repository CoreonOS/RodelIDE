#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AutoBracketTextEdit.h"
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

private:
    void setCurrentFile(const QString &file);
    bool maybeSave();
    bool saveToFile(const QString &fileName);
    bool loadFromFile(const QString &fileName);

    AutoBracketTextEdit *editor;
    QString currentFile;
};

#endif // MAINWINDOW_H
