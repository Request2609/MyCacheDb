
// va_arg_test.cpp : 定义控制台应用程序的入口点。
//
//访问可变参数流程
//va_list args;			//定义一个可变参数列表
//va_start(args,arg);	//初始化args指向强制参数arg的下一个参数
//va_arg(args,type);	//获取当前参数内容并将args指向下一个参数
//...					//循环获取所有可变参数内容
//va_end(args);			//释放args

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
 
/// @brief		利用变长参数函数进行求和运算
/// @param[in]	n : 指定后面有多少可变参数
///	@remark		第一个参数为强制参数
///	@return		得到求和结果
double sum(unsigned int n, ...)
{
	double sum = 0;
	va_list args = NULL;//定义一个可变参数列表
	va_start(args, n);//初始化args指向强制参数arg的下一个参数
	while(n > 0)
	{
		//通过va_arg(args, double)依次获取参数的值
		sum += va_arg(args, double);
		n--;
	}
	va_end(args);//释放args
	return sum;
}
 
/// @brief		利用变长参数函数格式化字符串
/// @param[in]	format : 类似printf中第一个参数
///	@remark		
///	@return		格式化后的字符串
std::string format(const char* format, ...)
{
	std::string var_str;
 
	va_list	ap;
	va_start(ap, format);
	int len = _vscprintf(format, ap);
	if (len > 0)
	{
		std::vector<char> buf(len + 1);
		vsprintf(&buf.front(), format, ap);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(ap);
 
	return var_str;
}
 
//
int main(int argc, _TCHAR* argv[])
{
	//求和
	double cnt = sum(6, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6);
	printf("sum: %f\n", cnt);
 
	//格式化字符串
	std::string szTmp = format("%s,%s,%s", "this", "is", "kandy");
	printf("%s \n", szTmp.c_str());
 
	system("pause");
 
	return 0;
}
