#include "logRecord.h"

std::shared_ptr<logRecord> logRecord::lr=nullptr ;

//std::shared_ptr<threadPool> logRecord::pool ;

std::map<int, long> logRecord::sizeMap ;

cmdSet*logRecord::cset;

mutex logRecord::mute ;
//当前加到缓存中的数据和
//只算key value的总长度
long logRecord::SUM = 0 ;

//文件大小限制4MB
long logRecord:: MAX_FILE_SIZE = pow(2, 24) ;

void logRecord::setcmdSet(cmdSet* cs) {
    cset =  cs;
}   

std::shared_ptr<logRecord> logRecord::getRecordObject() {
    if(lr == nullptr) {
        lr = std::shared_ptr<logRecord>(new logRecord) ;
   //     pool = make_shared<threadPool>(8) ;
        init() ;
    }
    return lr ;
}

void logRecord::init() {
    logErr::init() ;
}

void logRecord::changeCommand(const std::shared_ptr<Command>& cmd) {
    int type = cmd->type();
    //是读命令就返回
    if(type == READ_) {
        return ;
    }
    //数据库编号
    int num = cmd->num() ;
    std::string ml = cmd->cmd() ;
    string s ;
    int size = 0 ;
    //分类处理这些命令
    if(ml == "set") {
        s = formatStringAddLog(cmd, size) ;
    } 
    if(ml == "hset") {
        //hash对象日志的记录
        s = formatHashAddLog(cmd, size) ;        
    }
    if(ml == "lpush") {
        s = formatListAddLog(cmd, size) ;
    }
    //当前记录的键值的大小
    sizeMap[num] += size ;
    int len = lr->block.size() ;
    for(auto s = lr->block.begin(); s!=lr->block.end(); s++) {
        if(s->first == num) {
           s->second+=s ;
           break ;
        }
    }
    //持久化日志
    for(int i=0; i<len; i++){
        //要是i号数据库大于当前设置的缓存中的阀值，将数据存在log文件中
        if(sizeMap[i] > MAX_FILE_SIZE) {
            record(i, lr->block[i].second) ;
        }
    }
}

void logRecord::clearLogFile(int num) {
    //清空日志内容
    ftruncate(lr->ls[num], 0) ;
    lseek(lr->ls[num], 0, 0) ;
    sizeMap[num] = 0 ; 
}

string logRecord::formatStringAddLog(const std::shared_ptr<Command>&cmd, int& size) {
    char buf[BUFFER_SIZE] ;
    bzero(buf, sizeof(buf)) ;
    //类型
    string value = cmd->vals(0).val(0) ;
    string key = cmd->keys(0).key(0) ;
    //校验和
    long all = 1+key.size()+value.size() ;
    size = all ;
    //对sizemap中的共享变量加锁
    sprintf(buf, "%ld %s %s %s\r\n", all, cmd->cmd().c_str(), key.c_str(), value.c_str()) ;  
    //记录当前数据库日志长度已经超过了规定长度
    return buf ;
}   


string logRecord::formatHashAddLog(const std::shared_ptr<Command>&cmd, int& size) {
    char buf[BUFFER_SIZE] ;
    bzero(buf, sizeof(buf)) ;
    string key = cmd->keys(0).key(0) ;
    int len = cmd->vals_size() ;
    string values = "" ;
    long all = 0 ;
    for(int i=0; i<len; i++) {
        size = cmd->vals(i).val_size() ;
        for(int j=0; j<size; j++) {
            all += cmd->vals(i).val(j).size()+cmd->keys(i+1).key(j).size() ;
            values+=cmd->vals(i).val(j)+" "+ cmd->keys(i+1).key(j)+" ";
        }
        break ;
    }
    //去掉末尾的空格
    values = values.substr(0, values.size()-1) ;
    all = 1+key.size() ;
    size = all ;
    sprintf(buf, "%ld %s %s %s\r\n", all, cmd->cmd().c_str(), key.c_str(), values.c_str()) ;
    return buf ;
}

string logRecord::formatListAddLog(const std::shared_ptr<Command>&cmd, int& size) {
    char buf[BUFFER_SIZE] ;
    bzero(buf, sizeof(buf)) ;
    ListObject lob = cmd->lob(0) ;
    string key = lob.key() ;
    int len = lob.vals_size() ;
    string values = "" ;
    long all = 0 ;
    for(int i=0; i<len; i++) {
        int size = lob.vals(i).val_size() ;
        for(int j=0; j<size; j++) {
            all+=lob.vals(i).val(j).size() ;
            values+=lob.vals(i).val(j)+" " ;
        }
    }
    values = values.substr(0, values.size()-1) ;
    all = 1+key.size() ;
    size = all ;
    sprintf(buf, "%ld %s %s %s\r\n", all, cmd->cmd().c_str(), key.c_str(), values.c_str()) ;
    return buf ;
}

