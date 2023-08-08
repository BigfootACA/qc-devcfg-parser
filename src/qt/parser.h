#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include<QTreeWidget>
#include<QSplitter>
#include<QLineEdit>
#include<QMenu>
#include<QLabel>
#include<QComboBox>
#include<QPushButton>
#include"devcfg.h"
#include"modes.h"
#include"log.h"

class DevcfgParser:public QMainWindow{
	Q_OBJECT
	public:
		explicit DevcfgParser(QWidget*parent=nullptr);
		~DevcfgParser() override;
		void Retranslate();
		void LoadItems();
		bool Open(const char*file);
		bool Open(const QString*file);
		void Close();
	protected slots:
		void OnMenuOpenClick();
		void OnMenuCloseClick();
		void OnMenuReloadClick();
		void OnMenuLogClick();
		void OnMenuAboutClick();
		void OnBtnStructViewClick();
		void OnBtnSaveTextClick();
		void OnBtnSaveBinaryClick();
		void OnBtnCopyClick();
		void OnLanguageItemClicked();
		void OnTreeItemClick(QTreeWidgetItem*item,int v);
		void OnViewItemChanged(int v);
		void dragEnterEvent(QDragEnterEvent*event) override;
		void dragMoveEvent(QDragMoveEvent*event) override;
		void dragLeaveEvent(QDragLeaveEvent*event) override;
		void dropEvent(QDropEvent*event) override;
	private:
		void LoadItem(devcfg_prop*prop=nullptr);
		void LoadViewMode(devcfg_prop*prop=nullptr);
		void LoadValue(devcfg_prop*prop=nullptr,enum view_mode view=VIEW_NONE);
		void LoadLanguages();
		void SetupGUIWidgets();
		void SetupGUIEvents();
		void SetupGUIMenus();
		void SetupGUI();
		void ClearItems();
		void AddDevcfgProp(devcfg_prop*prop,QTreeWidgetItem*parent);
		void AddDevcfgDevice(devcfg_device*device,QTreeWidgetItem*parent);
		void AddDevcfgTarget(devcfg_target*target,QTreeWidgetItem*parent);
		void AddDevcfgSection(devcfg_section*section,QTreeWidgetItem*parent);
		QSplitter*root=nullptr;
                QTreeWidget*tree=nullptr;
		QAction*menu_open=nullptr,*menu_close=nullptr;
		QAction*menu_reload=nullptr,*menu_log=nullptr;
		QAction*menu_about=nullptr;
		QMenu*menu_language=nullptr;
		QLabel*text_section=nullptr,*text_target=nullptr;
		QLineEdit*text_device=nullptr,*text_prop=nullptr;
		QLabel*text_type=nullptr,*text_length=nullptr;
		QComboBox*sel_view=nullptr;
		QTextEdit*text_value=nullptr;
		QPushButton*btn_struct=nullptr,*btn_save_text=nullptr,*btn_save_bin=nullptr,*btn_copy=nullptr;
		QLabel*lbl_section=nullptr,*lbl_target=nullptr,*lbl_device=nullptr,*lbl_prop=nullptr;
		QLabel*lbl_type=nullptr,*lbl_length=nullptr,*lbl_view=nullptr,*lbl_value=nullptr;
		std::vector<devcfg_prop*>props;
		devcfg_prop*cur_prop=nullptr;
		devcfg*cfg=nullptr;
};
#endif
