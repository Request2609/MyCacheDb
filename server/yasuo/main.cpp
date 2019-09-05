#include <iostream>
#include "lzf.h"
#include "lzfP.h"

int main() {

    string value = "hello world hello world hello world hello world"; 
	size_t len = value.size();  // 字符串未压缩前的长度
	cout << "压缩前：val = " << value << endl;
	cout << "压缩前：len = " << len << endl << endl;
	// -------------------压缩---------------------------------
	size_t comprlen;  // 压缩后的长度
	size_t outlen;    // 输出缓存的最大长度
	void *out;
	
	//当字节至少四字节长以上时才能压缩
	if (len <= 4) {
		cout << "len <= 4" << endl;
		return 0;
	}
	
	outlen = len-4;
	
	if ((out = malloc(outlen+1)) == NULL) 
	{
		cout << "out = malloc(outlen+1)" << endl;
		return 0;
	}
	
    //传入数据，数据长度，任意类型的指针
	comprlen = lzf_compress(value.data(), len, out, outlen);  
	
	if (comprlen == 0) 
	{
		cout << "outlen == " << outlen << endl;
		cout << "comprlen == 0" << endl;
        		free(out);
        		return 0;
	}

	cout << "压缩后：val = " << out << endl;
	cout << "压缩后：len = " << comprlen << endl << endl;
	
	// -------------------解压缩---------------------------------
	char  *val = NULL;

   	 // 字符串空间
    	if ((val = (char*)malloc(len)) == NULL) 
	{
		cout << "lzf_decompress" << endl;
		return 0;
	}
    
   	 // 解压，得出字符串
   	 if (lzf_decompress(out, comprlen, val, len) == 0) 
	{
		cout << "lzf_decompress" << endl;
		return 0;
	}
	cout << "解压后：val = " << val << endl;
	cout << "解压后：len = " << len << endl;

   	 free(out);
	free(val);
   	return 0;

}
