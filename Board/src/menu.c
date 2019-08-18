/**********************718����ʵ���ҿ���������*********************
*  ��д��718����ʵ����
*  ƽ̨��STM32F103ZET6
*  ˵�����������ߵ�ˮƽ���ޣ����в���֮�����������½⡣
*		 �����Ҷ࿴�������ֲᡣ     
******************************************************************/
#include "stm32f10x.h"
#include "menu.h"
#include "flash.h"
#include "menu_key.h"
#include "oled.h"
#include "delay.h"

Menu MenuHead={NULL,0,"quit",0,NULL,NULL};				//�˳���
Menu Read={NULL,0,"read",0,NULL,NULL};						//��flash��
Menu Write={NULL,0,"write",0,NULL,NULL};					//д��flash��

/**
 *@brief      	menu�ṹ���ȡ��������
 *@param      	par_name:�ַ�����ַ   
								name:�ַ�����
 *@SampleUsage 	GetName("718lab",menu->name);
 **/
void GetName(const char *par_name, char *name)
{
	int i = 0;
	for (i=0;i<MENUSIZE;i++)//�����ַֽ���ַ������˸�ֵ
	{
		if (*(par_name + i) == '\0') {
			name[i] = '\0';
			break;
		}
		else {
			name[i] = *(par_name + i);
		}
	}
	name[i] = '\0';
}


/**
 *@brief      	menu�ṹ���ȡ����������Ϣ�������֣���ַ��������
 *@param      	par_name���ַ�����ַ   
								par��������ַ   
								menu���ṹ���ַ  
								add��ÿ�εĵ�����
 *@SampleUsage  GetPar("718lab", &a, add, &menu);
 **/
void GetPar(const char *par_name, double *par,double add,Menu *menu )
{
	menu->par = (double*)par;				//�ṹ���ȡ������ַ
	menu->add = add;
	GetName(par_name,menu->name);			//��ȡ����
}


/**
 *@brief      	����һ���˵���
 *@param      	par_name���ַ�����ַ   
								par��������ַ  
								add��ÿ�εĵ����� 
								menu���ṹ���ַ  
 *@SampleUsage  CreateMenu(��718lab��,&a,add,par_menu);
 **/
void CreateMenu(const char *par_name, double *par, double add,Menu *menu)
{
	//Menu *menu = (Menu *)malloc(sizeof(Menu));
	static int i=0;														//��¼�ڼ����˵�
	i++;		
	GetPar(par_name, par,add,menu);
	menu->up = NULL;
	menu->down = NULL;
	menu->num=i;								

}

/**
 *@brief      	 ����һ���˵���
 *@param      	 par_name���ַ�����ַ   
								 par��������ַ 
								 add��ÿ�εĵ�����  
								 par_menu���ṹ���ַ  
 *@SampleUsage   CreateMenu(��718lab��,&a,add,&menupar);
 **/
void AddMenu(const char *par_name, double *par,double add,Menu *par_menu)
{
	Menu *p=&Write;
	Menu *menu = par_menu;
	CreateMenu(par_name,par,add,par_menu);
	while (p->down!=NULL)
	{
		p = p->down;
	}
	p->down = menu;					//����ָ������
	menu->up = p;
}

/**
 *@brief      	 ��ʾ����
 *@param      	 p���˵����ַ   
								 y����ʾλ��  
 *@SampleUsage   printnum(&menu,2)
 **/
void printnum(Menu *p,int y)
{
	int i=0,n=0;
	double a=*(p->par)>0?*(p->par):-*(p->par);
	uint8_t temp=((int)a)%10;
	  if(a-(int)a==0)
		{
			if(a==0){OLED_ShowChar(127-8,y,'0',16);}
			else
			{
				while(i++<9&&a!=0)
				{
					temp=((int)a)%10;
					OLED_ShowChar(127-i*8,y,temp+'0',16);
					a=(a-temp)/10;
				}
			}
		}
		else{
			a=(int)(a/0.001);
			for(i=1;i<4;i++){
				temp=((int)a)%10;
				if(temp!=0) {n++;OLED_ShowChar(127-n*8,y,temp+'0',16);}
				a=(a-temp)/10;
			}
			i=++n;
			
			if (n>1)
			{
				OLED_ShowChar(127-i*8,y,'.',16);
			}
			
			do{
				i++;
				temp=((int)a)%10;
				OLED_ShowChar(127-i*8,y,temp+'0',16);
				a=(a-(int)temp)/10;
			}while(i<9&&a!=0);
		}
		if(*(p->par)<0)
		{
			OLED_ShowChar(127-(i+1)*8,y,'-',16);
		}
}

/**
 *@brief      	  ��ʾ����
 *@param      	  p���˵����ַ  
									p_now��ѡ�еĲ˵����ַ 
									y����ʾλ��  
 *@SampleUsage    PrintMenu(&menu,&menu_now,2)
 **/
void PrintMenu(Menu *p,Menu *p_now,int y)
{
	int i = 0;
	int x=0;
	if(p==p_now){OLED_ShowChar(x,y,'-',16);x+=8;OLED_ShowChar(x,y,'>',16);x+=8;}
	for (;i<SIZE;i++)
	{
		if (p->name[i] != '\0')
		{
			OLED_ShowChar(x,y,p->name[i],16);
			x+=8;
		}
		else {
			break;
		}
	}
	if(p->par!=NULL){
	printnum(p,y);
	}
}


