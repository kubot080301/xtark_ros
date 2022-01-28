/**************************************************************************
功能：命令控制器，命令词识别结果转化为对应的执行动作
**************************************************************************/
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <iostream>
#include <stdio.h>
#include <std_msgs/Int8.h>
#include <geometry_msgs/PoseStamped.h>


using namespace std;
ros::Publisher vel_pub;    //创建底盘运动话题发布者
ros::Publisher cmd_vel_pub;
ros::Publisher speak_pub;  //发布语音播报数据
ros::Publisher follow_flag_pub;    //创建寻找声源标志位话题发布者
ros::Publisher cmd_vel_flag_pub;    //创建底盘运动控制器标志位话题发布者
ros::Publisher awake_flag_pub;    //创建唤醒标志位话题发布者
ros::Publisher navigation_auto_pub;    //创建自主导航目标点话题发布者
geometry_msgs::Twist cmd_msg;    //底盘运动话题消息数据
geometry_msgs::PoseStamped target;    //导航目标点消息数据

float A_position_x ;
float A_position_y ;
float A_orientation_z ;
float A_orientation_w ;

float B_position_x ;
float B_position_y ;
float B_orientation_z ;
float B_orientation_w ;

float C_position_x ;
float C_position_y ;
float C_orientation_z ;
float C_orientation_w ;

float line_vel_x ;
float ang_vel_z ;
float turn_line_vel_x ;
 
