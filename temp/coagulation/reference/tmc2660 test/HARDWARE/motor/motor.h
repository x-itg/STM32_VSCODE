
#ifndef MOTOR_H_
#define MOTOR_H_




#define  ctrl_steps		320
#define  run					1
#define		stop				0

#define	motor_en_run()			GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define motor_en_standby()	GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define motor_dir_high()		GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define motor_dir_low()		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define motor_step_high()	  GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define motor_step_low()		GPIO_ResetBits(GPIOC,GPIO_Pin_2)






#define  dir_zero_pos	1



 void Motor_init(void);
 void motor_run(int steps,int dir);
 void motor_standby(void);





#endif
