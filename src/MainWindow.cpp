#include "MainWindow.h"
#include "AutoBracketTextEdit.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), editor(new AutoBracketTextEdit(this))
{
    setCentralWidget(editor);

    // Use Consolas or system monospace if unavailable
    QFont monoFont(QStringLiteral("Consolas"));
    monoFont.setStyleHint(QFont::Monospace);
    editor->setFont(monoFont);

    // Create File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = fileMenu->addAction(tr("&New"), this, &MainWindow::newFile, QKeySequence::New);
    QAction *openAct = fileMenu->addAction(tr("&Open…"), this, &MainWindow::openFile, QKeySequence::Open);
    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, &MainWindow::saveFile, QKeySequence::Save);
    QAction *saveAsAct = fileMenu->addAction(tr("Save &As…"), this, &MainWindow::saveFileAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);

    // Status bar message
    statusBar()->showMessage(tr("Ready"));

    resize(800, 600); //Size of the window
    setCurrentFile(QString());
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        editor->clear();
        setCurrentFile(QString());
    }
}



void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Text Files (*.txt *.cpp *.h *.hpp *.c *.py);;All Files (*)"));
    if (!fileName.isEmpty())
        loadFromFile(fileName);
}

void MainWindow::saveFile()
{
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        saveToFile(currentFile);
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"), currentFile.isEmpty() ? QString("untitled.txt") : currentFile);
    if (!fileName.isEmpty()) {
        saveToFile(fileName);
    }
}

bool MainWindow::maybeSave()
{
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Unsaved Changes"),
            tr("The document has been modified.\nDo you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile(), true;
        if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << editor->toPlainText();
    file.close();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }
    QTextStream in(&file);
    editor->setPlainText(in.readAll());
    file.close();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &file)
{
    currentFile = file;
    editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFile.isEmpty() ? tr("untitled.txt") : QFileInfo(currentFile).fileName();
    setWindowTitle(tr("%1[*] - RodelIDE").arg(shownName));
}
