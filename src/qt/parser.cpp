#include"parser.h"
#include "language.h"

bool DevcfgParser::Open(const char*file){
	auto*log=LogGetDefaultInstance();
	if(!file||!log)return false;
	uint64_t last=log->CountLog(LEVEL_WARN);
	if(!(cfg=devcfg_new())){
		log->ShowError(_("New devcfg instance failed"));
		return false;
	}
	if(devcfg_load_path(cfg,file)!=0){
		log->ShowError(_("Open devcfg failed"));
		devcfg_unref(cfg);
		cfg=nullptr;
		return false;
	}
	devcfg_dump(cfg,0);
	if(log->CountLog(LEVEL_WARN)!=last)
		log->ShowError(_("Error occurred while opening devcfg"));
	menu_open->setEnabled(false);
	menu_close->setEnabled(true);
	menu_reload->setEnabled(true);
	LoadItems();
	return true;
}

bool DevcfgParser::Open(const QString*file){
	return file&&Open(file->toUtf8().constData());
}

void DevcfgParser::Close(){
	if(!devcfg_is_loaded(cfg))return;
	ClearItems();
	devcfg_unref(cfg);
	cfg=nullptr;
	menu_open->setEnabled(true);
	menu_close->setEnabled(false);
	menu_reload->setEnabled(false);
}

static void RetranslateCallback(
	void*data,
	language*o maybe_unused,
	language*n maybe_unused
){
	DevcfgParser*i=(DevcfgParser*)data;
	i->Retranslate();
}

DevcfgParser::DevcfgParser(
	QWidget*parent
):QMainWindow(parent){
	SetupGUI();
	setAcceptDrops(true);
	language_add_callback(RetranslateCallback,this);
}

DevcfgParser::~DevcfgParser(){
	Close();
	language_del_callback(RetranslateCallback,this);
}
