//
// Created by bigfoot on 2023/7/24.
//

#ifndef LOG_H
#define LOG_H
#include<QDialog>
#include<QTextEdit>
#include<QMessageBox>
#include<QHBoxLayout>
#include"defines.h"
class Log:public QDialog{
	Q_OBJECT
	public:
		explicit Log(QWidget*parent=nullptr);
		~Log() override;
		void Retranslate();
		void LogPrint(lib_log_level level,const QString&text);
		void BoxAddLogButton(QMessageBox*box);
		void ShowFatal(const QString&text);
		void ShowError(const QString&text);
		uint64_t count[LEVEL_MAX]={0};
		uint64_t CountLog(lib_log_level min=LEVEL_MIN);
	protected slots:
		void OnBoxLogBtnClick();
		void OnMenuSaveClick();
	private:
		void SetupGUI();
		void SetupGUIWidgets();
		void SetupGUIMenus();
		QWidget*root=nullptr;
		QTextEdit*edit=nullptr;
		QAction*menu_save=nullptr;
		QHBoxLayout*hl=nullptr;
};
extern Log*LogGetDefaultInstance();
#endif
