#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<float.h>

#define DATASIZE 101
#define PROMPT ">>> " //提示符
#define OPERATOR "+-*/%%" //支持的操作符
 
double add_d(double a, double b)
{
	return a + b;
}
double sub_d(double a, double b)
{
	return a - b;
}
double mul_d(double a, double b)
{
	return a * b;
}
double div_d(double a, double b)
{
	return a / b;
}
double mod_d(double a, double b)
{
	return fmod(a, b);
}

void help_info(void)
{
	printf("默认进入普通计算模式，-p 进入程序员模式\n");
	printf("[标准模式]\n");
	printf("\t+: 加法\n");
	printf("\t-: 减法\n");
	printf("\t*: 乘法\n");
	printf("\t/: 除法\n");
	printf("\t%%: 取余\n");
	printf("[程序员模式]\n");
	printf("\tBIN：二进制\n");
	printf("\tOCT：八进制\n");
	printf("\tDEC：十进制\n");
	printf("\tHEX：十六进制\n");
	printf("q: 退出\n");

}

void error_msg(int error_code)
{
	switch(error_code)
	{
	case 1:
		printf("[001] 读取数据失败\n");
		break;

	case 2:
		printf("[002] 无效命令\n");
		break;
	case 3:
		printf("[003] 未找到操作符\n");
		break;
	case 4:
		printf("[004] 操作数解析失败\n");
		break;
	case 5:
		printf("[005] 未定义运算符\n");
		break;
	case 6:
		printf("[006][系统错误] 字符串解析返回值错误\n");
		break;
	case 7:
		printf("[007] 输入超过范围\n");
		break;
	case 8:
		printf("[008][系统错误] 未知模式\n");
		break;

	default:
			#ifdef Debug
			printf("[Debug][%d] wrong error_code\n", __LINE__);
			#endif
		break;
	}
}

/********************************************************************************
 *读取数据函数
 * 
 * 返回值：
 * 0 正常运行
 * -1 输入超过范围 
 * -2 fgets返回异常 
********************************************************************************/
int read_data(char *data)
{

	char *ret=NULL;
	ret = fgets(data, DATASIZE, stdin);
	if (ret)
	{
		char *get_enter;
		get_enter = strchr(data, '\n');
		if (get_enter)	//get_enter不为NULL
		{
			*get_enter = '\0';
		}
		else
		{
			error_msg(7);//输入超过范围
			while(getchar() != '\n');
			return -1;
		}
		return 0;
	}
		#ifdef Debug
		printf("[Debug][%d] 字符串地址:%p\n", __LINE__, ret);
		printf("[Debug][%d] 字符串内容:%s\n", __LINE__, data);
		#endif

	error_msg(1);//读取数据失败
	return -2;
}

/********************************************************************************
 *字符串解析函数
 * 
 * 返回值：
 * 0 正常运行
 * 1 进入程序员模式
 * -1 结束本次循环
 * -2 结束程序
********************************************************************************/
int parse_str(char *data, char *op, double *num1, double *num2)
{
	if (data[0] == '-')
	{
		//帮助
		if((data[1]=='h'&&data[2]=='\0')||	
		(data[1]=='-'&&data[2]=='h'&&data[3]=='e'&&data[4]=='l'&&data[5]=='p'&&data[6]=='\0'))
		{
			help_info();
			return -1;
		}
		//程序员模式
		else if(data[1]=='p'&&data[2]=='\0')
		{
			return 1;
		}
		else
		{
			error_msg(2);//无效命令
			return -1;
		}
	}
	//退出
	else if (data[0] == 'q' && data[1] == '\0')
		return -2;

	//获取操作符
	int op_bit;

	op_bit = strcspn(data, OPERATOR);	//查找操作符的在字符串中的下标
		#ifdef Debug
		printf("[Debug][%d] 操作符:%c\n", __LINE__, data[op_bit]);
		#endif
	*op = data[op_bit];
	if(*op == '\0')
	{
		error_msg(3);	//未找到操作符
		return -1;
	}
	
	//获取操作数
	int ret;
	char error[1];
	ret = sscanf(data, "%lf%*[^0-9.eE]%lf%1[^0-9.eE]", num1, num2, error);
		#ifdef Debug
		printf("[Debug][%d] 操作数返回值:%d\n",__LINE__, ret);
		printf("[Debug][%d] 操作数1:  %lf\n",__LINE__, *num1);
		printf("[Debug][%d] 操作数2:  %lf\n",__LINE__, *num2);
		#endif
	if(ret!=2)
	{
		error_msg(4);	//操作数解析失败
		return -1;
	}

	return 0;
}

void check_operand()
{
	printf("==================%g\n", DBL_MAX);
	printf("==================%g\n", DBL_MIN);
}



void calculator(char op, double num1, double num2)
{
	switch(op)
	{
	case '+':
		printf("%g\n",add_d(num1, num2));
		break;
	case '-':
		printf("%g\n",sub_d(num1, num2));
		break;
	case '*':
		printf("%g\n",mul_d(num1, num2));
		break;
	case '/':
		printf("%g\n",div_d(num1, num2));
		break;
	case '%':
		printf("%g\n",mod_d(num1, num2));
		break;
	default:
		error_msg(5);//未定义运算符
	}
	
}

int main()
{
	char data[DATASIZE]={};
	char op;
	double num1, num2;
	int ret;
	printf("[CALCULATOR v1.0]\nType \"--help\" or \"-h\" for more infomation\n");

	bool flag = true;
	char mode_flag = 0; 
	while(flag)
	{
		printf(PROMPT);

		//读取数据
		if(read_data(data) != 0)
		{
			continue;
		}


		//字符串解析
		ret = parse_str(data ,&op, &num1, &num2);
			#ifdef Debug
			printf("[Debug][%d] 字符串解析返回值: %d\n", __LINE__, ret);
			printf("[Debug][%d] 计算表达式: %lf %c %lf\n", __LINE__, num1, op, num2);
			#endif
		if(ret == 1) //进入程序员模式
		{
			printf("进入[程序员模式]\n");
			mode_flag=1;
			continue;
		}
		else if(ret == -2)    //退出
			break;
		else if(ret == -1)
			continue;
		else if(ret != 0)
		{
			error_msg(6); //字符串解析返回值错误
			continue;
		}

		//检查数据是否超过范围
		//check_operand();

		//运算
		if(mode_flag == 0)
		{
			calculator(op, num1, num2);
		}
		else if(mode_flag == 1)
		{
			printf("[程序员模式]\n");
		}
		else
		{
			error_msg(8);	//未知模式
			continue;
		}
	}
	return 0;
}