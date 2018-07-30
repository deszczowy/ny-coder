#include "nystatusarea.h"

NyStatusArea::NyStatusArea()
{
    setFixedHeight(20);
    CreateLayout();
}

void NyStatusArea::CreateLayout()
{
    CreateLeftPanel();
    CreateRightPanel();
    CreateCenterPanel();
    ArrangeLayout();
}

void NyStatusArea::CreateLeftPanel()
{
    _leftPanel = new QLabel(this);
    _leftPanel->setAlignment(Qt::AlignLeft);
    _leftPanel->setText("askdjasdkjaksjdhkajdhkajsdhkjashd");
}

void NyStatusArea::CreateRightPanel()
{
    _rightPanel = new QLabel(this);
    _rightPanel->setAlignment(Qt::AlignRight);
    _leftPanel->setText(",xcnz,mxcnz,mxcn,zmxcn,zmxcn");
}

void NyStatusArea::CreateCenterPanel()
{
    _centerPanel = new QLabel(this);
    _centerPanel->setAlignment(Qt::AlignCenter);
    _leftPanel->setText("qowieuqowieuoqwueoqwueo");
}

void NyStatusArea::ArrangeLayout()
{
    _layout = new QHBoxLayout(this);
    _layout->addWidget(_leftPanel);
    _layout->addWidget(_centerPanel);
    _layout->addWidget(_rightPanel);
}
