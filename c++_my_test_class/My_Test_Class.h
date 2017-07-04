class My_Test_Class                            //类声明        
{ 
	public:
		int a,b,c;		//常用变量
		void* point1;	//void型指针
		void* point2;
		int i,j;		//循环控制
		int temp_val;	//临时值
		int count_num;	//计数
		int st_flag;	//状态标识
		int op_code;		//操作码
		int sum;
		char temp_str[200];
		
		void io_test();  
		void point_test();
		void sscanf_test();
		void sprintf_test();
		void strstr_test();
		void malloc_test();
        void fread_test();
        void array_test();
		void udp_client_test();
		void udp_server_test();
		void tcp_client_test();
		void tcp_server_test();
		 
	private:
		int unuse; 
                           
};