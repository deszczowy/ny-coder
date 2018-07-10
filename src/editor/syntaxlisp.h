/*
Copyright (c) 2018 Krystian Szklarek <szklarek@protonmail.com>
All rights reserved.
This file is part of "Nyquist Coder" project licensed under MIT License.
See LICENSE file in the project root for license information.

Nyquist Copyright (c) by Roger B. Dannenberg
Qt Framework Copyright (c) The Qt Company Ltd.
*/

#ifndef SYNTAXLISP_H
#define SYNTAXLISP_H


#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

class SyntaxLisp : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxLisp(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat lispMethodsFormat;
    QTextCharFormat nyquistMethodsFormat;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat globalFormat;

    QTextCharFormat listDelimiter;

    void ApplyLispMethodRules(HighlightingRule &rule);
    void ApplyNyquistMethodRule(HighlightingRule &rule);
    void ApplyBlockCommentRule(HighlightingRule &);
    void ApplyInlineCommentRule(HighlightingRule &rule);
    void ApplyListRule(HighlightingRule &rule);
    void ApplyQuotationRule(HighlightingRule &rule);
    void ApplyGlobalVariableRule(HighlightingRule &rule);
};

#endif // SYNTAXLISP_H
