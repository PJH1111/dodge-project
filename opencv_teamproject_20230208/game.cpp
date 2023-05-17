
#include "game.h"

game::game()
{
	
	srand(time(NULL));
	base = cv::imread("img\\dodge.png");   // 400X400
	player = cv::imread("img\\player.png");   // 30X25
	cv::resize(base, base, cv::Size(MAP_SIZE, MAP_SIZE));
	cv::resize(player, player, cv::Size(PLAYER_SIZE, PLAYER_SIZE));
	cvtColor(player, p_bin, cv::COLOR_BGR2GRAY);
	threshold(p_bin, p_bin, 230, 255, cv::THRESH_BINARY);
	base.copyTo(img2);
	base.copyTo(img);
	player.copyTo(img(cv::Rect(MAP_SIZE/2, MAP_SIZE / 2, player.cols, player.rows)));
	hit_flag = false;
	for (int i = 0; i < B_SIZE; i++)
	{
		e_speed[i].x = 0;
		e_speed[i].y = 0;
		p_e[i].x = -1;
		p_e[i].y = -1;
	}
	a.x = MAP_SIZE / 2;
	a.y = MAP_SIZE / 2;
	effect = cv::Mat(cv::Size(3, 3), CV_8UC3, cv::Scalar(0, 0, 255));
	bullet = B_SIZE;
}



cv::Mat game::getstartMap()
{
	first = cv::imread("img\\start.png");
	return ~first;
}

void game::map_read()
{
	base = cv::imread("img\\dodge.png");   // 400X400
	player = cv::imread("img\\player.png");   // 30X25
	cv::resize(base, base, cv::Size(MAP_SIZE, MAP_SIZE));
	cv::resize(player, player, cv::Size(PLAYER_SIZE, PLAYER_SIZE));
	cvtColor(player, p_bin, cv::COLOR_BGR2GRAY);
	threshold(p_bin, p_bin, 230, 255, cv::THRESH_BINARY);
	base.copyTo(img2);
	base.copyTo(img);
	player.copyTo(img(cv::Rect(MAP_SIZE / 2, MAP_SIZE / 2, player.cols, player.rows)));
	hit_flag = false;
	for (int i = 0; i < B_SIZE; i++)
	{
		e_speed[i].x = 0;
		e_speed[i].y = 0;
		p_e[i].x = -1;
		p_e[i].y = -1;
	}
	a.x = MAP_SIZE / 2;
	a.y = MAP_SIZE / 2;
	effect = cv::Mat(cv::Size(3, 3), CV_8UC3, cv::Scalar(0, 0, 255));

	bullet = B_SIZE;
}

cv::Mat game::gameover(clock_t start_time, clock_t end_time)
{
	char t[10];
	sprintf_s(t, " % .2f", difftime(end_time, start_time) / 1000.0);  // 게임 시간 ms를 s로 변환하고 저장
	game_end = cv::imread("img\\over.png");
	cv::putText(game_end, "Score", cv::Point(30, 300), 1, 3, cv::Scalar(255, 255, 255), 2);
	cv::putText(game_end, t, cv::Point(0, 350), 1, 3, cv::Scalar(255, 255, 255), 2);
	return game_end;
}

void game::key_input(int key)
{
	
	if (key == 1)  // left
	{
			a.x -= PLAYER_SPEED;
			if (a.x <= 0)
				a.x = 0;
	}
	else if (key == 2)  // up
	{
			a.y -= PLAYER_SPEED;
			if (a.y <= 0)
				a.y = 0;
	}
	else if (key == 3)  // right
	{
			a.x += PLAYER_SPEED;
			if (a.x >= MAP_SIZE - PLAYER_SIZE)
				a.x = MAP_SIZE - PLAYER_SIZE;
	}
	else if (key == 4) // down
	{
			a.y += PLAYER_SPEED;
			if (a.y >= MAP_SIZE - PLAYER_SIZE)
				a.y = MAP_SIZE - PLAYER_SIZE;
	}

	base.copyTo(img);
	cv::Mat temp = img(cv::Rect(a.x, a.y, player.cols, player.rows));
	bitwise_and(temp, temp, temp, ~p_bin);
	bitwise_and(player, player, player, p_bin);
	add(temp, player, temp);
	temp.copyTo(img(cv::Rect(a.x, a.y, player.cols, player.rows)));
	attack();
	
}

void game::spawn()
{
	bullet = B_SIZE;

	for (int i = 0; i < bullet; i++)
	{
		if (e_speed[i].x == 0 && e_speed[i].y==0 && p_e[i].x == -1 && p_e[i].y == -1)
		{
			e_speed[i].x = rand() % Bullet_SPEED+1  + (rand() % 4 * 100);  // 속도 (1,2,3), 위치( 왼쪽, 위, 오른쪽, 아래 끝 쪽) 
			e_speed[i].y = rand() % Bullet_SPEED+1;
			vec[i] = rand() % 3;   // 탄막들의 이동 방향
			p_e[i].x = rand() % (MAP_SIZE-10)+5;
			p_e[i].y = rand() % (MAP_SIZE-10)+5;


			if (e_speed[i].x/100 == 0)
			{
				p_e[i].y = 5;
			}
			else if (e_speed[i].x / 100 == 1)
			{
				p_e[i].x = MAP_SIZE - effect.cols - 2;
			}
			else if(e_speed[i].x / 100 == 2)
			{
				p_e[i].y = MAP_SIZE - effect.rows - 2;
			}
			else if (e_speed[i].x / 100 == 3)
			{
				p_e[i].x = 5;
			}
			cv::circle(img2, p_e[i], 3, cv::Scalar(0, 0, 255), -1);

		}

	}

}

