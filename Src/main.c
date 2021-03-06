
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dac.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "math.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

int i, j, k, l;
struct Snake s1;
float valueX = 0;
float valueY = 0;

//snake

int alive = 1;
int xa, ya;
int A;
int last = 4;
int check;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

struct coordinate {
	int x, y;
};
struct Snake {
	struct coordinate length[525];
	int L;
};

int randomx() {
	return(rand() % 3100 + 300);
}
int randomy() {
	return(rand() % 3100 + 300);
}

void table() {
    	for(i = 0; i <= 4000; i++){
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,4000);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,4001);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,4002);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,i);
		}
		for(i = 0; i <= 4000; i++){
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,i);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,4000);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,4001);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,4002);
		}
		for(i = 0; i <= 4000; i++){
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,2);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,1);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,0);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,i);
		}
		for(i = 0; i <= 4000; i++){
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,i);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,2);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,1);
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,0);
		}
}

void showSnake(){
	  // chap e mar e avvalie
	  for (k = (s1.L - 1); k > 0 ; k--) {
		  if(last == 3 || last == 4){
			  for(l = 0; l < 100; l++){
				  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,s1.length[k].x + l);
				  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,s1.length[k].y + l);
			  }
		  }
		  else{
			  for(l = 0; l < 100; l++){
				  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,s1.length[k].y + l);
				  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,s1.length[k].x + l);
			  }
		  }

	  }
}

void shift() {
	int i;
	for (i = 524; i >= 1; i--) {
		s1.length[i].x = s1.length[i - 1].x;
		s1.length[i].y = s1.length[i - 1].y;
	}
}

int die() {
	int i;
	for (i = 1; i <= s1.L; i++) {
		if (s1.length[0].x == s1.length[i].x && s1.length[0].y == s1.length[i].y) return 0;
	}
	return 1;
}

void Apple() {
	int sw, m;
	do {
		sw = 0;
		xa = randomx();
		ya = randomy();
		for (m = 0; m < s1.L; m++) {
			if(last == 1 || last == 2){
				for(int k = -100; k <= 100; k++){
					if (s1.length[m].x + k == xa){
						for(int l = - 10; l <= 10; l++){
							if(s1.length[m].y + l == ya){
								sw = 1;
								break;
							}
						}
					}
					break;
				}
			}
			else {
				for(int k = -100; k <= 100; k++){
					if (s1.length[m].y + k == ya){
						for(int l = - 10; l <= 10; l++){
							if(s1.length[m].x + l == xa){
								sw = 1;
								break;
							}
						}
					}
					break;
				}
			}
			break;
		}
	} while (sw == 1);
}

int eatApple() {
	if(last == 1 || last == 2){
		for(int o = -100; o <= 100; o++){
			if (s1.length[0].x + o == xa){
				for(int p = -50; p <= 50; p++){
					if(s1.length[0].y + p == ya){
						return 1;
					}
				}
			}
		}
	}
	else {
		for(int o = -100; o <= 100; o++){
			if (s1.length[0].y + o == ya){
				for(int p = -50; p <= 50; p++){
					if(s1.length[0].x + p == xa){
						return 1;
					}
				}
			}
		}
	}
	return 0;
}


void readADC(){
    valueX = (HAL_ADC_GetValue(&hadc1) - 2048.0) / 2048.0;
    valueY = (HAL_ADC_GetValue(&hadc2) - 2048.0) / 2048.0;
}


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  srand(time(NULL));

  	s1.L = 5;
	for(j = 4; j >= 0; j--){
	  s1.length[j].x = 200 + (j + 1) * 100;
	  s1.length[j].y = 500;

	}

  HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac,DAC_CHANNEL_2);

  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);


  showSnake(s1);
  table(); // table
//  srand(time(NULL)); // random avvalin sib
  Apple(s1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (alive == 1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	  table(); // table
	  readADC();
	  for(int i = -10; i < 10; i++){  //apple
		for(int j = -10; j < 10; j++){
				HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,xa + i);
				HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,ya + j);
			}
	  }
	  if (valueX >= -0.22 && valueX <= 0.22 && valueY >= -0.22 && valueY <= 0.22) {
			 check = eatApple();
			 if(check == 1){
				 s1.L++;
				 showSnake(s1);
				 Apple(s1);
				 check = 0;
			 }
			 shift();
			 if (last == 1) {                   // up
				 s1.length[0].y+=100;
			 }
			 if (last == 2) {                  // down
				 s1.length[0].y-=100;
			 }
			 if (last == 3) {                   //  left
				 s1.length[0].x-=100;
			 }
			 if (last == 4) {                    //  right
				 s1.length[0].x+=100;
			 }
			 alive = die(s1);
			 showSnake(s1);
		 }
		 else{
			 check = eatApple();
			 if(check == 1){
				 s1.L++;
				 showSnake(s1);
				 Apple(s1);
				 check = 0;
			 }
			 shift();
			  if (valueY > 0.2  && valueX >= 0 && valueX < 0.2 && last != 2) {// up
				  s1.length[0].y+=100;
				  last = 1;
			  }
			  else if (valueY < -0.2 && valueX >= 0 && valueX < 0.2 && last != 1) {// down
				  s1.length[0].y-=100;
				  last = 2;
			  }
			  else if (valueX > 0.2  && valueY >= 0 && valueY < 0.25 && last != 4) {// left
				  s1.length[0].x-=100;
				  last = 3;
			  }
			  else if (valueX < -0.2 && valueY >= -0.25 && valueY < 0.25 && last != 3) { // right
				  s1.length[0].x+=100;
				  last = 4;
			  }
			  else{
				 if (last == 1) {                   // up
					 s1.length[0].y+=100;
				 }
				 if (last == 2) {                  // down
					 s1.length[0].y-=100;
				 }
				 if (last == 3) {                   //  left
					 s1.length[0].x-=100;
				 }
				 if (last == 4) {                    //  right
					 s1.length[0].x+=100;
				 }
			  }
			 alive = die(s1);
			 showSnake(s1);
		  }

		  for (i = 0; i < s1.L; i++) {
			  if(s1.length[i].x >= 4000){
				s1.length[i].x -= 4000;
			  }
			  if(s1.length[i].y >= 4000){
				s1.length[i].y -= 4000;
			  }
			  if(s1.length[i].x <= 2){
				s1.length[i].x += 4000;
			  }
			  if(s1.length[i].y <= 2){
				s1.length[i].y += 4000;
			  }
		 }
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
