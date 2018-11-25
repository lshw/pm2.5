#ifndef __SETTING_H__
#define __SETTING_H__
/*
struct
{
  char url[100];
} setting;
void load_setting(){
FILE *fp;
fp=fopen("/setting.dat",'r');
if(fp) {
fread(fp,&setting,1,sizeof(setting));
fclose(fp);
}else{
strncpy(setting.url,sizeof(setting.url),"http://www.wf163.com:88/lxy.php");
}
}

void save_setting(){
FILE *fp;
fp=fopen("/setting.dat","w");
if(fp) {
fwrite(fp,1,sizeof(setting),setting);
fclose(fp);
}
}
*/
#endif //__SETTING_H__
