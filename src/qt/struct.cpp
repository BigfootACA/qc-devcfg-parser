//
// Created by bigfoot on 2023/8/2.
//

// TODO: implement struct viewer
#include<QHeaderView>
#include<QHBoxLayout>
#include"struct.h"
#include"language.h"

std::vector<StructViewer*>StructViewer::instances={};

void StructViewer::Retranslate(){
        setWindowTitle(_("Struct View"));

}

void StructViewer::SetupGUIEvents(){

}

void StructViewer::SetupGUIWidgets(){
	auto*hl=new QHBoxLayout(this);
        tree=new QTreeWidget(this);
        tree->setRootIsDecorated(true);
	auto*header=tree->header();
        header->setVisible(false);
	header->setSectionResizeMode(0,QHeaderView::ResizeToContents);
        tree->setColumnCount(2);
	hl->addWidget(tree);
	hl->setContentsMargins(0,0,0,0);
}

void StructViewer::SetupGUI(){
	resize(640,480);
	setWindowIcon(QIcon(":/icon-256.png"));
	SetupGUIWidgets();
	SetupGUIEvents();
	Retranslate();
}

static void RetranslateCallback(
	void*data,
	language*o maybe_unused,
	language*n maybe_unused
){
	StructViewer*i=(StructViewer*)data;
	i->Retranslate();
}

StructViewer::StructViewer(devcfg_prop*prop,QWidget*parent):QDialog(parent){
	SetupGUI();
	language_add_callback(RetranslateCallback,this);
	if(!devcfg_prop_check(prop))
		throw std::runtime_error(_("invalid devcfg prop"));
	if(devcfg_prop_get_type(prop)!=DEVCFG_PROP_STRUCT_PTR)
		throw std::runtime_error(_("property not a struct pointer"));
	for(auto*obj:instances)if(obj->prop==prop)
		throw std::runtime_error(_("duplicate struct view instance"));
	if(!(this->struct_info=devcfg_prop_find_struct(prop)))
		throw std::runtime_error(_("property struct not supported"));
	if(!(this->prop=devcfg_prop_ref(prop)))
		throw std::runtime_error(_("invalid devcfg prop"));
	instances.push_back(this);
	LoadItems();
}

StructViewer::~StructViewer(){
	language_del_callback(RetranslateCallback,this);
	std::remove(instances.begin(),instances.end(),this);
}

StructViewer*StructViewer::ShowStructViewer(devcfg_prop*prop,QWidget*parent){
	StructViewer*viewer=nullptr;
	for(auto*obj:instances)if(obj->prop==prop)viewer=obj;
	if(!viewer)viewer=new StructViewer(prop,parent);
	viewer->show();
	return viewer;
}

void StructViewer::AddStructItem(
	QTreeWidgetItem*parent,
	devcfg_struct_item*st,
	uintptr_t ptr
){
	devcfg_num_val val;
	QString str=nullptr;
	devcfg*cfg=devcfg_prop_get_devcfg(prop);
	auto*item=new QTreeWidgetItem();
	uintptr_t p=ptr+st->offset;
	item->setText(0,st->name);
	parent->addChild(item);
	if(st->size>sizeof(uintmax_t))return;
	val.val=0;
	if(!devcfg_read(cfg,p,&val,st->size,false))return;
	if(!st->array&&!st->pointer)switch(st->type){
		case TYPE_BOOL:str=val.number.boolean?"true":"false";break;
		case TYPE_INTN:str=QString("0x%1").arg(val.number.intptr,0,16);break;
		case TYPE_UINTN:str=QString("0x%1").arg(val.number.uintptr,0,16);break;
		case TYPE_INT8:str=QString("0x%1").arg(val.number.int8,0,16);break;
		case TYPE_UINT8:str=QString("0x%1").arg(val.number.uint8,0,16);break;
		case TYPE_INT16:str=QString("0x%1").arg(val.number.int16,0,16);break;
		case TYPE_UINT16:str=QString("0x%1").arg(val.number.uint16,0,16);break;
		case TYPE_INT32:str=QString("0x%1").arg(val.number.int32,0,16);break;
		case TYPE_UINT32:str=QString("0x%1").arg(val.number.uint32,0,16);break;
		case TYPE_INT64:str=QString("0x%1").arg(val.number.int64,0,16);break;
		case TYPE_UINT64:str=QString("0x%1").arg(val.number.uint64,0,16);break;
		case TYPE_CHAR:str=QString("%1").arg(val.number.c8);break;
		default:break;
	}
	if(str!=nullptr)
		item->setText(1,str);
}

void StructViewer::AddStruct(
	QTreeWidgetItem*parent,
	devcfg_struct*st,
	uintptr_t ptr
){
	for(size_t i=0;st->items[i].name;i++)
		AddStructItem(parent,&st->items[i],ptr);
}

void StructViewer::LoadItems(){
	uintptr_t ptr;
	if(!devcfg_prop_check(prop))return;
	if(!struct_info||!struct_info->name)return;
	ptr=devcfg_prop_get_value_address(prop);
	tree->clear();
	auto*top=new QTreeWidgetItem();
	top->setText(0,QString("%1 (%2@%3)").arg(
		struct_info->name,
		devcfg_prop_get_device_name(prop),
		devcfg_prop_get_name(prop)
	));
	tree->addTopLevelItem(top);
	AddStruct(top,struct_info,ptr);
	top->setExpanded(true);
}
