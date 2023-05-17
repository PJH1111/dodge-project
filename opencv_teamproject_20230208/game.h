#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <iostream>
#pragma comment(lib, "User32.lib")
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
using namespace std;

#define B_SIZE 20   // 탄막 개수
#define MAP_SIZE 500  // 맵의 크기 500: 500X500 크기
#define PLAYER_SIZE 20  // 플레이어의 크기 (20X20) 
#define PLAYER_SPEED 3  // 플레이어 속도
#define Bullet_SPEED 2  // 탄막 최대 속도 (1~Bullet_SPEED 중 랜덤)


class game
{
protected:
	int bullet;  // 탄막 최대 개수
	int vec[B_SIZE];  // 탄막 이동 방향
	bool hit_flag;   // 피격 판단 변수
	cv::Mat first;  // 처음 시작 화면
	cv::Mat game_end;    // 게임 오버 화면
	cv::Mat base;   // 베이스가 되는 맵 원본
	cv::Mat img,img2;  // 플레이어 적용 맵1, 탄막 적용된 맵 2
	cv::Mat img_last;  // 최종으로 보여줄 맵
	cv::Mat player, effect;   // 플레이어 , 탄막
	cv::Point e_speed[B_SIZE];  // 탄막 별 2차원 속도 (x축 속도, y축 속도)
	cv::Point p_e[B_SIZE];  // 탄막 별 좌표
	cv::Mat p_bin;  // 플레이어 이진화
	cv::Point a;   // player 좌표

public:
	game(); // 맵, 플레이어 생성
	cv::Mat getstartMap();  // 맵 시작 화면 반환
	void map_read();  //  다시 시작할 때 맵 초기화
	cv::Mat gameover(clock_t, clock_t);    // 게임 오버 화면 반환
	void key_input(int);   // 방향키 입력
	void spawn();    // 탄막 생성
	void b_move();   // 탄막 이동
	void attack();   // 탄막 피격
	bool map_show(clock_t, clock_t);  // 이미지 연산 후 게임 화면 show (게임 진행이면 false, 게임 오버시 true 반환)
	~game();  // 게임 종료
};