void logRecord::recordLog(int num, const char* buf) {
    //写一条日志到日志文件中
    int ret = write(ls[num], buf, strlen(buf)) ;
    if(ret < 0) {
        LOG(ERROR) << __FILE__ <<"  " << __LINE__ << "  " << strerror(errno) ;
    }   
    return ;
    /*
    struct stat st ;
    ret = fstat(ls[num], &st) ;
    if(ret < 0) {
        LOG(ERROR) << __LINE__ << "  " << __FILE__ << "  " << strerror(errno)  ;
    }

    //文件大小大于4MB的话，就变成不可改变的文件
    if(st.st_size >= MAX_FILE_SIZE) {
        //将文件内容映射出来
        const char* info = (char*)mmap(0, st.st_size, PROT_READ, MAP_SHARED, ls[num], 0) ;                  
        char buf[BUFFER_SIZE] ;
        bzero(buf, sizeof(buf)) ;
        sprintf(buf, "../logInfo/%d_tmp.log", num) ;
        int fd = open(buf, O_CREAT, 0666) ;
        if(fd < 0) {
            LOG(ERROR)<< __LINE__ << "   "<< __FILE__ << "  "<< strerror(errno) ;
        }
        ret = write(fd, info, strlen(buf)) ;
        if(ret < 0) {
            LOG(ERROR)<< __FILE__<<"  " <<__LINE__ << "    " << strerror(errno)  ;
        }
        close(ls[num]) ;
        ftruncate(ls[num], 0) ;
        lseek(fd, 0,  0) ;
    }*/
}

//第一个参数是数据库编号，第二个参数是要写的数据
void logRecord::record(int num, string buf) {
    auto ret = getRecordObject() ;
    if(!lr->isExistRedisLog(num)) {
        lr->addLog(num) ;
    }
    ///记录日志
    int size = buf.size() ;
    //如果要写的数据不是块大小的整数倍，则将数据块填充成块大小的整数倍
    //一个块是4096字节
    if(!(size%BLOCK_SIZE)) {
        int remain = 4096-size%BLOCK_SIZE ;
        while(remain) {
            buf+='\r' ;
            remain-- ;
            buf+='\n' ;
            remain-- ;
        }
    }
    //写日志
    lr->recordLog(num, buf.c_str()) ;
    //lock_guard<mutex>lk(logRecord::mute) ;
    /*if(sizeMap[num]<=0) {
        SUM+=logRecord::MAX_FILE_SIZE ;
        SUM-=sizeMap[num] ;
        sizeMap[num] = logRecord::MAX_FILE_SIZE ;
        if(SUM >= MAX_FILE_SIZE*cmdSet::REDIS_NUM) {
            cset->saveToFrozenRedis(cset->dbLs) ;
        }   
        SUM = 0 ;
    }*/
}   

bool logRecord::isExistRedisLog(int num) {
    auto ret = ls.find(num) ;
    if(ret == ls.end()) {
        return false ;
    }
    return true ;
}

int logRecord :: setRecordFileFd() {
    int fd = open("../logInfo/allLogFileName", O_RDWR) ;
}

void logRecord::addLog(int num) {
    char buf[512] ;
    bzero(buf, sizeof(buf)) ;
    sprintf(buf, "../logInfo/%d.log", num) ;
    //创建日志文件
    int fd = open(buf,O_CREAT) ;
    if(fd < 0) {
        std::cout<< __FILE__ << "   " << __LINE__ << std::endl ;
        return ;
    }
    int ret = close(fd) ;
    if(ret < 0) {
        std::cout << __FILE__ << "  " << __LINE__ << std:: endl ;
    }
    //以读写的行形式打开，写的时候强制将内核缓冲区的中的数据
    //直接刷到磁盘块
    fd = open(buf, O_RDWR|O_DIRECT) ;

    //讲日志文件名称记录到文件中
    strcat(buf, "\r\n") ;
    write(recordFileFd, buf, sizeof(buf)) ;

    ls.insert({num, fd}) ;

    //设置当前新数据库的编号和最大容量
    sizeMap.insert({num, MAX_FILE_SIZE}) ;
}

