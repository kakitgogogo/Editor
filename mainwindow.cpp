#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    ui->actionBold->setCheckable(true);
    ui->actionItalic->setCheckable(true);
    ui->actionUnderline->setCheckable(true);
    
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(New()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(Open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(Save()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->action_Export_as_PDF,SIGNAL(triggered()),this,SLOT(Export_as_PDF()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(Exit()));

    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(Undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(Redo()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(Cut()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(Copy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(Paste()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(Find()));

    connect(ui->actionImage,SIGNAL(triggered()),this,SLOT(insertImage()));
    connect(ui->actionTable,SIGNAL(triggered()),this,SLOT(insertTable()));
    connect(ui->actionList,SIGNAL(triggered()),this,SLOT(insertList()));

    connect(ui->actionBold,SIGNAL(toggled(bool)),this,SLOT(setFontBold(bool)));
    connect(ui->actionItalic,SIGNAL(toggled(bool)),this,SLOT(setFontItalic(bool)));
    connect(ui->actionUnderline,SIGNAL(toggled(bool)),this,SLOT(addUnderLine(bool)));
    connect(ui->actionFont,SIGNAL(triggered()),this,SLOT(setFont()));
    connect(ui->actionLeft,SIGNAL(triggered()),this,SLOT(alignLeft()));
    connect(ui->actionCenter,SIGNAL(triggered()),this,SLOT(alignCenter()));
    connect(ui->actionRight,SIGNAL(triggered()),this,SLOT(alignRight()));
    connect(ui->actionJustify,SIGNAL(triggered()),this,SLOT(alignJustify()));
    connect(ui->actionColor,SIGNAL(triggered()),this,SLOT(setFontColor()));

    ui->mainToolBar->addAction(ui->actionFont);
    ui->mainToolBar->addAction(ui->actionColor);
    ui->mainToolBar->addAction(ui->actionBold);
    ui->mainToolBar->addAction(ui->actionItalic);
    ui->mainToolBar->addAction(ui->actionUnderline);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionLeft);
    ui->mainToolBar->addAction(ui->actionCenter);
    ui->mainToolBar->addAction(ui->actionRight);
    ui->mainToolBar->addAction(ui->actionJustify);
    ui->mainToolBar->addSeparator();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setPath(const QString &filename)
{
    curFileName=filename;
    curDir=QFileInfo(curFileName).absoluteDir().dirName();
}

MainWindow* MainWindow::create()
{
    MainWindow *newWindow=new MainWindow;
    newWindow->show();
    return newWindow;
}

void MainWindow::load(const QString& filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&file);
        while(!textStream.atEnd())
        {
            ui->textEdit->append(textStream.readLine());
        }
        textStream.flush();
        file.close();

        setPath(filename);
        setWindowTitle(curFileName);
    }
}

void MainWindow::_save(const QString& filename)
{
    if(!filename.isEmpty()&&!QFileInfo(filename).suffix().isEmpty())
    {
        QFile file(filename);
        //qDebug()<<"filename: "<<filename;
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this,"Error","Can't open file.");
        }
        QTextStream textStream(&file);
        textStream<<ui->textEdit->toPlainText();
        textStream.flush();
        file.close();

        setPath(filename);
        setWindowTitle(curFileName);
    }
}

void MainWindow::saveas()
{
    if(curFileName.isEmpty()) curFileName="???.txt";
    QString filename=QFileDialog::getSaveFileName(this,"Save",curFileName," *.txt;; *.docx");
    _save(filename);
}

void MainWindow::save()
{
    if(curFileName.isEmpty())
    {
        saveas();
    }
    else
    {
        _save(curFileName);
    }
}

void MainWindow::_close()
{
    int resp=QMessageBox::question(this,"Question","Save before closing?");
    if(resp==QMessageBox::Yes)
    {
        save();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int resp=QMessageBox::question(this,"Question","Are you sure?");
    if(resp==QMessageBox::Yes)
    {
        _close();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::New()
{
    create();
}

void MainWindow::Open()
{
    QString filename=QFileDialog::getOpenFileName(this,"Open",curFileName,"*.txt;;*.docx");
    if(!filename.isEmpty()&&!QFileInfo(filename).suffix().isEmpty())
    {
        if(ui->textEdit||ui->textEdit->document()->isEmpty())
        {
            curFileName=filename;
            load(filename);
        }
        else
        {
            MainWindow* newWindow=create();
            newWindow->load(filename);
        }
    }
}

void MainWindow::Save()
{
    save();
}

void MainWindow::SaveAs()
{
    saveas();
}

void MainWindow::Export_as_PDF()
{
    QString filename=QFileDialog::getSaveFileName(this,"Export as PDF",curDir+"/???.pdf","*.pdf");
    if(!filename.isEmpty())
    {
        if(QFileInfo(filename).suffix().isEmpty())
        {
            filename.append(".pdf");
        }
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        ui->textEdit->print(&printer);
    }
}

void MainWindow::Exit()
{

}


void MainWindow::Undo()
{
    ui->textEdit->undo();
}

void MainWindow::Redo()
{
    ui->textEdit->redo();
}

void MainWindow::Cut()
{

}

void MainWindow::Copy()
{

}

void MainWindow::Paste()
{

}

void MainWindow::Find()
{

}


void MainWindow::insertImage()
{
    QTextImageFormat format;
    QString filename=QFileDialog::getOpenFileName(this,"Choose",curFileName,"*.png;;*.jpg");
    if(!filename.isEmpty())
    {
        format.setName(filename);
    }
    ui->textEdit->textCursor().insertImage(format);
}

void MainWindow::insertTable()
{
    QTextTableFormat format;
    format.setCellSpacing(2);
    format.setCellPadding(10);
    ui->textEdit->textCursor().insertTable(2,2,format);
}

void MainWindow::insertList()
{
    QTextListFormat format;
    format.setStyle(QTextListFormat::ListDecimal);
    ui->textEdit->textCursor().insertList(format);
}

void MainWindow::setFontBold(bool checked)
{
    QTextCharFormat format;
    format.setFontWeight(checked?QFont::Bold:QFont::Normal);
    ui->textEdit->textCursor().mergeCharFormat(format);
}

void MainWindow::setFontItalic(bool checked)
{
    QTextCharFormat format;
    format.setFontItalic(checked);
    ui->textEdit->textCursor().mergeCharFormat(format);
}

void MainWindow::addUnderLine(bool checked)
{
    QTextCharFormat format;
    format.setFontUnderline(checked);
    ui->textEdit->textCursor().mergeCharFormat(format);
}

void MainWindow::setFont()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,this);
    QTextCharFormat format;
    format.setFont(font);
    if(ok) ui->textEdit->textCursor().setCharFormat(format);
}

void MainWindow::alignLeft()
{
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignLeft);
    ui->textEdit->textCursor().setBlockFormat(blockFormat);
}

void MainWindow::alignCenter()
{
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);
    ui->textEdit->textCursor().mergeBlockFormat(blockFormat);
}

void MainWindow::alignRight()
{
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignRight);
    ui->textEdit->textCursor().setBlockFormat(blockFormat);
}

void MainWindow::alignJustify()
{
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignJustify);
    ui->textEdit->textCursor().setBlockFormat(blockFormat);
}

void MainWindow::setFontColor()
{
    QColorDialog dialog(Qt::black,this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    dialog.exec();
    QColor color=dialog.currentColor();
    QTextCharFormat format;
    format.setForeground(color);
    ui->textEdit->textCursor().mergeCharFormat(format);
}
