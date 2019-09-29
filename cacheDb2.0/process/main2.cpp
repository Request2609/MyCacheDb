#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include<iostream>
using namespace std;

/* 描述一个子进程的类 */
class process
{
	public:
		process():m_pid(-1){}
		pid_t m_pid;//子进程的PID
		int m_pipefd[2];//父子进程通信的管道
};

template<typename T>
class processpool
{
	public:
		/* 单例模式,以保证最多创建一个processpoll实例，这是程序正确处理信号的必要条件 */
		static processpool<T>* create(int listenfd,int process_number = 8)
		{
			if(!m_instance)m_instance = new processpool<T>(listenfd,process_number);
			return m_instance;
		}
		~processpool()
		{
			delete[] m_sub_process;
		}
		void run();
	private:
		processpool(int listenfd,int process_number = 8);
		void setup_sig_pipe();
		void run_parent();
		void run_child();

		static const int MAX_PROCESS_NUMBRE = 16;//进程池允许的最大子进程数量
		static const int USER_PRE_PROCESS = 65535;//每个进程最多能处理的客户数量
		static const int MAX_EVENT_NUMBRE = 10000;//epoll最多能处理的事件数
		int m_process_number;//进程池中的进程总数
		int m_idx;//子进程在进程池中的序号
		int m_epollfd;
		int m_listenfd;//监听socket
		bool m_stop;
		process* m_sub_process;//保存所有子进程的描述信息
		static processpool<T>* m_instance;//进程池静态实力
};
template<typename T> processpool<T>* processpool<T>::m_instance = NULL;

static int setnonblock(int fd)
{
	int old_flag = fcntl(fd,F_GETFL);
	assert(fcntl(fd,F_SETFL,old_flag | O_NONBLOCK) >= 0);
	return old_flag;
}

static void addfd(int epollfd,int fd)
{
	epoll_event ee;
	ee.data.fd = fd;
	ee.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ee);
	setnonblock(fd);
}

static void removefd(int epollfd,int fd)
{
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	close(fd);
}

void addsig(int sig,void(handler)(int),bool restart = true)
{
	struct sigaction sa;
	memset(&sa,'\0',sizeof(sa));
	sa.sa_handler = handler;
	if(restart)sa.sa_flags | SA_RESTART;
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig,&sa,NULL) != -1);
}

/* 创建一个进程池，listenfd是监听套接字，由父进程负责监听，然后通知子进程处理 */
template<typename T>
processpool<T>::processpool(int listenfd,int process_number):m_listenfd(listenfd),m_process_number(process_number),m_idx(-1),m_stop(false)
{
	assert(process_number > 0 && process_number <= MAX_PROCESS_NUMBRE);
	m_sub_process = new process[process_number];
	int i;
	/* 创建process_number个子进程，并建立他们和父进程之间的通信管道 */
	for(i = 0;i < process_number ; ++i)
	{
		int ret = socketpair(AF_UNIX,SOCK_STREAM,0,m_sub_process[i].m_pipefd);
		assert( ret == 0);
		m_sub_process[i].m_pid = fork();
		assert(m_sub_process[i].m_pid >= 0);
		if(m_sub_process[i].m_pid > 0)//父进程
		{
			close(m_sub_process[i].m_pipefd[1]);
			continue;//继续创建子进程
		}
		else//子进程
		{
			close(m_sub_process[i].m_pipefd[0]);
			m_idx = i;//自己在进程池的下标
			break;
		}
	}
}

/* 父进程中的m_idx == -1,子进程中的m_idx为进程池的下标 */
template<typename T>
void processpool<T>::run()
{
	if(m_idx != -1) run_child();
	else run_parent();
}

/* 统一事件源 */
template<typename T>
void processpool<T>::setup_sig_pipe()
{
	m_epollfd = epoll_create(1000);
	assert(m_epollfd != -1);

	addsig(SIGPIPE,SIG_IGN);//防止向关闭的客户端写数据导致服务器进程的终止
}

