#ifndef AUTOBRACKETTEXTEDIT_H
#define AUTOBRACKETTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>

class AutoBracketTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit AutoBracketTextEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // AUTOBRACKETTEXTEDIT_H
