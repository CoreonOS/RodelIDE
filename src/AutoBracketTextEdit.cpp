#include "AutoBracketTextEdit.h"

#include <QTextCursor>

AutoBracketTextEdit::AutoBracketTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

static QChar closingFor(const QChar &opening)
{
    switch (opening.unicode()) {
    case '(': return ')';
    case '{': return '}';
    case '[': return ']';
    case '"': return '"';
    case '\'': return '\'';
    default: return QChar();
    }
}

void AutoBracketTextEdit::keyPressEvent(QKeyEvent *event)
{
    const auto key = event->key();
    const auto text = event->text();

    // Characters that have automatic closing counterparts
    if (text == "(" || text == "{" || text == "[" || text == "\"" || text == "'") {
        QTextEdit::keyPressEvent(event); // insert the opening char first

        QChar closeChar = closingFor(text[0]);
        if (!closeChar.isNull()) {
            QTextCursor cursor = this->textCursor();
            cursor.insertText(QString(closeChar));
            cursor.movePosition(QTextCursor::Left);
            this->setTextCursor(cursor);
        }
        return;
    }

    // If user types closing char when next char is already that closing char, just move cursor
    if (text == ")" || text == "}" || text == "]" || text == "\"" || text == "'") {
        QTextCursor cursor = this->textCursor();
        QChar nextChar = document()->characterAt(cursor.position());
        if (nextChar == text[0]) {
            cursor.movePosition(QTextCursor::Right);
            this->setTextCursor(cursor);
            return; // don't insert duplicate
        }
    }

    QTextEdit::keyPressEvent(event);
}