/* 父进程的执行函数 */
template<typename T>
void processpool<T>::run_parent()
{
	setup_sig_pipe();
	addfd(m_epollfd,m_listenfd);//父进程监听m_listenfd
	epoll_event events[MAX_EVENT_NUMBRE];
	int sub_process_counter = 0;
	char new_conn = 'c';
	int ret = -1;
	while( !m_stop)
	{
		int number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBRE,-1);
		if((number < 0) && (errno != EINTR))
		{
			printf("epoll failure\n");
			break;
		}
		int i;
		for(i = 0;i < number;i++)
		{
			int sockfd = events[i].data.fd;
			//如果有新连接到来，就选择一个子进程进行处理
			if(sockfd == m_listenfd)
			{
				int index = sub_process_counter;
				do
				{
					if(m_sub_process[index].m_pid != -1)break;
					index = (index + 1) % m_process_number;
				}
				while(index != sub_process_counter);
				if(m_sub_process[index].m_pid == -1)//没有子进程
				{
					m_stop =  true;
					break;
				}
				sub_process_counter = (index + 1) % m_process_number;
				send(m_sub_process[index].m_pipefd[0],&new_conn,sizeof(new_conn),0);
				printf("send request to child %d\n",index);
			}
		}
	}
	close(m_epollfd);
}

/*  子进程的执行函数 */ 
template<typename T>
void processpool<T>::run_child()
{
	setup_sig_pipe();
	int pipefd = m_sub_process[m_idx].m_pipefd[1];//每个子进程通过其在进程池中的序号找到与父进程的通信管道
	addfd(m_epollfd,pipefd);
	epoll_event events[MAX_EVENT_NUMBRE];
	T* users = new T[USER_PRE_PROCESS];//子进程处理的客户端对象池
	int ret;

	while( !m_stop)
	{
		int number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBRE,-1);
		if(number < 0 && errno != EINTR)
		{
			printf("epoll failure\n");
			break;
		}
		int i;
		for(i = 0;i < number;++i)
		{
			int sockfd = events[i].data.fd;
			//父进程发送来的新连接的通知
			if(sockfd == pipefd && events[i].events & EPOLLIN)
			{
				char client;
				ret = recv(sockfd,&client,sizeof(client),0);
				if((ret < 0 && errno != EAGAIN )||ret == 0)continue;
				struct sockaddr_in client_address;
				socklen_t client_length = sizeof(client_address);
				int connfd = accept(m_listenfd,(sockaddr*)&client_address,&client_length);
				if(connfd < 0)
				{
					printf("errno is : %d \n",errno);
					continue;
				}
				addfd(m_epollfd,connfd);
				users[connfd].init(m_epollfd,connfd,client_address);//调用模板T的初始化方法
			}
			else if(events[i].events & EPOLLIN)//客户端发来数据
			{
				users[sockfd].process();
			}
		}
	}
	delete[] users;
	users = NULL;
	close(pipefd);
	close(m_epollfd);
}

//回射类
class echo_conn
{
	public:
		void init(int epollfd,int sockfd,const sockaddr_in& client_addr)
		{
			m_epollfd = epollfd;
			m_sockfd = sockfd;
			m_address = client_addr;
			memset(m_buf,'\0',BUFFER_SIZE);
			m_read_idx = 0;
		}
		void process()
		{
			while(true)
			{
				int ret = recv(m_sockfd,m_buf,BUFFER_SIZE-1,0);
				if(ret < 0)
				{
					if(errno != EAGAIN)removefd(m_epollfd,m_sockfd);
					break;
				}
				else if(ret == 0)//客户端关闭
				{
					removefd(m_epollfd,m_sockfd);
					break;
				}
				else 
				{
					printf("client data is : %s\n",m_buf); 
					send(m_sockfd,m_buf,ret,0);
				}
			}
		}
	private:
		static const int BUFFER_SIZE = 1024;
		static int m_epollfd;
		int m_sockfd;
		sockaddr_in m_address;
		char m_buf[BUFFER_SIZE];
		int m_read_idx;//标记读缓冲区已经读入的客户数据的最后一个字节的下一个位置
};
int echo_conn::m_epollfd = 1;

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("usage %s server_ip server_port \n",basename(argv[0]));
		return -1;
	}

	struct sockaddr_in address;
	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET,argv[1],&address.sin_addr);
	address.sin_port = htons(atoi(argv[2]));
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	assert(listenfd != -1);
	int use = 1;
	int ret = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&use,sizeof(use));
	assert(ret == 0);
	ret = bind(listenfd,(const sockaddr*)&address,sizeof(address));
	assert(ret != -1);
	ret = listen(listenfd,100);
	assert(ret != -1);
	
	processpool<echo_conn>* pool = processpool<echo_conn>::create(listenfd);//创建进程池
	if(pool)
	{
		pool -> run();//父子进程返回后都会调用该方法
		delete pool;
	}

	close(listenfd);
	return 0;
}
