/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunwidget.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QFileSystemModel>
#include "butianyunfilesystemmodel.h"

ButianyunWidget::ButianyunWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout();
    setLayout(main_layout);

    ButianyunFileSystemModel* model = new ButianyunFileSystemModel(this);
    model->setRootPath(R"(d:\butianyun)");
    QTreeView* tree = new QTreeView();
    tree->setModel(model);
    tree->setRootIndex(model->index(model->rootPath()));
    main_layout->addWidget(tree);

    QPushButton* reset_button = new QPushButton();
    reset_button->setText(tr("Reset"));
    reset_button->setFixedSize(100, 40);
    connect(reset_button, &QPushButton::clicked, model, &ButianyunFileSystemModel::clearCache);
    main_layout->addWidget(reset_button, 0, Qt::AlignHCenter);
}

ButianyunWidget::~ButianyunWidget()
{
}
