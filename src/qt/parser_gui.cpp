//
// Created by bigfoot on 2023/7/31.
//

#include<QMenuBar>
#include<QGridLayout>
#include<QHeaderView>
#include"parser.h"
#include"language.h"

void DevcfgParser::LoadLanguages(){
	size_t i;
	language*cur=language_get_current();
	for(auto v:menu_language->actions())delete v;
	for(i=0;languages[i].id;i++){
		auto*lang=new QAction();
		lang->setText(languages[i].name);
		lang->setData(QVariant(languages[i].id));
		lang->setCheckable(true);
		if(cur==&languages[i])
			lang->setChecked(true);
		connect(
			lang,SIGNAL(triggered(bool)),
			this,SLOT(OnLanguageItemClicked())
		);
		menu_language->addAction(lang);
	}
	if(i<=0){
		auto*lang=new QAction();
		lang->setText(_("No any language found"));
		lang->setEnabled(false);
		menu_language->addAction(lang);
	}
}

void DevcfgParser::Retranslate(){
	setWindowTitle(_("Qualcomm DEVCFG Parser"));
        menu_open->setText(_("Open"));
        menu_close->setText(_("Close"));
        menu_reload->setText(_("Reload"));
        menu_log->setText(_("Log"));
        menu_language->setTitle(_("Language"));
        menu_about->setText(_("About"));
	lbl_section->setText(_("Section:"));
	lbl_target->setText(_("Target:"));
	lbl_device->setText(_("Device:"));
	lbl_prop->setText(_("Property:"));
	lbl_type->setText(_("Data Type:"));
	lbl_length->setText(_("Data Length:"));
	lbl_view->setText(_("View:"));
	lbl_value->setText(_("Value:"));
	btn_struct->setText(_("Struct View"));
	btn_save_text->setText(_("Save Text"));
	btn_save_bin->setText(_("Save Binary"));
	btn_copy->setText(_("Copy"));
	LoadItem();
	LoadItems();
	LoadLanguages();
}

void DevcfgParser::SetupGUIMenus(){
        auto*menubar=new QMenuBar(this);
        setMenuBar(menubar);

        menu_open=new QAction(menubar);
	menubar->addAction(menu_open);

        menu_close=new QAction(menubar);
	menu_close->setEnabled(false);
	menubar->addAction(menu_close);

        menu_reload=new QAction(menubar);
	menu_reload->setEnabled(false);
	menubar->addAction(menu_reload);

        menu_log=new QAction(menubar);
	menubar->addAction(menu_log);

	menu_language=new QMenu(menubar);
	menubar->addMenu(menu_language);

        menu_about=new QAction(menubar);
	menubar->addAction(menu_about);
}

void DevcfgParser::SetupGUIWidgets(){
        tree=new QTreeWidget(root);
        tree->setRootIsDecorated(true);
        tree->header()->setVisible(false);
	root->addWidget(tree);

	auto*right=new QWidget(root);
        auto*grid=new QGridLayout(right);
	right->setLayout(grid);
	root->addWidget(right);

	lbl_section=new QLabel(right);
	text_section=new QLabel(right);
	grid->addWidget(lbl_section,0,0);
	grid->addWidget(text_section,0,1);

	lbl_target=new QLabel(right);
	text_target=new QLabel(right);
	grid->addWidget(lbl_target,0,2);
	grid->addWidget(text_target,0,3);

	lbl_device=new QLabel(right);
	text_device=new QLineEdit(right);
	text_device->setReadOnly(true);
	grid->addWidget(lbl_device,1,0);
	grid->addWidget(text_device,1,1,1,3);

	lbl_prop=new QLabel(right);
	text_prop=new QLineEdit(right);
	text_prop->setReadOnly(true);
	grid->addWidget(lbl_prop,2,0);
	grid->addWidget(text_prop,2,1,1,3);

	lbl_type=new QLabel(right);
	text_type=new QLabel(right);
	grid->addWidget(lbl_type,3,0);
	grid->addWidget(text_type,3,1);

	lbl_length=new QLabel(right);
	text_length=new QLabel(right);
	grid->addWidget(lbl_length,3,2);
	grid->addWidget(text_length,3,3);

	lbl_view=new QLabel(right);
	sel_view=new QComboBox(right);
	grid->addWidget(lbl_view,4,0);
	grid->addWidget(sel_view,4,1,1,2);

	lbl_value=new QLabel(right);
	text_value=new QTextEdit(right);
	text_value->setReadOnly(true);
	auto f=text_value->font();
	f.setStyleHint(QFont::Monospace);
	text_value->setFont(f);
	grid->addWidget(lbl_value,4,3);
	grid->addWidget(text_value,5,0,1,4);

	btn_struct=new QPushButton(right);
	btn_struct->setEnabled(false);
	grid->addWidget(btn_struct,6,0);

	btn_save_text=new QPushButton(right);
	btn_save_text->setEnabled(false);
	grid->addWidget(btn_save_text,6,1);

	btn_save_bin=new QPushButton(right);
	btn_save_bin->setEnabled(false);
	grid->addWidget(btn_save_bin,6,2);

	btn_copy=new QPushButton(right);
	btn_copy->setEnabled(false);
	grid->addWidget(btn_copy,6,3);

	LoadItem();
}

void DevcfgParser::SetupGUI(){
	resize(800,600);
	setWindowIcon(QIcon(":/icon-256.png"));
	root=new QSplitter(this);
	setCentralWidget(root);
	SetupGUIWidgets();
	SetupGUIMenus();
	SetupGUIEvents();
	Retranslate();
}
