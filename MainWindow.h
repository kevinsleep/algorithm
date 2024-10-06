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
* ����һ���Թ����ƴ���MazeWidget��һ����ʾ��Ϣ����ShowInfoWidget
* ������һ��DrawThread�̣߳����������Թ�
*/

class MazeWidget;
class ShowInfoWidget;
class DrawThread;




class MazeWidget : public QWidget
{
	Q_OBJECT
public:
	Generate_method method = Generate_method::DeepFirstSearch;

	AdjacencyList* maze; //���ڽӱ��ʾ�Թ�����ӵ����ӹ�ϵ

	MazeWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event) override;
    

	//�������������ѱ��������书����DrawThread���еķ������
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

	AdjacencyList* maze; //���ڽӱ��ʾ�Թ�����ӵ����ӹ�ϵ

	QImage paintMaze(); //�����Թ��ķ���
	//~MazeWidget();

	void generateMaze(int row, int column); //�����Թ��ķ���

	void generateMazeByDeepFirstSearch(int row, int column);
	
	void run() override;
	bool isDrawing;
	int row;
	int column;
	DrawThread(MazeWidget* mazeWidget, int row, int column, Generate_method method);
signals:

	void imageReady(const QImage &image);

private:

};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public slots:
	void onImageReady(const QImage& image) {
		mazeWidget->image = new QImage(image);
		mazeWidget->update();
	}

private:
	//Ui::QtWidgetsApplication1Class ui;
	QSpinBox* row_cnt;
	QSpinBox* col_cnt;
	QLabel* label_row;
	QLabel* label_col;
	QPushButton* button_generate;
	MazeWidget* mazeWidget;
	ShowInfoWidget* showInfoWidget;
	DrawThread* drawThread;

	void generateButtonClicked();
};

