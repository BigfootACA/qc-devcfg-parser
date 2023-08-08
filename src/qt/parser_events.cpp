//
// Created by bigfoot on 2023/7/31.
//

#include<QClipboard>
#include<QFileDialog>
#include<QGuiApplication>
#include<QDragEnterEvent>
#include<QEvent>
#include<QMimeData>
#include"parser.h"
#include"language.h"
#include"about.h"
#include"struct.h"

void DevcfgParser::SetupGUIEvents(){
	connect(menu_open,&QAction::triggered,this,qOverload<>(&DevcfgParser::OnMenuOpenClick));
	connect(menu_close,&QAction::triggered,this,qOverload<>(&DevcfgParser::OnMenuCloseClick));
	connect(menu_reload,&QAction::triggered,this,qOverload<>(&DevcfgParser::OnMenuReloadClick));
	connect(menu_log,&QAction::triggered,this,qOverload<>(&DevcfgParser::OnMenuLogClick));
	connect(menu_about,&QAction::triggered,this,qOverload<>(&DevcfgParser::OnMenuAboutClick));
	connect(tree,&QTreeWidget::itemClicked,this,qOverload<QTreeWidgetItem*,int>(&DevcfgParser::OnTreeItemClick));
	connect(sel_view,&QComboBox::currentIndexChanged,this,qOverload<int>(&DevcfgParser::OnViewItemChanged));
	connect(btn_struct,&QPushButton::clicked,this,qOverload<>(&DevcfgParser::OnBtnStructViewClick));
	connect(btn_save_text,&QPushButton::clicked,this,qOverload<>(&DevcfgParser::OnBtnSaveTextClick));
	connect(btn_save_bin,&QPushButton::clicked,this,qOverload<>(&DevcfgParser::OnBtnSaveBinaryClick));
	connect(btn_copy,&QPushButton::clicked,this,qOverload<>(&DevcfgParser::OnBtnCopyClick));
}

void DevcfgParser::OnMenuOpenClick(){
	if(devcfg_is_loaded(cfg))return;
	QFileDialog file;
	file.setFileMode(QFileDialog::ExistingFile);
	file.setAcceptMode(QFileDialog::AcceptOpen);
	file.setNameFilter(_(
		"All devcfg files (*.mbn *.img *.bin *.elf);;"
		"Qualcomm MBN (*.mbn);;"
		"Disk Image (*.img);;"
		"ELF File (*.elf);;"
		"Binary File (*.bin);;"
		"Any File (*);;"
	));
	if(!file.exec())return;
	auto files=file.selectedFiles();
	if(files.length()!=1)return;
	Open(&files.at(0));
}

void DevcfgParser::OnMenuCloseClick(){
	Close();
}

void DevcfgParser::OnMenuReloadClick(){
	if(devcfg_is_loaded(cfg))LoadItems();
}

void DevcfgParser::OnMenuLogClick(){
	auto*log=LogGetDefaultInstance();
	if(log)log->show();
}

void DevcfgParser::OnMenuAboutClick(){
	static About*about=nullptr;
	if(!about)about=new About();
	about->exec();
}

void DevcfgParser::OnBtnStructViewClick(){
	if(!cur_prop)return;
	// TODO: implement struct viewer
	//try{
	//	StructViewer::ShowStructViewer(cur_prop,this);
	//}catch(std::exception&e){
	//	auto*log=LogGetDefaultInstance();
	//	if(log)log->ShowError(e.what());
	//}
}

void DevcfgParser::OnBtnSaveTextClick(){
	QFile f;
	QString reason;
	QFileDialog file;
	QList<QString>files;
	file.setAcceptMode(QFileDialog::AcceptSave);
	file.setNameFilter(_(
		"Text File (*.txt);;"
		"Any File (*);;"
	));
	if(!file.exec())goto fail;
	files=file.selectedFiles();
	if(files.length()!=1)goto fail;
	f.setFileName(files.at(0));
	if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
		reason=QString(_("Open file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	if(f.write(text_value->toPlainText().toUtf8())==-1){
		reason=QString(_("Write file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	f.close();
	return;
	fail:
	auto*log=LogGetDefaultInstance();
	if(log)log->ShowError(QString(_("Save file failed: %1")).arg(reason));
	if(f.isOpen())f.close();
}

void DevcfgParser::OnBtnSaveBinaryClick(){
	QFile f;
	size_t len=0;
	void*data=NULL;
	QString reason;
	QFileDialog file;
	QList<QString>files;
	if(!(data=devcfg_prop_get_value_bytes(cur_prop,&len,0))){
		reason=QString(_("No data available for current property"));
		goto fail;
	}
	file.setAcceptMode(QFileDialog::AcceptSave);
	file.setNameFilter(_(
		"Binary File (*.bin);;"
		"Any File (*);;"
	));
	if(!file.exec())goto fail;
	files=file.selectedFiles();
	if(files.length()!=1)goto fail;
	f.setFileName(files.at(0));
	if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
		reason=QString(_("Open file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	if(f.write(QByteArray((const char*)data,len))==-1){
		reason=QString(_("Write file %1 failed: %2"))
			.arg(files.at(0),f.errorString());
		goto fail;
	}
	f.close();
	return;
	fail:
	auto*log=LogGetDefaultInstance();
	if(log)log->ShowError(QString(_("Save file failed: %1")).arg(reason));
	if(f.isOpen())f.close();
	if(data)free(data);
}

void DevcfgParser::OnBtnCopyClick(){
	QClipboard*clipboard=QGuiApplication::clipboard();
	if(clipboard)clipboard->setText(text_value->toPlainText());
}

void DevcfgParser::OnLanguageItemClicked(){
	QAction*target=qobject_cast<QAction*>(sender());
	if(!target)return;
	QString id=target->data().toString();
	for(size_t i=0;languages[i].id;i++){
		if(id!=QString(languages[i].id))continue;
		language_set(&languages[i]);
		info(_("changed language to %s"),languages[i].name);
		return;
	}
}

void DevcfgParser::OnViewItemChanged(int v){
	int var=sel_view->itemData(v).toInt();
	auto mode=static_cast<view_mode>(var);
	if(mode==VIEW_NONE)return;
	LoadValue(cur_prop,mode);
}

void DevcfgParser::OnTreeItemClick(QTreeWidgetItem*item,int v){
	if(!item||v!=0)return;
	int val=item->data(0,Qt::UserRole).toInt();
	LoadItem(val>0?props.at(val-1):nullptr);
}

void DevcfgParser::dragEnterEvent(QDragEnterEvent*event){
	if(event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void DevcfgParser::dragMoveEvent(QDragMoveEvent*event){
	if(event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void DevcfgParser::dragLeaveEvent(QDragLeaveEvent*event){
	event->accept();
}

void DevcfgParser::dropEvent(QDropEvent*event){
	auto*mime=event->mimeData();
	if(!mime->hasUrls())return;
	auto urls=mime->urls();
	if(urls.size()<=0)return;
	Close();
	auto file=urls.at(0).toLocalFile();
	if(Open(&file))event->acceptProposedAction();
}
