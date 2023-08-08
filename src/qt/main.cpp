#include<QApplication>
#include"construct.h"
#include"language.h"
#include"parser.h"
#include"log.h"

static void RetranslateApp(QApplication*app){
	app->setApplicationName(_("Qualcomm DEVCFG Parser"));
}

static void RetranslateCallback(
	void*data,
	language*o maybe_unused,
	language*n maybe_unused
){
	RetranslateApp((QApplication*)data);
}

int main(int argc,char**argv){
	QApplication a(argc,argv);
	DevcfgParser w;
	Log log(&w);
	run_constructor();
	atexit(run_destructor);
	RetranslateApp(&a);
	language_add_callback(RetranslateCallback,&a);
	if(argc<1||argc>2)
		r_err(-1,_("Usage: qc-devcfg-parser-qt [DEVCFG]"));
	w.show();
	if(argc==2){
		info(_("open file %s from commandline argument"),argv[1]);
		w.Open(argv[1]);
	}
	info(_("devcfg parser initialized"));
	return QApplication::exec();
}
