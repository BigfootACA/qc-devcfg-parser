//
// Created by bigfoot on 2023/7/31.
//

#ifndef ABOUT_H
#define ABOUT_H
#include<QLabel>
#include<QDialog>
#include<QPushButton>
#include<QGridLayout>
#include<QDialogButtonBox>
class About:public QDialog{
	Q_OBJECT
	public:
		void Retranslate();
		void SetupGUI();
		void SetupGUIEvents();
		void SetupGUIWidgets();
		About(QWidget*parent=nullptr);
		~About();
	protected slots:
		void OnOpenClick();
	private:
		QGridLayout*layout;
		QLabel*lbl_title;
		QLabel*lbl_url;
		QLabel*lbl_copyright;
		QLabel*lbl_warranty;
		QLabel*lbl_license;
		QDialogButtonBox*btn_buttons;
		QPushButton*btn_open;
		QPushButton*btn_close;
};
#endif
