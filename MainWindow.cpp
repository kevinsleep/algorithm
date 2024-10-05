#include "MainWindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* fileMenu = menuBar->addMenu("File");
	QMenu* GenerationMenu = menuBar->addMenu("Generation Algorithm");
	QMenu* SolveMenu = menuBar->addMenu("Solve Algorithm");
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    setMenuBar(menuBar);

    QToolBar* toolBar = new QToolBar(this);
    toolBar->addAction(exitAction);
    addToolBar(toolBar);


    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

	mazeWidget = new MazeWidget(this);

	showInfoWidget = new ShowInfoWidget(this);
	showInfoWidget->setGeometry(10, 60, 160, 140);

	row_cnt = new QSpinBox(this);
	row_cnt->setGeometry(100, 210, 50, 20);
	row_cnt->setMinimum(10);
	col_cnt = new QSpinBox(this);
	col_cnt->setGeometry(100, 240, 50, 20);
	col_cnt->setMinimum(10);


	QFont monospaceFont("Courier New", 10, true);
	label_row = new QLabel("Row", this);
	label_row->setGeometry(20, 210, 50, 20);
	label_row->setFont(monospaceFont);
	label_col = new QLabel("Column", this);
	label_col->setGeometry(20, 240, 50, 20);
	label_col->setFont(monospaceFont);

	button_generate = new QPushButton("Generate", this);
	button_generate->setGeometry(20, 270, 120, 30);
	button_generate->setFont(monospaceFont);
	connect(button_generate, &QPushButton::clicked, this, &MainWindow::generateButtonClicked);

	//button_generate->setStyleSheet("background-color: lightgrey");
    /*QDockWidget* dockWidget = new QDockWidget("Dockable", this);
    QLabel* dockLabel = new QLabel("This is a dockable widget", dockWidget);
    dockWidget->setWidget(dockLabel);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);*/
}

MainWindow::~MainWindow()
{}

void MainWindow::generateButtonClicked()
{
	int row = row_cnt->value();
	int column = col_cnt->value();
	mazeWidget->generateMaze(row, column);
}

MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent)
{
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	maze = new AdjacencyList(50, 50);

	setGeometry(170, 60, 600, 500);
	repaint();
}


void MazeWidget::paintEvent(QPaintEvent* event)
{
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	paintMaze(painter);
    
}

void MazeWidget::paintMaze(QPainter& painter)
{
	painter.setPen(QPen(Qt::black, 1));
	QRect r = rect().adjusted(1, 1, -1, -1);

	painter.drawRect(r);
	int row = maze->getRow();
	int column = maze->getColumn();

	int cell_width = r.width() / column;
	int cell_height = r.height() / row;

	int less_pixel_x = r.width() - cell_width * column;
	int less_pixel_y = r.height() - cell_height * row;

	int start_x = 1;
	int start_y = 1;

	int temp_x = 0;
	int temp_y = 0;

	for (int i = 0; i < row; i++)
	{
		if (temp_y > row) {
			start_y += 1;
			temp_y -= row;
		}

		temp_x = 0;
		start_x = 0;
		for (int j = 0; j < column; j++)
		{
			if (temp_x > column) {
				start_x += 1;
				temp_x -= column;
			}

			int index = i * column + j;
			if (!maze->isNeighbor(index, index + 1))
			{
				painter.drawLine(start_x + cell_width, start_y, start_x + cell_width, start_y + cell_height);
			}
			if (!maze->isNeighbor(index, index + column))
			{
				painter.drawLine(start_x, start_y + cell_height, start_x + cell_width, start_y + cell_height);
			}

			temp_x += less_pixel_x;
			start_x += cell_width;
		}

		start_y += cell_height;
		temp_y += less_pixel_y;
	}
}

void MazeWidget::generateMaze(int row, int column)
{
	maze = new AdjacencyList(row, column);

	switch ( method)
	{
	case Generate_method::DeepFirstSearch:
		generateMazeByDeepFirstSearch(row, column);
		break;

	case Generate_method::Prim:

		break;

	case Generate_method::Kruskal:

		break;
	}
}

void MazeWidget::generateMazeByDeepFirstSearch(int row, int column)
{
	std::vector<int> visited(row * column, 0);
	std::stack<int> stack;
	stack.push(0);
	visited[0] = 1;

	while (!stack.empty())
	{
		int current = stack.top();

		std::vector<int> surround = *maze->getSurround(current);
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(surround.begin(), surround.end(), g);
		//std::random_shuffle(surround.begin(), surround.end());
		//c++17删除了这条特性
		int i = 0;
		for (; i < surround.size(); i++)
		{
			int temp = surround[i];
			if (visited[temp] == 0)
			{
				maze->connect(current, temp);
				visited[temp] = 1;
				stack.push(temp);
				break;
			}
		}

		if (i == surround.size())
		{
			stack.pop();
		}

		repaint();
		//update();
	}
}

ShowInfoWidget::ShowInfoWidget(QWidget* parent) : QWidget(parent)
{
	

	int label_x = 50;
	int checkbox_y_offset = 30;
	int label_width = 50;
	int label_height = 25;
	int checkbox_cnt = 0;
    QFont monospaceFont("Courier New", 10, true);

    label_show = new QLabel("Show", this);
    label_show->setGeometry(20, 0, 50, 20);
	label_show->setFont(monospaceFont);
    label_show->show();

	/*auto setLabel = [&](QLabel* label, const QString& text)
		{
			label = new QLabel(text, this);
			label->setGeometry(label_x, 20 + label_y_offset * label_cnt, label_width, label_height);
			label_cnt++;
			label->setFont(monospaceFont);
			label->adjustSize();
			label->show();
		};

	setLabel(label_wall, "Wall");
	setLabel(label_path, "Path");
	setLabel(label_solution, "Solution");
	setLabel(label_accessed, "Accessed");*/

	int checkbox_width = 20;
	int checkbox_height = 20;

    auto setCheckBox = [&](QCheckBox* checkbox, const QString& text) {
		checkbox = new QCheckBox(text,this);
		checkbox->setGeometry(20, 20 + checkbox_y_offset * checkbox_cnt, checkbox_width, checkbox_height);
		checkbox_cnt++;
		//checkbox->setText(text);
		checkbox->setFont(monospaceFont);
		checkbox->adjustSize();
		checkbox->show();
		};

	setCheckBox(checkbox_show_wall, "Wall");
	setCheckBox(checkbox_show_path, "Path");
	setCheckBox(checkbox_show_solution, "Solution");
	setCheckBox(checkbox_show_accessed, "Accessed");

}

void ShowInfoWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.setPen(QPen(Qt::gray, 1));
    QRect r = rect().adjusted(10, 10, -10, -10);

	painter.drawLine(r.topLeft(), r.topLeft() + QPoint(10,0));
	painter.drawLine(r.topRight() - QPoint(95,0), r.topRight());
    painter.drawLine(r.bottomLeft(), r.bottomRight());
    painter.drawLine(r.topLeft(), r.bottomLeft());  
    painter.drawLine(r.topRight(), r.bottomRight()); 

}