/**
 *@brief      	   ��ʾ�˵���
 *@param      	   p_now��ѡ�еĲ˵����ַ  
 *@SampleUsage     PrintPar(&menu_now)
 **/
void PrintPar(Menu *p_now)
{
	int x=0,i=0;
	OLED_Clear();
	for (;i<SIZE;i++)
	{
		if (p_now->name[i] != '\0')
		{
			OLED_ShowChar(x,2,p_now->name[i],16);
			x+=8;
		}
		else {
			break;
		}
	}
	printnum(p_now,4);
}

/**
 *@brief      	��ӡһҳ�˵�
 *@param      	p_now��ѡ�еĲ˵����ַ 
								serial��ѡ�еĲ˵�Ϊ�ڼ���
 *@SampleUsage  PrintAllMenu(&menu_now,5)
 **/
void PrintAllMenu(Menu *p_now,int serial)
{
	int i=0;
	Menu *p = p_now;
	serial--;
	serial=serial%4+1;
	for(;i<serial-1;i++)			//Ѱ��һҳ�ĵ�һ��˵�
	{
		p=p->up;
	}
	OLED_Clear();
	for(i=0;i<4;i++)					//���˴�ӡ
	{
		PrintMenu(p,p_now,i*2);
		if(p->down==NULL){break;}
		else{p=p->down;}
	}
}
/**
 *@brief      	ɾ���˵���
 *@param      	head������ַ
 *@SampleUsage  DeleteMenu(&menu_now);
 **/
/*void DeleteMenu(Menu *head)
{
	if (head->down!=NULL)
	{
		DeleteMenu(head->down);
	}
	else {
		free(head);
	}
}*/

/**
 *@brief      	�ı����ֵ
 *@param      	menu���˵����ַ 
 *@SampleUsage  Menu_AdjustPar(&menu)
 **/
void Menu_AdjustPar(Menu *menu)
{
	int state=1;
	PrintPar(menu);
	while(1)
	{
		state=1;
		switch(Key_detect())					//�жϰ���ֵ
		{
			case KEY_UP:
				*(menu->par)+=menu->add;			//����ֵ����
				break;
			case KEY_DOWN:
				*(menu->par)-=menu->add;			//����ֵ����
				break;
			case KEY_RIGHT:
				*(menu->par)+=menu->add;
				break;
			case KEY_LEFT:
				*(menu->par)-=menu->add;
				break;
			//����flash���ص��˵�ѡ�����
			case KEY_ENDER:
				STMFLASH_Write(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);
				return;
			default:state=0;
		}
		if(state==1){PrintPar(menu);}			//�а�����ˢ����ʾ
	}
}


/**
 *@brief      		�������ģʽ
 *@param      		None
 *@SampleUsage   
									while(1)
									{
									if(key==1){Menu_ShowPar();}		//����������£��������
									///////������������/////////
									}
 **/
void Menu_ShowPar(void)
{
	int i=0;int key=0;
	int serialnumber=1;
	Menu *NowMenu=&MenuHead;
	PrintAllMenu(NowMenu,serialnumber);
	while(1)
	{
		key=1;
		switch(Key_detect())
		{
			//���˵���һλ
			case KEY_UP:
				if(NowMenu->up!=NULL) {serialnumber--;NowMenu=NowMenu->up;}
				break;
			//���˵���һλ
			case KEY_DOWN:
				if(NowMenu->down!=NULL) {serialnumber++;NowMenu=NowMenu->down;}
				break;
			//���˵�����λ
			case KEY_RIGHT:
				for (i=0;i<4&&NowMenu->down!=NULL;i++) {serialnumber++;NowMenu=NowMenu->down;}
				break;
			//���˵�����λ
			case KEY_LEFT:
				for (i=0;i<4&&NowMenu->up!=NULL;i++) {serialnumber--;NowMenu=NowMenu->up;}
				break;
			//��������ı���Ӧ�ĺ�������
			case KEY_ENDER:
				if (NowMenu==&MenuHead){return;}
				else if(NowMenu==&Read){ReadFlash();}
				else if(NowMenu==&Write){WriteFlash();}
				else{Menu_AdjustPar(NowMenu);}
				break;
			default:key=0;
		}
		if (key==1){
			PrintAllMenu(NowMenu,serialnumber);
		}
	}
}
/**
 *@brief      	��flash
 *@param      	None
 *@SampleUsage  ReadFlash();
 **/
void ReadFlash(void)
{
	Menu *menu=&Write;
	while(menu->down!=NULL)
	{
		menu=menu->down;
		STMFLASH_Read(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);//��ȡflash
	}
}
/**
 *@brief      		 дflash
 *@param      		 None
 *@SampleUsage     WriteFlash();
 **/
void WriteFlash(void)
{
	Menu *menu=&Write;
	while(menu->down!=NULL)
	{
		menu=menu->down;
		STMFLASH_Write(FLASH_SAVE_ADDR+menu->num*sizeof(double),(uint16_t*)menu->par,sizeof(double)/2);//дflash
	}
}

/**
 *@brief      		��ʼ��OLED
 *@param      		None
 *@SampleUsage    Menu_Init();
 **/
void Menu_Init(void)
{
	//���Ӹ��ڵ�
	MenuHead.down=&Read;
	Read.up=&MenuHead;
	
	Read.down=&Write;
	Write.up=&Read;
	
	Adc_Init();			//��ʼ��ADC
	delay_init();		//��ʼ����ʱ����
	OLED_Init();		//��ʼ��oled
}
