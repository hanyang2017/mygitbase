class My_Test_Class                            //������        
{ 
	public:
		int a,b,c;		//���ñ���
		void* point1;	//void��ָ��
		void* point2;
		int i,j;		//ѭ������
		int temp_val;	//��ʱֵ
		int count_num;	//����
		int st_flag;	//״̬��ʶ
		int op_code;		//������
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