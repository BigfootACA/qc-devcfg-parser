//
// Created by bigfoot on 2023/8/2.
//

#ifndef STRUCT_H
#define STRUCT_H
#include<QDialog>
#include<QTreeWidget>
#include"devcfg.h"

class StructViewer:public QDialog{
	Q_OBJECT
	public:
		void Retranslate();
		void SetupGUI();
		void SetupGUIEvents();
		void SetupGUIWidgets();
		StructViewer(devcfg_prop*prop,QWidget*parent=nullptr);
		~StructViewer();
		static StructViewer*ShowStructViewer(devcfg_prop*prop,QWidget*parent=nullptr);
	private:
		void LoadItems();
		void AddStruct(QTreeWidgetItem*parent,devcfg_struct*st,uintptr_t ptr);
		void AddStructItem(QTreeWidgetItem*parent,devcfg_struct_item*st,uintptr_t ptr);
		devcfg_prop*prop;
		devcfg_struct*struct_info;
		QTreeWidget*tree=nullptr;
		static std::vector<StructViewer*>instances;
};
#endif
