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

    QTextCharFormat listDelimiter;

    void ApplyLispMethodRules(HighlightingRule &rule);
    void ApplyNyquistMethodRule(HighlightingRule &rule);
    void ApplyBlockCommentRule(HighlightingRule &rule);
    void ApplyInlineCommentRule(HighlightingRule &rule);
    void ApplyListRule(HighlightingRule &rule);
    void ApplyQuotationRule(HighlightingRule &rule);
};

#endif // SYNTAXLISP_H
