
#include "game.h"
int start_x;
int start_y;
bool start = true;
bool over = false;
bool time_check_flag=false;  // 시간 판단 변수
clock_t start_time, end_time;  // 게임 시작, 종료 시간

void start_score()
{
	if (time_check_flag == false)
	{
		start_time = 0;
		end_time = 0;
		start_time = clock();
		time_check_flag = true;
	}

}

void end_score()
{
	if (time_check_flag == true)
	{
		end_time = clock();
		time_check_flag = false;
	}
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)  // 마우스 버튼 클릭시 동작
{

	if (event == cv::EVENT_LBUTTONUP && over==true)
	{
		start_x = x;
		start_y = y;
		cout << start_x << " " << start_y << endl;
	}
}

int main()
{
	game simplegame;
	bool gameover;
	cv::namedWindow("Dodge");
	simplegame.map_read();
	cv::setMouseCallback("Dodge", mouse_callback, 0);
	while (1)
	{
		if (start && !over)
		{
			cv::imshow("Dodge", simplegame.getstartMap());
			int key = cv::waitKey(0);
			if (key == 27)
				break;
			start_time = 0;
			end_time = 0;
			if (time_check_flag == false)
			{
				time_check_flag = true;
				start_time = clock();
			}
			start = false;
		}
		simplegame.spawn();

		int key = cv::waitKeyEx(1);
		if (key == 27)
			break;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //왼쪽
			key = 1;
			simplegame.key_input(key);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //오른쪽
			key = 3;
			simplegame.key_input(key);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) { //위
			key = 2;
			simplegame.key_input(key);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //아래
			key = 4;
			simplegame.key_input(key);
		}
		if (!start)
		{
			simplegame.b_move();
			gameover = simplegame.map_show(start_time,end_time);
			if (gameover)
			{
				if (time_check_flag == true)
				{
					time_check_flag = false;
					end_time = clock();
				}
				cv::imshow("Dodge", simplegame.gameover(start_time,end_time));
				key =cv::waitKey(1000);
				if (key == 27)
					break;
				over = true;
			}
		}
		if (start_x >= 233 && start_x <= 468 && start_y >= 261 && start_y <= 390 && over == true && start ==false)
		{
			start_x = 0;
			start_y = 0;
			over = false;
			start = true;
			simplegame.map_read();
		}
	}

}
