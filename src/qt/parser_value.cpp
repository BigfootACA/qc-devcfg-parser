//
// Created by bigfoot on 2023/7/31.
//

#include"parser.h"

static QString GenBytes(void*data,size_t len){
	QString buffer="";
	auto*val=static_cast<uint8_t*>(data);
	for(size_t i=0;i<len;i++){
		if(i>0)buffer+=(i%16)==0?"\n":" ";
		buffer+=QString("%1").arg(val[i],2,16,QChar('0'));
	}
	return buffer;
}

static size_t NumberLength(uint64_t val,uint8_t base=10){
	size_t cnt;
	switch(base){case 2:case 8:case 10:case 16:break;default:return 0;}
	for(cnt=0;val>0;cnt++)val/=base;
	return cnt;
}

static size_t NumberLength(uint64_t val,enum view_mode view=VIEW_NUM_BASE_DEC){
	uint8_t base;
	switch(view&VIEW_NUM_BASE_MASK){
		case VIEW_NUM_BASE_HEX:base=16;break;
		case VIEW_NUM_BASE_DEC:base=10;break;
		case VIEW_NUM_BASE_OCT:base=8;break;
		case VIEW_NUM_BASE_BIN:base=2;break;
		default:return 0;
	}
	return NumberLength(val,base);
}

