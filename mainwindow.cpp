#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>
#include <QTextDocumentWriter>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // setWindowState(Qt::WindowMaximized);
    //setWindowOpacity(0.8);

    ui->actionBold->setCheckable(true);
    ui->actionItalic->setCheckable(true);
    ui->actionUnderline->setCheckable(true);
    
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(New()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(Open()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(Save()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->actionPrintPreview,SIGNAL(triggered()),this,SLOT(PrintPreview()));
    connect(ui->action_Export_as_PDF,SIGNAL(triggered()),this,SLOT(Export_as_PDF()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(Exit()));

    connect(ui->actionUndo,SIGNAL(triggered()),ui->textEdit,SLOT(undo()));
    connect(ui->textEdit->document(), SIGNAL(undoAvailable(bool)),ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->actionRedo,SIGNAL(triggered()),ui->textEdit,SLOT(redo()));
    connect(ui->textEdit->document(), SIGNAL(redoAvailable(bool)),ui->actionRedo, SLOT(setEnabled(bool)));
    connect(ui->actionCut,SIGNAL(triggered()),ui->textEdit,SLOT(cut()));
    connect(ui->textEdit,SIGNAL(copyAvailable(bool)),ui->actionCut,SLOT(setEnabled(bool)));
    connect(ui->actionCopy,SIGNAL(triggered()),ui->textEdit,SLOT(copy()));
    connect(ui->textEdit,SIGNAL(copyAvailable(bool)),ui->actionCopy,SLOT(setEnabled(bool)));
    connect(ui->actionPaste,SIGNAL(triggered()),ui->textEdit,SLOT(paste()));
    connect(ui->textEdit,SIGNAL(pasteAvailable(bool)),ui->actionPaste,SLOT(setEnabled(bool)));
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
    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentCharFormatChanged(QTextCharFormat)));

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

    //qDebug()<<children();
    initWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
    QFont textFont("Helvetica");
    textFont.setWeight(15);
    ui->textEdit->setFont(textFont);

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(true);
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
        QByteArray data = file.readAll();
        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);
        if (Qt::mightBeRichText(str))
        {
            ui->textEdit->setHtml(str);
        }
        else
        {
            str = QString::fromLocal8Bit(data);
            ui->textEdit->setPlainText(str);
        }
    }
    else
    {
        QMessageBox::warning(this,"Waring","Open failed");
    }
}

void MainWindow::_save()
{
    qDebug()<<curFileName;
    if(!curFileName.isEmpty())
    {
        QTextDocumentWriter writer(curFileName);
        bool ok=writer.write(ui->textEdit->document());
        if (ok)
            ui->textEdit->document()->setModified(false);
        else
        {
            QMessageBox::warning(this,"Waring","Save failed");
        }

        setWindowTitle(curFileName);
    }
}

void MainWindow::saveas()
{
    QString temp=curFileName;
    if(temp.isEmpty()) temp="???.odt";
    curFileName=QFileDialog::getSaveFileName(this, tr("Save as..."), temp,
                                              tr("ODF Files (*.odt);;HTML Files (*.htm *.html);;TXT Files (*.txt);;All Files (*)"));
    _save();
}

void MainWindow::save()
{
    if(curFileName.isEmpty())
    {
        saveas();
    }
    else
    {
        _save();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!ui->textEdit->document()->isModified())
    {
        event->accept();
    }
    else
    {
        int ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
        if (ret == QMessageBox::Save)
        {
            save();
        }
        event->accept();
    }
}

void MainWindow::New()
{
    create();
}

void MainWindow::Open()
{
    QString filename=QFileDialog::getOpenFileName(this,"Open",curFileName,
                                tr("ODF Files (*.odt);;HTML Files (*.htm *.html);;TXT Files (*.txt);;All Files (*)"));
    if(filename==curFileName) return;
    if(!filename.isEmpty()&&!QFileInfo(filename).suffix().isEmpty())
    {
        if(ui->textEdit->document()->isEmpty())
        {
            curFileName=filename;
            setWindowTitle(curFileName);
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

void MainWindow::Preview(QPrinter* printer)
{
    ui->textEdit->print(printer);
}

void MainWindow::PrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(Preview(QPrinter*)));
    preview.exec();
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


void MainWindow::mergeCharFormat(const QTextCharFormat& format)
{
    QTextCursor cursor=ui->textEdit->textCursor();
    if(!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
}

void MainWindow::setFontBold(bool checked)
{
    QTextCharFormat format;
    format.setFontWeight(checked?QFont::Bold:QFont::Normal);
    mergeCharFormat(format);
}

void MainWindow::setFontItalic(bool checked)
{
    QTextCharFormat format;
    format.setFontItalic(checked);
   mergeCharFormat(format);
}

void MainWindow::addUnderLine(bool checked)
{
    QTextCharFormat format;
    format.setFontUnderline(checked);
    mergeCharFormat(format);
}

void MainWindow::setFont()
{
    bool ok;
    QTextCursor cursor=ui->textEdit->textCursor();
    QFont font=QFontDialog::getFont(&ok,this);
    QTextCharFormat format;
    format.setFont(font);
    if(ok) mergeCharFormat(format);
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
    QColor color = QColorDialog::getColor(ui->textEdit->textColor(), this);
    if (!color.isValid())
        return;
    QTextCharFormat format;
    format.setForeground(color);
    mergeCharFormat(format);
}

void MainWindow::currentCharFormatChanged(QTextCharFormat format)
{
    QFont font=format.font();
    ui->actionBold->setChecked(font.bold());
    ui->actionItalic->setChecked(font.italic());
    ui->actionUnderline->setChecked(font.underline());
}
