//
// Created by bigfoot on 2023/7/24.
//

#include<QPushButton>
#include<QMenuBar>
#include<QFileDialog>
#include"log.h"
#include "language.h"

static Log*log_instance=nullptr;

Log*LogGetDefaultInstance(){
	return log_instance;
}

void Log::OnMenuSaveClick(){
	QString reason;
	QFileDialog file;
	if(!edit)return;
	file.setAcceptMode(QFileDialog::AcceptSave);
	file.setNameFilter(_(
		"Log File (*.log);;"
		"Text File (*.txt);;"
		"Any File (*);;"
	));
	if(!file.exec())return;
	auto files=file.selectedFiles();
	if(files.length()!=1)return;
	QFile f=QFile(files.at(0));
	if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
		reason=QString(_("Open file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	if(f.write(edit->toPlainText().toUtf8())==-1){
		reason=QString(_("Write file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	f.close();
	return;
	fail:
	ShowError(QString(_("Save log failed: %1")).arg(reason));
	f.close();
}

void Log::OnBoxLogBtnClick(){
	show();
}

void Log::Retranslate(){
	setWindowTitle(_("Log Viewer"));
        menu_save->setText(_("Save"));
}

void Log::SetupGUIWidgets(){
	hl=new QHBoxLayout(this);
	edit=new QTextEdit(this);
	edit->setReadOnly(true);
	hl->addWidget(edit);
	hl->setContentsMargins(0,0,0,0);
}

void Log::SetupGUIMenus(){
        auto*menubar=new QMenuBar(this);

        menu_save=new QAction(menubar);
	connect(menu_save,SIGNAL(triggered()),this,SLOT(OnMenuSaveClick()));
	menubar->addAction(menu_save);

        hl->setMenuBar(menubar);
}

void Log::SetupGUI(){
	Qt::WindowFlags flags=windowFlags();
	flags|=Qt::Window;
	flags|=Qt::CustomizeWindowHint;
	flags|=Qt::WindowCloseButtonHint;
	//flags&=~Qt::WindowMinimizeButtonHint;
	//flags&=~Qt::WindowMaximizeButtonHint;
	setWindowFlags(flags);
	setWindowIcon(QIcon(":/icon-256.png"));
	resize(640,480);
	SetupGUIWidgets();
	SetupGUIMenus();
	Retranslate();
}

static void RetranslateCallback(
	void*data,
	language*o maybe_unused,
	language*n maybe_unused
){
	Log*i=(Log*)data;
	i->Retranslate();
}

Log::Log(QWidget*parent):QDialog(parent){
	SetupGUI();
	log_instance=this;
	language_add_callback(RetranslateCallback,this);
}

Log::~Log(){
	if(log_instance==this)
		log_instance=nullptr;
	language_del_callback(RetranslateCallback,this);
}

void Log::BoxAddLogButton(QMessageBox*box){
	auto*btn=box->addButton(
		_("Show Log"),
		QMessageBox::ActionRole
	);
	connect(btn,SIGNAL(clicked()),this,SLOT(OnBoxLogBtnClick()));
}

void Log::ShowError(const QString&text){
	QMessageBox msg;
	msg.setWindowTitle(_("Error"));
	msg.setText(text);
	msg.setStandardButtons(QMessageBox::Cancel);
	msg.button(QMessageBox::Cancel)->setText(_("Cancel"));
	BoxAddLogButton(&msg);
	msg.setIcon(QMessageBox::Warning);
	LogPrint(LEVEL_ERROR,text);
	msg.exec();
}

void Log::ShowFatal(const QString&text){
	QMessageBox msg;
	msg.setWindowTitle(_("Fatal"));
	msg.setText(text);
	msg.setStandardButtons(QMessageBox::Cancel);
	msg.button(QMessageBox::Cancel)->setText(_("Cancel"));
	msg.setIcon(QMessageBox::Critical);
	LogPrint(LEVEL_ERROR,text);
	msg.exec();
	exit(1);
}

uint64_t Log::CountLog(lib_log_level min){
	uint64_t cnt=0;
	for(uint32_t i=min;i<LEVEL_MAX;i++)
		cnt+=count[i];
	return cnt;
}

void Log::LogPrint(lib_log_level level,const QString&text){
	QColor color;
	if(!edit)return;
	switch(level){
		case LEVEL_DEBUG:color=Qt::gray;break;
		case LEVEL_INFO:color=Qt::blue;break;
		case LEVEL_WARN:color=Qt::darkYellow;break;
		case LEVEL_ERROR:color=Qt::red;break;
		default:return;
	}
	edit->moveCursor(QTextCursor::End);
	edit->setTextColor(color);
	time_t t=time(nullptr);
	struct tm*m=localtime(&t);
	edit->insertPlainText(
		QString("%1:%2:%3")
		.arg(m->tm_hour,2,10,QChar('0'))
		.arg(m->tm_min,2,10,QChar('0'))
		.arg(m->tm_sec,2,10,QChar('0'))
	);
	edit->insertPlainText(": ");
	edit->insertPlainText(text);
	edit->insertPlainText("\n");
	count[level]++;
}

extern "C"{
	void lib_print(lib_log_level level,const char*str){
		if(!str)return;
		fprintf(stdout,"%s\r\n",str);
		if(log_instance)log_instance->LogPrint(level,str);
	}
}