void game::b_move()
{
	base.copyTo(img2);

	for (int i = 0; i < bullet; i++)
	{
		
		if (e_speed[i].x / 100 == 0)  // 위쪽 탄막일때
		{
			if (vec[i] == 0) 
			{
				p_e[i].x -= e_speed[i].x%100;
				p_e[i].y += e_speed[i].y;
				if (p_e[i].x <= 4 || p_e[i].y>= MAP_SIZE -effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 1)
			{
				p_e[i].y += e_speed[i].y;
				if (p_e[i].x%2==0)
				{
					p_e[i].x += e_speed[i].x % 100;
				}
				else
				{
					p_e[i].x -= e_speed[i].x % 100;
				}

				if (p_e[i].y >= MAP_SIZE -effect.rows-1 || p_e[i].x<= 4 || p_e[i].x>= MAP_SIZE -effect.cols-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 2)
			{
				p_e[i].x += e_speed[i].x % 100;
				p_e[i].y += e_speed[i].y;
				if (p_e[i].x >= MAP_SIZE -effect.cols-1 || p_e[i].y >= MAP_SIZE - effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
		}
		else if (e_speed[i].x / 100 == 1)  // 오른쪽 탄막일 때
		{
			if (vec[i] == 0)
			{
				p_e[i].x -= e_speed[i].x % 100;
				p_e[i].y -= e_speed[i].y;
				if (p_e[i].x <= 4 || p_e[i].y <= 4)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 1)
			{
				p_e[i].x -= e_speed[i].x % 100;
				if (p_e[i].y % 2 == 0)
				{
					p_e[i].y += e_speed[i].y;
				}
				else
				{
					p_e[i].y -= e_speed[i].y;
				}

				if (p_e[i].x <= 4 || p_e[i].y <= 4 || p_e[i].y >= MAP_SIZE - effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 2)
			{
				p_e[i].x -= e_speed[i].x % 100;
				p_e[i].y += e_speed[i].y;
				if (p_e[i].x <= 4 || p_e[i].y >= MAP_SIZE - effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
		}
		else if (e_speed[i].x / 100 == 2)  // 아래 탄막일 때
		{
			if (vec[i] == 0)
			{
				p_e[i].x -= e_speed[i].x % 100;
				p_e[i].y -= e_speed[i].y;
				if (p_e[i].x <= 4 || p_e[i].y <= 4)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 1)
			{
				p_e[i].y -= e_speed[i].y;
				if (p_e[i].x % 2 == 0)
				{
					p_e[i].x += e_speed[i].x % 100;
				}
				else
				{
					p_e[i].x -= e_speed[i].x % 100;
				}

				if (p_e[i].y <= 4 || p_e[i].x <= 4 || p_e[i].x >= MAP_SIZE - effect.cols-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 2)
			{
				p_e[i].x += e_speed[i].x % 100;
				p_e[i].y -= e_speed[i].y;
				if (p_e[i].x >= MAP_SIZE-effect.cols-1 || p_e[i].y <= 4)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
		}
		else if (e_speed[i].x / 100 == 3)  // 왼쪽 탄막일 때
		{
			if (vec[i] == 0)
			{
				p_e[i].x += e_speed[i].x % 100;
				p_e[i].y -= e_speed[i].y;
				if (p_e[i].x <= MAP_SIZE-effect.cols-1 || p_e[i].y <= 4)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 1)
			{
				p_e[i].x += e_speed[i].x % 100;
				if (p_e[i].y % 2 == 0)
				{
					p_e[i].y += e_speed[i].y;
				}
				else
				{
					p_e[i].y -= e_speed[i].y;
				}

				if (p_e[i].x >= MAP_SIZE-effect.cols-1 || p_e[i].y <= 4 || p_e[i].y >= MAP_SIZE - effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
			else if (vec[i] == 2)
			{
				p_e[i].x += e_speed[i].x % 100;
				p_e[i].y += e_speed[i].y;
				if (p_e[i].x >= MAP_SIZE-effect.cols-1 || p_e[i].y >= MAP_SIZE - effect.rows-1)
				{
					e_speed[i].x = 0;
					e_speed[i].y = 0;
					p_e[i].x = -1;
					p_e[i].y = -1;
				}
			}
		}
		if (e_speed[i].x != 0) // 탄막이 초기값이 아닐 때
		{
			cv::circle(img2, p_e[i], 3, cv::Scalar(0, 0, 255), -1);
		}
	}

	attack();  
}



void game::attack() // 피격 판별
{
	cv::Mat t = img2(cv::Rect(a.x, a.y, player.cols, player.rows));   // 연산량을 줄이기 위해 탄막 이미지에서 플레이어 위치의 영역 추출

	for (int i = 0; i < t.rows; i++)
	{
		for (int j = 0; j < t.cols; j++)
		{
			
			if (t.at<cv::Vec3b>(i, j)[2] == 255) // 플레이어 현 위치의 픽셀이 탄막과 같을 때
			{
				hit_flag = true;
				break;
			}
		}
	}
}

bool game::map_show(clock_t start_time, clock_t end_time)
{
	if (hit_flag == false)
	{
		cv::add(img, img2, img_last);

		cv::imshow("Dodge", img_last);
		return false;
	}
	else
	{
		return true;
	}
}

game::~game()
{
	std::cout << "게임 종료" << std::endl;
}