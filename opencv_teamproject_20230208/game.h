#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <iostream>
#pragma comment(lib, "User32.lib")
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
using namespace std;

#define B_SIZE 20   // ź�� ����
#define MAP_SIZE 500  // ���� ũ�� 500: 500X500 ũ��
#define PLAYER_SIZE 20  // �÷��̾��� ũ�� (20X20) 
#define PLAYER_SPEED 3  // �÷��̾� �ӵ�
#define Bullet_SPEED 2  // ź�� �ִ� �ӵ� (1~Bullet_SPEED �� ����)


class game
{
protected:
	int bullet;  // ź�� �ִ� ����
	int vec[B_SIZE];  // ź�� �̵� ����
	bool hit_flag;   // �ǰ� �Ǵ� ����
	cv::Mat first;  // ó�� ���� ȭ��
	cv::Mat game_end;    // ���� ���� ȭ��
	cv::Mat base;   // ���̽��� �Ǵ� �� ����
	cv::Mat img,img2;  // �÷��̾� ���� ��1, ź�� ����� �� 2
	cv::Mat img_last;  // �������� ������ ��
	cv::Mat player, effect;   // �÷��̾� , ź��
	cv::Point e_speed[B_SIZE];  // ź�� �� 2���� �ӵ� (x�� �ӵ�, y�� �ӵ�)
	cv::Point p_e[B_SIZE];  // ź�� �� ��ǥ
	cv::Mat p_bin;  // �÷��̾� ����ȭ
	cv::Point a;   // player ��ǥ

public:
	game(); // ��, �÷��̾� ����
	cv::Mat getstartMap();  // �� ���� ȭ�� ��ȯ
	void map_read();  //  �ٽ� ������ �� �� �ʱ�ȭ
	cv::Mat gameover(clock_t, clock_t);    // ���� ���� ȭ�� ��ȯ
	void key_input(int);   // ����Ű �Է�
	void spawn();    // ź�� ����
	void b_move();   // ź�� �̵�
	void attack();   // ź�� �ǰ�
	bool map_show(clock_t, clock_t);  // �̹��� ���� �� ���� ȭ�� show (���� �����̸� false, ���� ������ true ��ȯ)
	~game();  // ���� ����
};

