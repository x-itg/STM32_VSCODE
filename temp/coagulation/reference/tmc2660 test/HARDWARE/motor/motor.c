
#include "motor.h"
#include "sys.h"

 int Motor_steps;
 int Motor_cnt;
 int motor_pos;
 char motor_dir;
 char motor_flag;
const unsigned short speed_inc[]=
{
        60000,59623,58536,56848,54713,52290,49723,47128,44584,42145,39843,37690,35691,33841,32135,30562,29112,27775,26540,25399,
        24342,23362,22451,21604,20814,20076,19385,18739,18131,17560,17023,16516,16037,15584,15155,14749,14363,13996,13647,13314,
        12997,12694,12405,12129,11864,11610,11367,11133,10909,10694,10486,10287,10095,9909,9731,9558,9392,9231,9075,8925,
        8779,8639,8502,8370,8241,8117,7996,7879,7765,7654,7547,7442,7340,7241,7145,7051,6959,6870,6783,6698,
        6615,6535,6456,6379,6304,6231,6159,6089,6020,5953,5888,5824,5761,5700,5640,5581,5523,5467,5411,5357,
        5304,5252,5201,5150,5101,5053,5006,4959,4914,4869,4825,4782,4739,4698,4657,4616,4577,4538,4500,4462,
        4425,4389,4353,4318,4283,4249,4215,4182,4150,4118,4087,4056,4026,3997,3969,3941,3913,3887,3860,3835,
        3810,3785,3761,3737,3713,3691,3668,3646,3624,3603,3582,3561,3541,3521,3502,3483,3464,3445,3427,3409,
        3391,3373,3356,3339,3323,3306,3290,3274,3258,3243,3227,3212,3198,3183,3168,3154,3140,3126,3112,3099,
        3086,3072,3059,3047,3034,3021,3009,2997,2985,2973,2961,2949,2938,2926,2915,2904,2894,2883,2873,2863,
        2853,2843,2834,2824,2815,2806,2797,2788,2780,2771,2763,2755,2747,2739,2731,2724,2716,2709,2702,2695,
        2688,2681,2675,2668,2662,2655,2649,2643,2637,2631,2625,2620,2614,2608,2603,2598,2593,2587,2582,2577,
        2573,2568,2563,2559,2554,2550,2545,2541,2537,2533,2529,2525,2521,2517,2513,2510,2506,2503,2499,2496,
        2492,2489,2486,2483,2480,2477,2474,2471,2468,2466,2463,2460,2458,2455,2453,2450,2448,2446,2444,2442,
        2439,2437,2435,2434,2432,2430,2428,2426,2425,2423,2422,2420,2419,2417,2416,2415,2414,2412,2411,2410,
        2409,2408,2407,2407,2406,2405,2404,2404,2403,2402,2402,2401,2401,2401,2400,2400,2400,2400,2400,2400,

};
const unsigned short speed_dec[]=
{
  2400,2400,2400,2400,2400,2400,2400,2401,2401,2401,2402,2402,2403,2404,2404,2405,2406,2407,2407,2408,
        2409,2410,2411,2412,2414,2415,2416,2417,2419,2420,2422,2423,2425,2426,2428,2430,2432,2434,2435,2437,
        2439,2442,2444,2446,2448,2450,2453,2455,2458,2460,2463,2466,2468,2471,2474,2477,2480,2483,2486,2489,
        2492,2496,2499,2503,2506,2510,2513,2517,2521,2525,2529,2533,2537,2541,2545,2550,2554,2559,2563,2568,
        2573,2577,2582,2587,2593,2598,2603,2608,2614,2620,2625,2631,2637,2643,2649,2655,2662,2668,2675,2681,
        2688,2695,2702,2709,2716,2724,2731,2739,2747,2755,2763,2771,2780,2788,2797,2806,2815,2824,2834,2843,
        2853,2863,2873,2883,2894,2904,2915,2926,2938,2949,2961,2973,2985,2997,3009,3021,3034,3047,3059,3072,
        3086,3099,3112,3126,3140,3154,3168,3183,3198,3212,3227,3243,3258,3274,3290,3306,3323,3339,3356,3373,
        3391,3409,3427,3445,3464,3483,3502,3521,3541,3561,3582,3603,3624,3646,3668,3691,3713,3737,3761,3785,
        3810,3835,3860,3887,3913,3941,3969,3997,4026,4056,4087,4118,4150,4182,4215,4249,4283,4318,4353,4389,
        4425,4462,4500,4538,4577,4616,4657,4698,4739,4782,4825,4869,4914,4959,5006,5053,5101,5150,5201,5252,
        5304,5357,5411,5467,5523,5581,5640,5700,5761,5824,5888,5953,6020,6089,6159,6231,6304,6379,6456,6535,
        6615,6698,6783,6870,6959,7051,7145,7241,7340,7442,7547,7654,7765,7879,7996,8117,8241,8370,8502,8639,
        8779,8925,9075,9231,9392,9558,9731,9909,10095,10287,10486,10694,10909,11133,11367,11610,11864,12129,12405,12694,
        12997,13314,13647,13996,14363,14749,15155,15584,16037,16516,17023,17560,18131,18739,19385,20076,20814,21604,22451,23362,
        24342,25399,26540,27775,29112,30562,32135,33841,35691,37690,39843,42145,44584,47128,49723,52290,54713,56848,58536,59623,
};





