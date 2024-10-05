#pragma once

#include <QtWidgets/QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <qpainter.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <random>
#include <stack>
#include "AdjacencyList.h"
//#include "ui_QtWidgetsApplication1.h"

enum class Generate_method
{
	DeepFirstSearch,
	Prim,
	Kruskal
};

/*
* MainWindow�Ǹó�������崰��
*/





class MazeWidget : public QWidget
{
	Q_OBJECT
public:
	Generate_method method = Generate_method::DeepFirstSearch;

	AdjacencyList* maze; //���ڽӱ��ʾ�Թ�����ӵ����ӹ�ϵ

	MazeWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event) override;
    
	void paintMaze(QPainter& painter); //�����Թ��ķ���
	//~MazeWidget();
	void generateMaze(int row, int column); //�����Թ��ķ���

	void generateMazeByDeepFirstSearch(int row, int column);
	//void generateMaze();



};

class ShowInfoWidget : public QWidget
{
	Q_OBJECT
public:

	ShowInfoWidget(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* event) override;

private:
	QLabel* label_show;
	QLabel* label_wall;
	QLabel* label_path;
	QLabel* label_solution;
	QLabel* label_accessed;

	QCheckBox* checkbox_show_wall;
	QCheckBox* checkbox_show_path;
	QCheckBox* checkbox_show_solution;
	QCheckBox* checkbox_show_accessed;
};


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();


private:
	//Ui::QtWidgetsApplication1Class ui;
	QSpinBox* row_cnt;
	QSpinBox* col_cnt;
	QLabel* label_row;
	QLabel* label_col;
	QPushButton* button_generate;
	MazeWidget* mazeWidget;
	ShowInfoWidget* showInfoWidget;

	void generateButtonClicked();
};