/**************************************************************************
函数功能：离线命令词识别结果sub回调函数
入口参数：命令词字符串  voice_control.cpp等
返回  值：无
**************************************************************************/
void voice_words_callback(const std_msgs::String& msg)
{
	/***指令***/
	string str1 = msg.data.c_str();    //取传入数据
	printf("收到指令: %s\r\n",str1);
	string str2 = "小克前进";
	string str3 = "小克后退"; 
	string str4 = "小克左转";
	string str5 = "小克右转";
	string str6 = "小克停止";
	string str7 = "小克休眠";
	string str8 = "小克过来";
	string str9 = "小克去1点";
	string str10 = "小克去2点";
	string str11 = "小克去3点";
	string str12 = "失败5次";
	string str13 = "失败10次";
	string str14 = "遇到障碍物";
	string str15 = "小克唤醒";


/***********************************
指令：小车前进
动作：底盘运动控制器使能，发布速度指令
***********************************/
	if(str1 == str2)
	{
		cmd_msg.linear.x = line_vel_x;
		cmd_msg.angular.z = 0;
		vel_pub.publish(cmd_msg);

		std_msgs::Int8 cmd_vel_flag_msg;
    	cmd_vel_flag_msg.data = 1;
    	cmd_vel_flag_pub.publish(cmd_vel_flag_msg);

		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，向前";
		speak_pub.publish(speak_msg_);

		cout<<"好的：前进"<<endl;
	}
/***********************************
指令：小车后退
动作：底盘运动控制器使能，发布速度指令
***********************************/
	else if(str1 == str3)
	{
		cmd_msg.linear.x = -line_vel_x;
		cmd_msg.angular.z = 0;
		vel_pub.publish(cmd_msg);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 1;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，向后";
		speak_pub.publish(speak_msg_);

		cout<<"好的：后退"<<endl;
	}
/***********************************
指令：小车左转
动作：底盘运动控制器使能，发布速度指令
***********************************/
	else if(str1 == str4)
	{
		cmd_msg.linear.x = turn_line_vel_x;
		cmd_msg.angular.z = ang_vel_z;
		vel_pub.publish(cmd_msg);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 1;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，向左";
		speak_pub.publish(speak_msg_);

		cout<<"好的：左转"<<endl;
	}
/***********************************
指令：小车右转
动作：底盘运动控制器使能，发布速度指令
***********************************/
	else if(str1 == str5)
	{
		cmd_msg.linear.x = turn_line_vel_x;
		cmd_msg.angular.z = -ang_vel_z;
		vel_pub.publish(cmd_msg);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 1;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，向右";
		speak_pub.publish(speak_msg_);

		cout<<"好的：右转"<<endl;
	}
/***********************************
指令：小车停
动作：底盘运动控制器失能，发布速度空指令
***********************************/
	else if(str1 == str6)
	{
		cmd_msg.linear.x = 0;
		cmd_msg.angular.z = 0;
		vel_pub.publish(cmd_msg);


		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 1;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，停";
		speak_pub.publish(speak_msg_);

		cout<<"好的：停止"<<endl;
	}
/***********************************
指令：小车休眠
动作：底盘运动控制器失能，发布速度空指令，唤醒标志位置零
***********************************/
	else if(str1 == str7)
	{
		cmd_msg.linear.x = 0;
		cmd_msg.angular.z = 0;
		vel_pub.publish(cmd_msg);

		std_msgs::Int8 awake_flag_msg;
        awake_flag_msg.data = 0;
        awake_flag_pub.publish(awake_flag_msg);

        std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 1;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，机器人等待下一次唤醒";
		speak_pub.publish(speak_msg_);

		cout<<"休眠，等待下一次唤醒"<<endl;
	}
/***********************************
指令：小车过来
动作：寻找声源标志位置位
***********************************/
	else if(str1 == str8)
	{
		std_msgs::Int8 follow_flag_msg;
		follow_flag_msg.data = 1;
		follow_flag_pub.publish(follow_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，寻找声源";
		speak_pub.publish(speak_msg_);
		cout<<"好的：寻找声源"<<endl;
	}
/***********************************
指令：小车去1点
动作：底盘运动控制器失能(导航控制)，发布目标点
***********************************/
	else if(str1 == str9)
	{
		target.pose.position.x = A_position_x;
		target.pose.position.y = A_position_y;
		target.pose.orientation.z = A_orientation_z;
		target.pose.orientation.w = A_orientation_w;
		navigation_auto_pub.publish(target);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 0;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，前往1目标点";
		speak_pub.publish(speak_msg_);

		cout<<"好的：自主导航至1点"<<endl;
		
	}
/***********************************
指令：小车去2点
动作：底盘运动控制器失能(导航控制)，发布目标点
***********************************/
	else if(str1 == str10)
	{
		target.pose.position.x = B_position_x;
		target.pose.position.y = B_position_y;
		target.pose.orientation.z = B_orientation_z;
		target.pose.orientation.w = B_orientation_w;
		navigation_auto_pub.publish(target);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 0;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，前往2目标点";
		speak_pub.publish(speak_msg_);

		cout<<"好的：自主导航至2点"<<endl;
	}
/***********************************
指令：小车去3点
动作：底盘运动控制器失能(导航控制)，发布目标点
***********************************/
	else if(str1 == str11)
	{
		target.pose.position.x = C_position_x;
		target.pose.position.y = C_position_y;
		target.pose.orientation.z = C_orientation_z;
		target.pose.orientation.w = C_orientation_w;
		navigation_auto_pub.publish(target);

		std_msgs::Int8 cmd_vel_flag_msg;
        cmd_vel_flag_msg.data = 0;
        cmd_vel_flag_pub.publish(cmd_vel_flag_msg);
		std_msgs::String speak_msg_;
		speak_msg_.data = "好的，前往3目标点";
		speak_pub.publish(speak_msg_);

		cout<<"好的：自主导航至3点"<<endl;
	}
/***********************************
辅助指令：失败5次
动作：用户界面打印提醒
***********************************/
	else if(str1 == str12)
	{
		cout<<"您已经连续【输入空指令or识别失败】5次，累计达15次自动进入休眠，输入有效指令后计数清零"<<endl;
	}
/***********************************
辅助指令：失败10次
动作：用户界面打印提醒
***********************************/
	else if(str1 == str13)
	{
		cout<<"您已经连续【输入空指令or识别失败】10次，累计达15次自动进入休眠，输入有效指令后计数清零"<<endl;
	}
/***********************************
辅助指令：遇到障碍物
动作：用户界面打印提醒
***********************************/
	else if(str1 == str14)
	{
		std_msgs::String speak_msg_;
		speak_msg_.data = "机器人发现障碍物，停止运动";
		speak_pub.publish(speak_msg_);
		cout<<"遇到障碍物，已停止运动"<<endl;
	}
/***********************************
辅助指令：小车唤醒
动作：用户界面打印提醒
***********************************/
	else if(str1 == str15)
	{
		std_msgs::String speak_msg_;
		speak_msg_.data = "机器人已唤醒,请说指令";
		speak_pub.publish(speak_msg_);
		cout<<"已被唤醒，请说语音指令"<<endl;
	}
}

/**************************************************************************
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/
int main(int argc, char** argv)
{

	ros::init(argc, argv, "cmd_rec");     //初始化ROS节点

	ros::NodeHandle n;    //创建句柄
	
	bool is_akm;
	
	/***创建寻找声源标志位话题发布者***/
	follow_flag_pub = n.advertise<std_msgs::Int8>("follow_flag",1);

	/***创建底盘运动控制器标志位话题发布者***/
	cmd_vel_flag_pub = n.advertise<std_msgs::Int8>("cmd_vel_flag",1);

	/***创建底盘运动话题发布者***/
	vel_pub = n.advertise<geometry_msgs::Twist>("ori_vel",10);

	cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

	speak_pub = n.advertise<std_msgs::String>("/speak",1);

	/***创建唤醒标志位话题发布者***/
	awake_flag_pub = n.advertise<std_msgs::Int8>("awake_flag", 1);

  /***创建自主导航目标点话题发布者***/
	navigation_auto_pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);

	/***创建离线命令词识别结果话题订阅者***/
	ros::Subscriber voice_words_sub = n.subscribe("voice_words",10,voice_words_callback);


	n.param<float>("/1_position_x", A_position_x, 1);
	n.param<float>("/1_position_y", A_position_y, 0);
	n.param<float>("/1_orientation_z", A_orientation_z, 0);
	n.param<float>("/1_orientation_w", A_orientation_w, 1);
	n.param<float>("/2_position_x", B_position_x, 2);
	n.param<float>("/2_position_y", B_position_y, 0);
	n.param<float>("/2_orientation_z", B_orientation_z, 0);
	n.param<float>("/2_orientation_w", B_orientation_w, 1);
	n.param<float>("/3_position_x", C_position_x, 3);
	n.param<float>("/3_position_y", C_position_y, 0);
	n.param<float>("/3_orientation_z", C_orientation_z, 0);
	n.param<float>("/3_orientation_w", C_orientation_w, 1);
	n.param<float>("/line_vel_x", line_vel_x, 0.2);
	n.param<float>("/ang_vel_z", ang_vel_z, 0.2);
	n.param<bool>("/is_ackman", is_akm, false);

	if(is_akm)
		turn_line_vel_x = 0.2;
	else 
		turn_line_vel_x = 0;

	/***自主导航目标点数据初始化***/
	target.header.seq = 0;
	//target.header.stamp;
	target.header.frame_id = "map";
	target.pose.position.x = 0;
	target.pose.position.y = 0;
	target.pose.position.z = 0;
	target.pose.orientation.x = 0;
	target.pose.orientation.y = 0;
	target.pose.orientation.z = 0;
	target.pose.orientation.w = 1;

  /***用户界面***/
	printf("<--             塔克机器人语音控制命令          -->\r\n");
	printf("<--              唤醒词:\033[32m你好小克\033[0m    -->\r\n");
	printf("<--        \033[32m小克前进\033[0m ---> 机器人前进           -->\r\n");
	printf("<--        \033[32m小克后退\033[0m ---> 机器人后退           -->\r\n");
	printf("<--        \033[32m小克左转\033[0m ---> 机器人左转           -->\r\n");
	printf("<--        \033[32m小克右转\033[0m ---> 机器人右转           -->\r\n");
	printf("<--        \033[32m小克停止\033[0m   ---> 机器人停止         -->\r\n");
	printf("<--        \033[32m小克休眠\033[0m ---> 机器人休眠           -->\r\n");
	printf("<--        \033[32m小克过来\033[0m ---> 机器人寻找声源       -->\r\n");
	printf("<--        \033[32m小克去1点\033[0m ---> 机器人导航到1点     -->\r\n");
	printf("<--        \033[32m小克去2点\033[0m ---> 机器人导航到2点     -->\r\n");
	printf("<--        \033[32m小克去3点\033[0m ---> 机器人导航到3点     -->\r\n");

	cout<<"\n"<<endl;
	ros::spin();
}