void Motor_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	Motor_steps=0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;        //PC.1 is En
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;     //PC.2 is Step.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //PC.3 is Dir.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	
	motor_en_standby();
			//开启定时器外设时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		//配置定时器参数
		TIM_DeInit(TIM4); 
		TIM_TimeBaseStructure.TIM_Period = 1200 - 1; //24M 								 	//10ms定时			 
		TIM_TimeBaseStructure.TIM_Prescaler = (3 - 1);              
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
		//中断配置
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级2 低优先级别中断 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	 //响应优先级0 高级别的响应中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
		NVIC_Init(&NVIC_InitStructure);	  
		//开中断
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);					  
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
		//开启定时器			 
		TIM_Cmd(TIM4, DISABLE); 
	
}

void TIM4_Reload(const unsigned short Period)
{
	TIM_Cmd(TIM4,DISABLE);
	if(motor_dir==dir_zero_pos)//零点方向
	{
			motor_pos--;
	}
	else
	{
			motor_pos++;	
	}
	TIM_SetAutoreload(TIM4,Period -  1); // 重新加载频率
	TIM_SetCompare3  (TIM4,Period >> 1); // 占空比50%
  TIM_Cmd(TIM4,ENABLE);
}
char motor_timer_flag;
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	if(motor_timer_flag==0)
	{
		motor_step_high();
		motor_timer_flag=1;
	}
	else
	{
		motor_step_low();
		motor_timer_flag=0;
	}	
	if(motor_timer_flag==0)
	{		
			Motor_cnt++;
			if(Motor_cnt>= Motor_steps)
			{
				TIM_Cmd(TIM4,DISABLE);
				//MortorArunstatus=2;AdelayStopCnt=0;	
			}
			else
			{
							if(Motor_steps<(ctrl_steps*2))
							{
										if(Motor_cnt<(Motor_steps/2))
										{
													TIM4_Reload(speed_inc[Motor_cnt]);
										}
										else
										{
												TIM4_Reload(speed_dec[(ctrl_steps-Motor_steps/2)+(Motor_cnt-Motor_steps/2)]);//从（ctrl_steps-Motor_steps/2）到ctrl_steps;
										}
							}
							else
							{
								if(Motor_cnt<ctrl_steps)
								{
											TIM4_Reload(speed_inc[Motor_cnt]);
								}
								else if((Motor_steps-Motor_cnt)<ctrl_steps)
								{
											TIM4_Reload(speed_dec[ctrl_steps-(Motor_steps-Motor_cnt)]);//
								}	
								else
								{
											TIM4_Reload(speed_inc[ctrl_steps-1]);
								}		
							}			
			}
		}
}

void motor_reset()
{
	motor_pos=0;
}
void motor_run(int steps,int dir)
{
	Motor_steps=steps;
	Motor_cnt=0;
	motor_timer_flag=0;
	motor_dir=dir;
	motor_en_run();
	if(dir==0)
		motor_dir_low();
	else
		motor_dir_high();
	
	TIM4_Reload(speed_inc[0]);
	TIM_Cmd(TIM4,ENABLE);
}
void motor_standby()
{
	motor_en_standby();
}
