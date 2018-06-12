#include "syntaxlisp.h"
#include "storage.h"

#include <QDebug>

SyntaxLisp::SyntaxLisp(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    ApplyLispMethodRules(rule);
    ApplyNyquistMethodRule(rule);
    ApplyBlockCommentRule(rule);
    ApplyInlineCommentRule(rule);
    ApplyListRule(rule);
    ApplyQuotationRule(rule);
    ApplyGlobalVariableRule(rule);
}


void SyntaxLisp::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);


        while (startIndex >= 0) {
                QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
                int endIndex = match.capturedStart();
                int commentLength = 0;
                if (endIndex == -1) {
                    setCurrentBlockState(1);
                    commentLength = text.length() - startIndex;
                } else {
                    commentLength = endIndex - startIndex
                                    + match.capturedLength();
                }
                setFormat(startIndex, commentLength, multiLineCommentFormat);
                startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
}

void SyntaxLisp::ApplyLispMethodRules(HighlightingRule &rule)
{
    //
    // style
    lispMethodsFormat.setForeground(QColor(Storage::getInstance().themeValue("lisp")));
    lispMethodsFormat.setFontWeight(QFont::Bold);

    //QStringList words = (QStringList);
    // applying rule
    foreach (const QString &pattern, Storage::getInstance().lispWordsRegEx())
    {
        qDebug() << pattern;
        rule.pattern = QRegularExpression(
            pattern,
            QRegularExpression::CaseInsensitiveOption
        );
        rule.format = lispMethodsFormat;
        highlightingRules.append(rule);
    }
}

void SyntaxLisp::ApplyNyquistMethodRule(HighlightingRule &rule)
{
    // style
    nyquistMethodsFormat.setForeground(QColor(Storage::getInstance().themeValue("nyquist")));
    nyquistMethodsFormat.setFontWeight(QFont::Bold);

    // select '<< "\\(' || help || '\\b"' from nldata where source = 'N' order by word desc;

    QStringList words(Storage::getInstance().nyquistWordsRegEx());
    // applying rule
    foreach (const QString &pattern, words)
    {
        qDebug() << pattern;
        rule.pattern = QRegularExpression(
            pattern,
            QRegularExpression::CaseInsensitiveOption
        );
        rule.format = nyquistMethodsFormat;
        highlightingRules.append(rule);
    }
}

void SyntaxLisp::ApplyBlockCommentRule(HighlightingRule &rule)
{
    commentStartExpression = QRegularExpression("#\\|");
    commentEndExpression = QRegularExpression("\\|#");
    multiLineCommentFormat.setForeground(QColor(Storage::getInstance().themeValue("comment")));
}

void SyntaxLisp::ApplyInlineCommentRule(HighlightingRule &rule)
{
    singleLineCommentFormat.setForeground(QColor(Storage::getInstance().themeValue("comment")));
    rule.pattern = QRegularExpression(";[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void SyntaxLisp::ApplyListRule(HighlightingRule &rule)
{
    listDelimiter.setForeground(QColor(Storage::getInstance().themeValue("parenthesis")));
    rule.pattern = QRegularExpression("\\(");
    rule.format = listDelimiter;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\)");
    rule.format = listDelimiter;
    highlightingRules.append(rule);
}

void SyntaxLisp::ApplyQuotationRule(HighlightingRule &rule)
{
    quotationFormat.setForeground(QColor(Storage::getInstance().themeValue("string")));
    rule.pattern = QRegularExpression("\".+?\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void SyntaxLisp::ApplyGlobalVariableRule(SyntaxLisp::HighlightingRule &rule)
{
    globalFormat.setForeground(QColor(Storage::getInstance().themeValue("global")));
    rule.pattern = QRegularExpression("\\*.+?\\*");
    rule.format = globalFormat;
    highlightingRules.append(rule);
}