static QString GenNumber(
	devcfg_num_val val,
	enum view_mode view=VIEW_NUM_BASE_DEC,
	size_t offset=0,
	uint32_t field=0
){
	QChar c('0');
	uint32_t size=view&VIEW_NUM_SIZE_MASK;
	bool is_array=(view&VIEW_NUM_ARRAY)==VIEW_NUM_ARRAY;
	bool is_signed=(view&VIEW_NUM_SIGNED)==VIEW_NUM_SIGNED;
	bool is_unsigned=(view&VIEW_NUM_UNSIGNED)==VIEW_NUM_UNSIGNED;
	#define DECL_NUM(num)do{switch(view&VIEW_NUM_BASE_MASK){\
		case VIEW_NUM_BASE_BOOL: return num?"true":"false";\
		case VIEW_NUM_BASE_BIN:  return QString("0b%1") .arg(num,field,2,c);\
		case VIEW_NUM_BASE_OCT:  return QString("0%1")  .arg(num,field,8,c);\
		case VIEW_NUM_BASE_HEX:  return QString("0x%1") .arg(num,field,16,c);\
		case VIEW_NUM_BASE_DEC:  return QString("%1")   .arg(num,field,10,c);\
	}}while(0)
	#define DECL_NUM_VAL(type) \
		DECL_NUM((type##_t)(is_array?val.array.type[offset]:val.number.type))
	if(is_unsigned)switch(size){
		case VIEW_NUM_SIZE_8BIT:DECL_NUM_VAL(uint8);break;
		case VIEW_NUM_SIZE_16BIT:DECL_NUM_VAL(uint16);break;
		case VIEW_NUM_SIZE_32BIT:DECL_NUM_VAL(uint32);break;
		case VIEW_NUM_SIZE_64BIT:DECL_NUM_VAL(uint64);break;
	}else if(is_signed)switch(size){
		case VIEW_NUM_SIZE_8BIT:DECL_NUM_VAL(int8);break;
		case VIEW_NUM_SIZE_16BIT:DECL_NUM_VAL(int16);break;
		case VIEW_NUM_SIZE_32BIT:DECL_NUM_VAL(int32);break;
		case VIEW_NUM_SIZE_64BIT:DECL_NUM_VAL(int64);break;
	}
	#undef DECL_NUM
	return nullptr;
}

static uint64_t GetNumber(
	devcfg_num_val val,
	enum view_mode view=VIEW_NUM_BASE_DEC,
	size_t offset=0
){
	uint32_t size=view&VIEW_NUM_SIZE_MASK;
	bool is_array=(view&VIEW_NUM_ARRAY)==VIEW_NUM_ARRAY;
	if(is_array)switch(size){
		case VIEW_NUM_SIZE_8BIT:return val.number.uint8;
		case VIEW_NUM_SIZE_16BIT:return val.number.uint16;
		case VIEW_NUM_SIZE_32BIT:return val.number.uint32;
		case VIEW_NUM_SIZE_64BIT:return val.number.uint64;
	}else switch(size){
		case VIEW_NUM_SIZE_8BIT:return val.array.uint8[offset];
		case VIEW_NUM_SIZE_16BIT:return val.array.uint16[offset];
		case VIEW_NUM_SIZE_32BIT:return val.array.uint32[offset];
		case VIEW_NUM_SIZE_64BIT:return val.array.uint64[offset];
	}
	return 0;
}

static void InsertArrayNumber(devcfg_prop*prop,enum view_mode view,QTextEdit*edit){
	uint8_t field=0;
	size_t count=0,i;
	devcfg_num_val val;
	bool sign=(view&VIEW_NUM_SIGNED)==VIEW_NUM_SIGNED;
	uint32_t base=(view&VIEW_NUM_BASE_MASK);
	uint8_t size=(view&VIEW_NUM_SIZE_MASK)>>5;
	val=devcfg_prop_get_value_number_array(prop,sign,size,&count);
	if(base!=10)for(i=0;i<count;i++)
		field=max_uint8_t(field,NumberLength(GetNumber(val,view,i),view));
	for(i=0;i<count;i++){
		if(i>0)edit->insertPlainText(((i%16)==0)?"\n":" ");
		edit->insertPlainText(GenNumber(val,view,i,field));
	}
}

static void InsertSingleNumber(devcfg_prop*prop,enum view_mode view,QTextEdit*edit){
	devcfg_num_val val;
	bool sign=(view&VIEW_NUM_SIGNED)==VIEW_NUM_SIGNED;
	uint8_t base=(view&VIEW_NUM_SIZE_MASK)>>5;
	if(devcfg_prop_get_value_number(prop,sign,base,&val))
		edit->setText(GenNumber(val,view));
}

static void InsertNumber(devcfg_prop*prop,enum view_mode view,QTextEdit*edit){
	if((view&VIEW_NUM_ARRAY)!=VIEW_NUM_ARRAY)
		InsertSingleNumber(prop,view,edit);
	else InsertArrayNumber(prop,view,edit);
}

void DevcfgParser::LoadValue(devcfg_prop*prop,enum view_mode view){
	text_value->setText("");
	if(prop&&!devcfg_prop_check(prop))prop=nullptr;
	if(prop&&view!=VIEW_NONE)switch(view){
		case VIEW_FILE_OFFSET:{
			ssize_t offset=devcfg_prop_get_value_file_offset(prop,NULL);
			if(offset>0)text_value->setText(QString("0x%1").arg(offset,0,16));
		}break;
		case VIEW_LOAD_ADDRESS:{
			uintptr_t address=devcfg_prop_get_value_address(prop);
			if(address>0)text_value->setText(QString("0x%1").arg(address,0,16));
		}break;
		case VIEW_STRING_UTF8:{
			char*str;
			if(!(str=devcfg_prop_get_value_string(prop)))break;
			text_value->setText(str);
			free(str);
		}break;
		case VIEW_BYTES:{
			void*data;
			size_t size=0;
			if(!(data=devcfg_prop_get_value_bytes(prop,&size,0)))break;
			text_value->setText(GenBytes(data,size));
			free(data);
		}break;
		case VIEW_NUM ... VIEW_NUM_END:InsertNumber(prop,view,text_value);break;
		default:;
	}
	bool en=text_value->toPlainText().length()>0;
	btn_save_text->setEnabled(en);
	btn_save_bin->setEnabled(en);
	btn_copy->setEnabled(en);
}

void DevcfgParser::LoadViewMode(devcfg_prop*prop){
	size_t i,j,k;
	view_mode mode=VIEW_NONE;
	devcfg_prop_type type;
	if(prop&&!devcfg_prop_check(prop))prop=nullptr;
	sel_view->clear();
	text_value->setText("");
	if(!prop)return;
	type=devcfg_prop_get_type(prop);
	for(i=0;view_modes[i].type!=DEVCFG_PROP_UNKNOWN;i++)if(type==view_modes[i].type){
		mode=view_modes[i].def;
		for(j=0;view_modes[i].modes[j]!=VIEW_NONE;j++)for(k=0;view_items[k].name;k++){
			if(view_items[k].mode!=view_modes[i].modes[j])continue;
			sel_view->addItem(_(view_items[k].name),QVariant(view_items[k].mode));
			if(view_items[k].mode==mode)sel_view->setCurrentIndex(sel_view->count()-1);
			break;
		}
		break;
	}
	btn_struct->setEnabled(type==DEVCFG_PROP_STRUCT_PTR);
	LoadValue(prop,mode);
}
