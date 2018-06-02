#include "syntaxlisp.h"

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
    // style
    lispMethodsFormat.setForeground(styler.Lisp());
    lispMethodsFormat.setFontWeight(QFont::Bold);

    // words
    QStringList keywordPatterns;
    keywordPatterns
            << "\\blist\\b"
            << "\\bclass\\b"
            << "\\bdot\\b>"
    ;

    // applying rule
    foreach (const QString &pattern, keywordPatterns)
    {
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
    nyquistMethodsFormat.setForeground(styler.Nyquist());
    nyquistMethodsFormat.setFontWeight(QFont::Bold);

    // words
    QStringList keywordPatterns;
    keywordPatterns
            << "\\bplay\\b"
            << "\\bload\\b"
            << "\\bget\\b>"
    ;

    // applying rule
    foreach (const QString &pattern, keywordPatterns)
    {
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
    multiLineCommentFormat.setForeground(styler.Comment());
}

void SyntaxLisp::ApplyInlineCommentRule(HighlightingRule &rule)
{
    singleLineCommentFormat.setForeground(styler.Comment());
    rule.pattern = QRegularExpression(";[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void SyntaxLisp::ApplyListRule(HighlightingRule &rule)
{
    listDelimiter.setForeground(styler.Parenthesis());
    rule.pattern = QRegularExpression("\\(");
    rule.format = listDelimiter;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\)");
    rule.format = listDelimiter;
    highlightingRules.append(rule);
}

void SyntaxLisp::ApplyQuotationRule(HighlightingRule &rule)
{
    quotationFormat.setForeground(styler.Text());
    rule.pattern = QRegularExpression("\".+\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}
