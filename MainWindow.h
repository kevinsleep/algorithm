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
#include <QThread>
#include <QImage>
#include <string>
#include <set>
#include <QComboBox>
#include "AdjacencyList.h"
//#include "ui_QtWidgetsApplication1.h"

enum class Generate_method
{
	DeepFirstSearch = 0,
	Prim = 1,
	Kruskal = 2
};

enum class FindPath_method
{
	DeepFirstSearch = 0,
	BreadthFirstSearch = 1,
	Dijkstra = 2,
	AStar = 3
};

extern const char* generate_method_str[];

/*
* MainWindow是该程序的主体窗口
* 包含一个迷宫绘制窗口MazeWidget和一个显示信息窗口ShowInfoWidget
* 还包含一个DrawThread线程，用于生成迷宫
*/

class MazeWidget;
class ShowInfoWidget;
class DrawThread;




class MazeWidget : public QWidget
{
	Q_OBJECT
public:
	Generate_method method = Generate_method::DeepFirstSearch;

	AdjacencyList* maze; //用邻接表表示迷宫间格子的连接关系

	MazeWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event) override;
    

	//下面三个方法已被废弃，其功能由DrawThread类中的方法替代
	//void paintMaze(QPainter& painter);

	//void generateMaze(int row, int column);

	//void generateMazeByDeepFirstSearch(int row, int column);

	QImage* image = nullptr;

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

class DrawThread : public QThread
{
	Q_OBJECT
public:
	Generate_method method = Generate_method::DeepFirstSearch;
	FindPath_method findPathMethod = FindPath_method::DeepFirstSearch;

	AdjacencyList* maze; //用邻接表表示迷宫间格子的连接关系
	std::vector<std::pair<int, int>>* path;
	std::vector<std::pair<int, int>>* unaccessed;

	QImage paintMaze(); //绘制迷宫的方法
	QImage paintPath(); //绘制路径的方法
	//~MazeWidget();

	void generateMaze(int row, int column); //生成迷宫的方法

	void generateMazeByDeepFirstSearch(int row, int column);
	void generateMazeByPrim(int row, int column);
	void generateMazeByKruskal(int row, int column);
	
	void findPath(int start, int end); //寻找路径的方法

	void findPathByDeepFirstSearch(int start, int end);
	void findPathByBreadthFirstSearch(int start, int end);
	void findPathByDijkstra(int start, int end);
	void findPathByAStar(int start, int end);



	void run() override;
	bool isDrawing = false;
	bool isGenerated = false;
	bool isFindingPath = false;

	int row;
	int column;
	QImage BaseImage;

	DrawThread(
		MazeWidget* mazeWidget, 
		int row, int column, 
		Generate_method method = Generate_method::DeepFirstSearch,
		FindPath_method findPathMethod = FindPath_method::DeepFirstSearch
	);

signals:
	void imageReady(const QImage &image);


};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

signals:
	void methodChanged(Generate_method method);

public slots:
	void onImageReady(const QImage& image) {
		mazeWidget->image = new QImage(image);
		mazeWidget->update();
	}

	void onMethodChanged(Generate_method method);

	void onComboboxFindPathMethodChanged(int index);
	
private:
	//Ui::QtWidgetsApplication1Class ui;
	QSpinBox* row_cnt;
	QSpinBox* col_cnt;
	QLabel* label_row;
	QLabel* label_col;
	QLabel* label_generate_method;
	QPushButton* button_generate;
	QPushButton* button_findpath;
	QComboBox* combobox_findpath_method;
	MazeWidget* mazeWidget;
	ShowInfoWidget* showInfoWidget;
	DrawThread* drawThread;

	void generateButtonClicked();
	void findPathButtonClicked();
};

