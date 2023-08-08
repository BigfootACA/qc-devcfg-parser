//
// Created by bigfoot on 2023/7/31.
//

#include"about.h"
#include"language.h"
#include<QDesktopServices>

void About::Retranslate(){
        setWindowTitle(_("About"));
        lbl_title->setText("qc-devcfg-parser v0.1");
        lbl_url->setText("https://github.com/BigfootACA/qc-devcfg-parser");
        lbl_copyright->setText(_("Copyright 2023 BigfootACA"));
        lbl_warranty ->setText(_("This program comes with absolutely no warranty."));
        lbl_license->setText(_("License: GPL3"));
	btn_open->setText(_("Open"));
	btn_close->setText(_("Close"));
}

static void SetPaletteColor(QWidget*lbl,QPalette::ColorRole role,QColor&color){
	QPalette p=lbl->palette();
	p.setColor(role,color);
	lbl->setPalette(p);
}

static void SetPaletteColor(QWidget*lbl,QPalette::ColorRole role,Qt::GlobalColor color){
	QColor c(color);
	SetPaletteColor(lbl,role,c);
}

void About::OnOpenClick(){
	QDesktopServices::openUrl(QUrl(lbl_url->text()));
}

void About::SetupGUIEvents(){
        connect(btn_open,&QAbstractButton::clicked,this,qOverload<>(&About::OnOpenClick));
        connect(btn_buttons,&QDialogButtonBox::rejected,this,qOverload<>(&QDialog::reject));
}

void About::SetupGUIWidgets(){
	layout=new QGridLayout(this);

	lbl_title=new QLabel(this);
	SetPaletteColor(lbl_title,lbl_title->foregroundRole(),Qt::black);
	layout->addWidget(lbl_title,0,0);

	lbl_url=new QLabel(this);
	SetPaletteColor(lbl_url,lbl_url->foregroundRole(),Qt::blue);
	layout->addWidget(lbl_url,1,0);

	lbl_copyright=new QLabel(this);
	layout->addWidget(lbl_copyright,2,0);

	lbl_warranty=new QLabel(this);
	layout->addWidget(lbl_warranty,3,0);

	lbl_license=new QLabel(this);
	layout->addWidget(lbl_license,4,0);

	btn_buttons=new QDialogButtonBox(this);
	btn_buttons->setOrientation(Qt::Horizontal);
	btn_buttons->setStandardButtons(
		QDialogButtonBox::Open|QDialogButtonBox::Close
	);
	layout->addWidget(btn_buttons,5,0);
	btn_open=btn_buttons->button(QDialogButtonBox::Open);
	btn_close=btn_buttons->button(QDialogButtonBox::Close);
}

void About::SetupGUI(){
	setWindowIcon(QIcon(":/icon-256.png"));
	setWindowFlags(Qt::Dialog);
	setFixedSize(400,200);
	SetupGUIWidgets();
	SetupGUIEvents();
	Retranslate();
}

static void RetranslateCallback(
	void*data,
	language*o maybe_unused,
	language*n maybe_unused
){
	About*i=(About*)data;
	i->Retranslate();
}

About::About(QWidget*parent):QDialog(parent){
	SetupGUI();
	language_add_callback(RetranslateCallback,this);
}

About::~About(){
	language_del_callback(RetranslateCallback,this);
}
