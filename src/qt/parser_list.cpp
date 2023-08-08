//
// Created by bigfoot on 2023/7/31.
//

#include"parser.h"

void DevcfgParser::AddDevcfgProp(
	devcfg_prop*prop,
	QTreeWidgetItem*parent
){
	if(!devcfg_prop_check(prop))return;
	auto*item=new QTreeWidgetItem();
	item->setText(0,devcfg_prop_get_name(prop));
	parent->addChild(item);
	item->setExpanded(true);
	props.push_back(devcfg_prop_ref(prop));
	QVariant v((int)props.size());
	item->setData(0,Qt::UserRole,v);
}

void DevcfgParser::AddDevcfgDevice(
	devcfg_device*device,
	QTreeWidgetItem*parent
){
	size_t i;
	devcfg_prop**val;
	if(!devcfg_device_check(device))return;
	auto*item=new QTreeWidgetItem();
	item->setText(0,devcfg_device_get_name(device));
	parent->addChild(item);
	if(!(val=devcfg_device_get_props(device)))return;
	for(i=0;val[i];i++){
		AddDevcfgProp(val[i],item);
		devcfg_prop_unref(val[i]);
	}
	free(val);
	item->setExpanded(true);
}

void DevcfgParser::AddDevcfgTarget(
	devcfg_target*target,
	QTreeWidgetItem*parent
){
	size_t i;
	devcfg_device**val;
	if(!devcfg_target_check(target))return;
	auto*item=new QTreeWidgetItem();
	item->setText(
		0,QString(_("Target SOC %1"))
		.arg(devcfg_target_get_soc(target))
	);
	parent->addChild(item);
	if(!(val=devcfg_target_get_devices(target)))return;
	for(i=0;val[i];i++){
		AddDevcfgDevice(val[i],item);
		devcfg_device_unref(val[i]);
	}
	free(val);
	item->setExpanded(true);
}

void DevcfgParser::AddDevcfgSection(
	devcfg_section*section,
	QTreeWidgetItem*parent
){
	size_t i;
	devcfg_target**val;
	if(!devcfg_section_check(section))return;
	auto*item=new QTreeWidgetItem();
	item->setText(
		0,QString(_("Section %1"))
		.arg(devcfg_section_get_id(section))
	);
	parent->addChild(item);
	if(!(val=devcfg_section_get_targets(section)))return;
	for(i=0;val[i];i++){
		AddDevcfgTarget(val[i],item);
		devcfg_target_unref(val[i]);
	}
	free(val);
	item->setExpanded(true);
}

void DevcfgParser::ClearItems(){
	for(devcfg_prop*prop:props)
		devcfg_prop_unref(prop);
	cur_prop=nullptr;
	props.clear();
	tree->clear();
	LoadItem();
}

void DevcfgParser::LoadItems(){
	size_t i;
	int32_t*array;
	devcfg_section*section;
	auto*log=LogGetDefaultInstance();
	if(!devcfg_is_loaded(this->cfg)||!log)return;
	ClearItems();
	auto*top=new QTreeWidgetItem();
	top->setText(0,"devcfg");
	tree->addTopLevelItem(top);
	uint64_t last=log->CountLog(LEVEL_WARN);
	if(!(array=devcfg_list_section_by_id(cfg,nullptr)))return;
	for(i=0;array[i]>=0;i++){
		section=devcfg_get_section_by_id(cfg,array[i]);
		if(!section)continue;
		AddDevcfgSection(section,top);
		devcfg_section_unref(section);
	}
	free(array);
	top->setExpanded(true);
	if(log->CountLog(LEVEL_WARN)!=last)
		log->ShowError(_("Error occurred while loading devcfg"));
}

void DevcfgParser::LoadItem(devcfg_prop*prop){
	auto*log=LogGetDefaultInstance();
	if(!log)return;
	if(prop&&!devcfg_prop_check(prop))prop=nullptr;
	cur_prop=prop;
	if(prop){
		uint64_t last=log->CountLog(LEVEL_WARN);
		int32_t id=devcfg_prop_get_section_id(prop);
		text_section->setText(QString::number(id));
		int64_t soc=devcfg_prop_get_target_soc(prop);
		text_target->setText(
			QString("%1 (0x%2)")
			.arg(soc).arg(soc,0,16)
		);
		text_device->setText(devcfg_prop_get_device_name(prop));
		text_prop->setText(devcfg_prop_get_name(prop));
		text_type->setText(devcfg_prop_get_type_readable(
			prop,_("(Unknown)")
		));
		ssize_t len=devcfg_prop_get_value_length(prop);
		if(len<0)len=0;
		text_length->setText(
			QString(_("%1 bytes (0x%2)"))
			.arg(len).arg(len,4,16,QChar('0'))
		);
		LoadViewMode(prop);
		if(log->CountLog(LEVEL_WARN)!=last)
			log->ShowError(_("Error occurred while loading devcfg"));
	}else{
		text_section->setText("-");
		text_target->setText("-");
		text_device->setText("");
		text_prop->setText("");
		text_type->setText("-");
		text_length->setText("0");
		text_value->setText("");
		text_value->setText("");
		btn_struct->setEnabled(false);
		btn_save_text->setEnabled(false);
		btn_save_bin->setEnabled(false);
		btn_copy->setEnabled(false);
		sel_view->clear();
	}
}
